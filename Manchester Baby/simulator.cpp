/**
 * TEAM 6:
 * 
 * Anas Saad (2510059)
 * Ahmid Omarzada (2527809)
 * Sayed Hashemi (2508673)
 * Ahmed Youssef (2507690)
 * Usama Bin Fakhar (2530869)
*/

#include "simulator.hpp"

using namespace std;

// converts binary to decimal
int BabySimulator::btod(const string& binaryString) {
    bitset<32> normalOrder(binaryString);
    bitset<32> reversedBinary;

    int binLen = binaryString.length();

    for (int i = 0; i < binLen; ++i) {
        reversedBinary[i] = normalOrder[(binLen - 1) - i];
    }

    return static_cast<int>(reversedBinary.to_ulong());
}

// converts decimal to binary
string BabySimulator::dtob(int decimalNumber) {
    bitset<32> normalOrder(decimalNumber);
    bitset<32> reversedBinary;

    for (int i = 0; i < 32; ++i) {
        reversedBinary[i] = normalOrder[31 - i];
    }

    return reversedBinary.to_string();
}

// reads from file
vector<string> BabySimulator::loadFile(const string& filename) {
    vector<string> lines;
    ifstream file(filename);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (!isValidMachineCodeLine(line)) {
                cout << "Invalid machine code line format: " << line << endl;
                file.close();
                exit(EXIT_FAILURE);
            }
            lines.push_back(line);
        }
        file.close();
    } else {
        cout << "Unable to open file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    return lines;
}

bool BabySimulator::doesFileExist(const string& filename) const {
    std::ifstream file(filename);
    return file.good();
}

// constructor
BabySimulator::BabySimulator() {
    // reset everything to 0 when object is created
    CI = 0;
    PI = "00000000000000000000000000000000"; 
    A = "00000000000000000000000000000000";

    string fileName;
    cout << "Enter machine code file name: ";
    cin >> fileName;

    // validate if the file exists
    if (!doesFileExist(fileName)) {
        cout << "File does not exist: " << fileName << ". Exiting program." << endl;
        exit(EXIT_FAILURE);
    }

    store = loadFile(fileName);

    running = true; // if false, program will never run

    // fill rest of memory vector so it's 32x32 bits
    for (int i = store.size(); i < 32; i++) {
        store.push_back("00000000000000000000000000000000");
    }
}

vector<string> BabySimulator::getStore() const {
    return store;
}

string BabySimulator::extractBits(const string& source, int start, int length) {
    return source.substr(start, length);
}

// validate the loaded machine code
bool BabySimulator::isValidMachineCode(const vector<string>& machineCode) {
    for (const string& instruction : machineCode) {
        // validate instruction length
        if (instruction.length() != 32) {
            cout << "Invalid instruction length: " << instruction << endl;
            return false;
        }
    }
    return true;
}

bool BabySimulator::isValidMachineCodeLine(const string& line) const {
    return line.length() == 32 && line.find_first_not_of("01") == string::npos;
}

// validate opcode
bool BabySimulator::isValidOpcode(const string& opcode) {
    return opcode == "000" || opcode == "001" || opcode == "010" ||
           opcode == "011" || opcode == "100" || opcode == "101" ||
           opcode == "110" || opcode == "111";
}

// validate operand
bool BabySimulator::isValidOperand(const string& operand) {
    // validation for operand format
    return operand.length() == 32 && operand.find_first_not_of("01") == string::npos;
}

bool BabySimulator::isValidMemoryAddress(int address) {
    return address >= 0 && address < static_cast<int>(store.size());
}

void BabySimulator::displayStore() {
    cout << "Store: \n" << endl; 

    for (int i = 0; i < store.size(); i++) {
        cout << store[i] << " " << endl;
    }
    cout << endl;
}

void BabySimulator::displayCI() {
    cout << "CI: " << dtob(CI) << "\n" << endl;
}

void BabySimulator::displayPI() {
    cout << "PI: " << PI << "\n" << endl;
}

void BabySimulator::displayAccumulator() {
    cout << "A: " << A << "\n" << endl; 
}

void BabySimulator::displayAll() {
    cout << "\n-----------------------------------------\n" << endl;

    displayCI();
    displayPI();
    displayAccumulator();

    displayStore();
}

void BabySimulator::incrementCI() {
    CI += 1;
}

/*
    11100000000000100000000000000000
    ^^^^^        ^^^
    operand      opcode
    bits 1->5    bits 14-15-16

    absolute addressing took a while to understand, but finally
    from what i understand, the operand itself is not embedded 
    within the instruction, but the specified bits for the "operand"
    are the memory/store address where the actual operand can be found

    that explains why the program halts at memory address 6, but there
    are still more lines of memory
*/
void BabySimulator::fetch() {
    if (isValidMemoryAddress(CI)) {
        PI = store[CI];
        opcode = extractBits(PI, 13, 3);
        int operandIndex = btod(extractBits(PI, 0, 5));

        if (isValidMemoryAddress(operandIndex)) {
            operand = store[operandIndex];
        } else {
            cout << "Invalid operand memory address at CI " << CI << endl;
            exit(EXIT_FAILURE);
        }

        // Validate opcode and operand
        if (!isValidOpcode(opcode) || !isValidOperand(operand)) {
            cout << "Invalid opcode or operand at CI " << CI << ": " << opcode << " " << operand << endl;
            exit(EXIT_FAILURE);
        }
    } else {
        cout << "Invalid memory address at CI " << CI << endl;
        exit(EXIT_FAILURE);
    }
}

void BabySimulator::decodeAndExecute() {
    int operandIndex = btod(extractBits(PI, 0, 5));

    if (isValidMemoryAddress(operandIndex)) {
        if (isValidOpcode(opcode) && isValidOperand(operand)) {
            if (opcode == "000") {  // JMP
                CI = btod(operand);
            } else if (opcode == "100") {  // JRP
                CI += btod(operand);
            } else if (opcode == "010") {  // LDN
                A = dtob(-btod(operand));
            } else if (opcode == "110") {  // STO
                store[operandIndex] = A;
            } else if (opcode == "001" || opcode == "101") {  // SUB
                A = dtob(btod(A) - btod(operand));
            } else if (opcode == "011") {  // CMP
                if (btod(A) < 0) CI++;
            } else if (opcode == "111") {  // STP
                running = false;
            }
        } else {
            cout << "Invalid opcode or operand at CI " << CI << ": " << opcode << " " << operand << endl;
            exit(EXIT_FAILURE);
        }
    } else {
        cout << "Invalid memory address at CI " << CI << endl;
        exit(EXIT_FAILURE);
    }
}

void BabySimulator::run() {
    while (running == true) {
        incrementCI();
        fetch();
        decodeAndExecute();

        displayAll();

        if (opcode == "111") {
            cout << "Program Halted." << endl;
            break;
        }

        cout << "Press Enter to continue to the next step...";
        cin.get();
    }
}

int main() {
    BabySimulator baby;

    // validate machine code before running
    if (!baby.isValidMachineCode(baby.getStore())) {
        cout << "Invalid machine code. Exiting program." << endl;
        return EXIT_FAILURE;
    }

    baby.run();
    
    return 0;
}
