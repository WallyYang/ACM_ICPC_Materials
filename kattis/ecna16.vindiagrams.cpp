// https://open.kattis.com/problems/ecna16.vindiagrams

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

char graph[128][128];
int row, column;
int countA, countB, countAB;

#define SCAN1(func) \
    for (int i = 1; i <= row; ++i) { \
        for (int j = 1; j <= column; ++j) { \
            char &self = graph[i][j]; \
            char &left = graph[i - 1][j]; \
            char &right = graph[i + 1][j]; \
            char &top = graph[i][j - 1]; \
            char &bottom = graph[i][j + 1]; \
            char &lt = graph[i - 1][j - 1]; \
            char &lb = graph[i - 1][j + 1]; \
            char &rt = graph[i + 1][j - 1]; \
            char &rb = graph[i + 1][j + 1]; \
            bool end = j == column; \
            func \
        } \
    }

#define SCAN128(func) \
    for (int iter = 0; iter < 128; ++iter) { \
        SCAN1(func) \
    }

// #define EXITSCAN \
//     i = 128; \
//     j = 128; \
//     break;

void find_outside() {
    SCAN128(
        if (self == '.') {
            if (left == ' ' || right == ' ' || top == ' ' || bottom == ' ') {
                self = ' ';
            }
        }
    )
}

void find_zone(char k) {
    bool get = false;
    SCAN128(
        if (self == '.') {
            if (!get || left == k || right == k || top == k || bottom == k) {
                get = true;
                self = k;
            }
        }
    )
}

bool try_explain(char a, char b, char ab) {
    bool ok = true;

    char bak[128][128];
    SCAN1(
        bak[i][j] = self;
    )

    int cross = 0;
    SCAN1(
        if (self == 'X' || self == 'A' || self == 'B') {
            bool has_a = left == a || right == a || top == a || bottom == a
                || lt == a || lb == a || rt == a || rb == a;
            bool has_b = left == b || right == b || top == b || bottom == b
                || lt == b || lb == b || rt == b || rb == b;
            bool has_ab = left == ab || right == ab || top == ab || bottom == ab
                || lt == ab || lb == ab || rt == ab || rb == ab;

            if (has_a && has_b) {
                self = '&';
                cross += 1;
            } else if (has_a ^ has_ab) {
                if (self == 'B') {
                    self = '&';
                } else {
                    self = 'A';
                }
            } else if (has_b ^ has_ab) {
                if (self == 'A') {
                    self = '&';
                } else {
                    self = 'B';
                }
            } else if (has_ab) {
                self = '!';
            } else {
                self = '?';
            }
        }
    )
    ok &= cross == 2;

    char isA = 0;
    char isB = 0;
    SCAN1(
        if (self == '!' || self == '?') {
            if (left == 'A' || right == 'A' || top == 'A' || bottom == 'A') {
                ok &= isA == self || !isA;
                isA = self;
            }
            if (left == 'B' || right == 'B' || top == 'B' || bottom == 'B') {
                ok &= isB == self || !isB;
                isB = self;
            }
        }
    )

    SCAN1(
        if (self == isA) {
            self = 'A';
        }
        if (self == isB) {
            self = 'B';
        }
    )

    SCAN1(
        if (self == 'A' || self == 'B') {
            ok &= (
                int(left == self || left == '&')
                + int(right == self || right == '&')
                + int(top == self || top == '&')
                + int(bottom == self || bottom == '&')
            ) == 2;
        }
        if (self == '&') {
            ok &= (left == 'A' && right == 'A' && top == 'B' && bottom == 'B')
                || (left == 'B' && right == 'B' && top == 'A' && bottom == 'A');
        }
    )

    if (ok) {
        SCAN1(
            if (self == a) {
                countA += 1;
            }
            if (self == b) {
                countB += 1;
            }
            if (self == ab) {
                countAB += 1;
            }
        )

        cout << countA << ' ' << countB << ' ' << countAB << endl;
    }

    // SCAN1(
    //     cout << self;
    //     if (end) cout << endl;
    // )

    SCAN1(
        self = bak[i][j];
    )

    return ok;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.precision(10);

    memset(graph, ' ', sizeof(graph));
    cin >> row >> column;

    SCAN1(
        cin >> self;
    )

    find_outside();

    find_zone(',');
    find_zone('_');
    // find_zone('.')

    try_explain(',', '_', '.');
    try_explain(',', '.', '_');
    try_explain('_', ',', '.');
    try_explain('_', '.', ',');
    try_explain('.', ',', '_');
    try_explain('.', '_', ',');

    // SCAN1(
    //     cout << self;
    //     if (end) cout << endl;
    // )

    return 0;
}