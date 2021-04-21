
// // https://ladydebug.com/blog/2019/08/29/kill-process-programmatically-by-pid/
// // Signal id 
// // quit 3
// // abort 6
// // kill 9

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <signal.h>
// #include <errno.h>


// int main(int nn, char ** ss)
// {
//    if(nn!=3)
//    {
//       printf("Arguments required: signal id, process id to kill\n"
//          "For example: killtask 6666 9\n");
//       return -1;
//    }
//    if(strspn(ss[2],"0123456789")<strlen(ss[2]))
//    {
//       printf("Process id should be positive integeri\n"
//          "Take process id using \"pidof name_of_process\" command\n");
//       return -1;
//    }
//    int pid = atoi(ss[2]);
//    if(strspn(ss[1],"0123456789")<strlen(ss[1]))
//    {
//       printf("Signal id should be positive integer\n"
//          "Take signal id using \"kill -l\" command\n");
//       return -1;
//    }
//    int signal = atoi(ss[1]);
//    int retVal = kill(pid, signal);
//    if(retVal == 0)
//    {
//       printf("Process with pid=%d has been killed\n", pid);
//    return 0;
//    } else {
//       switch(errno)
//       {
//          case EINVAL:
//             printf("An invalid signal=%d was specified\n", signal);
//          break;
//          case EPERM:
//             printf("Current process does not have permission to send the signal to process with pid=%d\n",pid);
//          break;
//          case ESRCH:
//             printf("Process with pid=%d does not exist\n", pid);
//          break;
//          default:
//             printf("Other error with code=%d\n", errno);
//          break;
//       }
//       return -2;
//    }
// }