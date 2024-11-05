//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <thread>
#include <chrono>
#include <cstring>
#include <string>
#include <direct.h>

extern "C"{
    #include "python-vm.h"
}
char name[] = "program\\program.bin";

uint8_t* loadFile(int length) {
    std::ifstream file(name, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << name << std::endl;
        return nullptr;
    }

    file.ignore(5);
    std::vector<uint8_t> byteArray(length);

    file.read(reinterpret_cast<char*>(byteArray.data()), length);

    if (!file) {
        std::cerr << "Error reading file." << std::endl;
        return nullptr;
    }

    file.close();

    uint8_t* result = (uint8_t*)malloc(byteArray.size() * sizeof(uint8_t));
    std::copy(byteArray.begin(), byteArray.end(), result);

    return result;
}

uint32_t loadFileLength() {
    std::ifstream file(name, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << name << std::endl;
        return 0;
    }

    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> byteArray(fileSize);

    if (!file.read(reinterpret_cast<char*>(byteArray.data()), fileSize)) {
        std::cerr << "Error reading file." << std::endl;
        return 0;
    }

    uint32_t length = (byteArray[1] << 24) | (byteArray[2] << 16) | (byteArray[3] << 8) | byteArray[4];

    file.close();
    return length;
}

PythonVM vm;
void startProgramThread(){
    vm.run(loadFileLength, loadFile, 1);
}

void executionFromIDE(){
        std::thread programThread(startProgramThread);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //vm.stopProgram();
        while(vm.isRunning()){}
        if (programThread.joinable()) {
            programThread.join();
        }
        printf("Exit: %d\n", vm.getExitStatus());
        printf("Exit Type: %s", vm.getExitInfo());
        /*
        printf("\nrunning: %d\n", vm.isRunning());
        std::this_thread::sleep_for(std::chrono::seconds(2));
        printf("running: %d\n", vm.isRunning());
        */
}

void executionNoThread(){
     vm.run(loadFileLength, loadFile, 0);
     printf("Exit: %d\n", vm.getExitStatus());
     printf("Exit Type: %s", vm.getExitInfo());
}

void executionWithThreadAndInterrupt(int wait){
    std::thread programThread(startProgramThread);
    std::this_thread::sleep_for(std::chrono::seconds(wait));
    vm.stopProgram();
    while(vm.isRunning()){}
    if (programThread.joinable()) {
        programThread.join();
    }
    printf("Exit: %d\n", vm.getExitStatus());
    printf("Exit Type: %s", vm.getExitInfo());
}

int main(int argc, char* argv[]) {
    if(argc > 1){
         std::string argument = argv[1];
         strcpy(name, argv[1]);
    }

    switch(argc){
        case 1:
            executionFromIDE();
            break;
        case 2:
            executionNoThread();
            break;
        case 3:
            int parameter = atoi(argv[2]);
            if(parameter){
                executionWithThreadAndInterrupt(parameter);
            }else{
                executionNoThread();
            }
            break;
    }

    return 0;
}
