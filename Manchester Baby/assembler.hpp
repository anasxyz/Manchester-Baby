/**
 * TEAM 6:
 * 
 * Anas Saad (2510059)
 * Ahmid Omarzada (2527809)
 * Sayed Hashemi (2508673)
 * Ahmed Youssef (2507690)
 * Usama Bin Fakhar (2530869)
*/

#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <fstream>
#include <functional>

using namespace std;

// struct containing info about instruction lines
struct instructionLine {
    string label; // instruction's label ex: START, DoCalc
    string opcode; // instruction's opcode ex: LDN, STO, SUB
    string operand; // instructions's operand
};

class Assembler {
public:
    void run(); // runs assembler

private:
    unordered_map<string, int> symbolTable; // symbol table using unordered map
    vector<string> assemblyCode; // vector that will hold assembly code being read from file
    vector<string> machineCode; // vector that will hold machine code to be written to file
    vector<instructionLine> instructionList; // list that holds all instruction lines

    bool doesFileExist(const string& filename) const;

    string removeSpaces(string& line); // remove spaces from a line
    string removeComments(string& line);
    bool isComment(const string& line); // checks if a line is a comment
    void filterCode(vector<string>& assemblyCode); // removes all comments from the code
    string dtob(int decimalNumber); // converts decimal to binary
    string getOpcode(string opcode); // returns binary from opcode mnemonic
    vector<string> readFromFile(const string& filename); // reads assembly from file
    void writeToFile(const string& filename, vector<string>& lines); // writes machine code to file
    void parseAssembly(); // 1st pass - extracts instruction info
    void writeMachineCode(); // 2nd pass - writes and stores machine code
    void displayOutputCodeBuffer(); // displays output code buffer
    void displaySymbolTable(); // displays symbol table keys and values
    void displayAssemblyCode(vector<string>& assemblyCode);
};

#endif