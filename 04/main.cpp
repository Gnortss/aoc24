#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <array>

using namespace std;

struct input_t {
    FILE *fp;
    char *_line = NULL;
    string line = "";
    size_t len = 0;
    ssize_t read;

    int num_lines = 0;

    bool next() {
        if (fp == NULL) return false;
        read = getline(&_line, &len, fp);
        line = _line;
        return read != -1;
    }

    void reset() {
        fseek(fp, 0, SEEK_SET);
    }

    void get_num_lines() {
        reset();
        num_lines = 0;
        while(next()) {
            num_lines+=1;
        }
        reset();
    }

    input_t() {
        fp = fopen("in.txt", "r");
        get_num_lines();
    }

    ~input_t() {
        if(fp)
            fclose(fp);
        if (_line)
            free(_line);
    }
};

int main() {
    auto input = input_t();
    int count1 = 0;
    int count2 = 0;
    char M[input.num_lines][input.line.size()];
    char M_T[input.line.size()][input.num_lines];
    const int NUM_LINES = input.num_lines;
    const int SIZE = input.line.size();
    for(int i = 0; input.next(); i++) {
        for(int j = 0; j < SIZE; j++) { // skip \n
            M[i][j] = input.line[j];
            M_T[j][i] = input.line[j];
        }
    }
    // P2
    for(int i = 0; i < NUM_LINES - 2; i++) { // FIRST DIAGONAL
        for(int j = 0; j < SIZE - 2; j++) {
            string sw = "";
            sw += M[i][j]; sw += M[i+1][j+1]; sw += M[i+2][j+2];
            if (sw.compare("MAS") == 0 || sw.compare("SAM") == 0) {
                string sw1 = "";
                sw1 += M[i][j+2]; sw1 += M[i+1][j+1]; sw1 += M[i+2][j];
                if (sw1.compare("MAS") == 0 || sw1.compare("SAM") == 0) {
                    count2++;
                }
            }
        }
    }

    // P1
    for(int i = 0; i < NUM_LINES; i++) { // HORIZONTAL && VERTICAL
        for(int j = 0; j < SIZE - 3; j++) {
            string sw = "";
            string sw1 = "";
            sw += M[i][j]; sw += M[i][j+1]; sw += M[i][j+2]; sw += M[i][j+3];
            sw1 += M_T[i][j]; sw1 += M_T[i][j+1]; sw1 += M_T[i][j+2]; sw1 += M_T[i][j+3];
            if (sw.compare("XMAS") == 0 || sw.compare("SAMX") == 0) {
                count1++;
            }
            if (sw1.compare("XMAS") == 0 || sw1.compare("SAMX") == 0) {
                count1++;
            }
        }
    }
    for(int i = 0; i < NUM_LINES - 3; i++) { // FIRST DIAGONAL
        for(int j = 0; j < SIZE - 3; j++) {
            string sw = "";
            sw += M[i][j]; sw += M[i+1][j+1]; sw += M[i+2][j+2]; sw += M[i+3][j+3];
            if (sw.compare("XMAS") == 0 || sw.compare("SAMX") == 0) {
                count1++;
            }
        }
    }
    for(int i = 0; i < NUM_LINES - 3; i++) { // SECOND DIAGONAL
        for(int j = 3; j < SIZE; j++) {
            string sw = "";
            sw += M[i][j]; sw += M[i+1][j-1]; sw += M[i+2][j-2]; sw += M[i+3][j-3];
            if (sw.compare("XMAS") == 0 || sw.compare("SAMX") == 0) {
                count1++;
            }
        }
    }
    cout << count1 << endl;
    cout << count2 << endl;
}