
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <mach/mach.h>

#include <iostream>
#include "CPU.cpp"
#include "Processes.cpp"
#include "RAM.cpp"
// #include "Disk.cpp"
#include "DiskStats.cpp"
#include "Hardwares.cpp"
#include "AboutMac.cpp"
#include "SysCommands.cpp"

using namespace std;

class CommandsHandler
{

public:
    int totalCommands;

    CommandsHandler()
    {

        int x;
        totalCommands = 15;
        cout << "\n\n************--*--********** \n";
        cout << "Welcome to RMAM" << endl;
        cout << "\n************--*--********** \n";
        showInstructions();

        do
        {
            x = takeInput();
            if (x <= totalCommands)
            {
                executeCammand(x);
            }
            else if (x != 0)
            {
                // clear console
                system("clear");
                // show instructions
                showInstructions();
                cout << "Error: "
                     << "command not found"
                     << "(" << x << ")\n\n";
            }
        } while (x != 0);
    }

    void showInstructions()
    {
        cout << "Please follow instructions to execute appropriate action:\n\n";
        cout << "1 : CPU Stats\n";
        cout << "2 : Get all processes\n";
        cout << "3 : Kill any process\n";
        cout << "4 : RAM Stats\n";
        cout << "5 : Disk Stats\n";
        cout << "6 : Hardware Stats\n";
        cout << "7 : About Mac Stats\n";
        cout << "8 : About Physical memory\n";
        cout << "0 : to exit enter\n";
        cout << "************--*--********** \n\n";
    }

    int takeInput()
    {
        int input;
        cout << "Please enter command \n";
        cin >> input;
        return input;
    }

    // string* getCommandsList() {
    //     return {"",""};
    // }

    void executeCammand(int input)
    {
        if (input == 1)
        {
            CPU cpu;
            cpu.read();
        }
        else if (input == 2)
        {
            Processes process;
            process.getAllProcess();
        }
        else if (input == 3)
        {
            int pid;
            cout << "Please enter process Id \n";
            cin >> pid;
            Processes process;
            process.killProcess(pid);
        }
        else if (input == 4)
        {
            RAM ram;
            ram.read();
        }
        else if (input == 5)
        {
            DiskStats disk;
            // disk.read();
            // disk.read3();
            disk.read2();
            // disk.partitions();
            // disk.GetSerial("/");
            disk.allDrives();
        }
        else if (input == 6)
        {
            Hardware hw;
            hw.read();
            hw.aboutMac();
        }
        else if (input == 7)
        {
            AboutMac aboutMac;
            aboutMac.read();
        }
        else if (input == 8)
        {
            SysCommands commands;
            commands.executeSysctlCommands();
        }
        else
        {
            cout << "Not implemented yet \n";
        }
    }
};

int main()
{
    CommandsHandler commands; // Create an object (this will call the constructor)

    return 0;
}
