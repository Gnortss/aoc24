#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>

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

bool can_move(vector<vector<char>> &M, int rx, int ry, int dx, int dy) {
    int nx = rx + dx;
    int ny = ry + dy;
    while(M[nx][ny] == 'O') {
        nx += dx;
        ny += dy;
    }
    return M[nx][ny] == '.';
}

bool can_move_horizontally(vector<vector<char>> &M, int rx, int ry, int dy) {
    int ny = ry + dy;
    while(M[rx][ny] == '[' || M[rx][ny] == ']') {
        ny += dy;
    }
    return M[rx][ny] == '.';
}

bool can_move_vertically(vector<vector<char>> &M, int rx, int ry, int dx) {
    int nx = rx + dx;
    if (M[nx][ry] == '.') return true;
    if (M[nx][ry] == '#') return false;
    if (M[nx][ry] == '[') {
        return can_move_vertically(M, nx, ry, dx) && can_move_vertically(M, nx, ry+1, dx);
    } else if(M[nx][ry] == ']') {
        return can_move_vertically(M, nx, ry, dx) && can_move_vertically(M, nx, ry-1, dx);
    }
    return true;
}

void move(vector<vector<char>> &M, int &rx, int &ry, char m) {
    int dx=0, dy=0;
    switch(m) {
        case '<': dx = 0; dy = -1; break;
        case '>': dx = 0; dy = 1; break;
        case '^': dx = -1; dy = 0; break;
        case 'v': dx = 1; dy = 0; break;
    }
    if(can_move(M, rx, ry, dx, dy)) {
        int nx = rx + dx;
        int ny = ry + dy;
        char mem = '@';
        M[rx][ry] = '.';
        while(true) {
            char tmp = M[nx][ny];
            M[nx][ny] = mem;
            if (tmp == '.') break;
            mem = tmp;

            nx += dx;
            ny += dy;
        }
        rx += dx;
        ry += dy;
    }
}

void move_horizontal(vector<vector<char>> &M, int &rx, int &ry, int dy) {
    if(can_move_horizontally(M, rx, ry, dy)) {
        int ny = ry + dy;
        char mem = '@';
        M[rx][ry] = '.';
        while(true) {
            char tmp = M[rx][ny];
            M[rx][ny] = mem;
            if (tmp == '.') break;
            mem = tmp;
            ny += dy;
        }
        ry += dy;
    }
}

void move_vertical_(vector<vector<char>> &M, int rx, int ry, int dx, char c = '.') {
    int nx = rx + dx;
    char mem = M[rx][ry];
    M[rx][ry] = c;
    if (mem == '.') return;
    move_vertical_(M, nx, ry, dx, mem);
    if (c != '.') {
        if (mem == ']') move_vertical_(M, rx, ry-1, dx);
        if (mem == '[') move_vertical_(M, rx, ry+1, dx);
    }
}

void move_vertical(vector<vector<char>> &M, int &rx, int ry, int dx) {
    if (can_move_vertically(M, rx, ry, dx)) {
        move_vertical_(M, rx, ry, dx);
        rx += dx;
    }
}

void print_map(vector<vector<char>> &M) {
    for(const auto &r : M) {
        for(const auto &c : r) {
            cout << c;
        }
        cout << endl;
    }
}

void get_robot_pos(vector<vector<char>> &M, int &rx, int &ry) {
    for(int i = 0; i < M.size(); ++i)
        for(int j = 0; j < M[0].size(); ++j)
            if (M[i][j] == '@') {
                rx = i;
                ry = j;
                return;
            }
}

int main() {
    auto input = input_t();

    vector<vector<char>> M1;
    vector<vector<char>> M2;
    vector<char> moves;


    bool reading_moves = false;
    while (input.next()) {
        rtrim(input.line);
        if (input.line.length() == 0) {
            reading_moves = true;
            continue;
        }

        if (!reading_moves) {
            vector<char> row1;
            vector<char> row2;
            for(const auto &c : input.line) {
                row1.push_back(c);
                switch(c) {
                    case '#': row2.push_back('#'); row2.push_back('#'); break;
                    case 'O': row2.push_back('['); row2.push_back(']'); break;
                    case '.': row2.push_back('.'); row2.push_back('.'); break;
                    case '@': row2.push_back('@'); row2.push_back('.'); break;
                }
            }
            M1.push_back(row1);
            M2.push_back(row2);
        } else {
            for(const auto &c : input.line) moves.push_back(c);
        }
    }

    // find robot position for M1
    int rx1, ry1;
    get_robot_pos(M1, rx1, ry1);

    for(char &m : moves) {
        move(M1, rx1, ry1, m);
    }
    // print_map(M1);
    int p1 = 0;
    for(int i = 0; i < M1.size(); ++i) {
        for (int j = 0; j < M1[0].size(); ++j) {
            if (M1[i][j] == 'O') p1 += 100*i+j;
        }
    }
    cout << p1 << endl;

    // find robot position for M2
    int rx2, ry2;
    get_robot_pos(M2, rx2, ry2);
    for(char &m : moves) {
        switch(m) {
            case '<': move_horizontal(M2, rx2, ry2, -1); break;
            case '>': move_horizontal(M2, rx2, ry2, 1); break;
            case '^': move_vertical(M2, rx2, ry2, -1); break;
            case 'v': move_vertical(M2, rx2, ry2, 1); break;
        }
    }

    int p2 = 0;
    for(int i = 0; i < M2.size(); ++i) {
        for (int j = 0; j < M2[0].size(); ++j) {
            if (M2[i][j] == '[') p2 += 100*i+j;
        }
    }
    cout << p2 << endl;
}