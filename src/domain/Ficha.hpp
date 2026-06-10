#pragma once
#include "Pieza.hpp"
#include <bits/stdc++.h>
using namespace std;

namespace domino {

class Ficha : public Pieza {
private:
    int a;
    int b;
public:
    Ficha(int extremoA, int extremoB) : a(extremoA), b(extremoB) {}

    int getExtremoA() const { return a; }
    int getExtremoB() const { return b; }
    void setExtremoA(int val) { a = val; }
    void setExtremoB(int val) { b = val; }
    void girar() { int t = a; a = b; b = t; }
    bool esDoble() const { return a == b; }

    int getValorTotal() const override { return a + b; }

    string toString() const override {
        return to_string(a) + "|" + to_string(b);
    }

    unique_ptr<Pieza> clonar() const override {
        return unique_ptr<Pieza>(new Ficha(a, b));
    }

    pair<int,int> getPar() const {
        return (a < b) ? make_pair(a, b) : make_pair(b, a);
    }
};

}