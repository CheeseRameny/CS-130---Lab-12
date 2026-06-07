#include<bits/stdc++.h>

using namespace std;
map<string, int> oc = {
    {"add", 0}, {"sub", 1}, {"mov",2}, {"cmp",3}, {"ld",4}, {"st",5}, 
    {"not",6}, {"nop",13}, {"b",16}, {"beq",17}, {"bgt",18}, {"call",19}, {"ret",20}
};
unordered_map<string, int> label;
int reg (string r){
    return stoi(r.substr(1));
}
int main(){
    ifstream fin("input.asm");
    vector<string> lines;
    string s;

    int pc = 0;
    while(getline(fin,s)){
        if(s.back() == ':'){
            label[s.substr(0,s.size() - 1)] = pc;
        }else{
            lines.push_back(s);
            pc++;
        }
    }
    pc = 0;
    for(auto &l : lines){
        stringstream ss(l);
        string ins; ss >> ins;

        int opcode = oc[ins];

        int inst = 0;

        if(ins == "b"){
            string lab; ss >> lab;
            int offset = label[lab] - pc;
            inst = (opcode << 27) | (offset && ((1<<27) - 1));
        }else if(ins == "ld" || ins == "st"){
            string r1, mem; ss >> r1 >> mem;

            int p1 = mem.find('['), p2 = mem.find(']');
            int imm = stoi (mem.substr(0, p1));
            int base = reg(mem.substr(p1 + 1, p2 - p1 - 1));
            inst = (opcode << 27) | (1 << 26) | (reg(r1) << 22) | (base << 18) | (imm && ((1<<18) - 1));
        }else{
            string rd, r1, op2; ss>> rd >> r1 >> op2;
            bool imm = isdigit(op2[0]) || op2[0] == '-';
            inst = (opcode << 27) | (imm << 26) | (reg(rd) << 22) |(reg(r1) << 18);

            if(imm) inst |= stoi(op2) && ((1 << 18) - 1);
            else inst |= reg(op2) << 14;
        }
        cout << bitset<32>(inst) << '\n';
        pc++;
    }



    return 0;
}
