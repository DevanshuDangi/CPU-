#include<bits/stdc++.h>
using namespace std;

// vector<string> MACHINE_CODE;

static map<string,string> FUNC3={
    //R TYPE INSTRUCTION
    {"add","000"},
    {"sub","000"},
    {"sll","001"},
    {"slt","010"},
    {"sltu","011"},
    {"xor","100"},
    {"srl","101"},
    {"sra","101"},
    {"or","110"},
    {"and","111"},
    {"mul","000"},
    
    //I-TYPE INSTRUCTION
    {"addi","000"},
    {"slti","010"},
    {"sltiu","011"},
    {"xori","100"},
    {"ori","110"},
    {"andi","111"},
    {"slli","001"},
    {"srli","101"},
    {"srai","101"},
    
    // S-Type Instruction
    {"sb","000"},
    {"sh","001"},
    {"sw","010"},
    
    //B-TYPE INSTRUCTION
    {"beq","000"},
    {"bne","001"},
    {"blt","100"},
    {"bge","101"},
    {"bltu","110"},
    {"bgeu","111"},
    
    //U-TYPE INSTRUCTINO
    {"lui","0110111"},
    {"auipc","0010111"},
    
    //J-TYPE INSTRUCTION
    {"jal","1101111"},
    {"jalr","000"},
    
    //I-TYPEINSTRUCTIon
    {"lb","000"},
    {"lh","001"},
    {"lw","010"},
    {"lbu","100"},
    {"lhu","101"},
    //S type
    {"sd","011"},
    //CSR INSTRUCTION
    {"csrrw","001"},
    {"csrrs","010"},
    {"csrrc","011"},
    {"csrrwi","101"},
    {"csrrsi","110"},
    {"csrrci","111"},
    
    //Additionals
    {"nop","000"},
    {"ecall","000"},
    {"ebreak","000"},
    //
};


static map<string,string> FUNC7={
    //R TYPE INSTRUCTION
    {"add","0000000"},
    {"sub","0100000"},
    {"sll","0000000"},
    {"slt","0000000"},
    {"sltu","0000000"},
    {"xor","0000000"},
    {"srl","0000000"},
    {"sra","0100000"},
    {"or","0000000"},
    {"and","00000000"},
    {"mul", "0000001"}
    
    
};


static map<string,string> IMM7={
    //logical shift instruction
    {"slli","0000000"},
    {"srli","0000000"},
    {"srai","0100000"},
    
    // logical instructions
    {"ansi","0000000"},
    {"ori","0000000"},
    {"xori","0000000"},
    
};


static map<string,string> Type={
    //R TYPE INSTRUCTION
    {"add","R"},
    {"sub","R"},
    {"sll","R"},
    {"slt","R"},
    {"sltu","R"},
    {"xor","R"},
    {"srl","R"},
    {"sra","R"},
    {"or","R"},
    {"and","R"},
    {"mul", "R"},
    
    //I-TYPE INSTRUCTION-arithmetic
    {"addi","ImmArith"},
    {"slti","ImmArith"},
    {"sltiu","ImmArith"},
    {"xori","ImmArith"},
    {"ori","ImmArith"},
    {"andi","ImmArith"},
    
    //I-TYPE INSTRUCTION-logical and shift
    {"slli","ImmLogical"},
    {"srli","ImmLogical"},
    {"srai","ImmLogical"},
    
    //I-TYPEINSTRUCTIon-load
    {"lb","ImmLoad"},
    {"lh","ImmLoad"},
    {"lw","ImmLoad"},
    {"lbu","ImmLoad"},
    {"lhu","ImmLoad"},
    
    // S-Type Instruction
    {"sb","S"},
    {"sh","S"},
    {"sw","S"},
    
    //B-TYPE INSTRUCTION
    {"beq","B"},
    {"bne","B"},
    {"blt","B"},
    {"bge","B"},
    {"bltu","B"},
    {"bgeu","B"},
    
    //U-TYPE INSTRUCTINO
    {"lui","U"},
    {"auipc","U"},
    
    //J-TYPE INSTRUCTION
    {"jal","J"},
    
    //i type instruction -jumps and links
    {"jalr","ImmLoad"},
    //Stype
    {"sd","S"},
    
    //fence instructoins
    {"fence","ImmArith"},
    {"fence.i","ImmArith"},
    
    
};


vector<string> MACHINE_CODE;

pair<string, string> seg_off_reg(const string &s)
{
    string offset, reg;
    int is_offset=1;
    for(char ch: s){
        if(ch=='(')is_offset=0;
        else if(ch==')')break;
        else if(is_offset)offset+=ch;
        else reg+=ch;
    }
    return {offset, reg};
}

class Register{
    public:
    string EncodeRegister(const string &s){return bitset<5>(stoll(s.substr(1))).to_string();}
};

class Immediate{
    public:
    string getImmediateCode(int num){return bitset<12>(num<0?(1<<12)+num:num).to_string();}
    
    string getShamtCode(int shamt){
        if(shamt<0 || shamt>=(1<<5))throw invalid_argument("shamt out of bounds");

        return bitset<5>(shamt).to_string();
    }
    
    vector<string> getBTypeImmediateCode(int num)
    {
        string imm_bin=bitset<13>((num<0?(1<<13)+num:num)).to_string();
        return {imm_bin.substr(0, 1), imm_bin.substr(1,1), imm_bin.substr(2, 6), imm_bin.substr(8, 4)};
    }
    
    
    string getJTypeImmediateCode(int num)
    {
        string imm_bin=bitset<21>((num<0?(1<<21)+num:num)).to_string();
        return imm_bin.substr(0, 1)+imm_bin.substr(10, 10)+imm_bin.substr(9, 1)+imm_bin.substr(1, 8);
    }
    
    string getUTypeImmediateCode(int num)
    {
        return bitset<20>(num).to_string();
        
    }
    
};

class Instruction
{
    public:
    Instruction(const vector<string> &v, ofstream &outFile)
    {
        auto inst_type=Type[v[0]];
        
        if(inst_type=="R")encodeRType(v, outFile);
        else if(inst_type=="ImmArith")encodeImmTypeArithemetic(v, outFile);
        else if(inst_type=="ImmLogical")encodeImmTypeLogical(v, outFile);
        else if(inst_type=="ImmLoad")encodeImmTypeLoad(v, outFile);
        else if(inst_type=="B")encodeBType(v, outFile);
        else if(inst_type=="U")encodeUType(v, outFile);
        else if(inst_type=="J")encodeJType(v, outFile);
        else if(inst_type=="S")encodeSType(v, outFile);
        else cout<<"Instruction Type not supported"<<endl;
    }
    
    private:
    Register reg;
    Immediate Imm;
    
    void encodeRType(const vector<string> &v, ofstream &outFile)
    {
        string func3=FUNC3[v[0]],func7=FUNC7[v[0]], opcode="0110011";
        cout<<func7+reg.EncodeRegister(v[3])+reg.EncodeRegister(v[2])+func3+reg.EncodeRegister(v[1])+opcode<<endl;
        MACHINE_CODE.push_back(func7+reg.EncodeRegister(v[3])+reg.EncodeRegister(v[2])+func3+reg.EncodeRegister(v[1])+opcode);
    }
    
    void encodeImmTypeArithemetic(const vector<string> &v, ofstream &outFile)
    {
        string func3=FUNC3[v[0]],opcode="0010011";
        cout<<Imm.getImmediateCode(stoi(v[3]))+reg.EncodeRegister(v[2])+func3+reg.EncodeRegister(v[1])+opcode<<endl;
        MACHINE_CODE.push_back(Imm.getImmediateCode(stoi(v[3]))+reg.EncodeRegister(v[2])+func3+reg.EncodeRegister(v[1])+opcode);
    }
    
    void encodeImmTypeLogical(const vector<string> &v, ofstream &outFile)
    {
        string opcode="0010011", func3=FUNC3[v[0]];
        cout<<IMM7[v[0]]+Imm.getShamtCode(stoi(v[3]))+reg.EncodeRegister(v[2])+func3+reg.EncodeRegister(v[1])+opcode<<endl;
        MACHINE_CODE.push_back(IMM7[v[0]]+Imm.getShamtCode(stoi(v[3]))+reg.EncodeRegister(v[2])+func3+reg.EncodeRegister(v[1])+opcode);
    }
    
    void encodeImmTypeLoad(const vector<string> &v, ofstream &outFile)
    {
        string opcode=(v[0]=="jalr")?"1100111":"0000011", func3=FUNC3[v[0]];
        // auto [offset, rs1]=seg_off_reg(v[2]);
        auto result=seg_off_reg(v[2]);
        string offset=result.first;
        string rs1=result.second;
        cout<<Imm.getImmediateCode(stoi(offset))+reg.EncodeRegister(rs1)+func3+reg.EncodeRegister(v[1])+opcode<<endl;
        MACHINE_CODE.push_back(Imm.getImmediateCode(stoi(offset))+reg.EncodeRegister(rs1)+func3+reg.EncodeRegister(v[1])+opcode);
    }
    
    
    void encodeSType(const vector<string> &v, ofstream &outFile)
    {
        string opcode="0100011", func3=FUNC3[v[0]];
        // auto [offset, rs1]=seg_off_reg(v[2]);
        auto result=seg_off_reg(v[2]);
        string offset=result.first;
        string rs1=result.second;
        string imm=Imm.getImmediateCode(stoi(offset));
        cout<<imm.substr(0, 7)+reg.EncodeRegister(v[1])+reg.EncodeRegister(rs1)+func3+imm.substr(7)+opcode<<endl;
        MACHINE_CODE.push_back(imm.substr(0, 7)+reg.EncodeRegister(v[1])+reg.EncodeRegister(rs1)+func3+imm.substr(7)+opcode);
    }
    
    void encodeBType(const vector<string> &v, ofstream &outFile)
    {
        string opcode="1100011", func3=FUNC3[v[0]];
        vector<string> imm_T=Imm.getBTypeImmediateCode(stoi(v[3]));
        string imm12=imm_T[0], imm11=imm_T[1], imm10_5=imm_T[2], imm4_1=imm_T[3];
        cout<<imm12+imm10_5+reg.EncodeRegister(v[2])+reg.EncodeRegister(v[1])+func3+imm4_1+imm11+opcode<<endl;
        MACHINE_CODE.push_back(imm12+imm10_5+reg.EncodeRegister(v[2])+reg.EncodeRegister(v[1])+func3+imm4_1+imm11+opcode);
    }
    
    void encodeJType(const vector<string>&v, ofstream &outFile)
    {
        string opcode="1101111";
        cout<<Imm.getJTypeImmediateCode(stoi(v[2]))+reg.EncodeRegister(v[1])+opcode<<endl;
        MACHINE_CODE.push_back(Imm.getJTypeImmediateCode(stoi(v[2]))+reg.EncodeRegister(v[1])+opcode);
    }
    

    void encodeUType(const vector<string> &v, ofstream &outFile)
    {
        string opcode=(v[0]=="lui")?"0110111":(v[0]=="auipc")?"0010111":throw invalid_argument("Unsupported U-Type instruction");
        cout<<Imm.getUTypeImmediateCode(stoi(v[2]))+reg.EncodeRegister(v[1])+opcode<<endl;
        MACHINE_CODE.push_back(Imm.getUTypeImmediateCode(stoi(v[2]))+reg.EncodeRegister(v[1])+opcode);
        
    }
};


void read_from_file(const string &filename, ofstream &outFile)
{
    ifstream inputFile(filename);
    string line;
    while(getline(inputFile, line))
    {
        stringstream ss(line);
        vector<string> instruction;
        string token;
        while(ss>>token)
        {
            instruction.push_back(token);
            
        }
        Instruction(instruction, outFile);
    }
}





//CPU DESIGN--------------------------------------------------------------------------------------------------------

vector<int> GPR(32, 0);
unordered_map<int, int> DM;

map<string, string> Operation = {
    {"0000", "AND"},
    {"0001", "OR"},
    {"0010", "ADD"},
    {"0110", "SUB"},
    {"1111", "MUL"}
};

vector<int> busy(32, 0);

string get_type(const string &opcode) {
    static map<string, string> opcode_map = {
        {"0100011", "S"},
        {"1100011", "B"},
        {"0110011", "R"},
        {"0010011", "I"},
        {"0000011", "Ld"},
        {"1101111", "U"}
    };

    auto it = opcode_map.find(opcode);
    return (it != opcode_map.end()) ? it->second : "";
}

string SignedExtend(string s, int len = 32) {
    s.insert(s.begin(), len - s.length(), s[0]);
    return s;
}

int to_decimal(const string &s) {
    bitset<32> bits(s);
    return static_cast<int>(bits.to_ulong()) - bits.test(31) * 2 * (1LL << 31);
}

int PC = 0;
int TPC = 0;
int NPC = 0;

class InstructionMemory {
public:
    map<int, string> IM;

    InstructionMemory(const vector<string> &machineCode) {
        for (int i = 0; i < machineCode.size(); ++i) {
            IM[i * 4] = machineCode[i];
        }
    }

    string instruction(int x, int y, int pc) {
        if (IM.find(pc) == IM.end()) return "NOP";
        return IM[pc].substr(min(31 - x, 31 - y), max(31 - x, 31 - y) - min(31 - x, 31 - y) + 1);
    }

    string instruction(int x, int pc) {
        if (IM.find(pc) == IM.end()) return "NOP";
        return string(1, IM[pc][31 - x]);
    }
};


string ImmGen(InstructionMemory *IM, const string &type, int pc) {
    if (type == "I" || type == "Ld") return IM->instruction(31, 20, pc);
    if (type == "J") {
        return IM->instruction(31, pc) + IM->instruction(19, 12, pc) +
               IM->instruction(20, pc) + IM->instruction(21, 30, pc);
    }
    if (type == "B") {
        return IM->instruction(31, pc) + IM->instruction(7, pc) +
               IM->instruction(25, 30, pc) + IM->instruction(11, 8, pc);
    }
    if (type == "S") {
        return IM->instruction(31, 25, pc) + IM->instruction(11, 7, pc);
    }
    return IM->instruction(31, 20, pc);
}

class ControlUnit {
public:
    string opcode;
    int ALUSrc, Mem2Reg, RegWrite, MemWrite, Branch, jump, ALUOp1, ALUOp0, MemRead;
    string ALUOp;

    ControlUnit(const string &op = "", const string &func3 = "", const string &func7 = "") {
        if (!op.empty()) {
            opcode = op;
            set_control_signals(op, func3, func7);
        }
    }

private:
    void set_control_signals(const string &op, const string &func3, const string &func7) {
        string type = get_type(op);
        ALUSrc = (type == "S" || type == "I" || type == "Ld") ? 1 : 0;
        Mem2Reg = (type == "Ld") ? 1 : 0;
        RegWrite = (type == "R" || type == "I" || type == "Ld") ? 1 : 0;
        MemWrite = (type == "S") ? 1 : 0;
        Branch = (type == "B") ? 1 : 0;
        jump = (type == "U") ? 1 : 0;
        ALUOp1 = (type == "R") ? 1 : 0;
        ALUOp0 = (type == "B") ? 1 : 0;
        MemRead = !MemWrite;
        ALUOp = to_string(ALUOp1) + to_string(ALUOp0);
    }
};

 string ALUControl(const string &ALUOp, const string &func7, const string &func3) {
    if (ALUOp == "00") return "0010"; // ADD
    if (ALUOp == "01") return "0110"; // SUB
    if (func7 == "0000000" && func3 == "000") return "0010"; // ADD
    if (func7 == "0100000" && func3 == "000") return "0110"; // SUB
    if (func7 == "0000000" && func3 == "111") return "0000"; // AND
    if (func7 == "0000000" && func3 == "110") return "0001"; // OR
    if (func7 == "0000001" && func3 == "000") return "1111"; // MUL
    return "";
}

class ALU {
public:
    int ALUresult;
    int ALUZeroFlag;

    ALU(const string &ALUSelect, int rs1, int rs2) {
        string op = Operation[ALUSelect];
        if (op == "ADD") ALUresult = rs1 + rs2;
        else if (op == "AND") ALUresult = rs1 & rs2;
        else if (op == "OR") ALUresult = rs1 | rs2;
        else if (op == "SUB") ALUresult = rs1 - rs2;
        else if (op == "MUL") ALUresult = rs1 * rs2;
        ALUZeroFlag = (rs1 == rs2) ? 1 : 0;
    }
};

class IFID {
public:
    string IR;
    int DPC;
};

class IDEX {
public:
    int DPC, JPC;
    string imm;
    string func3;
    int rs1, rs2;
    string rdl;
    ControlUnit CW;
    int SD_val;
};

class EXMO {
public:
    int ALUOut;
    ControlUnit CW;
    string rdl;
    int DPC;
    int rs2;
};

class MOWB {
public:
    int LDOut;
    ControlUnit CW;
    int ALUOut;
    int DPC;
    string rdl;
};

IFID ifid;
IDEX idex;
EXMO exmo;
MOWB mowb;





class CPU {
public:
    InstructionMemory *instructionMem;  // Instruction memory pointer
    vector<int> fetchStage, decodeStage, executeStage, memoryStage, writeBackStage;

    // Constructor: initializes instruction memory
    CPU(InstructionMemory *instrMem) {
        instructionMem = instrMem;
    }

    // Resets pipeline stage visibility tracking for a given program counter
    void reset_visibility(vector<int>& fetchStage, vector<int>& decodeStage, vector<int>& executeStage, 
                          vector<int>& memoryStage, vector<int>& writeBackStage, int pc) {
        fetchStage[pc] = 0;
        decodeStage[pc] = 0;
        executeStage[pc] = 0;
        memoryStage[pc] = 0;
        writeBackStage[pc] = 0;
    }

    // Detects if a branch hazard exists for the current instruction
    bool detect_hazard() {
        return instructionMem->instruction(0, 6, PC) == "1100011";
    }

    // Fetches the instruction from memory and updates program counters
    void fetch() {
        string instruction = instructionMem->instruction(0, 31, PC);

        ifid.IR = instruction;
        ifid.DPC = PC;
        TPC = PC + 4;

        if (!detect_hazard()) {
            PC = TPC;
            reset_visibility(fetchStage, decodeStage, executeStage, memoryStage, writeBackStage, PC);
        }
    }

    // Decodes the instruction and handles data dependencies
    bool decode() {
        string rs2Loc = instructionMem->instruction(24, 20, ifid.DPC);

        if (busy[to_decimal(rs2Loc)]) {
            string opcode = instructionMem->instruction(0, 6, ifid.DPC);
            if (get_type(opcode) != "I" && get_type(opcode) != "U") return false;
        }

        string rs1Loc = instructionMem->instruction(19, 15, ifid.DPC);
        if (busy[to_decimal(rs1Loc)]) return false;

        // Handle Immediate for Jump
        string immForJump = instructionMem->instruction(31, ifid.DPC) + 
                            instructionMem->instruction(19, 12, ifid.DPC) +
                            instructionMem->instruction(20, ifid.DPC) + 
                            instructionMem->instruction(21, 30, ifid.DPC);

        string extendedJumpImm = SignedExtend(immForJump);
        idex.JPC = ifid.DPC + (to_decimal(extendedJumpImm) << 1);

        // Instruction decode
        string opcode = instructionMem->instruction(6, 0, ifid.DPC);
        idex.DPC = ifid.DPC;
        string type = get_type(opcode);
        idex.imm = ImmGen(instructionMem, type, ifid.DPC);
        idex.func3 = instructionMem->instruction(14, 12, ifid.DPC);
        string func7 = instructionMem->instruction(25, 31, ifid.DPC);
        idex.rdl = instructionMem->instruction(11, 7, ifid.DPC);
        idex.rs1 = GPR[to_decimal(rs1Loc)];

        // Control signals
        ControlUnit controlSignals(opcode, idex.func3);
        idex.CW = controlSignals;

        // Handle ALUSrc and register dependencies
        if (idex.CW.ALUSrc) {
            string extendedImm = SignedExtend(idex.imm);
            idex.rs2 = to_decimal(extendedImm);
        } else {
            idex.rs2 = GPR[to_decimal(rs2Loc)];
        }
        idex.SD_val = GPR[to_decimal(rs2Loc)];

        if (idex.CW.RegWrite) busy[to_decimal(idex.rdl)] = 1;

        return true;
    }

    // Executes the ALU operation and evaluates branch conditions
    void execute() {
        string ALUOpSelect = ALUControl(idex.CW.ALUOp, instructionMem->instruction(25, 31, idex.DPC), idex.func3);
        ALU alu(ALUOpSelect, idex.rs1, idex.rs2);
        exmo.ALUOut = alu.ALUresult;
        int ALUZeroFlag = alu.ALUZeroFlag;

        exmo.rs2 = idex.SD_val;
        exmo.CW = idex.CW;
        exmo.rdl = idex.rdl;
        exmo.DPC = idex.DPC;

        // Branch logic
        int branchPC = ((to_decimal(SignedExtend(idex.imm))) << 1) + idex.DPC;

        if (idex.CW.Branch && ALUZeroFlag) {
            NPC = branchPC;
            PC = NPC;
            reset_visibility(fetchStage, decodeStage, executeStage, memoryStage, writeBackStage, PC);
        } else {
            NPC = TPC;
            if (idex.CW.Branch) {
                PC = NPC;
                reset_visibility(fetchStage, decodeStage, executeStage, memoryStage, writeBackStage, PC);
            }
        }

        // Jump operation
        if (idex.CW.jump) {
            NPC = idex.JPC;
            PC = NPC;
            reset_visibility(fetchStage, decodeStage, executeStage, memoryStage, writeBackStage, PC);
        }
    }

    // Memory operation stage: reads or writes data memory
    void memory_operation() {
        if (exmo.CW.MemWrite) {
            DM[exmo.ALUOut] = exmo.rs2;
        } else if (exmo.CW.MemRead && DM.find(exmo.ALUOut) != DM.end()) {
            mowb.LDOut = DM[exmo.ALUOut];
        }

        mowb.DPC = exmo.DPC;
        mowb.ALUOut = exmo.ALUOut;
        mowb.CW = exmo.CW;
        mowb.rdl = exmo.rdl;
    }

    // Write-back stage: updates register file if necessary
    void write_back() {
        if (mowb.CW.RegWrite) {
            if (mowb.CW.Mem2Reg) {
                GPR[to_decimal(mowb.rdl)] = mowb.LDOut;
            } else {
                GPR[to_decimal(mowb.rdl)] = mowb.ALUOut;
            }
            busy[to_decimal(mowb.rdl)] = 0;
        }
    }

    // Runs the CPU pipeline stages for a limited number of cycles
    void run_pipeline() {
        int numInstructions = instructionMem->IM.size();
        initialize_pipeline_stages(numInstructions);

        for (int cycle = 0; cycle < 10;) {
            if (memoryStage[mowb.DPC] && !writeBackStage[mowb.DPC]) {
                writeBackStage[mowb.DPC] = 1;
                write_back();
            } else if (executeStage[exmo.DPC] && !memoryStage[exmo.DPC]) {
                memoryStage[exmo.DPC] = 1;
                memory_operation();
            } else if (decodeStage[idex.DPC] && !executeStage[idex.DPC]) {
                executeStage[idex.DPC] = 1;
                execute();
            } else if (fetchStage[ifid.DPC] && !decodeStage[ifid.DPC]) {
                if (decode()) decodeStage[ifid.DPC] = 1;
                else continue;
            } else if (!fetchStage[PC] && PC < 4 * numInstructions) {
                fetchStage[PC] = 1;
                fetch();
            } else if (writeBackStage[4 * numInstructions - 4]) break;
        }

        print_registers();
        print_data_memory();
    }

private:
    // Initializes pipeline stages for tracking
    void initialize_pipeline_stages(int numInstructions) {
        fetchStage = vector<int>(4 * numInstructions + 1, 0);
        decodeStage = vector<int>(4 * numInstructions + 1, 0);
        executeStage = vector<int>(4 * numInstructions + 1, 0);
        memoryStage = vector<int>(4 * numInstructions + 1, 0);
        writeBackStage = vector<int>(4 * numInstructions + 1, 0);
    }

    // Prints the general-purpose register values
    void print_registers() {
        cout << "GeneralPurposeRegister values:\n\n";
        cout << " Reg No : " << "values\n";
        for (int i = 0; i < 32; i++) {
            if (i <= 9) cout << " x" << i << "     : " << GPR[i] << endl;
            else cout << " x" << i << "    : " << GPR[i] << endl;
        }
    }

    // Prints the data memory
    void print_data_memory() {
        cout << "\n\nData Memory\n";
        // Add logic for displaying data memory if required
    }
};




int main()
{
    cout<<"Machine Code\n";
    ofstream outFile("machine.txt");
    read_from_file("assembly.txt", outFile);
    outFile.close();
    freopen("machine.txt", "r", stdin);
    cout<<"\n\n\n\n\nCPU Design starts here"<<endl;
    
    
    MACHINE_CODE.push_back("00000000000000000000000000010011");
    InstructionMemory IM(MACHINE_CODE);
    CPU cpu(&IM);
    cpu.run_pipeline();
    return 0;
}


//factorial 7
// addi x0, x0, 1
// addi x1, x1, 7
// addi x1, x1, 1
// addi x2, x2, 1
// beq x0, x1, 16
// mul x2, x2, x0
// addi x0, x0, 1
// beq x0, x0, -12

// //sum uptill 10
// addi x0, x0, 1
// addi x1, x1, 10
// addi x1, x1, 1
// beq x0, x1, 16
// add x2, x2, x0
// addi x0, x0, 1
// beq x0, x0, -12
// sw x2, 0(x5)
