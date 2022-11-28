
#include "Juego.h"

Juego::Juego(Nat k, const Variante &v, const Repositorio &r): _variante(v), _repositorio(r), _tablero(Tablero(Variante.tamano)), _jugadores(k, Jugador(v,r)), _cantidadDeTurnos(0), _turnoDe(0) {}

void Juego::ubicar(const Ocurrencia &o) {
    ponerLetras(_tablero, o, _cantidadDeTurnos);
    _cantidadDeTurnos ++;
    _turnoDe ++;
    if (_turnoDe == _jugadores.size()){
        _turnoDe = 0;
    }
    _jugadores[_turnoDe]._ocurrenciasDelJugador.push(o);
    reponerFichas(o);
}

IdCliente Juego::turno() {
    return _turnoDe;
}

const Variante &Juego::variante() {
    return _variante;
}

Nat Juego::cantDeJugadores() {
    return _jugadores.size();
}

const Repositorio &Juego::repositorio() {
    return _repositorio;
}

const Tablero &Juego::tablero() {
    return _tablero;
}

const Tablero &Juego::posDeTablero(Nat i, Nat j) {
    return _tablero[i][j];
}

Nat Juego::turnoActual() {
    return _turnoDe;
}

const vector<Nat> Juego::fichasDeJugador(Nat i){
    return _jugadores[i]._fichasDelJugador;
}

Nat Juego::cantDeFichasLetra(IdCliente id, Letra l) {
    return _jugadores[id]._fichasDelJugador[ord(l)];
}


bool Juego::jugadaValida(const Ocurrencia &o) {
    return false;
}

bool Juego::hayLetra(Nat x, Nat y) {
    return _tablero.hayLetra(x, y);
}

Letra Juego::letra(Nat i, Nat j) {
    return _tablero.letra(i, j);
}

Nat Juego::puntaje(IdCliente id) {
    queue <Ocurrencia> ocus = _jugadores[id]._ocurrenciasDelJugador;
    Nat puntosACalcular;
    puntosACalcular = calcularPuntosPalabrasJugadas(ocus);
    _jugadores[id]._puntajeDelJugador += puntosACalcular;
    return _jugadores[id]._puntajeDelJugador;
}

Nat Juego::cantFicha(IdCliente id, Letra l) {
    return 0;
}

void Juego :: reponerFichas(const Ocurrencia o){
    auto it = o.begin();
    while (it != o.end()){
        tuple<Nat, Nat, Letra> ficha = *it;
        Letra l = get<2>(ficha);
        _jugadores[_turnoDe]._fichasDelJugador[ord(l)]--;
        Letra nueva_l = _repositorio.front();
        _jugadores[_turnoDe]._fichasDelJugador[ord(nueva_l)]++;
        _repositorio.pop_front();
        it ++;
    }
}

Nat Juego::calcularPuntos(queue<Repositorio> &palabras){
    Nat res = 0;
    while(!palabras.empty()){
        Repositorio p = palabras.front();
        auto it = p.front();
        while(it != p.back()){
            Letra l = p.front();
            res += _variante.puntajeLetra(l);
            it++;
        }
        palabras.pop();
    }
    return res;
}

tuple<bool, bool> Juego::HorizontalOVertical(Ocurrencia o){
    tuple<bool, bool> res;
    bool horizontal = get<0>(res);
    bool vertical = get<1>(res);
    horizontal = true;
    vertical = true;
    if (o.size()>0){
        auto it = o.begin();
        Nat fila0 = get<0>(*it);
        Nat col0 = get<1>(*it);
        it++;
        while(it != o.end() && (horizontal || vertical)){
            Nat fila1 = get<0>(*it);
            Nat col1 = get<1>(*it);
            if(horizontal){
                horizontal = fila1 == fila0;
            }
            if (vertical){
                vertical = col1 != col0;
            }
            fila0 = fila1;
            col0 = col1;
            it++;
        }
    }
    return res;
}

Nat  Juego::calcularPuntosPalabrasJugadas(queue<Ocurrencia> &ocus){
    Nat res = 0;
    while(!ocus.empty()){
        Ocurrencia p = ocus.front();
        queue<Repositorio> palabras = palabrasFormadas(p);
        ocus.pop();
        res += calcularPuntos(palabras);
    }
    return res;
}

queue<Repositorio> Juego::palabrasFormadas(Ocurrencia o){
    if (o.size()>0){
        tuple<bool,bool> esHorizontalOVertical = HorizontalOVertical(o);
        bool horizontal = get<0>(esHorizontalOVertical);
        bool vertical = get<1>(esHorizontalOVertical);
        bool sentido = true;
        if(vertical){
            sentido = false;
        }
        queue<Repositorio> res = palabrasFormadasTransversales(o, sentido);
        Repositorio palabraPrincipal = formarPalabraJugadaPrincial(o,sentido);
        res.push(palabraPrincipal);
        return res;
    } else {
        return queue<Repositorio>();
    }
}

Repositorio Juego::palabraFormadaVertical(Ficha f) {
    Repositorio res;
    Letra l = get<2>(f);
    res.push_front(l);
    Nat fila = get<0>(f);
    Nat columna = get<1>(f);
    Nat arriba = fila - 1;
    Nat abajo = fila + 1;
    Nat turno = turnoApoyado(fila, columna);
    while (0<= arriba && ocupada(arriba, columna) && turno>= turnoApoyado(arriba, columna)){
        Letra l0 = letraEnPos(arriba, columna);
        res.push_front(l0);
        arriba--;
    }
    while (abajo <=tablero.size() && ocupada(abajo, columna) && turno>= turnoApoyado(abajo, columna)){
        Letra l0 = letraEnPos(abajo, columna);
        res.push_back(l0);
        abajo++;
    }
    return res;
}

Repositorio Juego::palabraFormadaHorizontal(Ficha f) {
    Repositorio res;
    Letra l = get<2>(f);
    res.push_front(l);
    Nat fila = get<0>(f);
    Nat columna = get<1>(f);
    Nat izquierda = columna - 1;
    Nat derecha = columna + 1;
    Nat turno = turnoApoyado(fila, columna);
    while (derecha <=tablero.size() && ocupada(fila, derecha) && turno>= turnoApoyado(fila, derecha)){
        Letra l0 = letraEnPos(fila, derecha);
        res.push_back(l0);
        derecha++;
    }
    while (0<= izquierda && ocupada(fila, izquierda) && turno>= turnoApoyado(fila, derecha)){
        Letra l0 = letraEnPos(fila, izquierda);
        res.push_front(l0);
        izquierda--;
    }
    return res;
}

queue<Repositorio> Juego::palabrasFormadasTransversales(Ocurrencia o, bool sentido) {
    queue<Repositorio> res;
    auto it = o.begin();
    while(it != o.end()){
        Ficha f= *it;
        Repositorio palabra;
        if (sentido){
            palabra =  palabraFormadaVertical(f);
        } else {
            palabra =  palabraFormadaHorizontal(f);
        }
        res.push(palabra);
        it++;
    }
    return res;
}

Repositorio Juego::formarPalabraJugadaPrincial(Ocurrencia o, bool sentido) {
    auto it = o.begin();
    Ficha f = *it;
    Repositorio res;
    if(sentido){
        res = palabraFormadaHorizontal(f);
    } else {
        res = palabraFormadaVertical(f);
    }
    return res;
}







