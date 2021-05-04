
#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdio.h>
#include <iostream>

#include <stdlib.h>
#include <errno.h>
#include <string.h>

using namespace std;

class SysCommands
{

public:
    void executeSysctlCommands()
    {
        printf("Reading System info\n");
        systemInfo();
        printf("Reading memory\n");
        readPhyMemo();
        printf("Reading Disk Names\n");
        readDiskNames();
        printf("is Mac book\n");
        isMacBook();
        printf("CPU Frequency\n");
        getCPUfreq();
        printf("CPU Info\n");
        showCPUInfo();
    }

    void readPhyMemo()
    {

        int mib[2] = {CTL_HW, HW_PHYSMEM};

        unsigned int physmem;

        size_t len;

        len = sizeof(physmem);

        sysctl(mib, 2, &physmem, &len, NULL, 0);

        printf("%u B / %u MB / %u GB\n",
               physmem,       // in bytes
               physmem >> 20, // in megabytes
               physmem >> 30  // in gigabytes
        );
    }

    void systemInfo()
    {
        char *p = NULL;
        size_t len;
        sysctlbyname("hw.model", NULL, &len, NULL, 0);
        //  p = malloc(len);
        sysctlbyname("hw.model", p, &len, NULL, 0);
        printf("%s\n", p);
        /* CTL_MACHDEP variables are architecture dependent so doesn't work 
 for every one */
        sysctlbyname("machdep.cpu.brand_string", NULL, &len, NULL, 0);
        //  p = malloc(len);
        sysctlbyname("machdep.cpu.brand_string", p, &len, NULL, 0);
        printf("%s\n", p);
        int64_t mem;
        len = sizeof(mem);
        sysctlbyname("hw.memsize", &mem, &len, NULL, 0);
        printf("System Memory : %lld\n", mem);
    }

    void readDiskNames()
    {

        int mib[2] = {CTL_HW, HW_DISKNAMES};

        string names;

        size_t len = 0;

        // len = sizeof(names);

        sysctl(mib, 2, &names, &len, NULL, 0);

        cout << "DISKNAMES " << names << "\n";
    }

    bool isMacBook()
    {
        size_t len = 0;
        ::sysctlbyname("hw.model", nullptr, &len, nullptr, 0);
        std::string model(len, '\0');
        ::sysctlbyname("hw.model", const_cast<char *>(model.data()), &len, nullptr, 0);
        bool isMbook = model.find("MacBook") != std::string::npos;
        printf("%u \n", isMbook);
        return isMbook;
    }

    uint64_t getCPUfreq(void)
    {
        uint64_t freq = 0;
        size_t size = sizeof(freq);

        if (sysctlbyname("hw.cpufrequency", &freq, &size, NULL, 0) < 0)
        {
            perror("sysctl");
        }
        printf("%llu \n", freq);
        return freq;
    }

    void showCPUInfo(void)
    {
        char buffer[1024];
        size_t size = sizeof(buffer);
        if (sysctlbyname("machdep.cpu.brand_string", &buffer, &size, NULL, 0) < 0)
        {
            perror("sysctl");
        }
        std::cout << buffer << '\n';
    }
};