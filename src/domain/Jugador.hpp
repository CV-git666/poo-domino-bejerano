#pragma once
#include "Pieza.hpp"
#include "Ficha.hpp"
#include <bits/stdc++.h>
using namespace std;

namespace domino {

class Jugador {
private:
    string nombre;
    vector<unique_ptr<Pieza>> mano;
    int puntuacion;
    int rondasGanadas;
public:
    explicit Jugador(const string& nombre)
        : nombre(nombre), puntuacion(0), rondasGanadas(0) {}

    const string& getNombre() const { return nombre; }

    void agregarFicha(unique_ptr<Pieza> ficha) {
        mano.push_back(move(ficha));
    }

    void quitarFicha(int indice) {
        if (indice >= 0 && indice < static_cast<int>(mano.size()))
            mano.erase(mano.begin() + indice);
    }

    vector<unique_ptr<Pieza>>& getMano() { return mano; }
    const vector<unique_ptr<Pieza>>& getMano() const { return mano; }

    int getPuntuacion() const { return puntuacion; }
    void sumarPuntos(int puntos) { puntuacion += puntos; }
    void setPuntuacion(int puntos) {
        if (puntos >= 0) puntuacion = puntos;
    }
    void incrementarRondasGanadas() { ++rondasGanadas; }
    void setRondasGanadas(int n) {
        if (n >= 0) rondasGanadas = n;
    }
    int getRondasGanadas() const { return rondasGanadas; }
    int getCantidadFichas() const { return static_cast<int>(mano.size()); }

    bool tieneFicha(int extremo) const {
        for (const auto& f : mano) {
            Ficha* ficha = dynamic_cast<Ficha*>(f.get());
            if (ficha && (ficha->getExtremoA() == extremo ||
                          ficha->getExtremoB() == extremo))
                return true;
        }
        return false;
    }

    bool tieneFichaEnExtremos(int izq, int der) const {
        for (const auto& f : mano) {
            Ficha* ficha = dynamic_cast<Ficha*>(f.get());
            if (!ficha) continue;
            int a = ficha->getExtremoA();
            int b = ficha->getExtremoB();
            if (a == izq || b == izq || a == der || b == der)
                return true;
        }
        return false;
    }

    void limpiarMano() { mano.clear(); }

    vector<pair<int,int>> getManoSimple() const {
        vector<pair<int,int>> resultado;
        for (const auto& f : mano) {
            Ficha* ficha = dynamic_cast<Ficha*>(f.get());
            if (ficha)
                resultado.push_back(ficha->getPar());
        }
        return resultado;
    }

    void cargarManoSimple(const vector<pair<int,int>>& datos) {
        mano.clear();
        for (const auto& p : datos)
            mano.push_back(unique_ptr<Pieza>(new Ficha(p.first, p.second)));
    }
};

}