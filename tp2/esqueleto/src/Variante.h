#ifndef VARIANTE_H
#define VARIANTE_H

#include "Tipos.h"
#include "Letra.h"
#include "string_map"


class Variante {
public:
    /**
     * Construye una Variante a partir de los parametros de las variantes
     */
    Variante (
            Nat tamanoTab,
            Nat cantFichas,
            const map<Letra, Nat>& puntajes,
            const set<Palabra>& palabrasLegitimas
    ) ;

    /**
     * Retorna el tamano del tablero de la variante
     */
    Nat tamanoTablero() const;

    /**
     * Retorna la cantidad de fichas que cada jugador deberia tener en su mano.
     */
    Nat fichas() const;

    /**
     * Retorna el puntaje de la letra l
     */
    Nat puntajeLetra(Letra l) const;

    /**
     * Indica si una palabra es legitima o no
     */
    bool palabraLegitima(const Palabra &palabra) const;

    /**
     * Indica si las palabras son legitimas
     */
    bool palabrasLegitimas(const list<Palabra> &palabras) const;

private:

    const Nat _tamano;
    const Nat _cantidadDeFichas;
    const vector<Nat> _valorLetras;
    const string_map _palabrasPermitidas;
    const vector<Nat> mapAVector(const map<Letra, Nat> &puntajes);
};



#endif //VARIANTE_H