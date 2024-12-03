#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <sstream>

using namespace std;

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

bool is_safe(vector<int> &numbers) {
    int delta = 0;
    for(int i = 0; i < numbers.size() - 1; i++) {
        int l = numbers[i];
        int r = numbers[i+1];

        if (l == r || abs(l-r) > 3)
            return false;
        if(delta == 0) {
            delta = l - r;
            continue;
        } else if (delta < 0 && l-r >= 0 || delta > 0 && l-r <=0) {
            return false;
        }
    }
    return true;
}

int main() {
    auto input = input_t();

    int safe_num1 = 0;
    int safe_num2 = 0;
    while (input.next()) {
        istringstream is(input.line);
        vector<int> numbers;
        int n;
        while (is >> n) {
            numbers.push_back(n);
        }
        
        if (is_safe(numbers)) {
            safe_num1++;
        }

        for(int i = 0; i < numbers.size(); i++) {
            vector<int> tmp = numbers;
            tmp.erase(tmp.begin() + i);

            if (is_safe(tmp)) {
                safe_num2++;
                break;
            }
        }
    }
    cout << safe_num1 << endl;
    cout << safe_num2 << endl;
}