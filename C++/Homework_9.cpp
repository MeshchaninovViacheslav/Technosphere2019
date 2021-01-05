#include<iostream>
#include <algorithm>
#include <atomic>
#include <thread>
#include <mutex>
#include <chrono>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <future>

const int max_size = (int) 500000;

using namespace std;

mutex m, m_print;


void print_(int64_t *, int);

void print_file(const string &);

bool check_sort(const string &, atomic<bool> &);

void test(const string &, int);

string process_merge(vector<string> &, atomic<bool> &);

string fill_files(ifstream &, atomic<int> &, atomic<bool> &, atomic<bool> &);

string process(ifstream &, atomic<int> &, atomic<bool> &, atomic<bool> &);

string merge(const string &, const string &, atomic<bool> &);


void print_(int64_t *t, int n) {
    lock_guard<mutex> guard(m_print);
    cout << '\n';
    for (int i = 0; i < n; i++)
        cout << t[i] << ' ';
    cout << '\n';
}

void print_file(const string &s) {
    lock_guard<mutex> guard(m_print);
    ifstream fin(s, ios::binary | ios::in);
    int64_t a;
    int i = 0;
    while (fin.read((char *) &a, sizeof(int64_t))) {
        cout << a << " ";
        i++;
    }
    cout << '\n' << "out " << i << '\n';
}

bool check_sort(const string &f, atomic<bool> &error) {
    ifstream fin(f, ios::binary | ios::in);
    if (!fin) {
        error = true;
        return 0;
    }
    int64_t a, b;
    fin.read((char *) &a, sizeof(int64_t));
    while (fin.read((char *) &b, sizeof(int64_t))) {
        if (a > b) {
            cout << "BAN\n";
            cout << a << ' ' << b << '\n';
            return 0;
        }
        a = b;
    }
    return 1;
}

void test(const string &s, int i = 10) {
    ofstream fout(s, ios::binary | ios::out);
    srand(time(nullptr));
    int k = 0;
    for (int j = 0; j < max_size * i; j++) {
        int64_t t = rand() % 1000;
        fout.write((char *) &t, sizeof(t));
        k++;
    }
    cout << "in" << " " << k << '\n';
}

string fill_files(ifstream &fin, atomic<int> &ind, atomic<bool> &error, atomic<bool> &end_file) {
    unique_ptr<int64_t[]> t(new int64_t[max_size]);
    {
        lock_guard<mutex> guard(m);
        if (!fin.read((char *) t.get(), sizeof(t)))
            end_file = true;
        if (end_file) {
            return "";
        }
    }
    sort(t.get(), t.get() + max_size);
    string s = "input" + to_string(ind) + ".bin";
    (ind)++;
    ofstream fout(s, ios::binary | ios::out);
    if (!fout) {
        remove(s.c_str());
        error = true;
        return "";
    }
    fout.write((char *) t.get(), sizeof(t));
    return s;
}

string process(ifstream &fin, atomic<int> &ind, atomic<bool> &error, atomic<bool> &end_file) {
    vector<string> files;
    while (!end_file) {
        string s = fill_files(fin, ind, error, end_file);
        if (error) {
            return "";
        }
        if (s == "")
            break;
        files.push_back(move(s));
    }
    string tmp = process_merge(files, error);
    return tmp;
}

string merge(const string &s1, const string &s2, atomic<bool> &error) {
    ifstream f1(s1, ios::binary | ios::in);
    ifstream f2(s2, ios::binary | ios::in);
    string s3 = "input" + s1.substr(5, s1.length() - 9) + "_" + s2.substr(5, s2.length() - 9) + ".bin";
    ofstream fout(s3, ios::binary | ios::out);
    if (!f1 || !f2 || !fout) {
        remove(s3.c_str());
        error = true;
        return "";
    }
    int64_t a, b;
    f1.read((char *) &a, sizeof(int64_t));
    f2.read((char *) &b, sizeof(int64_t));
    while (true) {
        if (a > b) {
            fout.write((char *) &b, sizeof(int64_t));
            if (!f2.read((char *) &b, sizeof(int64_t))) {
                fout.write((char *) &a, sizeof(int64_t));
                break;
            }
        } else {
            fout.write((char *) &a, sizeof(int64_t));
            if (f1.read((char *) &a, sizeof(int64_t))) {
                fout.write((char *) &b, sizeof(int64_t));
                break;
            }
        }
    }

    while (f1.read((char *) &a, sizeof(int64_t))) {
        fout.write((char *) &a, sizeof(int64_t));
    }
    while (f2.read((char *) &b, sizeof(int64_t))) {
        fout.write((char *) &b, sizeof(int64_t));
    }
    return s3;
}

string process_merge(vector<string> &files, atomic<bool> &error) {
    int r = files.size();
    vector<string> f(2 * (r));
    f[f.size() - 1] = "";
    int off = r - 1;
    for (int i = 0; i != r; ++i) {
        f[i + off] = files[i];
    }
    for (int i = off - 1; i >= 0; i--) {
        if (f[2 * i + 2] == "")
            f[i] = f[2 * i + 1];
        else if (f[2 * i + 1] == "")
            f[i] = f[2 * i + 2];
        else
            f[i] = merge(f[2 * i + 1], f[2 * i + 2], error);
        if (error) {
            break;
        }
    }
    for (int i = 1; i < f.size(); ++i) {
        remove(f[i].c_str());
    }
    if (error)
        return "";
    return f[0];
}


int main() {
    atomic<int> ind(0);
    atomic<bool> error(false);
    atomic<bool> end_file(false);

    ifstream fin("input.bin", ios::binary | ios::in);
    if (!fin.is_open()) {
        cout << "File couldn't be opened";
        return -1;
    }

    future<string> t1 = async(process, ref(fin), ref(ind), ref(error), ref(end_file));
    future<string> t2 = async(process, ref(fin), ref(ind), ref(error), ref(end_file));
    string s1 = t1.get();
    string s2 = t2.get();
    string s = merge(s1, s2, error);
    remove(s1.c_str());
    remove(s2.c_str());


    if (check_sort(s, error))
        cout << "ALL Right\n";
    if (error) {
        cout << "File couldn't be opened";
        return -1;
    }
    cout << "Your file: " << s;
    return 0;
}
