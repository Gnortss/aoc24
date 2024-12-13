#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
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
    vector<vector<pair<int,int>>> collection_prizes_buttons;
    regex pattern(".*X.([0-9]+).*Y.([0-9]+).*");
    smatch res;
    while (input.next()) {
        rtrim(input.line); auto l1 = input.line; input.next();
        rtrim(input.line); auto l2 = input.line; input.next();
        rtrim(input.line); auto l3 = input.line; input.next();
        // cout << l1 << l2 << l3 << endl;
        vector<pair<int,int>> a_b_prize;
        regex_match(l1, res, pattern);
        a_b_prize.push_back({stoi(res[1].str()), stoi(res[2].str())});
        regex_match(l2, res, pattern);
        a_b_prize.push_back({stoi(res[1].str()), stoi(res[2].str())});
        regex_match(l3, res, pattern);
        a_b_prize.push_back({stoi(res[1].str()), stoi(res[2].str())});
        collection_prizes_buttons.push_back(a_b_prize);
    }

    int costA = 3, costB = 1;
    uint64_t p1 = 0;
    uint64_t p2 = 0;
    for(const auto &el : collection_prizes_buttons) { // el -> {a, b, prize}
        uint64_t ax = el[0].first; uint64_t ay = el[0].second;
        uint64_t bx = el[1].first; uint64_t by = el[1].second;
        uint64_t px = el[2].first; uint64_t py = el[2].second;
        uint64_t px1 = el[2].first + 10000000000000; uint64_t py1 = el[2].second + 10000000000000; //p2
        uint64_t sx = 0; uint64_t sy = 0;
        uint64_t sum_cost = -1;
        double A = ((double)px*by - bx*py)/((double)ax*by - bx*ay);
        double B = ((double)ax*py - px*ay)/((double)ax*by - bx*ay);        
        double A1 = ((double)px1*by - bx*py1)/((double)ax*by - bx*ay);
        double B1 = ((double)ax*py1 - px1*ay)/((double)ax*by - bx*ay);

        if(A == static_cast<double>(static_cast<int64_t>(A)) && B == static_cast<double>(static_cast<int64_t>(B))) { // if int solution
            uint64_t cost = A*costA + B*costB;
            // cout << A << " " << B << " " << cost << endl;
            p1 += cost;
        }
        if(A1 == static_cast<double>(static_cast<int64_t>(A1)) && B1 == static_cast<double>(static_cast<int64_t>(B1))) { // if int solution
            uint64_t cost = A1*costA + B1*costB;
            // cout << px << " " << py << " " << cost << endl;
            p2 += cost;
        }
    }
    cout << p1 << endl;
    cout << p2 << endl;
}