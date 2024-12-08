#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <string.h>
#include <numeric>

using namespace std;

// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

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

bool rec_solve(vector<int> &numbers, unsigned long long expected, unsigned long long cur, int i) {
    if (i == numbers.size()) return cur == expected;
    // p1
    // return rec_solve(numbers, expected, cur + numbers[i], i+1) || rec_solve(numbers, expected, cur * numbers[i], i+1);
    auto concat_num = stoull(to_string(cur) + to_string(numbers[i]));
    return rec_solve(numbers, expected, cur + numbers[i], i+1) || rec_solve(numbers, expected, cur * numbers[i], i+1) || rec_solve(numbers, expected, concat_num, i+1);

}

int main() {
    auto input = input_t();
    unsigned long long p1 = 0;
    while (input.next()) {
        rtrim(input.line);
        auto fi = input.line.find(":");
        unsigned long long res = stoull(input.line.substr(0, fi));
        string s = input.line.substr(fi+2, input.line.size()-fi-2);
        vector<int> numbers;

        char *cs = const_cast<char *>(s.c_str());
        char *tok = strtok(cs, " ");
        while(tok != NULL) {
            numbers.push_back(atoi(tok));
            tok = strtok(NULL, " ");
        }

        bool solvable = rec_solve(numbers, res, numbers[0], 1);
        if(solvable) {
            p1 += res;
        }
    }
    cout << p1 << endl;
}