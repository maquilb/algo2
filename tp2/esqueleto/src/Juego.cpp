
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








