/**
 * TEAM 6:
 * 
 * Anas Saad (2510059)
 * Ahmid Omarzada (2527809)
 * Sayed Hashemi (2508673)
 * Ahmed Youssef (2507690)
 * Usama Bin Fakhar (2530869)
*/

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <iostream>
#include <vector>
#include <bitset>
#include <fstream>

using namespace std;

class BabySimulator {
private:
    vector<string> store; // store/memory
    int CI; // control instruction
    string PI; // present instruction
    string A; // accumulator

    string opcode; // current opcode
    string operand; // current operand (binary memory location)
    int operandIndex;

    bool running; // whether simulator is running or not

    int btod(const string& binaryString); // converts binary to decimal
    string dtob(int decimalNumber); // converts decimal to binary
    vector<string> loadFile(const string& filename); // load machine code from file to store
    bool doesFileExist(const string& filename) const; // checks if file exists

public:
    BabySimulator(); // constructor to reset values

    vector<string> getStore() const; // returns store/memory

    string extractBits(const string& source, int start, int length); // extracts desired bits

    bool isValidMachineCode(const vector<string>& machineCode); // validate the loaded machine code
    bool isValidMachineCodeLine(const string& line) const; // validates the format of machine code lines
    bool isValidOpcode(const string& opcode); // validates opcode
    bool isValidOperand(const string& operand); // validates operand
    bool isValidMemoryAddress(int address); // validates memory address

    void displayStore(); // displays store
    void displayCI(); // displays control instruction in binary
    void displayPI(); // displays present instruction in binary
    void displayAccumulator(); // displays accumulator in binary
    void displayAll(); // displays store and registers in binary
    
    void incrementCI(); // increments control instruction by 1
    void fetch(); // fetches instruction and extracts opcode and operand
    void decodeAndExecute(); // decodes opcode and executes operation on operand
    void run(); // runs baby
};

#endif
