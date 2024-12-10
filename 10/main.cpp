#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <set>

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

int rec_count_trails(vector<vector<int>> &M, int x, int y, set<pair<int, int>> &reachable_nines) {
    if (M[x][y] == 9) {
        reachable_nines.insert({x, y});
        return 1;
    } 

    auto is_out_of_bounds = [&](int x, int y) -> bool {
        return x < 0 || y < 0 || x >= M.size() || y >= M[0].size();
    };

    vector<pair<int, int>> next;
    auto add_pair = [&](int nx, int ny) {
        if(!is_out_of_bounds(nx, ny) && M[nx][ny]-M[x][y] == 1) next.push_back({nx, ny});
    };
    add_pair(x+1, y);
    add_pair(x-1, y);
    add_pair(x, y+1);
    add_pair(x, y-1);

    // for (int i = 0; i < M[x][y]; ++i) cout << ' ';
    // for(const auto &p : next) {
    //     cout << "(" << p.first << "," << p.second << "," << M[p.first][p.second] << ")";
    // }
    // cout << endl;
    int s = 0;
    for(const auto &p : next) {
        s += rec_count_trails(M, p.first, p.second, reachable_nines);
    }
    return s;
}

int main() {
    auto input = input_t();
    vector<vector<int>> M;
    while(input.next()) {
        rtrim(input.line);
        vector<int> row;
        for(const auto &c : input.line) {
            row.push_back(c - '0');
        }
        M.push_back(row);
    }

    int p1 = 0;
    int p2 = 0;
    for(int i = 0; i < M.size(); ++i) {
        for(int j = 0; j < M[0].size(); ++j) {
            if (M[i][j] == 0) {
                set<pair<int, int>> result;
                p2 += rec_count_trails(M, i, j, result);
                p1 += result.size();
            }
        }
    }
    cout << p1 << "," << p2 << endl;
}