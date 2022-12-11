#include "Fachada_Variante.h"

Fachada_Variante::Fachada_Variante( Nat tamanoTab, Nat cantFichas, const map<Letra, Nat>& puntajes, const set<vector<Letra>>& palabrasLegitimas) : _variante(tamanoTab, cantFichas, puntajes, setvectorToList(palabrasLegitimas)){}

Nat Fachada_Variante::tamanoTablero() const{
    return variante().tamanoTablero();
}

Nat Fachada_Variante::fichas() const{
    return variante().fichas();
}

Nat Fachada_Variante::puntajeLetra(Letra l) const{
    return variante().puntajeLetra(l);
}

bool Fachada_Variante::palabraLegitima(const Palabra& palabra) const{
    list<Letra> palabra_aux = vectorToList(palabra);
    return variante().palabraLegitima(palabra_aux);
}


set<list<Letra>> Fachada_Variante::setvectorToList(const set<Palabra>& palabra) const{
    set<list<Letra>> res;
    auto it = palabra.begin();
    while (it != palabra.end()){
        Palabra pal = *it;
        list<Letra> pal_aux;
        for (int i=0; i<pal.size(); i++){
            pal_aux.push_back(pal[i]);
        }
        res.insert(pal_aux);
        ++it;
    }
    return res;
}
list<Letra> Fachada_Variante::vectorToList(const Palabra& pal) const{
    list<Letra> res;
    for (int i=0; i<pal.size(); i++){
        res.push_back(pal[i]);
    }
    return res;
}

const Variante& Fachada_Variante::variante() const {
    return _variante;
}
