#pragma once
#include "application/JuegoDomino.hpp"
#include "domain/ConfiguracionJuego.hpp"
#include <nlohmann/json.hpp>
#include <bits/stdc++.h>
using namespace std;

namespace domino {

class PersistenciaPartidaJson {
public:
    static bool guardar(const JuegoDomino& juego, const string& archivo) {
        const auto& config = juego.getConfiguracion();
        nlohmann::json j;
        j["modo"] = (config.modo == MODO_SOLO) ? "SOLO" : "CUATRO";
        j["puntosParaGanar"] = juego.getPuntosParaGanar();
        j["fichasPorJugador"] = juego.getFichasPorJugador();
        for (size_t i = 0; i < 4; ++i) {
            j["tipos"][i] = (config.tipos[i] == HUMANO) ? "H" : "IA";
            j["nombres"][i] = config.nombres[i];
        }
        j["turnoActual"] = juego.getTurnoActual();
        j["jugadorSalida"] = juego.getJugadorSalida();
        j["partidaTerminada"] = juego.isPartidaTerminada();
        j["rondaTerminada"] = juego.isRondaTerminada();
        j["huboTrancoUltimaRonda"] = juego.getHuboTrancoUltimaRonda();
        j["multiplicadorPuntos"] = juego.getMultiplicadorPuntos();
        j["ganadorPartida"] = juego.getGanadorPartida();
        j["ganadorRondaActual"] = juego.getGanadorRondaActual();
        j["rondaNumero"] = juego.getRondaNumero();
        for (size_t i = 0; i < juego.getJugadores().size(); ++i) {
            j["jugadores"][i]["nombre"] = juego.getJugadores()[i]->getNombre();
            j["jugadores"][i]["puntuacion"] = juego.getJugadores()[i]->getPuntuacion();
            j["jugadores"][i]["rondasGanadas"] = juego.getJugadores()[i]->getRondasGanadas();
            auto manoSimple = juego.getJugadores()[i]->getManoSimple();
            nlohmann::json manoJson = nlohmann::json::array();
            for (const auto& f : manoSimple)
                manoJson.push_back({f.first, f.second});
            j["jugadores"][i]["mano"] = manoJson;
        }
        nlohmann::json mesaJson;
        auto lineaSimple = juego.getMesa().getLineaSimple();
        nlohmann::json lineaJson = nlohmann::json::array();
        for (const auto& f : lineaSimple)
            lineaJson.push_back({f.first, f.second});
        mesaJson["linea"] = lineaJson;
        j["mesa"] = mesaJson;
        nlohmann::json jugadasJson = nlohmann::json::array();
        for (const auto& p : juego.getFichasJugadas())
            jugadasJson.push_back({p.first, p.second});
        j["fichasJugadas"] = jugadasJson;
        ofstream file(archivo);
        if (!file.is_open()) return false;
        try {
            file << j.dump(4);
        } catch (const exception& e) {
            return false;
        }
        return file.good();
    }

    static bool cargar(JuegoDomino& juego, const string& archivo) {
        ifstream file(archivo);
        if (!file.is_open()) return false;
        nlohmann::json j;
        try {
            file >> j;
        } catch (const exception& e) {
            return false;
        }
        if (!j.is_object()) return false;
        ConfiguracionJuego nueva;
        string modoStr;
        if (j.contains("modo") && j["modo"].is_string())
            modoStr = j["modo"].get<string>();
        else
            modoStr = "SOLO";
        nueva.modo = (modoStr == "CUATRO") ? MODO_CUATRO_JUGADORES : MODO_SOLO;
        nueva.tipos.clear();
        nueva.nombres.clear();
        for (int i = 0; i < 4; ++i) {
            string t = "H";
            if (j.contains("tipos") && j["tipos"].is_array() &&
                i < static_cast<int>(j["tipos"].size()) &&
                j["tipos"][i].is_string())
                t = j["tipos"][i].get<string>();
            nueva.tipos.push_back((t == "IA") ? IA : HUMANO);
            string n = "Jugador " + to_string(i + 1);
            if (j.contains("nombres") && j["nombres"].is_array() &&
                i < static_cast<int>(j["nombres"].size()) &&
                j["nombres"][i].is_string())
                n = j["nombres"][i].get<string>();
            nueva.nombres.push_back(n);
        }
        if (j.contains("puntosParaGanar") && j["puntosParaGanar"].is_number_integer())
            nueva.puntosParaGanar = j["puntosParaGanar"].get<int>();
        else
            nueva.puntosParaGanar = 100;
        if (nueva.puntosParaGanar < 10) nueva.puntosParaGanar = 10;
        if (nueva.puntosParaGanar > 500) nueva.puntosParaGanar = 500;
        if (j.contains("fichasPorJugador") && j["fichasPorJugador"].is_number_integer())
            nueva.fichasPorJugador = j["fichasPorJugador"].get<int>();
        else
            nueva.fichasPorJugador = 10;
        if (nueva.fichasPorJugador < 1) nueva.fichasPorJugador = 1;
        if (nueva.fichasPorJugador > 55) nueva.fichasPorJugador = 55;
        if (nueva.modo == MODO_SOLO) {
            nueva.tipos[0] = HUMANO;
            for (int i = 1; i < static_cast<int>(nueva.tipos.size()); ++i)
                nueva.tipos[i] = IA;
        }
        juego.setConfiguracion(nueva);
        juego.recrearJugadoresSegunConfig();
        if (j.contains("jugadores") && j["jugadores"].is_array()) {
            const auto& jugs = j["jugadores"];
            for (int i = 0; i < 4 && i < static_cast<int>(jugs.size()); ++i) {
                const auto& jj = jugs[i];
                if (!jj.is_object()) continue;
                if (jj.contains("puntuacion") && jj["puntuacion"].is_number_integer())
                    juego.setPuntuacionJugador(i, jj["puntuacion"].get<int>());
                if (jj.contains("rondasGanadas") && jj["rondasGanadas"].is_number_integer())
                    juego.setRondasGanadasJugador(i, jj["rondasGanadas"].get<int>());
                if (jj.contains("mano") && jj["mano"].is_array()) {
                    vector<pair<int,int>> manoDatos;
                    for (const auto& fp : jj["mano"]) {
                        if (fp.is_array() && fp.size() == 2 &&
                            fp[0].is_number_integer() && fp[1].is_number_integer()) {
                            int a = fp[0].get<int>();
                            int b = fp[1].get<int>();
                            if (a >= 0 && a <= 9 && b >= 0 && b <= 9)
                                manoDatos.push_back(make_pair(a, b));
                        }
                    }
                    if (i < static_cast<int>(juego.getJugadores().size()))
                        juego.getJugadores()[i]->cargarManoSimple(manoDatos);
                }
            }
        }
        if (j.contains("mesa") && j["mesa"].is_object() &&
            j["mesa"].contains("linea") && j["mesa"]["linea"].is_array()) {
            vector<pair<int,int>> lineaDatos;
            for (const auto& fp : j["mesa"]["linea"]) {
                if (fp.is_array() && fp.size() == 2 &&
                    fp[0].is_number_integer() && fp[1].is_number_integer()) {
                    int a = fp[0].get<int>();
                    int b = fp[1].get<int>();
                    if (a >= 0 && a <= 9 && b >= 0 && b <= 9)
                        lineaDatos.push_back(make_pair(a, b));
                }
            }
            juego.getMesa().cargarLineaSimple(lineaDatos);
        }
        if (j.contains("fichasJugadas") && j["fichasJugadas"].is_array()) {
            set<pair<int,int>> jugadasSet;
            for (const auto& fp : j["fichasJugadas"]) {
                if (fp.is_array() && fp.size() == 2 &&
                    fp[0].is_number_integer() && fp[1].is_number_integer()) {
                    int a = fp[0].get<int>();
                    int b = fp[1].get<int>();
                    if (a >= 0 && a <= 9 && b >= 0 && b <= 9)
                        jugadasSet.insert(make_pair(
                            min(a, b), max(a, b)));
                }
            }
            juego.setFichasJugadas(jugadasSet);
        }
        if (j.contains("turnoActual") && j["turnoActual"].is_number_integer())
            juego.setTurnoActual(j["turnoActual"].get<int>());
        if (j.contains("jugadorSalida") && j["jugadorSalida"].is_number_integer())
            juego.setJugadorSalida(j["jugadorSalida"].get<int>());
        if (j.contains("partidaTerminada") && j["partidaTerminada"].is_boolean())
            juego.setPartidaTerminada(j["partidaTerminada"].get<bool>());
        if (j.contains("rondaTerminada") && j["rondaTerminada"].is_boolean())
            juego.setRondaTerminada(j["rondaTerminada"].get<bool>());
        if (j.contains("huboTrancoUltimaRonda") && j["huboTrancoUltimaRonda"].is_boolean())
            juego.setHuboTrancoUltimaRonda(j["huboTrancoUltimaRonda"].get<bool>());
        if (j.contains("multiplicadorPuntos") && j["multiplicadorPuntos"].is_number_integer())
            juego.setMultiplicadorPuntos(j["multiplicadorPuntos"].get<int>());
        if (j.contains("ganadorPartida") && j["ganadorPartida"].is_number_integer())
            juego.setGanadorPartida(j["ganadorPartida"].get<int>());
        if (j.contains("ganadorRondaActual") && j["ganadorRondaActual"].is_number_integer())
            juego.setGanadorRondaActual(j["ganadorRondaActual"].get<int>());
        if (j.contains("rondaNumero") && j["rondaNumero"].is_number_integer())
            juego.setRondaNumero(j["rondaNumero"].get<int>());
        return true;
    }
};

}