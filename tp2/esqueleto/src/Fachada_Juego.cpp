#include "Fachada_Juego.h"

Fachada_Juego::Fachada_Juego(Nat k, const Fachada_Variante &v, const Repositorio &r) : _juego(k, v.variante(), r),
                                                                                       _fvariante(v) {}

void Fachada_Juego::ubicar(const Ocurrencia &o) {
    _juego.ubicar(o);
}

IdCliente Fachada_Juego::turno() {
    return _juego.turno();
}

const Fachada_Variante &Fachada_Juego::variante() {
    return _fvariante;
}

bool Fachada_Juego::jugadaValida(const Ocurrencia &o) {
    return _juego.jugadaValida(o);
}

bool Fachada_Juego::hayLetra(Nat x, Nat y) {
    return _juego.hayLetra(x, y);
}

Letra Fachada_Juego::letra(Nat i, Nat j) {
    return _juego.letra(i, j);
}

Nat Fachada_Juego::puntaje(IdCliente id) {
    return _juego.puntaje(id);
}

Nat Fachada_Juego::cantFicha(IdCliente id, Letra l) {
    return _juego.cantFicha(id, l);
}