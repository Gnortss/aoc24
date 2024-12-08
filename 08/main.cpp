#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <map>
#include <set>

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

    vector<vector<char>> m;
    map<char, set<pair<int,int>>> ant;
    set<pair<int,int>> antidotes;

    while (input.next()) {
        rtrim(input.line);
        vector<char> vl;
        for(int i = 0; i < input.line.size(); i++) {
            vl.push_back(input.line[i]);
            if (input.line[i] != '.') { // its an antenae
                ant[input.line[i]].insert({m.size(), i});
            }
        }
        m.push_back(vl);
    }

    auto out_of_bounds = [&](int x, int y) {
        return x < 0 || y < 0 || x > m[0].size()-1 || y > m.size()-1;
    };

    // get antidotes
    for (auto const& x : ant)
    {
        set<pair<int,int>>::iterator it1;
        for (it1 = x.second.begin(); it1 != x.second.end(); ++it1) {
            set<pair<int,int>>::iterator it2;
            for (it2 = x.second.begin(); it2 != x.second.end(); ++it2) {
                auto dx = it2->first - it1->first;
                auto dy = it2->second - it1->second;
                if(dx == 0 && dy == 0) continue;
                auto addel = [&](int x, int y, set<pair<int,int>> s) {
                    // if(!out_of_bounds(x, y) && s.count({x, y}) == 0) //p1
                    if(!out_of_bounds(x, y)) //p2
                        antidotes.insert({x, y});
                };
                //p1
                // addel(it1->first - dx, it1->second - dy, x.second);
                // addel(it1->first + dx, it1->second + dy, x.second);
                //p2
                int nx = it1->first;
                int ny = it1->second;
                while(!out_of_bounds(nx, ny)) {
                    addel(nx, ny, x.second);
                    nx -= dx;
                    ny -= dy;
                }
                nx = it1->first;
                ny = it1->second;
                while(!out_of_bounds(nx, ny)) {
                    addel(nx, ny, x.second);
                    nx += dx;
                    ny += dy;
                }

            }
        }
    }
    cout << antidotes.size() << endl;

    // print antidotes on map
    // for(int i = 0; i < m.size(); i++) {
    //     for(int j = 0; j < m[0].size(); j++) {
    //         if(antidotes.count({i, j}))
    //             cout << '#';
    //         else
    //             cout << m[i][j];
    //     }
    //     cout << endl;
    // }

}