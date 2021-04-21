
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mach/mach.h>
#include <iostream>

/// CPU load readings
static unsigned long long _previousTotalTicks = 0;
static unsigned long long _previousIdleTicks = 0;

class CPU {

public:

int read() 
{
        int nWaitSec = 1;
        unsigned long ulSystemPrevious;
        unsigned long ulUserPrevious;
        unsigned long ulNicePrevious;
        unsigned long ulIdlePrevious;
        int nErr = getCpuInfo(&ulSystemPrevious, &ulUserPrevious, &ulNicePrevious, &ulIdlePrevious);
        if(nErr != KERN_SUCCESS)
        {
                printf("Kernel error: %s\n", mach_error_string(nErr));
                return -1;
        }
        printf("Waiting for %d seconds\n", nWaitSec);
        sleep(nWaitSec);
        unsigned long ulSystemNext;
        unsigned long ulUserNext;
        unsigned long ulNiceNext;
        unsigned long ulIdleNext;
        nErr = getCpuInfo(&ulSystemNext, &ulUserNext, &ulNiceNext, &ulIdleNext);
        if(nErr != KERN_SUCCESS)
        {
                printf("Kernel error: %s\n", mach_error_string(nErr));
                return -1;
        }
        float fUsageTime = (float)(ulSystemNext - ulSystemPrevious) + (ulUserNext - ulUserPrevious) + (ulNiceNext - ulNicePrevious);
        float fTotalTime = fUsageTime + (float)(ulIdleNext - ulIdlePrevious);
        printf("Average CPU during last %d seconds is %f%%\n", nWaitSec, fUsageTime/fTotalTime*100);
        return 0;
}

int getCpuInfo(unsigned long * pulSystem, unsigned long * pulUser, unsigned long * pulNice, unsigned long * pulIdle)
{
        mach_msg_type_number_t  unCpuMsgCount = 0;
        processor_flavor_t nCpuFlavor = PROCESSOR_CPU_LOAD_INFO;;
        kern_return_t   nErr = 0;
        natural_t unCPUNum = 0;
        processor_cpu_load_info_t structCpuData;
        host_t host = mach_host_self();
        *pulSystem = 0;
        *pulUser = 0;
        *pulNice = 0;
        *pulIdle = 0;
        nErr = host_processor_info( host,nCpuFlavor,&unCPUNum,
                            (processor_info_array_t *)&structCpuData,&unCpuMsgCount );
        for(int i = 0; i<(int)unCPUNum; i++)
        {
                *pulSystem += structCpuData[i].cpu_ticks[CPU_STATE_SYSTEM];
                *pulUser += structCpuData[i].cpu_ticks[CPU_STATE_USER];
                *pulNice += structCpuData[i].cpu_ticks[CPU_STATE_NICE];
                *pulIdle += structCpuData[i].cpu_ticks[CPU_STATE_IDLE];
        }
        return nErr;
}

//CPU Load
float calculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
  unsigned long long totalTicksSinceLastTime = totalTicks-_previousTotalTicks;
  unsigned long long idleTicksSinceLastTime  = idleTicks-_previousIdleTicks;
  float ret = 1.0f-((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime)/totalTicksSinceLastTime : 0);
  _previousTotalTicks = totalTicks;
  _previousIdleTicks  = idleTicks;
    printf("_previousTotalTicks %llu seconds\n", _previousTotalTicks);
    printf("_previousIdleTicks %llu seconds\n", _previousIdleTicks);
    
  return ret;
}

// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
// You'll need to call this at regular intervals, since it measures the load between
// the previous call and the current one.
float getCPULoad()
{
   host_cpu_load_info_data_t cpuinfo;
   mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
   if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuinfo, &count) == KERN_SUCCESS)
   {
      unsigned long long totalTicks = 0;
      for(int i=0; i<CPU_STATE_MAX; i++) totalTicks += cpuinfo.cpu_ticks[i];
      return calculateCPULoad(cpuinfo.cpu_ticks[CPU_STATE_IDLE], totalTicks);
   }
   else return -1.0f;
}



};

//  int main() {
//   CPU cpu;    // Create an object (this will call the constructor)
//     cpu.read();
//     // cout << "Reading CPU load\n\n";
//     // cpu.getCPULoad();
//   return 0;
// }