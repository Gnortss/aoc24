#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <regex>

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

int main() {
    auto input = input_t();
    unsigned int sum = 0;

    // regex pattern(R"(mul\((\d+),(\d+)\))"); //p1
    regex pattern(R"(mul\((\d+),(\d+)\)|do\(\)|don\'t\(\))"); //p2
    smatch res;
    bool ignore = false;
    while (input.next()) {
        string::const_iterator search_start(input.line.cbegin());
        while(regex_search(search_start, input.line.cend(), res, pattern)) {
            // sum += stoul(res[1].str()) * stoul(res[2].str()); //p1

            if (res[0].str().compare("do()") == 0)
                ignore = false;
            else if(res[0].str().compare("don't()") == 0)
                ignore = true;
            else if(!ignore) {
                sum += stoul(res[1].str()) * stoul(res[2].str());
            }
            
            search_start = res.suffix().first;
        }
    }
    cout << sum << endl;
}