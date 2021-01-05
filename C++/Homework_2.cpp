#include <iostream>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <math.h>

using ll = long long;

using namespace std;

ll Add(const string &s, int &i, bool &error);

ll Mul(const string &, int &, bool &);

ll Get_Number(const string &, int &, bool &);

void Skip_Spaces(const string &s, int &i) {
    while (i < s.length() && (s[i] == ' ' || s[i] == '\n'))
        i++;
}

bool Is_Number(char t) {
    return t >= '0' && t <= '9';
}

bool Is_Sign(char t) {
    return t == '+' || t == '-' || t == '*' || t == '/';
}

ll Add(const string &s, int &i, bool &error) {
    Skip_Spaces(s, i);
    ll a = Mul(s, i, error);
    Skip_Spaces(s, i);
    if (i >= s.length())
        return a;
    while (s[i] == '+' || s[i] == '-') {
        Skip_Spaces(s, i);
        if (s[i] == '+') {
            ++i;
            if (i >= s.length())
                error = 1;
            a += Mul(s, i, error);
        } else if (s[i] == '-') {
            ++i;
            if (i >= s.length())
                error = 1;
            a -= Mul(s, i, error);
        } else
            error = 1;
        if (error)
            return 0;
    }
    return a;
}

ll Mul(const string &s, int &i, bool &error) {
    ll a = Get_Number(s, i, error);
    Skip_Spaces(s, i);
    if (i >= s.length())
        return a;
    while (s[i] == '*' || s[i] == '/') {
        Skip_Spaces(s, i);
        if (s[i] == '*') {
            ++i;
            if (i >= s.length())
                error = 1;
            a *= Get_Number(s, i, error);
        } else if (s[i] == '/') {
            ++i;
            if (i >= s.length())
                error = 1;
            ll t = Get_Number(s, i, error);
            if (t == 0)
                error = 1;
            else
                a /= t;
        } else {
            error = 1;
        }
        if (error)
            return 0;
    }
    return a;
}

ll Get_Number(const string &s, int &i, bool &error) {
    Skip_Spaces(s, i);
    int k = 1;
    if (s[i] == '-') {
        k = -1;
        i++;
        if (i >= s.length())
            error = 1;
    }
    Skip_Spaces(s, i);
    ll a = 0;
    if (s[i] == '+' || s[i] == '*' || s[i] == '/') {
        error = 1;
        return 0;
    }
    while (Is_Number(s[i])) {
        a = a * 10 + s[i] - '0';
        i++;
    }
    Skip_Spaces(s, i);
    if (!Is_Sign(s[i]) && i < s.length()) {
        error = 1;
        return 0;
    }
    a *= k;
    return a;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "error";
        return 1;
    }
    string h = argv[1];
    bool error = 0;
    int i = 0;
    ll tmp = Add(h, i, error);
    if (!error && !h.empty())
        cout << tmp;
    else {
        cout << "error";
        return 1;
    }
    return 0;
}
