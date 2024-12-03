#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <array>
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

int main(void)
{
    const size_t SIZE = 1000;
    array<int, SIZE> a;
    array<int, SIZE> b;

    auto input = input_t();

    int i = 0;
    while (input.next()) {
        int fi = input.line.find("   ");
        a[i] = stoi(input.line.substr(0, fi));
        b[i] = stoi(input.line.substr(fi + 3));
        i++;
    }
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    unsigned int dist = 0;
    for (int j = 0; j < 1000; j++) {
        dist += abs(a[j]-b[j]);
    }
    cout << dist << endl; // part 1

    unsigned int sim = 0;
    for (int j = 0; j < 1000; j++) {
        auto count = [&](int el) -> unsigned int {
            int ret = 0;
            for(int k = 0; k < 1000; k++) {
                if(b[k] == el)
                    ret++;
            }
            return ret;
        };
        sim += a[j] * count(a[j]);
    }
    cout << sim << endl; // part 2
}



