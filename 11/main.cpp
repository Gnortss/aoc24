#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <map>

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

uint64_t rec_solve(map<pair<string, int>, uint64_t> &memo, string stone, int blinks) {
    if (blinks == 0) {
        // cout << stone << " ";
        return 1;
    }
    if (memo.count({stone, blinks})) return memo[{stone, blinks}];
    uint64_t ret = 0;
    char *end;
    if (stone == "0") {
        ret = rec_solve(memo, "1", blinks - 1);
    } else if (stone.length() % 2 == 0) {
        int digits = stone.length();
        auto l = stone.substr(0, digits/2);
        auto r = stone.substr(digits/2, digits/2);
        r = to_string(strtoull(r.c_str(), &end, 10));
        ret = rec_solve(memo, l, blinks - 1) + rec_solve(memo, r, blinks - 1);
    } else {
        ret = rec_solve(memo, to_string(strtoull(stone.c_str(), &end, 10) * 2024), blinks - 1);
    }
    memo[{stone, blinks}] = ret;
    return ret;
}

int main() {
    auto input = input_t();
    input.next();
    vector<string> stones;
    char *tok = strtok(input._line, " ");
    char *end;
    while(tok != NULL) {
        stones.push_back((string) tok);
        tok = strtok(NULL, " ");
    }
    map<pair<string, int>, uint64_t> memo;

    uint64_t p1 = 0;
    for(const auto &el : stones) {
        uint64_t partial = rec_solve(memo, el, 75);
        cout << partial << "+";
        p1 += partial;
    }
    cout << endl << p1 << endl;

}