#pragma once
#include <bits/stdc++.h>
using namespace std;

namespace domino {

class Pieza {
public:
    virtual ~Pieza() {}
    virtual int getValorTotal() const = 0;
    virtual string toString() const = 0;
    virtual unique_ptr<Pieza> clonar() const = 0;
};

}