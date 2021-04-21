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

class Processes {
    public:
void getAllProcess()//const char *name)
{
    pid_t pids[2048];
    int bytes = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
    int n_proc = bytes / sizeof(pids[0]);
    for (int i = 0; i < n_proc; i++) {
        struct proc_bsdinfo proc;
        int st = proc_pidinfo(pids[i], PROC_PIDTBSDINFO, 0,
                             &proc, PROC_PIDTBSDINFO_SIZE);
        if (st == PROC_PIDTBSDINFO_SIZE) {
            // if (strcmp(name, proc.pbi_name) == 0) {
                /* Process PID */
                printf("%d [%s] [%s]\n", pids[i], proc.pbi_comm, proc.pbi_name);              
            // }
        }       
    }
}


void killProcess(int pid)
{
   /*if(nn!=3)
   {
      printf("Arguments required: signal id, process id to kill\n"
         "For example: killtask 6666 9\n");
      return -1;
   }
   if(strspn(ss[2],"0123456789")<strlen(ss[2]))
   {
      printf("Process id should be positive integeri\n"
         "Take process id using \"pidof name_of_process\" command\n");
      return -1;
   }
   int pid = atoi(ss[2]);
   if(strspn(ss[1],"0123456789")<strlen(ss[1]))
   {
      printf("Signal id should be positive integer\n"
         "Take signal id using \"kill -l\" command\n");
      return -1;
   }*/

   int signal = 9;//atoi(ss[1]);
   int retVal = kill(pid, signal);
   if(retVal == 0)
   {
      printf("Process with pid=%d has been killed\n", pid);
   // return 0;
   } else {
      switch(errno)
      {
         case EINVAL:
            printf("An invalid signal=%d was specified\n", signal);
         break;
         case EPERM:
            printf("Current process does not have permission to send the signal to process with pid=%d\n",pid);
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


// int main()
// {
//     Processes process;
//     process.find_pids("bash"); // for now printing all process info

//    uid_t uid = process.uidFromPid(2930);
//     printf("%d", uid);
//     return 0;
// }