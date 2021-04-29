
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

using namespace std;

class CommandsHandler {
    
    public:
    int totalCommands;

CommandsHandler(){

    int x;
    totalCommands = 7;
    cout << "\n\n************--*--********** \n";
    cout << "Welcome to RMAM" << endl;
    cout << "\n************--*--********** \n";
    showInstructions();

do {
    x = takeInput();
    if(x <= totalCommands) {
        executeCammand(x);
    }
    else if (x != 0){
        // clear console
        system ("clear");
       // show instructions
        showInstructions();
      cout << "Error: " << "command not found" << "(" << x << ")\n\n";

    }
}while(x != 0);

}


void showInstructions() {
    cout << "Please follow instructions to execute appropriate action:\n\n";
    cout << "1 : for CPU Stats\n";
    cout << "2 : Get all processes\n";
    cout << "3 : Kill any process\n";
    cout << "4 : for RAM Stats\n";
    cout << "5 : Disk Stats\n";
    cout << "6 : for GPU Stats\n";
    cout << "0 : to exit enter\n";
    cout << "************--*--********** \n\n";
}

int takeInput() {
    int input;
    cout << "Please enter command \n";
    cin >> input;
    return input;
}

// string* getCommandsList() {
//     return {"",""};
// }

void executeCammand(int input) {
    if(input == 1) {
        CPU cpu;
        cpu.read();
    }else if (input == 2) {
        Processes process;
        process.getAllProcess();
    }
    else if (input == 3) {
         int pid;
        cout << "Please enter process Id \n";
        cin >> pid;
        Processes process;
        process.killProcess(pid);
    }else if (input == 4) {
        RAM ram;
        ram.read();
    }
    else if (input == 5) {
        DiskStats disk;
        disk.read3();
        disk.read2();

    }
    else {
        cout << "Not implemented yet \n";
    }
}


};


int main() {
  CommandsHandler commands;    // Create an object (this will call the constructor)

  return 0;
}
