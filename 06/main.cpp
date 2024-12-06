#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <utility>
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

enum Direction {
    up = 0,
    ri,
    dow,
    le
};

int main() {
    auto input = input_t();
    const int NUM_LINES = input.num_lines;
    const int SIZE = input.line.size();

    char **M;
    M = new char *[NUM_LINES];
    for(int i = 0; i < NUM_LINES; i++)
        M[i] = new char[SIZE];
    int gx = 0, gy = 0;
    int sx = 0, sy = 0;
    Direction d = up;
    set<pair<int,int>> visited1;
    set<pair<Direction,pair<int, int>>> visited2;

    int i = 0;
    while (input.next()) {
        // cout << input.line;
        for(int j = 0; j < SIZE; j++) {
            // M[i][j] = input._line[j];
            M[i][j] = input._line[j];
            if (input._line[j] == '^') {
                sx = i; gx = i;
                sy = j; gy = j;
            }
        }
        i++;
    }

    auto print_map = [&](char **m) {
        for(int i = 0; i < NUM_LINES; i++) {
            for(int j = 0; j < SIZE; j++) {
                cout << m[i][j];
            }
            cout << endl;
        }
    };

    auto change_direction = [&](){
        switch(d) {
            case up: d = ri; break;
            case ri: d = dow; break;
            case dow: d = le; break;
            case le: d = up; break;
        }
    };

    auto out_of_bounds = [&](int x, int y) -> bool {
        if (x < 0 || y < 0 || x >= NUM_LINES || y >= SIZE) return true;
        return false;
    };

    auto move = [&](char **m) -> bool {
        switch(d) {
            case up:
                if (out_of_bounds(sx-1, sy)) { sx--; return true; }
                if (m[sx-1][sy] == '#') return false;
                sx--; return true;
            case dow:
                if (out_of_bounds(sx+1, sy)) { sx++; return true; }
                if (m[sx+1][sy] == '#') return false;
                sx++; return true;
            case le:
                if (out_of_bounds(sx, sy-1)) { sy--; return true; }
                if (m[sx][sy-1] == '#') return false;
                sy--; return true;
            case ri:
                if (out_of_bounds(sx, sy+1)) { sy++; return true; }
                if (m[sx][sy+1] == '#') return false;
                sy++; return true;
        }
    };

    while(!out_of_bounds(sx, sy)) {
        visited1.insert({sx, sy});
        // M[sx][sy] = 'X';
        if(!move(M)) {
            // print_map(M); cout << visited.size() << endl;
            change_direction();
        }
    }

    int possible_cycles = 0;
    for(const pair<int,int> &p : visited1) {
        if (p.first == gx && p.second == gy) continue; //guard start point
        M[p.first][p.second] = '#';

        // print_map(M);
        // test for cycle
        visited2.clear();
        sx = gx; sy = gy; d = up; // reset position and direction
        bool just_changed_dir = false;
        while(!out_of_bounds(sx, sy)) {
            if(!just_changed_dir) {
                auto it = visited2.find({d, {sx, sy}});
                if (it != visited2.end()) {
                    possible_cycles++;
                    break;
                }

                visited2.insert({d, {sx, sy}});
            }
            just_changed_dir = false;
            if(!move(M)) {
                change_direction();
                just_changed_dir = true;
            }
        }

        M[p.first][p.second] = '.';
    }

    // print_map(M); 
    cout << visited1.size() << endl;
    cout << possible_cycles << endl;
}