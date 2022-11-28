
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

bool Juego::jugadaValida(const Ocurrencia &o) {
    return false;
}

bool Juego::hayLetra(Nat x, Nat y) {
    return false;
}

Letra Juego::letra(Nat i, Nat j) {
    return 0;
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







