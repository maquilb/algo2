#include <type_traits>
#include "Letra.h"

class Tablero{
public:

    Tablero(Nat n);


    vector<vector<tuple<bool, Letra, Nat>>> crearTablero(Nat n);

    // es void o tablero??
    void ponerLetra(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna, Letra l, Nat turno);

    Nat tamanio(vector<vector<tuple<bool, Letra, Nat>>> tab);

    bool hayUnaLetraEnPos(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna);

    Letra letraEnPos(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna);

    bool enTablero(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna);

    bool estaLibre(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna);

    bool estaOcupada(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna);

    // es void o tablero??
    void ponerLetras(vector<vector<tuple<bool, Letra, Nat>>> tab, Ocurrencia ocur, Nat turno);

    bool sonCeldasLibres(vector<vector<tuple<bool, Letra, Nat>>> tab, Ocurrencia o);

    Nat turnoApoyado(vector<vector<tuple<bool, Letra, Nat>>> tab , Nat fila, Nat columna);




private:
    Nat _tamanio;
    Nat _fila;
    Nat _columna;



};

Tablero::Tablero(Nat n): _tamanio(n), _fila(n), _columna(n) {}

// pair<bool, letra, turno>
vector<vector<tuple<bool, Letra, Nat>>> Tablero::crearTablero(Nat n) {

    vector<vector<tuple<bool,Letra, Nat>>> tablero(n, vector<tuple<bool,Letra, Nat>>(n));

    tuple<bool,Letra, Nat> p = make_tuple(false, inversaDeOrd(1), 0);

    for (int i = 0; i < n ; ++i) {
        for (int j = 0; j < n ; ++j) {
            tablero[i][j] = p;
        }
    }

    return tablero;
}
// tab[x][y] es un par bool letra
void Tablero::ponerLetra(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna, Letra l, Nat turno) {
    get<0>(tab[fila][columna]) = true;
    get<1>(tab[fila][columna]) = l;
    get<2>(tab[fila][columna])= turno;
}

Nat Tablero::tamanio(vector<vector<tuple<bool, Letra, Nat>>> tab) {
    //return tab.size();
    return _tamanio;
}

bool Tablero::hayUnaLetraEnPos(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna) {
    return get<0>(tab[fila][columna]);
}

Letra Tablero::letraEnPos(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna) {
    return get<1>(tab[fila][columna]);
}

bool Tablero::enTablero(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna) {
    return ((fila < _tamanio ) && (columna < _tamanio));
}

bool Tablero::estaLibre(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna) {
    return enTablero(tab, fila, columna) && not (get<0>(tab[fila][columna]));
}

bool Tablero::estaOcupada(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna) {
    return enTablero(tab, fila, columna) && (get<0>(tab[fila][columna]));
}

// Ocurrencia = set<tuple<Nat, Nat, Letra>>

void Tablero::ponerLetras(vector<vector<tuple<bool, Letra, Nat>>> tab, Ocurrencia ocur, Nat turno) {
    Letra ficha;
    Nat filaDeFicha;
    Nat colDeFicha;
    Nat turnoDeJugada = turno;

    for (auto it = ocur.begin() ; it != ocur.end(); it++){
        ficha = get<2>(*it);
        filaDeFicha = get<0>(*it);
        colDeFicha = get<1>(*it);
        ponerLetra(tab, filaDeFicha, colDeFicha, ficha, turnoDeJugada);
    }

}

bool Tablero::sonCeldasLibres(vector<vector<tuple<bool, Letra, Nat>>> tab, Ocurrencia o) {
    bool res = true;
    for (auto it = o.begin() ; it != o.end(); it++){
        if (estaOcupada(tab, get<0>(*it), get<1>(*it))){
            res = false;
        }
    }
    return res;
}

Nat Tablero::turnoApoyado(vector<vector<tuple<bool, Letra, Nat>>> tab, Nat fila, Nat columna) {
    return get<2>(tab[fila][columna]);
}










