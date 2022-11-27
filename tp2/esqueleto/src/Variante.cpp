
#include "Variante.h"

Variante::Variante(Nat tamanoTab, Nat cantFichas, const map<Letra, Nat> &puntajes, const set<Palabra> &palabrasLegitimas):
_tamano(tamanoTab),
_cantidadDeFichas(cantFichas),
_valorLetras(mapAVector(puntajes)),
_palabrasPermitidas(setADiccTrie(&palabrasLegitimas)){}

Nat Variante::tamanoTablero() const {
    return _tamano;
}

Nat Variante::fichas() const {
    return _cantidadDeFichas;
}

Nat Variante::puntajeLetra(Letra l) const {
    return _valorLetras[ord(l)];
}

bool Variante::palabraLegitima(const Palabra &palabra) const {
    return at(palabra*);
}

bool Variante::palabrasLegitimas(const list<Palabra> &palabras) const {
    bool res = true;
    auto it = _palabrasPermitidas.begin();
    while(res && it != _palabrasPermitidas.end()){
        Palabra pal = _palabrasPermitidas.front();
        res = palabraLegitima(pal);
        it++;
    }
    return res;
}

const vector<Nat> Variante::mapAVector(const map<Letra, Nat> &puntajes) {
    vector<Nat> res(puntajes.size());
    auto it = puntajes.begin();
    while (it != puntajes.end()){
        Letra letra = it->first;
        Nat puntos = it->second;
        res[ord(letra)]= puntos;
        it++;
    }
    return res;
}
// Complejidad: O(#Claves(puntajes))

string_map Variante::setADiccTrie(const set<Palabra> &palabrasLegitimas) {
    string_map res();

    auto it = palabrasLegitimas.begin();
    while (it != palabrasLegitimas.end()){
        Palabra pal = *it;
        res.insert(pal, true);
        it++;
    }
    return res;
}
//Complejidad: O(#(palabrasLegitimas))