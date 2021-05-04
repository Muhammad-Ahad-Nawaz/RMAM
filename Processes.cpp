#include <libproc.h>
#include <stdio.h>
#include <string.h>
//
//#include <sys/sysctl.h>

// https://ladydebug.com/blog/2019/08/29/kill-process-programmatically-by-pid/
// Signal id
// quit 3
// abort 6
// kill 9

#include <stdlib.h>
#include <signal.h>
#include <errno.h>

class Processes
{
public:
   /* void IsInBSDProcessList(char *name)    { 
       assert( name != NULL); 
      kinfo_proc *result; 
      size_t count = 0; 
      result = (kinfo_proc *)malloc(sizeof(kinfo_proc)); 
      if(GetBSDProcessList(&result,&count) == 0) { 
       for (int i = 0; i < count; i++) { 
         kinfo_proc *proc = NULL; 
          proc = &result[i]; 
      }
   } 
   free(result);
   }
*/
   void getAllProcess() //const char *name)
   {
      pid_t pids[2048];
      int bytes = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
      int n_proc = bytes / sizeof(pids[0]);
      for (int i = 0; i < n_proc; i++)
      {
         struct proc_bsdinfo proc;
         int st = proc_pidinfo(pids[i], PROC_PIDTBSDINFO, 0,
                               &proc, PROC_PIDTBSDINFO_SIZE);
         if (st == PROC_PIDTBSDINFO_SIZE)
         {
            // if (strcmp(name, proc.pbi_name) == 0) {
            /* Process PID */
            printf("%d ppid:%d [%s] [%s]\n", pids[i], proc.pbi_ppid, proc.pbi_comm, proc.pbi_name);
            // printf("gid %d rgid:%d svgid:%d\n", proc.pbi_pgid, proc.pbi_rgid, proc.pbi_svgid);
            // }
         }
      }
   }

   void killProcess(int pid)
   {
      int signal = 9; //atoi(ss[1]);
      int retVal = kill(pid, signal);
      if (retVal == 0)
      {
         printf("Process with pid=%d has been killed\n", pid);
         // return 0;
      }
      else
      {
         switch (errno)
         {
         case EINVAL:
            printf("An invalid signal=%d was specified\n", signal);
            break;
         case EPERM:
            printf("Current process does not have permission to send the signal to process with pid=%d\n", pid);
            break;
         case ESRCH:
            printf("Process with pid=%d does not exist\n", pid);
            break;
         default:
            printf("Other error with code=%d\n", errno);
            break;
         }
         // return -2;
      }
   }

   //Option 2
   /*
uid_t uidFromPid(pid_t pid)
{
    uid_t uid = -1;

    struct kinfo_proc process;
    size_t procBufferSize = sizeof(process);

    // Compose search path for sysctl. Here you can specify PID directly.
    const u_int pathLenth = 4;
    int path[pathLenth] = {CTL_KERN, KERN_PROC, KERN_PROC_PID, pid};

    int sysctlResult = sysctl(path, pathLenth, &process, &procBufferSize, NULL, 0);

    // If sysctl did not fail and process with PID available - take UID.
    if ((sysctlResult == 0) && (procBufferSize != 0))
    {
        uid = process.kp_eproc.e_ucred.cr_uid;
    }
    
    return uid;
}
*/
};
