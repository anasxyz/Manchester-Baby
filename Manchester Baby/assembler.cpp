/**
 * TEAM 6:
 * 
 * Anas Saad (2510059)
 * Ahmid Omarzada (2527809)
 * Sayed Hashemi (2508673)
 * Ahmed Youssef (2507690)
 * Usama Bin Fakhar (2530869)
*/

#include "assembler.hpp"

// removes all spaces from a line
string Assembler::removeSpaces(string& line) {
    line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
    return line;
}

string Assembler::removeComments(string& line) {
        int commentPos = line.find(';');
        if (commentPos != string::npos) {
            line.erase(commentPos);
        }
        return line;
    }

// checks if a line is a comment by first character
bool Assembler::isComment(const string& line) {
    return !line.empty() && line[0] == ';';
}

// removes all comments from the assembly code
void Assembler::filterCode(vector<string>& assemblyCode) {
    // lambda function checks if a trimmed line is a comment or empty
    assemblyCode.erase(remove_if(assemblyCode.begin(), assemblyCode.end(), [this](string& line) {
        // line = removeSpaces(line);
        removeComments(line);
        return isComment(line) || line.empty();
    }), assemblyCode.end());
}

// converts decimal to binary
string Assembler::dtob(int decimalNumber) {
    bitset<32> normalOrder(decimalNumber);
    bitset<32> reversedBinary;

    for (int i = 0; i < 32; ++i) {
        reversedBinary[i] = normalOrder[31 - i];
    }

    return reversedBinary.to_string();
}

// determines binary code from opcode mnemonic
string Assembler::getOpcode(string opcode) {
    string binOpcode;

    if (opcode == "JMP") {
        binOpcode = "000";
    } else if (opcode == "JRP") {
        binOpcode = "100";
    } else if (opcode == "LDN") {
        binOpcode = "010";
    } else if (opcode == "STO") {
        binOpcode = "110";
    } else if (opcode == "SUB") {
        binOpcode = "001";
    } else if (opcode == "SUB") {
        binOpcode = "101";
    } else if (opcode == "CMP") {
        binOpcode = "011";
    } else if (opcode == "STP") {
        binOpcode = "111";
    } else if (opcode == "VAR") {
        binOpcode = "VAR"; // i know that VAR is not an opcode but it's easier and faster this way to set it to VAR for later
    }

    return binOpcode;
}

bool Assembler::doesFileExist(const string& filename) const {
    std::ifstream file(filename);
    return file.good();
}

// reads line by line from file and returns vector
vector<string> Assembler::readFromFile(const string& filename) {
    vector<string> lines;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        return lines; // Return empty vector indicating failure to open the file
    }

    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    return lines;
}

// writes line by line to a file
void Assembler::writeToFile(const string& filename, vector<string>& lines) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        return; // Return without writing if unable to open the file
    }

    for (auto& line : lines) {
        file << line << endl;
    }

    file.close();
}

/*
    assembler's 1st pass; filling in output code buffer / extracting labels, opcodes,
    operands, and variables
*/ 
void Assembler::parseAssembly() {
    vector<string> copyCode = assemblyCode;

    for (int i = 0; i < copyCode.size(); i++) {
        instructionLine line;
        string currentLine = removeSpaces(copyCode[i]);

        int colonPos = currentLine.find(':');
        // if colon is found 
        if (colonPos != string::npos) {
            line.label = currentLine.substr(0, colonPos);
            symbolTable[line.label] = i;

            currentLine = currentLine.substr(colonPos + 1); // move past the colon
        }

        // line.opcode = currentLine.substr(0, 3); 
        line.opcode = currentLine.substr(0, 3);
        currentLine.erase(0, 3); // move past the opcode

        int commentPos = currentLine.find(';');
        // if semicolon is found (i.e. if there's a comment)
        if (commentPos != string::npos) {
                line.operand = currentLine.substr(0, commentPos);
        } else {
                line.operand = currentLine;
        }

        // trim any leading or trailing whitespace from operand
        int firstNotSpace = line.operand.find_first_not_of(" \t");
        int lastNotSpace = line.operand.find_last_not_of(" \t");
        if (firstNotSpace != string::npos && lastNotSpace != string::npos) {
                line.operand = line.operand.substr(firstNotSpace, lastNotSpace - firstNotSpace + 1);
        }

        instructionList.push_back(line); // add the instruction line to the list of all instructions
    }
}

/*
    assembler's 2nd pass; gets operand addresses from symbol table using labels from
    the instruction list's instruction lines
*/
void Assembler::writeMachineCode() {
    cout << "\n============== Machine Code ================\n" << endl;

    string machineCodeLine;
    
    for (int i = 0; i < instructionList.size(); i++) {
        // if opcode is VAR then the whole line of memory is an operand, otherwise it's an instruction
        if (getOpcode(instructionList[i].opcode) != "VAR") { 
            machineCodeLine.append(dtob(symbolTable[instructionList[i].operand])); // appends operand address of label from symbol table
            machineCodeLine.replace(13, 3, getOpcode(instructionList[i].opcode)); // replaces digits 13, 14, and 15 with opcode using getOpcode function
        } else {
            machineCodeLine.append(dtob(stoi(instructionList[i].operand))); // set machine code line to binary of operand
        }
        
        cout << machineCodeLine << endl;

        machineCode.push_back(machineCodeLine);

        machineCodeLine.clear(); // clear string before new loop
    }

    cout << "\n============================================" << endl;
}

// displays output code buffer
void Assembler::displayOutputCodeBuffer() {
    for (int i = 0; i < assemblyCode.size(); i++) {
        cout << "=============== Line: " << i << " ====================" << endl;
        cout << "Label: " << instructionList[i].label << " | Opcode: " << instructionList[i].opcode << " | Operand: " << instructionList[i].operand <<  endl;
        cout << "============================================" << endl;
    }
}

// displays symbol table
void Assembler::displaySymbolTable() {
    cout << "\n=============== Symbol Table ===============" << endl;
    for (auto& e : symbolTable) {
        cout << e.first << ": " << e.second << endl;
    }
    cout << "============================================" << endl;
}

void Assembler::displayAssemblyCode(vector<string>& assemblyCode) {
    cout << "\n=============== Assembly Code ==============" << endl;
    for (int i = 0; i < assemblyCode.size(); i++) {
        cout << assemblyCode[i] << endl;
    }
    cout << "============================================\n" << endl;
}

// runs assembler
void Assembler::run() {
    string fileName;
    cout << "Enter assembly file name: ";
    cin >> fileName;

    // validate if the file exists
    if (!doesFileExist(fileName)) {
        cout << "File does not exist: " << fileName << ". Exiting program." << endl;
        exit(EXIT_FAILURE);
    }

    assemblyCode = readFromFile(fileName);

    cout << "\nAssembler: " << endl;
    filterCode(assemblyCode);
    parseAssembly();
    displayAssemblyCode(assemblyCode);
    displayOutputCodeBuffer();
    displaySymbolTable();

    writeMachineCode();

    writeToFile("output.txt", machineCode);
    cout << "\nMachine code successfully written to output.txt" << endl;
}

int main() {
    Assembler assembler;
    
    assembler.run();

    return 0;
}
