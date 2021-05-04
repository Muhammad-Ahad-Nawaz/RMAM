#include <sys/utsname.h>
#include <stdio.h>
#include <unistd.h>

class AboutMac
{
public:
    void read()
    {

        struct utsname buf;

        if (uname(&buf))
            return;

        printf("System : %s\n", buf.sysname);
        printf("Node name: %s\n", buf.nodename);
        printf("Release : %s\n", buf.release);
        printf("Version : %s\n", buf.version);
        printf("Machine : %s\n", buf.machine);
    }
};