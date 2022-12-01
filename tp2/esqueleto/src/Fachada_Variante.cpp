#include "Fachada_Variante.h"

Fachada_Variante::Fachada_Variante( Nat tamanoTab, Nat cantFichas, const map<Letra, Nat>& puntajes, const set<vector<Letra>>& palabrasLegitimas) : _variante(tamanoTab, cantFichas, puntajes, setvectorToList(palabrasLegitimas)){}

Nat Fachada_Variante::tamanoTablero() const{
    return _variante.tamanoTablero();
}

Nat Fachada_Variante::fichas() const{
    return _variante.fichas();
}

Nat Fachada_Variante::puntajeLetra(Letra l) const{
    return _variante.puntajeLetra(l);
}

bool Fachada_Variante::palabraLegitima(const Palabra& palabra) const{
    Repositorio palabra_aux = vectorToList(palabra);
    return _variante.palabraLegitima(palabra_aux);
}


set<Repositorio> Fachada_Variante::setvectorToList(const set<Palabra> palabra) const{
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
Repositorio Fachada_Variante::vectorToList(Palabra pal) const{
    Repositorio res;
    for (int i=0; i<pal.size(); i++){
        res.push_back(pal[i]);
    }
    return res;
}
