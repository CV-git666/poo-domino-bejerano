#pragma once
#include <SFML/Graphics.hpp>
#include "domain/Pieza.hpp"
#include "domain/Ficha.hpp"
#include "domain/Jugador.hpp"
#include "domain/Mesa.hpp"
#include "domain/ReglasDomino.hpp"
#include "domain/ConfiguracionJuego.hpp"
#include "domain/MotorIA.hpp"
#include "application/JuegoDomino.hpp"
#include "interfaces/Tutorial.hpp"
#include "infrastructure/PersistenciaPartidaJson.hpp"
#include <bits/stdc++.h>
using namespace std;

namespace domino {

class VentanaSFML {
private:
    enum EstadoUI {
        ESTADO_TUTORIAL,
        ESTADO_MENU,
        ESTADO_PASAR_DISPOSITIVO,
        ESTADO_JUGANDO,
        ESTADO_FIN_RONDA,
        ESTADO_FIN_PARTIDA
    };

    sf::RenderWindow ventana;
    unique_ptr<JuegoDomino> juego;
    ModoJuego modoActual;

    sf::Font fuente;
    bool fuenteCargada;

    sf::Texture texFondo;
    sf::Image icono;
    sf::Texture texLogo;
    bool fondoCargado;
    bool iconoCargado;
    bool logoCargado;

    map<pair<int,int>, sf::Texture> texturasFichas;
    sf::Texture texturaFichaOculta;
    sf::Texture texturaGenerica;
    int texturasCargadas;
    int maxValor;

    sf::Sprite spriteFondo;

    EstadoUI estado;
    int slideActual;
    vector<SlideTutorial> slides;

    int fichaSeleccionada;
    int hoverFicha;
    bool esperandoLado;

    sf::Clock relojTransicion;
    bool esperandoAnimacionIA;
    static constexpr float DELAY_IA_SEGUNDOS = 1.0f;

    vector<string> logMensajes;
    static constexpr int LOG_MAX = 6;

    pair<int,int> ultimaFichaPares;
    int ultimoJugadorFicha;
    sf::Clock relojUltimaFicha;
    static constexpr float DESTACAR_FICHA_SEGUNDOS = 2.5f;

    sf::Clock relojPulso;
    static constexpr float PERIODO_PULSO_SEG = 0.9f;

    static constexpr float VIRTUAL_W = 1024.f;
    static constexpr float VIRTUAL_H = 720.f;

    mutable sf::Vector2f cachedVirtualMousePos;

    struct TilePlacement {
        int indiceEnCadena;
        int a, b;
        bool esDoble;
        float x, y, w, h;
        bool horizontal;
    };

    static constexpr float CADENA_NORMAL_W = 70.f;
    static constexpr float CADENA_NORMAL_H = 35.f;
    static constexpr float CADENA_DOBLE_W = 35.f;
    static constexpr float CADENA_DOBLE_H = 70.f;
    static constexpr float CADENA_ROW_GAP = 6.f;
    static constexpr float CADENA_MARGIN_X = 115.f;
    static constexpr float CADENA_BOARD_TOP = 150.f;
    static constexpr float CADENA_BOARD_BOTTOM = 495.f;
    static constexpr float CADENA_BOARD_LEFT = 110.f;
    static constexpr float CADENA_BOARD_RIGHT_MARGIN = 110.f;

    static constexpr float MANO_TILE_W = 35.f;
    static constexpr float MANO_TILE_H = 70.f;
    static constexpr float MANO_Y_CENTRO = 545.f;
    static constexpr float MANO_TOP = 495.f;
    static constexpr float MANO_BOTTOM = 595.f;

    static constexpr float BTN_Y = 670.f;
    static constexpr float BTN_H = 30.f;
    static constexpr float BTN_W = 110.f;
    static constexpr float BTN_SEP = 8.f;

    bool cargarFuente();
    void cargarTexturasFichas();
    sf::Texture* obtenerTexturaFicha(int a, int b);
    sf::Texture& obtenerTexturaOculta();

    void agregarLog(const string& msg);
    void limpiarLog();

    void iniciarJuego(ModoJuego modo);
    void salirAJuego();
    void salirAMenu();
    void mostrarSiguienteSlide();
    void mostrarSlideAnterior();

    vector<TilePlacement> calcularLayoutCadena() const;
    void dibujarFichaColocada(int a, int b, float x, float y,
                             float w, float h, bool horizontal,
                             const sf::Color& color = sf::Color::White,
                             bool destacar = false);

    void dibujarFondo();
    void dibujarMarco(float x, float y, float w, float h, sf::Color c,
                      float grosor = 2.f);
    void dibujarTextoCentrado(const string& s, float y,
                              unsigned size, sf::Color color,
                              bool bold = false);

    void dibujarPanelOponente(int posicionVisual, bool esSuTurno);
    void dibujarCadenaFichas();
    void dibujarIndicadoresLadoCadena();
    void dibujarManoActual();
    void dibujarIndicadoresEstado();
    void dibujarLogMensajes();
    void dibujarBotoneraJuego();
    void dibujarOverlayTransicion();
    void dibujarOverlayFin(bool esPartida);
    void dibujarPantallaTutorial();
    void dibujarPantallaMenu();
    void dibujarPantallaPasarDispositivo();

    void manejarEventos();
    void procesarClic(int x, int y);
    void procesarClicMenu(int x, int y);
    void procesarClicTutorial(int x, int y);
    void procesarClicJuego(int x, int y);
    void procesarClicPasarDispositivo(int x, int y);
    void procesarClicFin(int x, int y, bool esPartida);

    void actualizarIA();
    bool mouseEnRect(float rx, float ry, float rw, float rh, int mx, int my) const;
    void autoGuardarSiAplicable();

    vector<int> indicesManoOrdenados(const Jugador& jug, int izqMesa = -1, int derMesa = -1) const;
    int indiceOriginalEnManoOrdenada(const Jugador& jug, int idxOrdenado, int izqMesa = -1, int derMesa = -1) const;
    int indiceFichaEnManoOrdenada(const Jugador& jug, int idxOriginal, int izqMesa = -1, int derMesa = -1) const;

    float factorPulso() const;
    float obtenerEscalaUI() const;
    sf::Vector2f virtualToReal(float x, float y) const;
    void actualizarVistaVirtual();
    sf::Vector2f ventanaAVirtual(int mx, int my) const;

public:
    VentanaSFML(int ancho, int alto, const string& titulo);
    void ejecutar();
};

inline bool VentanaSFML::cargarFuente() {
    if (fuente.loadFromFile("assets/arial.ttf")) return true;
    if (fuente.loadFromFile("C:/Windows/Fonts/arial.ttf")) return true;
    if (fuente.loadFromFile("C:/Windows/Fonts/consola.ttf")) return true;
    if (fuente.loadFromFile("C:/Windows/Fonts/segoeui.ttf")) return true;
    if (fuente.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf")) return true;
    return false;
}

inline void VentanaSFML::cargarTexturasFichas() {
    fondoCargado = texFondo.loadFromFile("assets/fondo.png");
    iconoCargado = icono.loadFromFile("assets/logo.png");
    if (fondoCargado) spriteFondo.setTexture(texFondo);
    logoCargado = false;
    if (iconoCargado) {
        logoCargado = texLogo.loadFromImage(icono);
    }

    maxValor = 9;
    for (int a = 0; a <= maxValor; ++a) {
        for (int b = a; b <= maxValor; ++b) {
            string path = "assets/fichas/ficha_" +
                               to_string(a) + "_" +
                               to_string(b) + ".png";
            sf::Texture tex;
            if (tex.loadFromFile(path)) {
                texturasFichas[make_pair(a, b)] = move(tex);
                ++texturasCargadas;
            }
        }
    }
    if (!texturaFichaOculta.loadFromFile("assets/fichas/ficha_oculta.png")) {
        sf::Image img;
        img.create(60, 80, sf::Color(245, 245, 245));
        texturaFichaOculta.loadFromImage(img);
    }
    if (!texturasCargadas) {
        sf::Image imgGen;
        imgGen.create(60, 80, sf::Color(250, 250, 250));
        texturaGenerica.loadFromImage(imgGen);
    }
}

inline sf::Texture* VentanaSFML::obtenerTexturaFicha(int a, int b) {
    int mn = a < b ? a : b;
    int mx = a < b ? b : a;
    auto it = texturasFichas.find(make_pair(mn, mx));
    if (it != texturasFichas.end()) return &(it->second);
    if (texturasCargadas) return nullptr;
    return &texturaGenerica;
}

inline sf::Texture& VentanaSFML::obtenerTexturaOculta() {
    return texturaFichaOculta;
}

inline void VentanaSFML::dibujarFichaColocada(int a, int b, float x, float y,
                                              float w, float h, bool horizontal,
                                              const sf::Color& color,
                                              bool destacar) {
    sf::Texture* tex = obtenerTexturaFicha(a, b);
    if (!tex) {
        sf::RectangleShape r(sf::Vector2f(w, h));
        r.setPosition(x, y);
        r.setFillColor(sf::Color(120, 120, 120));
        ventana.draw(r);
    } else {
        sf::Sprite sp;
        sp.setTexture(*tex);
        if (horizontal) {
            sp.setOrigin(tex->getSize().x / 2.f, tex->getSize().y / 2.f);
            sp.setScale(h / tex->getSize().x, w / tex->getSize().y);
            float rot = -90.f;
            if (a > b) rot += 180.f;
            sp.setRotation(rot);
            sp.setPosition(x + w / 2.f, y + h / 2.f);
        } else {
            sp.setOrigin(tex->getSize().x / 2.f, tex->getSize().y / 2.f);
            sp.setScale(w / tex->getSize().x, h / tex->getSize().y);
            if (a > b) sp.setRotation(180.f);
            sp.setPosition(x + w / 2.f, y + h / 2.f);
        }
        sp.setColor(color);
        ventana.draw(sp);
    }
    sf::RectangleShape borde(sf::Vector2f(w, h));
    borde.setPosition(x, y);
    borde.setFillColor(sf::Color(0, 0, 0, 0));
    float grosorBorde = destacar ? 3.5f : 1.5f;
    borde.setOutlineThickness(grosorBorde);
    sf::Color colorBorde = destacar ? sf::Color(255, 230, 60)
                                     : sf::Color(30, 20, 15);
    borde.setOutlineColor(colorBorde);
    ventana.draw(borde);

    if (destacar) {
        sf::RectangleShape glow(sf::Vector2f(w + 8, h + 8));
        glow.setPosition(x - 4, y - 4);
        glow.setFillColor(sf::Color(0, 0, 0, 0));
        glow.setOutlineThickness(2.5f);
        glow.setOutlineColor(sf::Color(255, 230, 60, 130));
        ventana.draw(glow);
    }
}

inline vector<VentanaSFML::TilePlacement>
VentanaSFML::calcularLayoutCadena() const {
    vector<TilePlacement> resultado;
    const auto& linea = juego->getMesa().getLinea();
    if (linea.empty()) return resultado;

    float W = static_cast<float>(ventana.getSize().x);
    float maxChainW = W - CADENA_MARGIN_X - CADENA_BOARD_RIGHT_MARGIN;
    float availableH = CADENA_BOARD_BOTTOM - CADENA_BOARD_TOP;
    if (maxChainW < CADENA_NORMAL_W) maxChainW = CADENA_NORMAL_W;
    if (availableH < CADENA_NORMAL_H) availableH = CADENA_NORMAL_H;

    vector<vector<int>> rows;
    vector<float> rowHeights;
    {
        float curX = 0.f;
        rows.push_back({});
        rowHeights.push_back(0.f);
        for (size_t i = 0; i < linea.size(); ++i) {
            Ficha* f = dynamic_cast<Ficha*>(linea[i].get());
            if (!f) continue;
            bool doble = f->esDoble();
            float w = doble ? CADENA_DOBLE_W : CADENA_NORMAL_W;
            float h = doble ? CADENA_DOBLE_H : CADENA_NORMAL_H;

            bool wouldOverflow = (curX + w > maxChainW);
            if (wouldOverflow && !rows.back().empty()) {
                rows.push_back({});
                rowHeights.push_back(0.f);
                curX = 0.f;
            }
            rows.back().push_back(static_cast<int>(i));
            rowHeights.back() = max(rowHeights.back(), h);
            curX += w;
        }
    }

    float gap = CADENA_ROW_GAP;
    float totalH = 0.f;
    for (size_t r = 0; r < rows.size(); ++r) {
        totalH += rowHeights[r];
        if (r + 1 < rows.size()) totalH += gap;
    }
    if (totalH > availableH && rows.size() > 1) {
        float overflow = totalH - availableH;
        float reducible = gap * (rows.size() - 1);
        if (reducible > overflow) {
            gap = (reducible - overflow) / (rows.size() - 1);
            if (gap < 0) gap = 0;
            totalH = availableH;
        }
    }
    float startY = CADENA_BOARD_TOP + max(0.f, (availableH - totalH) / 2.f);

    float y = startY;
    for (size_t r = 0; r < rows.size(); ++r) {
        float rowW = 0.f;
        for (int idx : rows[r]) {
            Ficha* f = dynamic_cast<Ficha*>(linea[idx].get());
            if (!f) continue;
            rowW += f->esDoble() ? CADENA_DOBLE_W : CADENA_NORMAL_W;
        }
        float x = CADENA_MARGIN_X + (maxChainW - rowW) / 2.f;
        for (int idx : rows[r]) {
            Ficha* f = dynamic_cast<Ficha*>(linea[idx].get());
            if (!f) continue;
            bool doble = f->esDoble();
            float w = doble ? CADENA_DOBLE_W : CADENA_NORMAL_W;
            float h = doble ? CADENA_DOBLE_H : CADENA_NORMAL_H;
            float yOff = (rowHeights[r] - h) / 2.f;
            resultado.push_back({idx, f->getExtremoA(), f->getExtremoB(),
                                 doble, x, y + yOff, w, h, !doble});
            x += w;
        }
        y += rowHeights[r] + gap;
    }
    return resultado;
}

inline void VentanaSFML::agregarLog(const string& msg) {
    logMensajes.push_back(msg);
    while (static_cast<int>(logMensajes.size()) > LOG_MAX)
        logMensajes.erase(logMensajes.begin());
}

inline void VentanaSFML::limpiarLog() {
    logMensajes.clear();
}

inline void VentanaSFML::iniciarJuego(ModoJuego modo) {
    modoActual = modo;
    ConfiguracionJuego cfg = (modo == MODO_SOLO)
        ? ConfiguracionJuego::solo()
        : ConfiguracionJuego::cuatroJugadores();
    juego.reset(new JuegoDomino(cfg));
    fichaSeleccionada = -1;
    hoverFicha = -1;
    esperandoLado = false;
    esperandoAnimacionIA = false;
    ultimaFichaPares = make_pair(-1, -1);
    ultimoJugadorFicha = -1;
    limpiarLog();
    agregarLog("Nueva partida: " +
        string(modo == MODO_SOLO ? "Modo Solo" : "Modo 4 Jugadores"));
    agregarLog("Sale J" + to_string(juego->getJugadorSalida() + 1) +
               " - " + juego->getJugadores()[juego->getJugadorSalida()]->getNombre());

    if (juego->getModo() == MODO_CUATRO_JUGADORES) {
        estado = ESTADO_PASAR_DISPOSITIVO;
        agregarLog("Turno J" + to_string(juego->getTurnoActual() + 1) +
                   " - Pasa el dispositivo");
    } else {
        estado = ESTADO_JUGANDO;
        if (!juego->esHumano(juego->getTurnoActual())) {
            esperandoAnimacionIA = true;
            relojTransicion.restart();
        }
    }
}

inline void VentanaSFML::salirAJuego() {
    if (!juego) iniciarJuego(modoActual);
    estado = ESTADO_JUGANDO;
    if (!juego->esHumano(juego->getTurnoActual())) {
        esperandoAnimacionIA = true;
        relojTransicion.restart();
    }
}

inline void VentanaSFML::salirAMenu() {
    autoGuardarSiAplicable();
    fichaSeleccionada = -1;
    hoverFicha = -1;
    esperandoLado = false;
    esperandoAnimacionIA = false;
    ultimaFichaPares = make_pair(-1, -1);
    ultimoJugadorFicha = -1;
    limpiarLog();
    estado = ESTADO_MENU;
    juego.reset();
}

inline void VentanaSFML::mostrarSiguienteSlide() {
    if (slideActual < (int)slides.size() - 1) {
        ++slideActual;
    } else {
        salirAMenu();
    }
}

inline void VentanaSFML::mostrarSlideAnterior() {
    if (slideActual > 0) --slideActual;
}

inline void VentanaSFML::dibujarFondo() {
    if (fondoCargado) {
        sf::Vector2u sz = ventana.getSize();
        spriteFondo.setScale(
            static_cast<float>(sz.x) / texFondo.getSize().x,
            static_cast<float>(sz.y) / texFondo.getSize().y);
        ventana.draw(spriteFondo);
    } else {
        ventana.clear(sf::Color(15, 60, 30));
    }
}

inline void VentanaSFML::dibujarMarco(float x, float y, float w, float h,
                                      sf::Color c, float grosor) {
    sf::RectangleShape r(sf::Vector2f(w, h));
    r.setPosition(x, y);
    r.setFillColor(sf::Color(0, 0, 0, 0));
    r.setOutlineThickness(grosor);
    r.setOutlineColor(c);
    ventana.draw(r);
}

inline void VentanaSFML::dibujarTextoCentrado(const string& s, float y,
                                              unsigned size, sf::Color color,
                                              bool bold) {
    sf::Text t;
    t.setFont(fuente);
    t.setString(s);
    t.setCharacterSize(size);
    t.setFillColor(color);
    if (bold) t.setStyle(sf::Text::Bold);
    sf::FloatRect b = t.getLocalBounds();
    t.setPosition((ventana.getSize().x - b.width) / 2.f, y);
    ventana.draw(t);
}

inline void VentanaSFML::dibujarPanelOponente(int posicionVisual,
                                              bool esSuTurno) {
    int idx = juego->getJugadorEnPosicionVisual(posicionVisual);
    if (idx < 0) return;
    const Jugador& jug = *juego->getJugadores()[idx];
    int cantFichas = jug.getCantidadFichas();
    bool esIA = !juego->esHumano(idx);

    float W = static_cast<float>(ventana.getSize().x);
    sf::Vector2f tamPanel;
    sf::Vector2f posPanel;
    if (posicionVisual == JuegoDomino::NORTE) {
        tamPanel = sf::Vector2f(W - 220, 75);
        posPanel = sf::Vector2f(110, 65);
    } else if (posicionVisual == JuegoDomino::ESTE) {
        tamPanel = sf::Vector2f(90, 340);
        posPanel = sf::Vector2f(W - 100, 155);
    } else if (posicionVisual == JuegoDomino::OESTE) {
        tamPanel = sf::Vector2f(90, 340);
        posPanel = sf::Vector2f(10, 155);
    } else {
        return;
    }

    sf::RectangleShape panel(tamPanel);
    panel.setPosition(posPanel);
    sf::Color colorFondo = esSuTurno
        ? sf::Color(45, 85, 55, 230)
        : sf::Color(25, 35, 55, 210);
    panel.setFillColor(colorFondo);
    float grosorBorde = esSuTurno ? 3.0f : 2.0f;
    panel.setOutlineThickness(grosorBorde);
    panel.setOutlineColor(esSuTurno
        ? sf::Color(140, 255, 170)
        : sf::Color(80, 100, 130));
    ventana.draw(panel);

    if (posicionVisual == JuegoDomino::NORTE) {
        sf::Text nombre;
        nombre.setFont(fuente);
        nombre.setCharacterSize(20);
        nombre.setFillColor(sf::Color::White);
        nombre.setStyle(sf::Text::Bold);
        nombre.setString("J" + to_string(idx + 1) + " - " + jug.getNombre() +
                         (esIA ? " (IA)" : ""));
        nombre.setPosition(posPanel.x + 15, posPanel.y + 8);
        ventana.draw(nombre);

        sf::Text info;
        info.setFont(fuente);
        info.setCharacterSize(14);
        info.setFillColor(sf::Color(200, 230, 200));
        stringstream ss;
        ss << "Fichas: " << cantFichas;
        info.setString(ss.str());
        info.setPosition(posPanel.x + 15, posPanel.y + 35);
        ventana.draw(info);

        const float ANCHO = 22.f, ALTO = 30.f, ESP = 2.f;
        float total = cantFichas * (ANCHO + ESP) - ESP;
        float x0 = posPanel.x + tamPanel.x / 2 - total / 2;
        float y = posPanel.y + 56;
        int maxMostrar = min(cantFichas, 28);
        for (int i = 0; i < maxMostrar; ++i) {
            sf::Sprite sp;
            sf::Texture& tex = obtenerTexturaOculta();
            sp.setTexture(tex);
            float px = x0 + i * (ANCHO + ESP);
            sp.setPosition(px, y);
            sp.setScale(ANCHO / tex.getSize().x, ALTO / tex.getSize().y);
            ventana.draw(sp);
            sf::RectangleShape borde(sf::Vector2f(ANCHO, ALTO));
            borde.setPosition(px, y);
            borde.setFillColor(sf::Color(0, 0, 0, 0));
            borde.setOutlineThickness(1.f);
            borde.setOutlineColor(sf::Color(50, 30, 20));
            ventana.draw(borde);
        }
    } else {
        sf::Text nombre;
        nombre.setFont(fuente);
        nombre.setCharacterSize(17);
        nombre.setFillColor(sf::Color::White);
        nombre.setStyle(sf::Text::Bold);
        nombre.setString("J" + to_string(idx + 1));
        nombre.setPosition(posPanel.x + 8, posPanel.y + 5);
        ventana.draw(nombre);

        sf::Text tipo;
        tipo.setFont(fuente);
        tipo.setCharacterSize(13);
        tipo.setFillColor(esIA ? sf::Color(255, 180, 100) : sf::Color(180, 220, 255));
        tipo.setString(esIA ? "IA" : "Humano");
        tipo.setPosition(posPanel.x + 8, posPanel.y + 25);
        ventana.draw(tipo);

        sf::Text info;
        info.setFont(fuente);
        info.setCharacterSize(13);
        info.setFillColor(sf::Color(200, 230, 200));
        stringstream ss;
        ss << cantFichas << " fichas\n";
        info.setString(ss.str());
        info.setPosition(posPanel.x + 8, posPanel.y + 45);
        ventana.draw(info);

        const float ANCHO = 22.f, ALTO = 28.f;
        const float STRIDE = 7.f;
        float x = posPanel.x + (tamPanel.x - ANCHO) / 2;
        float y0 = posPanel.y + 90;
        int maxMostrar = min(cantFichas, 10);
        for (int i = 0; i < maxMostrar; ++i) {
            sf::Sprite sp;
            sf::Texture& tex = obtenerTexturaOculta();
            sp.setTexture(tex);
            float px = x;
            float py = y0 + i * STRIDE;
            sp.setPosition(px, py);
            sp.setScale(ANCHO / tex.getSize().x, ALTO / tex.getSize().y);
            ventana.draw(sp);
            sf::RectangleShape borde(sf::Vector2f(ANCHO, ALTO));
            borde.setPosition(px, py);
            borde.setFillColor(sf::Color(0, 0, 0, 0));
            borde.setOutlineThickness(1.f);
            borde.setOutlineColor(sf::Color(50, 30, 20));
            ventana.draw(borde);
        }
    }
}

inline void VentanaSFML::dibujarCadenaFichas() {
    const auto& linea = juego->getMesa().getLinea();
    if (linea.empty()) return;
    vector<TilePlacement> layout = calcularLayoutCadena();
    bool destacarActivo = relojUltimaFicha.getElapsedTime().asSeconds() < DESTACAR_FICHA_SEGUNDOS;
    for (const auto& p : layout) {
        bool esUltima = destacarActivo &&
                        p.a == ultimaFichaPares.first &&
                        p.b == ultimaFichaPares.second;
        dibujarFichaColocada(p.a, p.b, p.x, p.y, p.w, p.h, p.horizontal,
                             sf::Color::White, esUltima);
    }
}

inline void VentanaSFML::dibujarIndicadoresLadoCadena() {
    if (juego->getMesa().vacia()) return;
    vector<TilePlacement> layout = calcularLayoutCadena();
    if (layout.empty()) return;
    int izq = juego->getMesa().getExtremoIzquierdo();
    int der = juego->getMesa().getExtremoDerecho();

    const TilePlacement& first = layout.front();
    const TilePlacement& last = layout.back();

    {
        sf::Text tIzq;
        tIzq.setFont(fuente);
        tIzq.setString("< IZQ " + to_string(izq));
        tIzq.setCharacterSize(15);
        tIzq.setStyle(sf::Text::Bold);
        tIzq.setFillColor(sf::Color(220, 240, 255));
        sf::FloatRect b = tIzq.getLocalBounds();
        float tx = first.x - b.width - 8;
        float ty;
        if (first.horizontal) ty = first.y + first.h / 2 - b.height / 2 - 4;
        else ty = first.y - 18;
        if (tx > 0) {
            tIzq.setPosition(tx, ty);
            ventana.draw(tIzq);
        }
    }
    {
        sf::Text tDer;
        tDer.setFont(fuente);
        tDer.setString(to_string(der) + " DER >");
        tDer.setCharacterSize(15);
        tDer.setStyle(sf::Text::Bold);
        tDer.setFillColor(sf::Color(255, 220, 140));
        sf::FloatRect b = tDer.getLocalBounds();
        float tx = last.x + last.w + 8;
        float ty;
        if (last.horizontal) ty = last.y + last.h / 2 - b.height / 2 - 4;
        else ty = last.y - 18;
        if (tx + b.width < ventana.getSize().x) {
            tDer.setPosition(tx, ty);
            ventana.draw(tDer);
        }
    }
}

inline vector<int> VentanaSFML::indicesManoOrdenados(const Jugador& jug, int izqMesa, int derMesa) const {
    vector<int> idxs;
    const auto& mano = jug.getMano();
    idxs.reserve(mano.size());
    for (size_t i = 0; i < mano.size(); ++i) idxs.push_back(static_cast<int>(i));
    bool mesaVacia = (izqMesa == -1 && derMesa == -1);
    
    sort(idxs.begin(), idxs.end(), [&](int x, int y) {
        Ficha* fx = dynamic_cast<Ficha*>(mano[x].get());
        Ficha* fy = dynamic_cast<Ficha*>(mano[y].get());
        if (!fx || !fy) return x < y;
        
        bool xJugable = false, yJugable = false;
        if (!mesaVacia) {
            xJugable = (fx->getExtremoA() == izqMesa || fx->getExtremoB() == izqMesa ||
                        fx->getExtremoA() == derMesa || fx->getExtremoB() == derMesa);
            yJugable = (fy->getExtremoA() == izqMesa || fy->getExtremoB() == izqMesa ||
                        fy->getExtremoA() == derMesa || fy->getExtremoB() == derMesa);
        }
        
        if (xJugable != yJugable) return xJugable;
        
        int sx = fx->getValorTotal();
        int sy = fy->getValorTotal();
        if (sx != sy) return sx < sy;
        if (fx->esDoble() != fy->esDoble()) return fx->esDoble();
        if (fx->getExtremoA() != fy->getExtremoA())
            return fx->getExtremoA() < fy->getExtremoA();
        return fx->getExtremoB() < fy->getExtremoB();
    });
    return idxs;
}

inline int VentanaSFML::indiceOriginalEnManoOrdenada(const Jugador& jug, int idxOrdenado, int izqMesa, int derMesa) const {
    vector<int> ord = indicesManoOrdenados(jug, izqMesa, derMesa);
    if (idxOrdenado >= 0 && idxOrdenado < (int)ord.size()) return ord[idxOrdenado];
    return -1;
}

inline int VentanaSFML::indiceFichaEnManoOrdenada(const Jugador& jug, int idxOriginal, int izqMesa, int derMesa) const {
    vector<int> ord = indicesManoOrdenados(jug, izqMesa, derMesa);
    for (size_t i = 0; i < ord.size(); ++i)
        if (ord[i] == idxOriginal) return static_cast<int>(i);
    return -1;
}

inline void VentanaSFML::dibujarManoActual() {
    if (estado != ESTADO_JUGANDO && estado != ESTADO_PASAR_DISPOSITIVO) return;
    int idx = juego->getJugadorEnPosicionVisual(JuegoDomino::SUR);
    if (idx < 0) return;
    const Jugador& jug = *juego->getJugadores()[idx];
    const auto& mano = jug.getMano();
    if (mano.empty()) return;

    float W = static_cast<float>(ventana.getSize().x);

    sf::RectangleShape panel(sf::Vector2f(W - 220, 100));
    panel.setPosition(110, MANO_TOP);
    panel.setFillColor(sf::Color(25, 35, 55, 220));
    panel.setOutlineThickness(2);
    panel.setOutlineColor(sf::Color(80, 100, 130));
    ventana.draw(panel);

    sf::RectangleShape franja(sf::Vector2f(W - 220, 22));
    franja.setPosition(110, MANO_TOP);
    franja.setFillColor(sf::Color(40, 55, 80, 240));
    ventana.draw(franja);

    sf::Text titulo;
    titulo.setFont(fuente);
    titulo.setCharacterSize(16);
    titulo.setStyle(sf::Text::Bold);
    titulo.setFillColor(sf::Color(180, 220, 255));
    titulo.setString("TU MANO  -  J" + to_string(idx + 1) +
                     "  -  " + jug.getNombre() +
                     "  (" + to_string(mano.size()) + " fichas)");
    titulo.setPosition(125, MANO_TOP + 4);
    ventana.draw(titulo);

    sf::Text turno;
    turno.setFont(fuente);
    turno.setCharacterSize(17);
    turno.setStyle(sf::Text::Bold);
    int idxTurno = juego->getTurnoActual();
    bool esMiTurno = (idxTurno == idx);
    stringstream ssT;
    if (esMiTurno) {
        float p = factorPulso();
        sf::Color col(255, static_cast<sf::Uint8>(200 + 55 * p), 100);
        turno.setFillColor(col);
        ssT << "* TU TURNO *";
    } else if (juego->esHumano(idxTurno)) {
        turno.setFillColor(sf::Color(180, 180, 200));
        ssT << "Turno: J" << (idxTurno + 1) << " (Humano)";
    } else {
        turno.setFillColor(sf::Color(255, 180, 100));
        ssT << "Turno: J" << (idxTurno + 1) << " (IA)";
    }
    turno.setString(ssT.str());
    sf::FloatRect tb = turno.getLocalBounds();
    turno.setPosition(W - 220 - tb.width - 5, MANO_TOP + 4);
    ventana.draw(turno);

    int izqMesa = juego->getMesa().getExtremoIzquierdo();
    int derMesa = juego->getMesa().getExtremoDerecho();
    bool mesaVacia = juego->getMesa().vacia();
    vector<int> ordenados = indicesManoOrdenados(jug, izqMesa, derMesa);
    int n = static_cast<int>(ordenados.size());

    float totalW = n * MANO_TILE_W;
    float x0 = W / 2.f - totalW / 2.f;
    float yFicha = MANO_Y_CENTRO - MANO_TILE_H / 2.f;

    bool esMiTurnoHumano = esMiTurno && juego->esHumano(idx);

    int posResaltada = -1;
    if (fichaSeleccionada >= 0) {
        posResaltada = indiceFichaEnManoOrdenada(jug, fichaSeleccionada, izqMesa, derMesa);
    }
    int posHover = hoverFicha;

    for (int pos = 0; pos < n; ++pos) {
        int idxOrig = ordenados[pos];
        Ficha* f = dynamic_cast<Ficha*>(jug.getMano()[idxOrig].get());
        if (!f) continue;
        float xCur = x0 + pos * MANO_TILE_W;
        bool jugable = false;
        if (esMiTurnoHumano) {
            if (mesaVacia) jugable = true;
            else jugable = (f->getExtremoA() == izqMesa || f->getExtremoB() == izqMesa ||
                            f->getExtremoA() == derMesa || f->getExtremoB() == derMesa);
        }
        sf::Color color = sf::Color::White;
        float drawY = yFicha;
        if (pos == posResaltada) {
            color = sf::Color(255, 255, 150);
            drawY -= 14;
        } else if (pos == posHover && !esperandoLado) {
            color = sf::Color(255, 255, 200);
            drawY -= 8;
        } else if (jugable) {
            color = sf::Color(220, 255, 220);
        } else if (esMiTurnoHumano) {
            color = sf::Color(160, 160, 170);
        } else {
            color = sf::Color(200, 200, 210);
        }
        dibujarFichaColocada(f->getExtremoA(), f->getExtremoB(),
                             xCur, drawY, MANO_TILE_W, MANO_TILE_H, false, color);
    }
}

inline void VentanaSFML::dibujarIndicadoresEstado() {
    float W = static_cast<float>(ventana.getSize().x);
    sf::RectangleShape panelTitulo(sf::Vector2f(W - 20, 50));
    panelTitulo.setPosition(10, 5);
    panelTitulo.setFillColor(sf::Color(20, 30, 50, 230));
    panelTitulo.setOutlineThickness(2);
    panelTitulo.setOutlineColor(sf::Color(120, 140, 180));
    ventana.draw(panelTitulo);

    sf::Text titulo;
    titulo.setFont(fuente);
    titulo.setCharacterSize(24);
    titulo.setFillColor(sf::Color(255, 220, 100));
    titulo.setStyle(sf::Text::Bold);
    stringstream sst;
    sst << "DOMINO ";
    if (juego->getModo() == MODO_SOLO) sst << "SOLO";
    else sst << "4 JUGADORES";
    sst << " - Doble-" << juego->getMaxValor();
    titulo.setString(sst.str());
    titulo.setPosition(25, 15);
    ventana.draw(titulo);

    sf::Text ronda;
    ronda.setFont(fuente);
    ronda.setCharacterSize(15);
    ronda.setFillColor(sf::Color::White);
    stringstream ssr;
    ssr << "Ronda #" << juego->getRondaNumero();
    ronda.setString(ssr.str());
    ronda.setPosition(W - 460, 8);
    ventana.draw(ronda);

    sf::Text meta;
    meta.setFont(fuente);
    meta.setCharacterSize(15);
    meta.setFillColor(sf::Color(180, 220, 255));
    stringstream ssm;
    ssm << "META: " << juego->getPuntosParaGanar() << " pts";
    meta.setString(ssm.str());
    meta.setPosition(W - 360, 8);
    ventana.draw(meta);

    sf::Text mult;
    mult.setFont(fuente);
    mult.setCharacterSize(15);
    mult.setStyle(sf::Text::Bold);
    stringstream ssmult;
    ssmult << "x" << juego->getMultiplicadorPuntos();
    mult.setString(ssmult.str());
    if (juego->getMultiplicadorPuntos() > 1) {
        float p = factorPulso();
        mult.setFillColor(sf::Color(static_cast<sf::Uint8>(255),
                                     static_cast<sf::Uint8>(100 * p + 80),
                                     static_cast<sf::Uint8>(100 * p + 80)));
    } else {
        mult.setFillColor(sf::Color(150, 150, 150));
    }
    mult.setPosition(W - 250, 8);
    ventana.draw(mult);

    sf::Text tranco;
    tranco.setFont(fuente);
    tranco.setCharacterSize(15);
    tranco.setStyle(sf::Text::Bold);
    if (juego->getHuboTrancoUltimaRonda()) {
        tranco.setString("*** TRANQUE ***");
        tranco.setFillColor(sf::Color(255, 80, 80));
    } else {
        tranco.setString("");
    }
    tranco.setPosition(W - 200, 8);
    ventana.draw(tranco);

    float W2 = static_cast<float>(ventana.getSize().x);
    sf::RectangleShape panelCentral(sf::Vector2f(W2 - 220, 340));
    panelCentral.setPosition(110, 155);
    sf::Color colorFondoMesa = juego->getMesa().vacia()
        ? sf::Color(20, 70, 40, 180)
        : sf::Color(15, 70, 35, 180);
    panelCentral.setFillColor(colorFondoMesa);
    panelCentral.setOutlineThickness(2);
    panelCentral.setOutlineColor(sf::Color(100, 140, 90));
    ventana.draw(panelCentral);

    sf::Text extremos;
    extremos.setFont(fuente);
    extremos.setCharacterSize(15);
    extremos.setStyle(sf::Text::Bold);
    sf::Color colorExt = juego->getMesa().vacia()
        ? sf::Color(255, 220, 130)
        : sf::Color(220, 240, 220);
    extremos.setFillColor(colorExt);
    stringstream ssExt;
    if (juego->getMesa().vacia()) {
        ssExt << "Mesa vacia - J" << (juego->getTurnoActual() + 1)
              << " coloca la primera ficha";
    } else {
        ssExt << "Izq: " << juego->getMesa().getExtremoIzquierdo()
              << "     Der: " << juego->getMesa().getExtremoDerecho()
              << "     Fichas en mesa: " << juego->getMesa().size()
              << " / 55";
    }
    extremos.setString(ssExt.str());
    sf::FloatRect bE = extremos.getLocalBounds();
    extremos.setPosition(110 + ((W2 - 220) - bE.width) / 2.f, 138);
    ventana.draw(extremos);
}

inline void VentanaSFML::dibujarLogMensajes() {
    float W = static_cast<float>(ventana.getSize().x);
    sf::RectangleShape panelLog(sf::Vector2f(W - 20, 30));
    panelLog.setPosition(10, 605);
    panelLog.setFillColor(sf::Color(20, 30, 50, 220));
    panelLog.setOutlineThickness(1);
    panelLog.setOutlineColor(sf::Color(80, 100, 130));
    ventana.draw(panelLog);

    if (!logMensajes.empty()) {
        sf::Text ultimo;
        ultimo.setFont(fuente);
        ultimo.setCharacterSize(16);
        ultimo.setFillColor(sf::Color(255, 255, 200));
        ultimo.setString("> " + logMensajes.back());
        ultimo.setPosition(20, 612);
        ventana.draw(ultimo);
    }
}

inline void VentanaSFML::dibujarBotoneraJuego() {
    float W = static_cast<float>(ventana.getSize().x);
    sf::RectangleShape panelBot(sf::Vector2f(W - 20, 70));
    panelBot.setPosition(10, 640);
    panelBot.setFillColor(sf::Color(20, 30, 50, 220));
    panelBot.setOutlineThickness(1);
    panelBot.setOutlineColor(sf::Color(80, 100, 130));
    ventana.draw(panelBot);

    int idxActual = juego->getTurnoActual();
    int idxSur = juego->getJugadorEnPosicionVisual(JuegoDomino::SUR);
    bool esMiTurno = (idxActual == idxSur) && juego->esHumano(idxActual);
    bool puedePasarAhora = esMiTurno && juego->puedePasar(idxActual);

    float btnY = BTN_Y;
    float btnH = BTN_H;
    float posX = 20;
    float sep = BTN_SEP;
    float ancho = BTN_W;

    auto dibujarBoton = [&](const string& texto, sf::Color color,
                            bool habilitado) {
        sf::RectangleShape btn(sf::Vector2f(ancho, btnH));
        btn.setPosition(posX, btnY);
        btn.setFillColor(habilitado ? color : sf::Color(80, 80, 80, 180));
        btn.setOutlineThickness(1);
        btn.setOutlineColor(sf::Color(20, 20, 30));
        ventana.draw(btn);

        sf::Text t;
        t.setFont(fuente);
        t.setString(texto);
        t.setCharacterSize(16);
        t.setStyle(sf::Text::Bold);
        t.setFillColor(habilitado ? sf::Color::White : sf::Color(180, 180, 180));
        t.setPosition(posX + 8, btnY + 5);
        ventana.draw(t);

        posX += ancho + sep;
    };

    sf::Color azul(60, 110, 160);
    sf::Color rojo(180, 70, 70);
    sf::Color verde(70, 150, 90);
    sf::Color gris(120, 120, 130);

    bool mesaVacia = juego->getMesa().vacia();

    if (esperandoLado && mesaVacia) {
        dibujarBoton("Jugar", verde, esMiTurno);
    } else if (esperandoLado) {
        dibujarBoton("<- Izq", azul, esMiTurno);
        dibujarBoton("Der ->", azul, esMiTurno);
    } else {
        dibujarBoton("Pasar", rojo, puedePasarAhora);
        dibujarBoton("<- Izq", azul, false);
        dibujarBoton("Der ->", azul, false);
    }
    dibujarBoton("Nueva Rnd", verde, estado == ESTADO_FIN_RONDA);
    dibujarBoton("Menu", gris, true);
}

inline void VentanaSFML::dibujarOverlayTransicion() {
    if (estado != ESTADO_PASAR_DISPOSITIVO) return;
    sf::RectangleShape overlay(sf::Vector2f(
        static_cast<float>(ventana.getSize().x),
        static_cast<float>(ventana.getSize().y)));
    overlay.setFillColor(sf::Color(0, 0, 0, 220));
    ventana.draw(overlay);

    int idx = juego->getTurnoActual();
    int idxAnterior = (idx + static_cast<int>(juego->getJugadores().size()) - 1)
                      % static_cast<int>(juego->getJugadores().size());
    string nombreActual = juego->getJugadores()[idx]->getNombre();
    string nombreAnterior = juego->getJugadores()[idxAnterior]->getNombre();

    float W = static_cast<float>(ventana.getSize().x);
    float cx = W / 2.f;

    sf::Text titulo;
    titulo.setFont(fuente);
    titulo.setCharacterSize(36);
    titulo.setFillColor(sf::Color(255, 220, 100));
    titulo.setStyle(sf::Text::Bold);
    stringstream ss;
    ss << "Pasa el dispositivo a J" << (idx + 1);
    titulo.setString(ss.str());
    sf::FloatRect b = titulo.getLocalBounds();
    titulo.setPosition((W - b.width) / 2.f, 130);
    ventana.draw(titulo);

    sf::Text sub;
    sub.setFont(fuente);
    sub.setCharacterSize(28);
    sub.setFillColor(sf::Color::White);
    sub.setStyle(sf::Text::Bold);
    sub.setString(nombreActual);
    sf::FloatRect b2 = sub.getLocalBounds();
    sub.setPosition((W - b2.width) / 2.f, 195);
    ventana.draw(sub);

    sf::RectangleShape cuadro(sf::Vector2f(560, 180));
    cuadro.setPosition(cx - 280, 250);
    cuadro.setFillColor(sf::Color(40, 50, 80, 220));
    cuadro.setOutlineThickness(2);
    cuadro.setOutlineColor(sf::Color(120, 140, 180));
    ventana.draw(cuadro);

    sf::Text antLbl;
    antLbl.setFont(fuente);
    antLbl.setCharacterSize(16);
    antLbl.setFillColor(sf::Color(180, 180, 200));
    antLbl.setString("Termino su turno:");
    sf::FloatRect bA = antLbl.getLocalBounds();
    antLbl.setPosition(cx - bA.width / 2.f, 265);
    ventana.draw(antLbl);

    sf::Text antNom;
    antNom.setFont(fuente);
    antNom.setCharacterSize(22);
    antNom.setFillColor(sf::Color(200, 200, 220));
    antNom.setString("J" + to_string(idxAnterior + 1) + " - " + nombreAnterior);
    sf::FloatRect bAn = antNom.getLocalBounds();
    antNom.setPosition(cx - bAn.width / 2.f, 290);
    ventana.draw(antNom);

    sf::Text sigLbl;
    sigLbl.setFont(fuente);
    sigLbl.setCharacterSize(16);
    sigLbl.setFillColor(sf::Color(180, 180, 200));
    sigLbl.setString("Siguiente turno:");
    sf::FloatRect bS = sigLbl.getLocalBounds();
    sigLbl.setPosition(cx - bS.width / 2.f, 335);
    ventana.draw(sigLbl);

    sf::Text sigNom;
    sigNom.setFont(fuente);
    sigNom.setCharacterSize(22);
    sigNom.setFillColor(sf::Color(255, 220, 100));
    sigNom.setStyle(sf::Text::Bold);
    sigNom.setString("J" + to_string(idx + 1) + " - " + nombreActual);
    sf::FloatRect bSn = sigNom.getLocalBounds();
    sigNom.setPosition(cx - bSn.width / 2.f, 360);
    ventana.draw(sigNom);

    sf::Text aviso;
    aviso.setFont(fuente);
    aviso.setCharacterSize(18);
    aviso.setFillColor(sf::Color(255, 200, 200));
    aviso.setString("Asegurate de que el siguiente jugador no vea tus fichas");
    sf::FloatRect bAv = aviso.getLocalBounds();
    aviso.setPosition((W - bAv.width) / 2.f, 460);
    ventana.draw(aviso);

    float btnY = 510;
    float btnW = 220, btnH = 60;
    sf::RectangleShape btn(sf::Vector2f(btnW, btnH));
    btn.setPosition(cx - btnW / 2, btnY);
    btn.setFillColor(sf::Color(70, 150, 90));
    btn.setOutlineThickness(2);
    btn.setOutlineColor(sf::Color(20, 20, 30));
    ventana.draw(btn);

    sf::Text listo;
    listo.setFont(fuente);
    listo.setString("LISTO");
    listo.setCharacterSize(28);
    listo.setStyle(sf::Text::Bold);
    listo.setFillColor(sf::Color::White);
    sf::FloatRect b4 = listo.getLocalBounds();
    listo.setPosition(cx - b4.width / 2, btnY + 13);
    ventana.draw(listo);
}

inline void VentanaSFML::dibujarOverlayFin(bool esPartida) {
    sf::RectangleShape overlay(sf::Vector2f(
        static_cast<float>(ventana.getSize().x),
        static_cast<float>(ventana.getSize().y)));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    ventana.draw(overlay);

    sf::Text titulo;
    titulo.setFont(fuente);
    titulo.setCharacterSize(36);
    titulo.setStyle(sf::Text::Bold);
    float W = static_cast<float>(ventana.getSize().x);

    if (esPartida) {
        int g = juego->getGanadorPartida();
        titulo.setFillColor(sf::Color(255, 220, 100));
        stringstream ss;
        ss << "*** PARTIDA TERMINADA ***";
        titulo.setString(ss.str());
        sf::FloatRect b = titulo.getLocalBounds();
        titulo.setPosition((W - b.width) / 2.f, 120);
        ventana.draw(titulo);

        sf::Text ganador;
        ganador.setFont(fuente);
        ganador.setCharacterSize(32);
        ganador.setFillColor(sf::Color(120, 255, 160));
        stringstream ssg;
        ssg << "Ganador: " << juego->getJugadores()[g]->getNombre();
        ganador.setString(ssg.str());
        sf::FloatRect b2 = ganador.getLocalBounds();
        ganador.setPosition((W - b2.width) / 2.f, 200);
        ventana.draw(ganador);

        sf::Text pts;
        pts.setFont(fuente);
        pts.setCharacterSize(22);
        pts.setFillColor(sf::Color(220, 220, 220));
        stringstream ssp;
        ssp << "Puntos: " << juego->getJugadores()[g]->getPuntuacion()
            << " / " << juego->getPuntosParaGanar();
        pts.setString(ssp.str());
        sf::FloatRect b3 = pts.getLocalBounds();
        pts.setPosition((W - b3.width) / 2.f, 250);
        ventana.draw(pts);

        sf::Text resumen;
        resumen.setFont(fuente);
        resumen.setCharacterSize(16);
        resumen.setFillColor(sf::Color(200, 200, 220));
        stringstream ssr;
        ssr << "Rondas: " << juego->getRondaNumero();
        resumen.setString(ssr.str());
        sf::FloatRect bR = resumen.getLocalBounds();
        resumen.setPosition((W - bR.width) / 2.f, 290);
        ventana.draw(resumen);

        sf::Text tabla;
        tabla.setFont(fuente);
        tabla.setCharacterSize(14);
        tabla.setFillColor(sf::Color(200, 200, 200));
        stringstream sst;
        for (size_t i = 0; i < juego->getJugadores().size(); ++i) {
            const auto& jj = juego->getJugadores()[i];
            sst << "J" << (i + 1) << " " << jj->getNombre() << "\n";
        }
        tabla.setString(sst.str());
        sf::FloatRect bT = tabla.getLocalBounds();
        tabla.setPosition((W - bT.width) / 2.f, 330);
        ventana.draw(tabla);

        float cx = W / 2.f;
        sf::RectangleShape btn(sf::Vector2f(220, 50));
        btn.setPosition(cx - 110, 470);
        btn.setFillColor(sf::Color(160, 60, 130));
        btn.setOutlineThickness(2);
        btn.setOutlineColor(sf::Color(20, 20, 30));
        ventana.draw(btn);
        sf::Text t;
        t.setFont(fuente);
        t.setString("Nueva Partida");
        t.setCharacterSize(20);
        t.setStyle(sf::Text::Bold);
        t.setFillColor(sf::Color::White);
        sf::FloatRect bb = t.getLocalBounds();
        t.setPosition(cx - bb.width / 2, 482);
        ventana.draw(t);

        sf::RectangleShape btn2(sf::Vector2f(220, 50));
        btn2.setPosition(cx - 110, 540);
        btn2.setFillColor(sf::Color(120, 120, 130));
        btn2.setOutlineThickness(2);
        btn2.setOutlineColor(sf::Color(20, 20, 30));
        ventana.draw(btn2);
        sf::Text t2;
        t2.setFont(fuente);
        t2.setString("Menu Principal");
        t2.setCharacterSize(20);
        t2.setStyle(sf::Text::Bold);
        t2.setFillColor(sf::Color::White);
        sf::FloatRect bb2 = t2.getLocalBounds();
        t2.setPosition(cx - bb2.width / 2, 552);
        ventana.draw(t2);
    } else {
        if (juego->getHuboTrancoUltimaRonda()) {
            titulo.setFillColor(sf::Color(255, 80, 80));
            titulo.setString("*** SE TRANCO ***");
        } else {
            titulo.setFillColor(sf::Color(120, 255, 120));
            titulo.setString("RONDA TERMINADA");
        }
        sf::FloatRect b = titulo.getLocalBounds();
        titulo.setPosition((W - b.width) / 2.f, 120);
        ventana.draw(titulo);

        if (!juego->getHuboTrancoUltimaRonda()) {
            int g = juego->getGanadorRondaActual();
            sf::Text ganador;
            ganador.setFont(fuente);
            ganador.setCharacterSize(26);
            ganador.setFillColor(sf::Color(220, 240, 255));
            stringstream ssg;
            ssg << "Ganador: " << juego->getJugadores()[g]->getNombre();
            ganador.setString(ssg.str());
            sf::FloatRect b2 = ganador.getLocalBounds();
            ganador.setPosition((W - b2.width) / 2.f, 200);
            ventana.draw(ganador);

            int pts = 0;
            for (size_t i = 0; i < juego->getJugadores().size(); ++i) {
                if ((int)i != g) {
                    int pm = 0;
                    const auto& m = juego->getJugadores()[i]->getMano();
                    for (const auto& ff : m) pm += ff->getValorTotal();
                    pts += pm;
                }
            }
            sf::Text ptsT;
            ptsT.setFont(fuente);
            ptsT.setCharacterSize(22);
            ptsT.setFillColor(sf::Color(255, 220, 100));
            stringstream ssp;
            ssp << "Puntos: +" << pts;
            if (juego->getMultiplicadorPuntos() > 1)
                ssp << " (x" << juego->getMultiplicadorPuntos() << " aplicado)";
            ptsT.setString(ssp.str());
            sf::FloatRect b3 = ptsT.getLocalBounds();
            ptsT.setPosition((W - b3.width) / 2.f, 250);
            ventana.draw(ptsT);
        } else {
            sf::Text info;
            info.setFont(fuente);
            info.setCharacterSize(20);
            info.setFillColor(sf::Color(255, 220, 220));
            info.setString("Empate en puntos - nadie gana la ronda");
            sf::FloatRect b2 = info.getLocalBounds();
            info.setPosition((W - b2.width) / 2.f, 200);
            ventana.draw(info);

            sf::Text mult;
            mult.setFont(fuente);
            mult.setCharacterSize(24);
            mult.setFillColor(sf::Color(255, 100, 100));
            mult.setStyle(sf::Text::Bold);
            mult.setString("Siguiente ronda vale x2");
            sf::FloatRect b3 = mult.getLocalBounds();
            mult.setPosition((W - b3.width) / 2.f, 250);
            ventana.draw(mult);
        }

        float cx = W / 2.f;
        sf::RectangleShape btn(sf::Vector2f(220, 50));
        btn.setPosition(cx - 110, 470);
        btn.setFillColor(sf::Color(70, 150, 90));
        btn.setOutlineThickness(2);
        btn.setOutlineColor(sf::Color(20, 20, 30));
        ventana.draw(btn);
        sf::Text t;
        t.setFont(fuente);
        t.setString("Nueva Ronda");
        t.setCharacterSize(20);
        t.setStyle(sf::Text::Bold);
        t.setFillColor(sf::Color::White);
        sf::FloatRect bb = t.getLocalBounds();
        t.setPosition(cx - bb.width / 2, 482);
        ventana.draw(t);

        sf::RectangleShape btn2(sf::Vector2f(220, 50));
        btn2.setPosition(cx - 110, 540);
        btn2.setFillColor(sf::Color(120, 120, 130));
        btn2.setOutlineThickness(2);
        btn2.setOutlineColor(sf::Color(20, 20, 30));
        ventana.draw(btn2);
        sf::Text t2;
        t2.setFont(fuente);
        t2.setString("Menu Principal");
        t2.setCharacterSize(20);
        t2.setStyle(sf::Text::Bold);
        t2.setFillColor(sf::Color::White);
        sf::FloatRect bb2 = t2.getLocalBounds();
        t2.setPosition(cx - bb2.width / 2, 552);
        ventana.draw(t2);
    }
}

inline void VentanaSFML::dibujarPantallaTutorial() {
    if (slides.empty()) return;
    const SlideTutorial& s = slides[slideActual];
    float W = static_cast<float>(ventana.getSize().x);

    sf::RectangleShape fondoPanel(sf::Vector2f(W - 40, 540));
    fondoPanel.setPosition(20, 60);
    fondoPanel.setFillColor(sf::Color(20, 30, 50, 230));
    fondoPanel.setOutlineThickness(2);
    fondoPanel.setOutlineColor(sf::Color(120, 140, 180));
    ventana.draw(fondoPanel);

    dibujarTextoCentrado("TUTORIAL - DOMINO", 75, 32, sf::Color(255, 220, 100), true);
    dibujarTextoCentrado(s.titulo, 130, 26, sf::Color::White, true);

    float y = 180;
    for (const auto& p : s.parrafos) {
        dibujarTextoCentrado(p, y, 18, sf::Color(220, 220, 220));
        y += 28;
    }

    if (!s.ejemplo.empty()) {
        sf::RectangleShape ej(sf::Vector2f(W - 200, 50));
        ej.setPosition(100, 380);
        ej.setFillColor(sf::Color(50, 90, 60, 220));
        ej.setOutlineThickness(2);
        ej.setOutlineColor(sf::Color(120, 200, 140));
        ventana.draw(ej);
        sf::Text t;
        t.setFont(fuente);
        t.setString("Ej: " + s.ejemplo);
        t.setCharacterSize(16);
        t.setFillColor(sf::Color(200, 240, 200));
        t.setStyle(sf::Text::Italic);
        sf::FloatRect b = t.getLocalBounds();
        t.setPosition((W - b.width) / 2.f, 395);
        ventana.draw(t);
    }

    stringstream ssp;
    ssp << "Slide " << (slideActual + 1) << " de " << slides.size();
    sf::Text p;
    p.setFont(fuente);
    p.setString(ssp.str());
    p.setCharacterSize(16);
    p.setFillColor(sf::Color(180, 180, 200));
    sf::FloatRect b = p.getLocalBounds();
    p.setPosition((W - b.width) / 2.f, 450);
    ventana.draw(p);

    float btnY = 490;
    float btnH = 50;
    float sep = 12;
    float posX = W / 2.f - 360;
    sf::Color gris(120, 120, 130);
    sf::Color azul(60, 110, 160);
    sf::Color verde(70, 150, 90);
    sf::Color naranja(180, 150, 60);

    if (slideActual > 0) {
        sf::RectangleShape btn(sf::Vector2f(160, btnH));
        btn.setPosition(posX, btnY);
        btn.setFillColor(gris);
        btn.setOutlineThickness(2);
        btn.setOutlineColor(sf::Color(20, 20, 30));
        ventana.draw(btn);
        sf::Text t;
        t.setFont(fuente);
        t.setString("< Atras");
        t.setCharacterSize(18);
        t.setStyle(sf::Text::Bold);
        t.setFillColor(sf::Color::White);
        sf::FloatRect b2 = t.getLocalBounds();
        t.setPosition(posX + (160 - b2.width) / 2, btnY + 14);
        ventana.draw(t);
    }
    posX += 160 + sep;

    {
        sf::RectangleShape btn(sf::Vector2f(160, btnH));
        btn.setPosition(posX, btnY);
        btn.setFillColor(naranja);
        btn.setOutlineThickness(2);
        btn.setOutlineColor(sf::Color(20, 20, 30));
        ventana.draw(btn);
        sf::Text t;
        t.setFont(fuente);
        t.setString("Saltar");
        t.setCharacterSize(18);
        t.setStyle(sf::Text::Bold);
        t.setFillColor(sf::Color::White);
        sf::FloatRect b2 = t.getLocalBounds();
        t.setPosition(posX + (160 - b2.width) / 2, btnY + 14);
        ventana.draw(t);
    }
    posX += 160 + sep;

    {
        sf::RectangleShape btn(sf::Vector2f(160, btnH));
        btn.setPosition(posX, btnY);
        btn.setFillColor(azul);
        btn.setOutlineThickness(2);
        btn.setOutlineColor(sf::Color(20, 20, 30));
        ventana.draw(btn);
        sf::Text t;
        t.setFont(fuente);
        string txt = (slideActual < (int)slides.size() - 1)
                          ? "Siguiente >" : "Menu";
        t.setString(txt);
        t.setCharacterSize(18);
        t.setStyle(sf::Text::Bold);
        t.setFillColor(sf::Color::White);
        sf::FloatRect b2 = t.getLocalBounds();
        t.setPosition(posX + (160 - b2.width) / 2, btnY + 14);
        ventana.draw(t);
    }
    posX += 160 + sep;

    {
        sf::RectangleShape btn(sf::Vector2f(160, btnH));
        btn.setPosition(posX, btnY);
        btn.setFillColor(verde);
        btn.setOutlineThickness(2);
        btn.setOutlineColor(sf::Color(20, 20, 30));
        ventana.draw(btn);
        sf::Text t;
        t.setFont(fuente);
        t.setString("Menu");
        t.setCharacterSize(18);
        t.setStyle(sf::Text::Bold);
        t.setFillColor(sf::Color::White);
        sf::FloatRect b2 = t.getLocalBounds();
        t.setPosition(posX + (160 - b2.width) / 2, btnY + 14);
        ventana.draw(t);
    }
}

inline void VentanaSFML::dibujarPantallaMenu() {
    float W = static_cast<float>(ventana.getSize().x);

    sf::RectangleShape fondo(sf::Vector2f(W - 40, 540));
    fondo.setPosition(20, 60);
    fondo.setFillColor(sf::Color(20, 30, 50, 230));
    fondo.setOutlineThickness(2);
    fondo.setOutlineColor(sf::Color(120, 140, 180));
    ventana.draw(fondo);

    if (logoCargado) {
        sf::Sprite logo(texLogo);
        float logoSize = 80.f;
        logo.setScale(logoSize / texLogo.getSize().x, logoSize / texLogo.getSize().y);
        logo.setPosition(W / 2.f - logoSize / 2.f, 30.f);
        ventana.draw(logo);
    }

    dibujarTextoCentrado("DOMINO", 120, 56, sf::Color(255, 220, 100), true);
    dibujarTextoCentrado("Doble-9 - 4 Jugadores", 180, 20, sf::Color(200, 220, 255));
    dibujarTextoCentrado("Selecciona una opcion", 210, 16, sf::Color(180, 180, 200));

    float btnW = 360, btnH = 45, sep = 8;
    float posX = W / 2.f - btnW / 2;
    float btnY = 240;
    sf::Color cSolo(60, 130, 90);
    sf::Color cCuatro(110, 80, 160);
    sf::Color cTutorial(60, 110, 160);
    sf::Color cCargar(110, 130, 80);
    sf::Color cSalir(180, 70, 70);
    sf::Color cBorde(20, 20, 30);

    auto dibujarBotonMenu = [&](const string& texto,
                                 sf::Color color, float y) {
        sf::RectangleShape btn(sf::Vector2f(btnW, btnH));
        btn.setPosition(posX, y);
        btn.setFillColor(color);
        btn.setOutlineThickness(2);
        btn.setOutlineColor(cBorde);
        ventana.draw(btn);

        sf::Text t;
        t.setFont(fuente);
        t.setString(texto);
        t.setCharacterSize(18);
        t.setStyle(sf::Text::Bold);
        t.setFillColor(sf::Color::White);
        sf::FloatRect b = t.getLocalBounds();
        t.setPosition(posX + (btnW - b.width) / 2, y + 12);
        ventana.draw(t);
    };

    dibujarBotonMenu("TUTORIAL", cTutorial, btnY);
    btnY += btnH + sep;
    dibujarBotonMenu("JUGAR SOLO (vs IA)", cSolo, btnY);
    btnY += btnH + sep;
    dibujarBotonMenu("4 JUGADORES (local)", cCuatro, btnY);
    btnY += btnH + sep;
    dibujarBotonMenu("CARGAR PARTIDA", cCargar, btnY);
    btnY += btnH + sep;
    dibujarBotonMenu("SALIR", cSalir, btnY);

    sf::Text ver;
    ver.setFont(fuente);
    ver.setString("Proyecto de POO");
    ver.setCharacterSize(13);
    ver.setFillColor(sf::Color(140, 140, 160));
    sf::FloatRect bv = ver.getLocalBounds();
    ver.setPosition((W - bv.width) / 2.f, 580);
    ventana.draw(ver);

    sf::Text pista;
    pista.setFont(fuente);
    pista.setString("Tip: ESC = volver al menu  |  P = pasar turno  |  Flechas = navegar tutorial");
    pista.setCharacterSize(13);
    pista.setFillColor(sf::Color(160, 160, 180));
    sf::FloatRect bp = pista.getLocalBounds();
    pista.setPosition((W - bp.width) / 2.f, 640);
    ventana.draw(pista);
}

inline bool VentanaSFML::mouseEnRect(float rx, float ry, float rw, float rh,
                                     int mx, int my) const {
    sf::Vector2f vpos = cachedVirtualMousePos;
    if (vpos.x == 0 && vpos.y == 0) {
        vpos = ventanaAVirtual(mx, my);
    }
    return vpos.x >= rx && vpos.x <= rx + rw && vpos.y >= ry && vpos.y <= ry + rh;
}

inline float VentanaSFML::factorPulso() const {
    float t = relojPulso.getElapsedTime().asSeconds();
    float fase = fmod(t, PERIODO_PULSO_SEG) / PERIODO_PULSO_SEG;
    return 0.5f + 0.5f * sin(fase * 6.2831853f);
}

inline float VentanaSFML::obtenerEscalaUI() const {
    sf::Vector2u sz = ventana.getSize();
    float scaleX = static_cast<float>(sz.x) / VIRTUAL_W;
    float scaleY = static_cast<float>(sz.y) / VIRTUAL_H;
    return min(scaleX, scaleY);
}

inline sf::Vector2f VentanaSFML::virtualToReal(float x, float y) const {
    float scale = obtenerEscalaUI();
    return sf::Vector2f(x * scale, y * scale);
}

inline void VentanaSFML::procesarClic(int x, int y) {
    if (estado == ESTADO_TUTORIAL) procesarClicTutorial(x, y);
    else if (estado == ESTADO_MENU) procesarClicMenu(x, y);
    else if (estado == ESTADO_PASAR_DISPOSITIVO) procesarClicPasarDispositivo(x, y);
    else if (estado == ESTADO_JUGANDO) procesarClicJuego(x, y);
    else if (estado == ESTADO_FIN_RONDA || estado == ESTADO_FIN_PARTIDA)
        procesarClicFin(x, y, estado == ESTADO_FIN_PARTIDA);
}

inline void VentanaSFML::procesarClicTutorial(int x, int y) {
    float W = static_cast<float>(ventana.getSize().x);
    float btnY = 490, btnH = 50, sep = 12;
    float posX = W / 2.f - 360;
    float ancho = 160;

    if (mouseEnRect(posX, btnY, ancho, btnH, x, y) && slideActual > 0) {
        mostrarSlideAnterior();
        return;
    }
    posX += ancho + sep;
    if (mouseEnRect(posX, btnY, ancho, btnH, x, y)) {
        salirAMenu();
        return;
    }
    posX += ancho + sep;
    if (mouseEnRect(posX, btnY, ancho, btnH, x, y)) {
        if (slideActual < (int)slides.size() - 1) mostrarSiguienteSlide();
        else salirAMenu();
        return;
    }
    posX += ancho + sep;
    if (mouseEnRect(posX, btnY, ancho, btnH, x, y)) {
        salirAMenu();
        return;
    }
}

inline void VentanaSFML::procesarClicMenu(int x, int y) {
    float W = static_cast<float>(ventana.getSize().x);
    float btnW = 360, btnH = 45, sep = 8;
    float posX = W / 2.f - btnW / 2;
    float btnY = 240;

    if (mouseEnRect(posX, btnY, btnW, btnH, x, y)) {
        slideActual = 0;
        estado = ESTADO_TUTORIAL;
        return;
    }
    btnY += btnH + sep;
    if (mouseEnRect(posX, btnY, btnW, btnH, x, y)) {
        iniciarJuego(MODO_SOLO);
        return;
    }
    btnY += btnH + sep;
    if (mouseEnRect(posX, btnY, btnW, btnH, x, y)) {
        iniciarJuego(MODO_CUATRO_JUGADORES);
        return;
    }
    btnY += btnH + sep;
    if (mouseEnRect(posX, btnY, btnW, btnH, x, y)) {
        ConfiguracionJuego cfg = ConfiguracionJuego::solo();
        juego.reset(new JuegoDomino(cfg));
        fichaSeleccionada = -1;
        hoverFicha = -1;
        esperandoLado = false;
        esperandoAnimacionIA = false;
        limpiarLog();
        bool ok = PersistenciaPartidaJson::cargar(*juego, "datos/partida.json");
        if (!ok) {
            agregarLog("Error al cargar, partida nueva");
            juego->iniciarNuevaRonda();
            estado = ESTADO_JUGANDO;
            if (!juego->esHumano(juego->getTurnoActual())) {
                esperandoAnimacionIA = true;
                relojTransicion.restart();
            }
            return;
        }
        agregarLog("Partida cargada - Turno: J" + to_string(juego->getTurnoActual() + 1) +
                   " - " + juego->getJugadores()[juego->getTurnoActual()]->getNombre());
        if (juego->isPartidaTerminada()) {
            estado = ESTADO_FIN_PARTIDA;
        } else if (juego->isRondaTerminada()) {
            estado = ESTADO_FIN_RONDA;
        } else if (juego->getModo() == MODO_CUATRO_JUGADORES) {
            int idxSur = juego->getJugadorEnPosicionVisual(JuegoDomino::SUR);
            if (juego->getTurnoActual() != idxSur) {
                estado = ESTADO_PASAR_DISPOSITIVO;
                agregarLog("Turno J" + to_string(juego->getTurnoActual() + 1) +
                           " - Pasa el dispositivo");
            } else {
                estado = ESTADO_JUGANDO;
                agregarLog("Tu turno J" + to_string(juego->getTurnoActual() + 1));
            }
        } else {
            estado = ESTADO_JUGANDO;
            if (!juego->esHumano(juego->getTurnoActual())) {
                esperandoAnimacionIA = true;
                relojTransicion.restart();
            }
        }
        return;
    }
    btnY += btnH + sep;
    if (mouseEnRect(posX, btnY, btnW, btnH, x, y)) {
        autoGuardarSiAplicable();
        ventana.close();
        return;
    }
}

inline void VentanaSFML::procesarClicPasarDispositivo(int x, int y) {
    float W = static_cast<float>(ventana.getSize().x);
    float btnW = 220, btnH = 60;
    float cx = W / 2.f;
    if (mouseEnRect(cx - btnW / 2, 510, btnW, btnH, x, y)) {
        estado = ESTADO_JUGANDO;
        if (!juego->esHumano(juego->getTurnoActual())) {
            esperandoAnimacionIA = true;
            relojTransicion.restart();
        }
    }
}

inline void VentanaSFML::procesarClicJuego(int x, int y) {
    if (juego->isPartidaTerminada() || juego->isRondaTerminada()) {
        estado = juego->isPartidaTerminada() ? ESTADO_FIN_PARTIDA : ESTADO_FIN_RONDA;
        return;
    }
    if (mouseEnRect(20 + 4 * (BTN_W + BTN_SEP), BTN_Y, BTN_W, BTN_H, x, y)) {
        salirAMenu();
        return;
    }
    int idxSur = juego->getJugadorEnPosicionVisual(JuegoDomino::SUR);
    if (juego->getTurnoActual() != idxSur || !juego->esHumano(juego->getTurnoActual())) {
        return;
    }

    auto avanzarEstadoPostTurno = [&]() {
        if (juego->isPartidaTerminada()) {
            estado = ESTADO_FIN_PARTIDA;
        } else if (juego->isRondaTerminada()) {
            estado = ESTADO_FIN_RONDA;
        } else if (juego->getModo() == MODO_CUATRO_JUGADORES) {
            int nuevoSur = juego->getJugadorEnPosicionVisual(JuegoDomino::SUR);
            if (juego->getTurnoActual() != nuevoSur) {
                estado = ESTADO_PASAR_DISPOSITIVO;
                agregarLog("Turno J" + to_string(juego->getTurnoActual() + 1) +
                           " - Pasa el dispositivo");
            } else {
                agregarLog("Tu turno J" + to_string(juego->getTurnoActual() + 1));
            }
        } else {
            if (!juego->esHumano(juego->getTurnoActual())) {
                esperandoAnimacionIA = true;
                relojTransicion.restart();
            }
        }
    };

    auto jugarLado = [&](int lado) {
        if (fichaSeleccionada < 0) return;
        Ficha* fSel = dynamic_cast<Ficha*>(juego->getJugadores()[juego->getTurnoActual()]
                                            ->getMano()[fichaSeleccionada].get());
        if (juego->jugarTurno(juego->getTurnoActual(), fichaSeleccionada, lado)) {
            if (fSel) {
                ultimaFichaPares = fSel->getPar();
                ultimoJugadorFicha = juego->getTurnoActual();
                relojUltimaFicha.restart();
                stringstream ss;
                ss << "J" << (juego->getTurnoActual() + 1) << " juega "
                   << fSel->getExtremoA() << "|" << fSel->getExtremoB()
                   << (lado == 0 ? " (izq)" : " (der)");
            agregarLog(ss.str());
        }
        juego->siguienteTurno();
        juego->verificarFinRonda();
    } else {
        agregarLog("Jugada invalida");
    }
        esperandoLado = false;
        fichaSeleccionada = -1;
        avanzarEstadoPostTurno();
    };

    float btnY = BTN_Y, btnH = BTN_H;
    float posX = 20, sep = BTN_SEP;
    float ancho = BTN_W;

    bool mesaVacia = juego->getMesa().vacia();

    if (esperandoLado && mesaVacia) {
        if (mouseEnRect(posX, btnY, ancho, btnH, x, y)) { jugarLado(0); return; }
    } else if (esperandoLado) {
        if (mouseEnRect(posX, btnY, ancho, btnH, x, y)) { jugarLado(0); return; }
        posX += ancho + sep;
        if (mouseEnRect(posX, btnY, ancho, btnH, x, y)) { jugarLado(1); return; }
        posX += ancho + sep;
    } else {
        if (mouseEnRect(posX, btnY, ancho, btnH, x, y)) {
            if (juego->puedeJugar(juego->getTurnoActual())) {
                agregarLog("Tienes fichas jugables");
            } else if (juego->pasarTurno(juego->getTurnoActual())) {
                agregarLog("J" + to_string(juego->getTurnoActual() + 1) + " pasa");
                juego->siguienteTurno();
                juego->verificarFinRonda();
                avanzarEstadoPostTurno();
            }
            return;
        }
        posX += ancho + sep;
        posX += ancho + sep;
        posX += ancho + sep;
    }

    if (mouseEnRect(posX, btnY, ancho, btnH, x, y)) {
        if (juego->isRondaTerminada()) {
            juego->iniciarNuevaRonda();
            agregarLog("Ronda " + to_string(juego->getRondaNumero()));
            fichaSeleccionada = -1;
            esperandoLado = false;
            if (juego->getModo() == MODO_CUATRO_JUGADORES) {
                int idxSur = juego->getJugadorEnPosicionVisual(JuegoDomino::SUR);
                if (juego->getTurnoActual() != idxSur) {
                    estado = ESTADO_PASAR_DISPOSITIVO;
                    agregarLog("Turno J" + to_string(juego->getTurnoActual() + 1) +
                               " - Pasa el dispositivo");
                } else {
                    estado = ESTADO_JUGANDO;
                    agregarLog("Tu turno J" + to_string(juego->getTurnoActual() + 1));
                    if (!juego->esHumano(juego->getTurnoActual())) {
                        esperandoAnimacionIA = true;
                        relojTransicion.restart();
                    }
                }
            } else {
                estado = ESTADO_JUGANDO;
                if (!juego->esHumano(juego->getTurnoActual())) {
                    esperandoAnimacionIA = true;
                    relojTransicion.restart();
                }
            }
        }
        return;
    }
    posX += ancho + sep;

    if (mouseEnRect(posX, btnY, ancho, btnH, x, y)) {
        salirAMenu();
        return;
    }
    posX += ancho + sep;

    {
        const Jugador& jug = *juego->getJugadores()[juego->getTurnoActual()];
        int izqMesa = juego->getMesa().getExtremoIzquierdo();
        int derMesa = juego->getMesa().getExtremoDerecho();
        vector<int> ordenados = indicesManoOrdenados(jug, izqMesa, derMesa);
        float W = static_cast<float>(ventana.getSize().x);
        float totalW = ordenados.size() * MANO_TILE_W;
        float x0 = W / 2.f - totalW / 2.f;
        float xCur = x0;
        for (size_t pos = 0; pos < ordenados.size(); ++pos) {
            float tw = MANO_TILE_W;
            float th = MANO_TILE_H;
            float ty = MANO_Y_CENTRO - th / 2.f;
            if (mouseEnRect(xCur, ty - 14, tw, th + 16, x, y)) {
                int idxOrig = ordenados[pos];
                Ficha* f = dynamic_cast<Ficha*>(jug.getMano()[idxOrig].get());
                if (!f) return;
                if (mesaVacia) {
                    fichaSeleccionada = idxOrig;
                    esperandoLado = true;
                    agregarLog("Elige lado: <- Izq o Der ->");
                } else if (juego->puedeJugar(juego->getTurnoActual())) {
                    int izq = juego->getMesa().getExtremoIzquierdo();
                    int der = juego->getMesa().getExtremoDerecho();
                    bool jugableIzq = (f->getExtremoA() == izq || f->getExtremoB() == izq);
                    bool jugableDer = (f->getExtremoA() == der || f->getExtremoB() == der);
                    if (esperandoLado) {
                        fichaSeleccionada = idxOrig;
                    } else if (jugableIzq && jugableDer) {
                        fichaSeleccionada = idxOrig;
                        esperandoLado = true;
                        agregarLog("Elige lado: <- Izq o Der ->");
                    } else if (jugableIzq) {
                        fichaSeleccionada = idxOrig;
                        jugarLado(0);
                    } else if (jugableDer) {
                        fichaSeleccionada = idxOrig;
                        jugarLado(1);
                    } else {
                        agregarLog("Ficha no jugable");
                    }
                } else {
                    agregarLog("Sin jugadas, debes pasarte");
                }
                return;
            }
            xCur += tw;
        }
    }
}

inline void VentanaSFML::procesarClicFin(int x, int y, bool esPartida) {
    float W = static_cast<float>(ventana.getSize().x);
    float cx = W / 2.f;
    if (mouseEnRect(cx - 110, 470, 220, 50, x, y)) {
        if (esPartida) {
            juego->iniciarNuevaPartida();
            agregarLog("Nueva partida");
        } else {
            juego->iniciarNuevaRonda();
            agregarLog("Ronda " + to_string(juego->getRondaNumero()));
        }
        fichaSeleccionada = -1;
        esperandoLado = false;
        if (juego->getModo() == MODO_CUATRO_JUGADORES) {
            estado = ESTADO_PASAR_DISPOSITIVO;
            agregarLog("Turno J" + to_string(juego->getTurnoActual() + 1) +
                       " - Pasa el dispositivo");
        } else {
            estado = ESTADO_JUGANDO;
            if (!juego->esHumano(juego->getTurnoActual())) {
                esperandoAnimacionIA = true;
                relojTransicion.restart();
            }
        }
        return;
    }
    if (mouseEnRect(cx - 110, 540, 220, 50, x, y)) {
        salirAMenu();
        return;
    }
}

inline void VentanaSFML::manejarEventos() {
    sf::Event ev;
    while (ventana.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed) {
            autoGuardarSiAplicable();
            ventana.close();
        }
        if (ev.type == sf::Event::MouseButtonPressed &&
            ev.mouseButton.button == sf::Mouse::Left) {
            cachedVirtualMousePos = ventanaAVirtual(ev.mouseButton.x, ev.mouseButton.y);
            procesarClic(ev.mouseButton.x, ev.mouseButton.y);
        }
        if (ev.type == sf::Event::MouseMoved) {
            cachedVirtualMousePos = ventanaAVirtual(ev.mouseMove.x, ev.mouseMove.y);
            int mx = ev.mouseMove.x, my = ev.mouseMove.y;
            hoverFicha = -1;
            if (estado == ESTADO_JUGANDO) {
                int idxSur = juego->getJugadorEnPosicionVisual(JuegoDomino::SUR);
                if (juego->getTurnoActual() == idxSur &&
                    juego->esHumano(juego->getTurnoActual())) {
                    const Jugador& jug = *juego->getJugadores()[idxSur];
                    int izqMesa = juego->getMesa().getExtremoIzquierdo();
                    int derMesa = juego->getMesa().getExtremoDerecho();
                    vector<int> ordenados = indicesManoOrdenados(jug, izqMesa, derMesa);
                    float W = static_cast<float>(ventana.getSize().x);
                    float totalW = ordenados.size() * MANO_TILE_W;
                    float x0 = W / 2.f - totalW / 2.f;
                    float xCur = x0;
                    for (size_t pos = 0; pos < ordenados.size(); ++pos) {
                        float tw = MANO_TILE_W;
                        float th = MANO_TILE_H;
                        float ty = MANO_Y_CENTRO - th / 2.f;
                        if (mouseEnRect(xCur, ty - 14, tw, th + 16, mx, my)) {
                            hoverFicha = static_cast<int>(pos);
                            break;
                        }
                        xCur += tw;
                    }
                }
            }
        }
        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::Escape) {
                if (estado == ESTADO_JUGANDO || estado == ESTADO_PASAR_DISPOSITIVO)
                    salirAMenu();
                else {
                    autoGuardarSiAplicable();
                    ventana.close();
                }
            }
            if (estado == ESTADO_TUTORIAL) {
                if (ev.key.code == sf::Keyboard::Right ||
                    ev.key.code == sf::Keyboard::Space) mostrarSiguienteSlide();
                if (ev.key.code == sf::Keyboard::Left) mostrarSlideAnterior();
            }
            if (estado == ESTADO_PASAR_DISPOSITIVO &&
                ev.key.code == sf::Keyboard::Return) {
                estado = ESTADO_JUGANDO;
                if (!juego->esHumano(juego->getTurnoActual())) {
                    esperandoAnimacionIA = true;
                    relojTransicion.restart();
                }
            }
            if (estado == ESTADO_JUGANDO) {
                if (ev.key.code == sf::Keyboard::P &&
                    juego->puedePasar(juego->getTurnoActual())) {
                    if (juego->pasarTurno(juego->getTurnoActual())) {
                        agregarLog("J" + to_string(juego->getTurnoActual() + 1) +
                                   " pasa el turno (tecla P)");
                        juego->siguienteTurno();
                        juego->verificarFinRonda();
                        if (juego->isPartidaTerminada())
                            estado = ESTADO_FIN_PARTIDA;
                        else if (juego->isRondaTerminada())
                            estado = ESTADO_FIN_RONDA;
                        else if (juego->getModo() == MODO_CUATRO_JUGADORES) {
                            estado = ESTADO_PASAR_DISPOSITIVO;
                            agregarLog("Turno J" + to_string(juego->getTurnoActual() + 1) +
                                       " - Pasa el dispositivo");
                        } else if (!juego->esHumano(juego->getTurnoActual())) {
                            esperandoAnimacionIA = true;
                            relojTransicion.restart();
                        }
                    }
                }
                if (ev.key.code == sf::Keyboard::Escape) {
                    fichaSeleccionada = -1;
                    esperandoLado = false;
                }
            }
        }
        if (ev.type == sf::Event::Resized) {
            actualizarVistaVirtual();
        }
    }
}

inline void VentanaSFML::actualizarVistaVirtual() {
    sf::Vector2u sz = ventana.getSize();
    float windowRatio = static_cast<float>(sz.x) / static_cast<float>(sz.y);
    float virtualRatio = VIRTUAL_W / VIRTUAL_H;
    
    sf::FloatRect viewport;
    if (windowRatio > virtualRatio) {
        float scale = static_cast<float>(sz.y) / VIRTUAL_H;
        float vpWidth = VIRTUAL_W * scale / static_cast<float>(sz.x);
        viewport = sf::FloatRect((1.f - vpWidth) / 2.f, 0.f, vpWidth, 1.f);
    } else {
        float scale = static_cast<float>(sz.x) / VIRTUAL_W;
        float vpHeight = VIRTUAL_H * scale / static_cast<float>(sz.y);
        viewport = sf::FloatRect(0.f, (1.f - vpHeight) / 2.f, 1.f, vpHeight);
    }
    
    sf::View view(sf::FloatRect(0.f, 0.f, VIRTUAL_W, VIRTUAL_H));
    view.setViewport(viewport);
    ventana.setView(view);
}

inline sf::Vector2f VentanaSFML::ventanaAVirtual(int mx, int my) const {
    sf::Vector2i pixelPos(mx, my);
    return ventana.mapPixelToCoords(pixelPos);
}

inline void VentanaSFML::actualizarIA() {
    if (!esperandoAnimacionIA) return;
    if (relojTransicion.getElapsedTime().asSeconds() < DELAY_IA_SEGUNDOS) return;
    if (juego->isRondaTerminada() || juego->isPartidaTerminada()) {
        esperandoAnimacionIA = false;
        estado = juego->isPartidaTerminada() ? ESTADO_FIN_PARTIDA : ESTADO_FIN_RONDA;
        return;
    }
    if (juego->esHumano(juego->getTurnoActual())) {
        esperandoAnimacionIA = false;
        if (juego->getModo() == MODO_CUATRO_JUGADORES) {
            int idxSur = juego->getJugadorEnPosicionVisual(JuegoDomino::SUR);
            if (juego->getTurnoActual() != idxSur) {
                estado = ESTADO_PASAR_DISPOSITIVO;
                agregarLog("Turno J" + to_string(juego->getTurnoActual() + 1) +
                           " - Pasa el dispositivo");
            }
        }
        return;
    }
    ResultadoTurno r = juego->aplicarTurnoIA();
    if (r.paso) {
        agregarLog("J" + to_string(r.indiceJugador + 1) + " pasa");
    } else if (r.seJugo) {
        ultimaFichaPares = r.fichaJugada;
        ultimoJugadorFicha = r.indiceJugador;
        relojUltimaFicha.restart();
        stringstream ss;
        ss << "J" << (r.indiceJugador + 1) << " juega "
           << r.fichaJugada.first << "|" << r.fichaJugada.second
           << (r.lado == 0 ? " (izq)" : " (der)");
        agregarLog(ss.str());
    }
    juego->siguienteTurno();
    juego->verificarFinRonda();
    if (juego->isRondaTerminada() && !juego->isPartidaTerminada()) {
        esperandoAnimacionIA = false;
        estado = ESTADO_FIN_RONDA;
    } else if (juego->isPartidaTerminada()) {
        esperandoAnimacionIA = false;
        estado = ESTADO_FIN_PARTIDA;
    } else if (!juego->esHumano(juego->getTurnoActual())) {
        relojTransicion.restart();
    } else {
        esperandoAnimacionIA = false;
        if (juego->getModo() == MODO_CUATRO_JUGADORES) {
            int idxSur = juego->getJugadorEnPosicionVisual(JuegoDomino::SUR);
            if (juego->getTurnoActual() != idxSur) {
                estado = ESTADO_PASAR_DISPOSITIVO;
                agregarLog("Turno J" + to_string(juego->getTurnoActual() + 1) +
                           " - Pasa el dispositivo");
            } else {
                agregarLog("Tu turno J" + to_string(juego->getTurnoActual() + 1));
            }
        }
    }
}

inline void VentanaSFML::autoGuardarSiAplicable() {
    if (juego && !juego->isPartidaTerminada() && !juego->isRondaTerminada()) {
        PersistenciaPartidaJson::guardar(*juego, "datos/partida.json");
    }
}

inline VentanaSFML::VentanaSFML(int ancho, int alto, const string& titulo)
    : ventana(sf::VideoMode(ancho, alto), titulo, sf::Style::Resize),
      fuenteCargada(false), fondoCargado(false), iconoCargado(false), logoCargado(false),
      texturasCargadas(0),
      estado(ESTADO_TUTORIAL), slideActual(0),
      fichaSeleccionada(-1), hoverFicha(-1), esperandoLado(false),
      esperandoAnimacionIA(false),
      ultimaFichaPares(-1, -1), ultimoJugadorFicha(-1) {
    ventana.setFramerateLimit(60);
    fuenteCargada = cargarFuente();
    cargarTexturasFichas();
    if (iconoCargado)
        ventana.setIcon(icono.getSize().x, icono.getSize().y,
                        icono.getPixelsPtr());
    slides = Tutorial::obtenerSlides();
    modoActual = MODO_SOLO;
    actualizarVistaVirtual();
    agregarLog("Domino Doble-9");
}

inline void VentanaSFML::ejecutar() {
    while (ventana.isOpen()) {
        manejarEventos();
        actualizarIA();

        ventana.clear();
        dibujarFondo();

        if (estado == ESTADO_TUTORIAL) {
            dibujarPantallaTutorial();
        } else if (estado == ESTADO_MENU) {
            dibujarPantallaMenu();
        } else {
            dibujarIndicadoresEstado();
            for (int pos = 0; pos < 4; ++pos) {
                if (pos == JuegoDomino::SUR) continue;
                int idx = juego->getJugadorEnPosicionVisual(pos);
                bool esTurno = (juego->getTurnoActual() == idx) &&
                               !juego->isRondaTerminada() &&
                               !juego->isPartidaTerminada();
                dibujarPanelOponente(pos, esTurno);
            }
            dibujarCadenaFichas();
            dibujarIndicadoresLadoCadena();
            dibujarManoActual();
            dibujarLogMensajes();
            dibujarBotoneraJuego();
            if (estado == ESTADO_PASAR_DISPOSITIVO)
                dibujarOverlayTransicion();
            if (estado == ESTADO_FIN_RONDA)
                dibujarOverlayFin(false);
            if (estado == ESTADO_FIN_PARTIDA)
                dibujarOverlayFin(true);
        }

        ventana.display();
    }
}

}