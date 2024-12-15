#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <regex>

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
    vector<vector<int>> robots;
    int ROW_SIZE = 101;
    int COL_SIZE = 103;
    regex pattern("(-{0,1}[0-9]+)");
    smatch res;
    while (input.next()) {
        // cout << input.line << endl;
        vector<int> r;
        int i = 0;
        while(regex_search(input.line, res, pattern)) {
            r.push_back(stoi(res[0].str()));
            input.line = res.suffix().str();
        }
        robots.push_back(r);
    }
    auto next_step = [&](int seconds = 1) {
        for(int i = 0; i < robots.size(); ++i) {
            auto r = robots[i];
            // cout << r[0] << " " << r[1] << " " << r[2] << " " << r[3] << endl;
            int nx = (r[0] + (r[2]*seconds))%ROW_SIZE;
            int ny = (r[1] + (r[3]*seconds))%COL_SIZE;
            nx = nx < 0 ? ROW_SIZE + nx : nx;
            ny = ny < 0 ? COL_SIZE + ny : ny;
            // cout << nx << " " << ny << " " << r[2] << " " << r[3] << endl;
            robots[i][0] = nx;
            robots[i][1] = ny;
        }
    };
    // next_step(100); // p1
    // int quadrants[4] = {0, 0, 0, 0};
    // int mx = ROW_SIZE / 2;
    // int my = COL_SIZE / 2;
    // for(auto &r : robots) {
    //     if (r[0] < mx) {
    //         if (r[1] < my) { //q1
    //             quadrants[0]++;
    //         } else if (r[1] > my) { //q2
    //             quadrants[1]++;
    //         }
    //     } else if (r[0] > mx) {
    //         if (r[1] < my) { //q3
    //             quadrants[2]++;
    //         } else if (r[1] > my) { //q4
    //             quadrants[3]++;
    //         }
    //     }
    // }
    // int p1 = quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
    // cout << p1 << endl;
    map<pair<int, int>, int> rm;
    for(int i = 0; i < 100000; i++) {
        next_step();
        rm.clear();
        for(auto &r : robots) {
            if (rm.count({r[0], r[1]})) rm[{r[0], r[1]}]++;
            else rm[{r[0], r[1]}] = 1;
        }
        int min_adj_robots = 25;
        for(int j = 0; j < COL_SIZE; j++) {
            for (int k = 0; k < ROW_SIZE; k++) {
                if (min_adj_robots <= 0) { cout << i +1 << endl; return 1;}
                if (rm.count({k, j})) min_adj_robots--;
                else min_adj_robots = 8;
            }
        }
    }
}