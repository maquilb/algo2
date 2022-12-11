
#include "Variante.h"

Variante::Variante(Nat tamanoTab, Nat cantFichas, const map<Letra, Nat> &puntajes, const set<list<Letra>> &palabrasLegitimas)
: _palabrasPermitidas(conjunto_Trie()){
    _tamano = tamanoTab;
    _cantidadDeFichas = cantFichas;
    _valorLetras = vector<Nat>(TAMANIO_ALFABETO,1);
    // Asignamos el valor de cada letra del alfabeto

    auto it1 = puntajes.begin();
    while (it1 != puntajes.end()){
        Letra letra = it1->first;
        Nat puntos = it1->second;
        _valorLetras[ord(letra)]= puntos;
        it1++;
    }
    _palabrasPermitidas.agregarPalabras(palabrasLegitimas);
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

bool Variante::palabraLegitima(const list<Letra> &palabra) const {
    return _palabrasPermitidas.pertenece(palabra);
}

bool Variante::palabrasLegitimas(const list<list<Letra>> &palabras) const {
    bool res = true;
    auto it = palabras.begin();
    while(res && it != palabras.end()){
        const list<Letra> pal = *it;
        res = palabraLegitima(pal);
        it++;
    }
    return res;
}
Nat Variante::lmax() const {
    return _palabrasPermitidas.lmax();
}