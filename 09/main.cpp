#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>

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

int main() {
    auto input = input_t();
    input.next();
    // cout << input.line << endl;
    vector<string> disk;

    auto insert_file = [&](int l, string c) {
        for(int i = 0; i < l; ++i) {
            disk.push_back(c);
        }
    };

    for(int i = 0; i < input.line.size(); ++i) {
        if(!(i % 2)) { // file
            int fileID = i/2;
            insert_file(input.line[i] - '0', to_string(fileID));
        } else { // empty
            insert_file(input.line[i] - '0', ".");
        }
    }

    // p1
    // for(int i = 0; i < disk.size(); i++) {
    //     if(disk[i] != ".") continue;
    //     string c;
    //     do {
    //         c = disk.back();
    //         disk.pop_back();
    //     } while(c == ".");
    //     disk[i] = c;
    // }

    // p2
    string id = disk.back();
    auto file_length = [&](string id) -> int {
        int s = 0;
        for(int i = disk.size() - 1; i >= 0; --i) {
            if(disk[i] == id) s++;
        }
        return s;
    };
    auto find_enough_space = [&](int l, int before) {
        int free = 0;
        int si = 0;
        for (int i = 0; i < before; ++i) {
            if (disk[i] != ".") { free = 0; si = i+1;}
            else ++free;

            if (free >= l) return si;
        }
        return -1;
    };
    auto get_start_file_index = [&](string id) {
        for (int i = 0; i < disk.size(); ++i) {
            if(disk[i] == id) return i;
        }
    };
    auto remove_file = [&](string id) {
        for (int i = 0; i < disk.size(); ++i) {
            if (disk[i] == id) disk[i] = ".";
        }
    };
    while(id != "-1") {
        int l = file_length(id);
        int fi = find_enough_space(l, get_start_file_index(id));
        if (fi != -1) {
            remove_file(id);
            for(int i = 0; i < l; ++i) {
                disk[fi+i] = id;
            }
        }
        id = to_string(stoi(id)-1);
    }

    // checksum
    unsigned long long s = 0;
    for(int i = 0; i < disk.size(); i++) {
        // cout << disk[i];
        if (disk[i] == ".") continue;
        s += i * stoi(disk[i]);
    }
    cout << endl << s << endl;
}