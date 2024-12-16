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
struct Node {
    int score;
    pair<int, int> pos;
    Dir dir;
    vector<pair<int,int>> path;

    Node(int _score, pair<int, int> _pos, Dir _dir, vector<pair<int,int>> _path={}) {
        score = _score;
        pos = _pos;
        dir = _dir;
        path = _path;
        path.push_back(_pos);
    }
};

auto cmp = [](Node l, Node r) { return l.score > r.score; }; // for min queue

vector<Node> get_next_(vector<vector<char>> &M, Node *node) {
    vector<Node> ret;

    pair<int, int> pos = node->pos;
    pair<int, int> next_pos;
    switch(node->dir) {
        case NORTH: next_pos = {pos.first - 1, pos.second}; break;
        case SOUTH: next_pos = {pos.first + 1, pos.second}; break;
        case EAST: next_pos = {pos.first, pos.second + 1}; break;
        case WEST: next_pos = {pos.first, pos.second - 1}; break;
    }

    if (M[next_pos.first][next_pos.second] != '#')  {
        ret.push_back(Node(node->score + 1, next_pos, node->dir, node->path)); // move in the direction
    }

    // turn left/right
    Dir left = static_cast<Dir>((static_cast<int>(node->dir) - 1 + 4)%4);
    Dir right = static_cast<Dir>((static_cast<int>(node->dir) + 1 + 4)%4);
    ret.push_back(Node(node->score + 1000, pos, left, node->path));
    ret.push_back(Node(node->score + 1000, pos, right, node->path));

    return ret;
}

pair<int, int> search_(vector<vector<char>> &M, map<pair<pair<int, int>, Dir>, int> &visited, priority_queue<Node, vector<Node>, decltype(cmp)> &queue) {
    int min_score = -1;
    set<pair<int,int>> unique;
    vector<Node> all;
    while(queue.size()) {
        auto node = queue.top();

        visited[{node.pos, node.dir}] = node.score;
        auto pos = node.pos;
        // if (M[pos.first][pos.second] == 'E') return {node.score, 0}; // p1
        if (M[pos.first][pos.second] == 'E') {
            if (min_score == -1) min_score = node.score;
            if (min_score == node.score) {
                for(pair<int, int> pos : node.path) {
                    unique.insert(pos);
                }
            }
        }
        for(auto &n : get_next_(M, &node)) {
            if (min_score != -1 && n.score > min_score) continue;
            if (visited.count({n.pos, n.dir}) == 0 || visited[{n.pos, n.dir}] >= n.score) // valid only if not yet visited or we have found a faster way to this node (lower score)
                queue.push(n);
        }

        queue.pop();
    }
    return {min_score, unique.size()};
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
    priority_queue<Node, vector<Node>, decltype(cmp)> queue(cmp);
    // queue.push({10, {{0,0}, EAST}});

    while (input.next()) {
        rtrim(input.line);
        vector<char> row;
        for(auto &c: input.line) row.push_back(c);
        M.push_back(row);
    }

    queue.push(Node(0, find_start(M), EAST));
    pair<int, int> solution = search_(M, visited, queue);
    std::cout << solution.first << " " << solution.second << endl;
}