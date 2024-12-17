#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <regex>
#include <string.h>
#include <cmath>

using namespace std;

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

struct input_t {
    FILE *fp;
    char *_line = NULL;
    string line = "";
    size_t len = 0;
    ssize_t read;

    bool next() {
        if (fp == NULL) return false;
        read = getline(&_line, &len, fp);
        line = _line;
        return read != -1;
    }

    void reset() {
        fseek(fp, 0, SEEK_SET);
    }

    input_t() {
        fp = fopen("in.txt", "r");
    }

    ~input_t() {
        if(fp)
            fclose(fp);
        if (_line)
            free(_line);
    }
};

bool run_instruction(int opcode, int64_t in, int &instr_pointer, int64_t &A, int64_t &B, int64_t &C, vector<int> &out) {
    auto get_combo = [&]() -> int64_t {
        switch(in) {
            case 4: return A;
            case 5: return B;
            case 6: return C;
            default: return in;
        }
    };

    switch(opcode) {
        case 0: { //adv
            A = (int) (A/pow(2, get_combo()));
        } break;
        case 1: { //bxl
            B ^= in;
        } break;
        case 2: { //bst
            B = get_combo() % 8;
        } break;
        case 3: { //jnz
            if (A == 0) break;
            instr_pointer = in - 2;
        } break;
        case 4: { //bxc
            B ^= C;
        } break;
        case 5: { //out
            out.push_back(get_combo()%8);
        } break;
        case 6: { //bdv
            B = (int) (A/pow(2, get_combo()));
        } break;
        case 7: { //cdv
            C = (int) (A/pow(2, get_combo()));
        } break;
    }
    instr_pointer += 2;
    return false;
}

void run_program(int instr_pointer, int64_t &A, int64_t &B, int64_t &C, vector<int> &program, vector<int> &out) {
    if(instr_pointer >= program.size()) return; // halt
    int opcode = program[instr_pointer];
    int64_t in = program[instr_pointer+1];
    run_instruction(opcode, in, instr_pointer, A, B, C, out);
    run_program(instr_pointer, A, B, C, program, out);
}

// checked what the program does step by step
// see it always does ra%8
// see it always does ra/8 later
// ra is only registry we have to keep

// hardcoded the program into solve
bool solve(int instr_pointer, vector<int> &program, int64_t res) {
    if(instr_pointer < 0) {
        cout << res << endl;
        return true;
    }
    for(const auto x : {0, 1, 2, 3, 4, 5, 6, 7}) {
        int64_t ra = res << 3 | x;
        int64_t rb = 0;
        int64_t rc = 0;

        ra = ra/8; // example program
        int out = ra%8;
        // rb = ra % 8; // puzzle input
        // rb ^= 1;
        // rc = (int64_t) ra/pow(2, rb);
        // ra = (int64_t) ra/pow(2, 3);
        // rb ^= 4;
        // rb ^= rc;
        // int out = rb%8;
        if (out == program[instr_pointer] && solve(instr_pointer-1, program, res << 3 | x)) {
            return true;
        }
    }
    return false;
}

int main() {
    auto input = input_t();
    int64_t A = 117440;
    int64_t B = 0;
    int64_t C = 0;
    vector<int> program;
    vector<int> out;
    while (input.next()) {
        rtrim(input.line);
        if (input.line.length() == 0) {
            input.next();
            rtrim(input.line);
            for(const auto c : input.line.substr(9, input.line.length() - 9))
                if(c != ',') { string s(1, c); program.push_back(stoi(s)); }
        }
    }

    // p1
    run_program(0, A, B, C, program, out);
    cout << "A:" << A << endl;
    cout << "B:" << B << endl;
    cout << "C:" << C << endl;
    for(auto o: out) {
        cout << o << ",";
    }
    cout << endl << endl;

    solve(program.size()-1, program, 0);
}