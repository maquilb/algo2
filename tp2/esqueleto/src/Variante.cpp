
#include "Variante.h"

Variante::Variante(Nat tamanoTab, Nat cantFichas, const map<Letra, Nat> &puntajes, const set<Palabra> &palabrasLegitimas):
_tamano(tamanoTab),
_cantidadDeFichas(cantFichas),
_valorLetras(puntajes.size()),
_palabrasPermitidas(string_map<bool>()){
    // Asignamos el valor de cada letra del alfabeto
    auto it1 = puntajes.begin();
    while (it1 != puntajes.end()){
        Letra letra = it1->first;
        Nat puntos = it1->second;
        _valorLetras[ord(letra)]= puntos;
        it1++;
    }
    //Creamos el diccionario Trie
    auto it2 = palabrasLegitimas.begin();
    while (it2 != palabrasLegitimas.end()){
        Palabra pal = *it2;
        pair<Palabra, bool> palabraDef;
        get<0>(palabraDef) = pal;
        get<1>(palabraDef) = true;
        _palabrasPermitidas.insert(palabraDef);
        it2++;
    }

}

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
    string str(palabra.begin(),palabra.end());
    return _palabrasPermitidas.at(str);
}

bool Variante::palabrasLegitimas(const list<Palabra> &palabras) const {
    bool res = true;
    auto it = palabras.begin();
    while(res && it != palabras.end()){
        Palabra pal = palabras.front();
        res = palabraLegitima(pal);
        it++;
    }
    return res;
}
