#include "Tablero.h"

Tablero::Tablero(Nat n): _tamanio(n), _casillero(n, vector<tuple<bool,Letra, Nat>>(n)) {
    tuple<bool,Letra, Nat> p = make_tuple(false, inversaDeOrd(1), 0);

    for (int i = 0; i < n ; ++i) {
        for (int j = 0; j < n ; ++j) {
            _casillero[i][j] = p;
        }
    }
}

void Tablero::ponerLetra(Nat fila, Nat columna, Letra l, Nat turno) {
    get<0>(_casillero[fila][columna]) = true;
    get<1>(_casillero[fila][columna]) = l;
    get<2>(_casillero[fila][columna])= turno;
}

Nat Tablero::tamanio() {
    return _tamanio;
}

bool Tablero::hayUnaLetraEnPos(Nat fila, Nat columna) {
    return get<0>(_casillero[fila][columna]);
}

Letra Tablero::letraEnPos(Nat fila, Nat columna) {
    return get<1>(_casillero[fila][columna]);
}

bool Tablero::enTablero(Nat fila, Nat columna) {
    return ((fila < _tamanio ) && (columna < _tamanio));
}

bool Tablero::estaLibre(Nat fila, Nat columna) {
    return enTablero(fila, columna) && not (get<0>(_casillero[fila][columna]));
}

bool Tablero::estaOcupada(Nat fila, Nat columna) {
    return enTablero(fila, columna) && (get<0>(_casillero[fila][columna]));
}

void Tablero::ponerLetras(Ocurrencia ocur, Nat turno) {
    Letra ficha;
    Nat filaDeFicha;
    Nat colDeFicha;
    Nat turnoDeJugada = turno;

    for (auto it = ocur.begin() ; it != ocur.end(); it++){
        ficha = get<2>(*it);
        filaDeFicha = get<0>(*it);
        colDeFicha = get<1>(*it);
        ponerLetra(filaDeFicha, colDeFicha, ficha, turnoDeJugada);
    }

}

bool Tablero::sonCeldasLibres(Ocurrencia o) {
    bool res = true;
    for (auto it = o.begin() ; it != o.end(); it++){
        if (estaOcupada(get<0>(*it), get<1>(*it))){
            res = false;
        }
    }
    return res;
}

Nat Tablero::turnoApoyado(Nat fila, Nat columna) {
    return get<2>(_casillero[fila][columna]);
}





