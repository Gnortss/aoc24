#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <vector>
#include <string.h>
#include <algorithm>

using namespace std;

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
    map<int,set<int>> m_after; // all values need to be after key

    auto insert_into_map = [](map<int,vector<int>> *m, int k, int val) {
        map<int,vector<int>>::iterator it;
        it = m->find(k);
        if (it == m->end()) { // not found
            vector<int> vec; vec.insert(vec.begin(), val);
            m->insert(pair<int, vector<int>>(k, vec));
        } else { // insert into existing vector
            it->second.insert(it->second.begin(), val);
        }
    };
    int phase = 0;
    int sum1 = 0;
    int sum2 = 0;
    while (input.next()) {
        if (input.line == "\n") { // skip empty line and go to next phase
            phase++;
            continue;
        }
        if (phase == 0) {
            int fi = input.line.find("|");
            auto k = stoi(input.line.substr(0, fi));
            auto v = stoi(input.line.substr(fi+1, input.line.size()-fi-2));
            m_after[k].insert(v);
        }
        if (phase == 1) {
            vector<int> numbers;
            char *tok;
            char *line = input._line;
            if (line[input.line.size()-1] == '\n') line[input.line.size()-1] = '\0';
            tok = strtok(input._line, ",");
            while(tok != NULL) {
                numbers.push_back(atoi(tok));
                tok = strtok(NULL, ",");
            }

            auto vector_contains = [](vector<int> v, int k) -> bool {
                for(const int& i : v) {
                    if(i == k) return true;
                }
                return false;
            };

            auto check_before_not_valid = [&](int number_index) {
                for(int i = number_index; i > 0; i--) {
                    if (m_after[numbers[number_index]].count(numbers[i]))
                        return true;
                }
                return false;
            };

            int valid_order = true;
            for(int i = 0; i < numbers.size(); i++) {
                // for each number check:
                //  anything before it must not be in m_after[number]
                if (check_before_not_valid(i)) {
                    valid_order = false;
                    break;
                }
            }
            if(valid_order) {
                // cout << numbers[numbers.size()/2] << endl;
                sum1 += numbers[numbers.size()/2];
            } else {
                sort(numbers.begin(), numbers.end(), [&](int a, int b){return m_after[a].count(b) > 0;});
                sum2 += numbers[numbers.size()/2];
            }
        }
    }
    cout << sum1 << endl;
    cout << sum2 << endl;
}