#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
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

bool is_oob(vector<vector<char>> &M, pair<int, int> p) {
    return p.first < 0 || p.second < 0 || p.first >= M.size() || p.second >= M[0].size();
}

void expand_region(vector<vector<char>> &M, char region_char, pair<int, int> cur_pos, set<pair<int, int>> &r) {
    if (is_oob(M, cur_pos) || r.count(cur_pos) || M[cur_pos.first][cur_pos.second] != region_char) return;

    if (M[cur_pos.first][cur_pos.second] == region_char) r.insert(cur_pos);

    expand_region(M, region_char, {cur_pos.first + 1, cur_pos.second}, r);
    expand_region(M, region_char, {cur_pos.first - 1, cur_pos.second}, r);
    expand_region(M, region_char, {cur_pos.first, cur_pos.second + 1}, r);
    expand_region(M, region_char, {cur_pos.first, cur_pos.second - 1}, r);
}

vector<set<pair<int,int>>> get_regions(vector<vector<char>> &M) {
    vector<set<pair<int,int>>> regions;
    set<pair<int,int>> accounted_for;

    for(int i = 0; i < M.size(); ++i) {
        for(int j = 0; j < M[0].size(); ++j) {
            if(accounted_for.count({i, j})) continue;

            // create new region with this char
            set<pair<int,int>> r;
            // expand region
            expand_region(M, M[i][j], {i, j}, r);

            // cout << M[i][j] << "=" << r.size() << endl;
            for(const auto &el : r) accounted_for.insert(el);
            regions.push_back(r);
        }
    }
    return regions;
}

int get_region_perimeter(vector<vector<char>> &M, const set<pair<int, int>> &r, char region_char) {
    int per = 0;
    auto count_perimeter = [&](pair<int, int> p) -> int {
        auto calc = [&](pair<int, int> pos) { return is_oob(M, pos) || M[pos.first][pos.second] != region_char ? 1 : 0; };
        return calc({p.first + 1, p.second}) + calc({p.first - 1, p.second}) + calc({p.first, p.second + 1}) + calc({p.first, p.second - 1});
    };
    for(const auto &pos : r) {
        per += count_perimeter(pos);
    }
    return per;
}

enum SideType {
    LEFT,
    TOP,
    RIGHT,
    BOTTOM
};

int get_region_sides(vector<vector<char>> &M, const set<pair<int, int>> &r, char region_char) {
    // find a starting position (one which is on the side anywhere)
    auto determine_side_types = [&](pair<int, int> p) -> set<SideType> {
        set<SideType> s;
        if (is_oob(M, {p.first - 1, p.second}) || M[p.first -1][p.second] != region_char) s.insert(TOP);
        if (is_oob(M, {p.first + 1, p.second}) || M[p.first +1][p.second] != region_char) s.insert(BOTTOM);
        if (is_oob(M, {p.first, p.second - 1}) || M[p.first][p.second - 1] != region_char) s.insert(LEFT);
        if (is_oob(M, {p.first, p.second + 1}) || M[p.first][p.second + 1] != region_char) s.insert(RIGHT);
        return s;
    };

    set<pair<int, int>> visited;

    auto find_start = [&]() -> pair<int, int> {
        for(const auto &p : r) {
            if (determine_side_types(p).size() > 0 && !visited.count(p)) return p;
        }
        return {-1, -1};
    };

    int ret = 0;
    int sret = 0;
    pair<int, int> start;
    set<SideType> sides;
    pair<int, int> cur;
    SideType cur_side, start_side;

    auto move_to_edge_and_rotate = [&]() -> int {
        auto s = determine_side_types(cur); 
        if(cur_side == TOP) {
            while(true) {
                visited.insert(cur);
                if (sret >=4 && cur == start && start_side == cur_side) return 0;
                if (s.count(TOP) && s.count(RIGHT)) {
                    cur_side = RIGHT;
                    return 1;
                }
                pair<int, int> npos = {cur.first, cur.second + 1};
                if (!determine_side_types(npos).count(TOP)) {
                    cur = {npos.first - 1, npos.second};
                    cur_side = LEFT;
                    return 1;
                }
                cur = {npos.first, npos.second};
                s = determine_side_types(cur);
            }
        }
        if(cur_side == RIGHT) {
            while(true) {
                visited.insert(cur);
                if (sret >=4 && cur == start && start_side == cur_side) return 0;
                if (s.count(RIGHT) && s.count(BOTTOM)) {
                    cur_side = BOTTOM;
                    return 1;
                }
                pair<int, int> npos = {cur.first + 1, cur.second};
                if (!determine_side_types(npos).count(RIGHT)) {
                    cur = {npos.first, npos.second + 1};
                    cur_side = TOP;
                    return 1;
                }
                cur = {npos.first, npos.second};
                s = determine_side_types(cur);
            }
        }
        if(cur_side == BOTTOM) {
            while(true) {
                visited.insert(cur);
                if (sret >=4 && cur == start && start_side == cur_side) return 0;
                if (s.count(BOTTOM) && s.count(LEFT)) {
                    cur_side = LEFT;
                    return 1;
                }
                pair<int, int> npos = {cur.first, cur.second - 1};
                if (!determine_side_types(npos).count(BOTTOM)) {
                    cur = {npos.first + 1, npos.second};
                    cur_side = RIGHT;
                    return 1;
                }
                cur = {npos.first, npos.second};
                s = determine_side_types(cur);
            }
        }
        if(cur_side == LEFT) {
            while(true) {
                visited.insert(cur);
                if (sret >=4 && cur == start && start_side == cur_side) return 0;
                if (s.count(LEFT) && s.count(TOP)) {
                    cur_side = TOP;
                    return 1;
                }
                pair<int, int> npos = {cur.first - 1, cur.second};
                if (!determine_side_types(npos).count(LEFT)) {
                    cur = {npos.first, npos.second - 1};
                    cur_side = BOTTOM;
                    return 1;
                }
                cur = {npos.first, npos.second};
                s = determine_side_types(cur);
            }
        }
        return 1;
    };
    start = find_start();
    do {
        sides = determine_side_types(start);
        cur = {start.first, start.second};
        cur_side = *sides.begin();
        start_side = cur_side; // choose one side to start with
        sret = 0;
        while(move_to_edge_and_rotate()) {
            sret++;
        }
        ret += sret;
        start = find_start();
    } while(start != pair<int, int>{-1, -1});
    return ret;
}

int main() {
    auto input = input_t();
    vector<vector<char>> M;
    while(input.next()) {
        rtrim(input.line);
        vector<char> row;
        for(const auto &c : input.line) {
            row.push_back(c);
        }
        M.push_back(row);
    }

    vector<set<pair<int, int>>> regions = get_regions(M);
    int p1 = 0;
    int p2 = 0;
    for(const auto &r : regions) {
        char region_char = M[r.begin()->first][r.begin()->second];
        auto val = get_region_perimeter(M, r, region_char);
        auto val2 = get_region_sides(M, r, region_char);
        // cout << region_char << "=" << val << "," << val2 << endl;
        p1 += val * r.size();
        p2 += val2 * r.size();
    }
    cout << p1 << endl;
    cout << p2 << endl;
}