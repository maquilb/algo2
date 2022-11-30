
#include "Variante.h"

Variante::Variante(Nat tamanoTab, Nat cantFichas, const map<Letra, Nat> &puntajes, const set<Repositorio> &palabrasLegitimas):
_tamano(tamanoTab),
_cantidadDeFichas(cantFichas),
_valorLetras(puntajes.size()),
_palabrasPermitidas(conjunto_Trie()){
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
        Repositorio pal = *it2;
        _palabrasPermitidas.agregar(pal);
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

bool Variante::palabraLegitima(const Repositorio &palabra) const {
    return _palabrasPermitidas.pertenece(palabra);
}

bool Variante::palabrasLegitimas(const list<Repositorio> &palabras) const {
    bool res = true;
    auto it = palabras.begin();
    while(res && it != palabras.end()){
        const Repositorio pal = palabras.front();
        res = palabraLegitima(pal);
        it++;
    }
    return res;
}
Nat Variante::lmax(){
    return _palabrasPermitidas.lmax();
}