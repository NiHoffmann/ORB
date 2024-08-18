#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
extern "C"{
    #include "python-vm.h"
}

//TODO this is supposed to read the program length and program from flash
uint8_t* loadFile(){
    char name[] = "program.bin";
    int name_length = sizeof(name) - 1;

    std::ifstream file("Program/" + std::string(name, name_length), std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return new uint8_t[0];
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Allocate memory for the byte array
    uint8_t* byteArray = new uint8_t[fileSize];
     // Read the file into the byte array
    file.read(reinterpret_cast<char*>(byteArray), fileSize);

    // Close the file
    file.close();

    return byteArray;
}

void startProgramThread(){
    run(loadFile);
}

int main() {
    std::thread programThread(startProgramThread);

    //forceStopProgram();

    if (programThread.joinable()) {
        programThread.join();
    }

    return 0;
}
