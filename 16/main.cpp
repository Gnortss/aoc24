#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>

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

enum Dir {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

typedef pair<int, pair<pair<int, int>, Dir>> node_type;

auto cmp = [](node_type l, node_type r) { return l.first > r.first; }; // for min queue

vector<node_type> get_next_(vector<vector<char>> &M, node_type node) {
    vector<node_type> ret;

    pair<int, int> pos = node.second.first;
    pair<int, int> next_pos;
    switch(node.second.second) {
        case NORTH: next_pos = {pos.first - 1, pos.second}; break;
        case SOUTH: next_pos = {pos.first + 1, pos.second}; break;
        case EAST: next_pos = {pos.first, pos.second + 1}; break;
        case WEST: next_pos = {pos.first, pos.second - 1}; break;
    }

    if (M[next_pos.first][next_pos.second] != '#') ret.push_back({node.first + 1, {next_pos, node.second.second}}); // move in the direction

    // turn left/right
    Dir left = static_cast<Dir>((static_cast<int>(node.second.second) - 1 + 4)%4);
    Dir right = static_cast<Dir>((static_cast<int>(node.second.second) + 1 + 4)%4);
    ret.push_back({node.first + 1000, {pos, left}});
    ret.push_back({node.first + 1000, {pos, right}});

    return ret;
}

int search_(vector<vector<char>> &M, map<pair<pair<int, int>, Dir>, int> &visited, priority_queue<node_type, vector<node_type>, decltype(cmp)> &queue) {
    while(queue.size()) {
        auto node = queue.top();
        visited[node.second] = node.first;
        auto pos = node.second.first;
        if (M[pos.first][pos.second] == 'E') return node.first;
        for(auto &n : get_next_(M, node)) {
            if (visited.count(n.second) == 0 || visited[n.second] > n.first) // valid only if not yet visited or we have found a faster way to this node (lower score)
                queue.push(n);
        }

        queue.pop();
    }
}

auto find_start = [](vector<vector<char>> &M) -> pair<int, int> {
    for(int i = 0; i < M.size(); ++i) {
        for(int j = 0; j < M[0].size(); ++j) {
            if (M[i][j] == 'S') return {i, j};
        }
    }
    return {0, 0};
};

int main() {

    auto input = input_t();

    vector<vector<char>> M;
    map<pair<pair<int, int>, Dir>, int> visited;
    priority_queue<node_type, vector<node_type>, decltype(cmp)> queue(cmp);
    // queue.push({10, {{0,0}, EAST}});

    while (input.next()) {
        rtrim(input.line);
        vector<char> row;
        for(auto &c: input.line) row.push_back(c);
        M.push_back(row);
    }

    queue.push({0, {find_start(M), EAST}});
    int p1 = search_(M, visited, queue);
    cout << p1 << endl;
    // for(auto &r: M) {
    //     for(auto &c: r) {
    //         cout << c;
    //     }
    //     cout << endl;
    // }
}