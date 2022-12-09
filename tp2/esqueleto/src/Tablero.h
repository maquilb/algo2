
#ifndef TABLERO_H
#define TABLERO_H


#include <type_traits>
#include "Letra.h"

class Tablero{
public:

    Tablero(Nat n);

    void ponerLetra(Nat fila, Nat columna, Letra l, Nat turno);

    Nat tamanio();

    bool hayUnaLetraEnPos(Nat fila, Nat columna);

    Letra letraEnPos(Nat fila, Nat columna);

    bool enTablero(Nat fila, Nat columna);

    bool estaLibre(Nat fila, Nat columna);

    bool estaOcupada(Nat fila, Nat columna);

    void ponerLetras(Ocurrencia ocur, Nat turno);

    bool sonCeldasLibres(Ocurrencia o);

    Nat turnoApoyado(Nat fila, Nat columna);

    bool enTablero(Nat fila, Nat columna) const;
    bool estaOcupada(Nat fila, Nat columna) const;
    Nat turnoApoyado(Nat fila, Nat columna) const;
    Letra letraEnPos(Nat fila, Nat columna) const;

private:
    Nat _tamanio;
    vector<vector<tuple<bool, Letra, Nat>>> _casillero;
};

#endif //TABLERO_H
