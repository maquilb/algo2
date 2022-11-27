
#include "Juego.h"

Juego::Juego(Nat k, const Variante &v, const Repositorio &r): _variante(Variante), _repositorio(Repositorio), _tablero(Tablero(Variante.tamano)), jugadores(k, Jugador()), _cantidadDeTurnos(0), _turnoDe(0) {}

void Juego::ubicar(const Ocurrencia &o) {

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
    return 0;
}

Nat Juego::cantFicha(IdCliente id, Letra l) {
    return 0;
}

