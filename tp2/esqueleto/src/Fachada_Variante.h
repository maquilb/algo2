#ifndef FACHADA_VARIANTE_H
#define FACHADA_VARIANTE_H

#include "Tipos.h"
#include "Variante.h"

class Fachada_Variante {
public:


/**
     * Construye una Fachada_Variante a partir de los parametros de las variantes
     */
    Fachada_Variante(
            Nat tamanoTab,
            Nat cantFichas,
            const map<Letra, Nat>& puntajes,
            const set<vector<Letra>>& palabrasLegitimas
    ) : _variante(tamanoTab, cantFichas, puntajes, setvectorToList(palabrasLegitimas)){}

    /**
     * Retorna el tamano del tablero de la variante
     */
    Nat tamanoTablero() const{
        return _variante.tamanoTablero();
    }

    /**
     * Retorna la cantidad de fichas que cada jugador deberia tener en su mano.
     */
    Nat fichas() const{
        return _variante.fichas();
    }

    /**
     * Retorna el puntaje de la letra l
     */
    Nat puntajeLetra(Letra l) const{
        return _variante.puntajeLetra(l);
    }



/**
     * Indica si una palabra es legitima o no
     */
    bool palabraLegitima(const Palabra& palabra) const{
        Repositorio palabra_aux = vectorToList(palabra);
        return _variante.palabraLegitima(palabra_aux);
    }

    Variante _variante;

private:
    set<Repositorio> setvectorToList(const set<Palabra> palabra) {
        set<Repositorio> res;
        auto it = palabra.begin();
        while (it != palabra.end()){
            Palabra pal = *it;
            Repositorio pal_aux;
            for (int i=0; i<pal.size(); i++){
                pal_aux.push_back(pal[i]);
            }
            res.insert(pal_aux);
        }
        return res;
    }
    Repositorio vectorToList(const Palabra pal) const {
        Repositorio res;
        for (int i=0; i<pal.size(); i++){
            res.push_back(pal[i]);
        }
        return res;
    }
};

#endif //FACHADA_VARIANTE_H
