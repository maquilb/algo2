#ifndef TP_JUEGODEPALABRAS_CONJUNTO_TRIE_H
#define TP_JUEGODEPALABRAS_CONJUNTO_TRIE_H

#include "Letra.h"
#include "Tipos.h"

using namespace std;

/*  Conjunto implementado sobre un Trie */
class conjunto_Trie {
public:
    conjunto_Trie();

    ~conjunto_Trie();

    void agregar(Repositorio &palabra);

    bool esVacio();

    bool pertenece(const Repositorio &palabra) const;

    /* Longitud de la palabra mas larga */
    Nat lmax() const;

    void agregarPalabras(set<Repositorio> &palabras);

private:
    struct Nodo{
        vector<Nodo*> _siguientes;
        bool _palabraDefinida;
        Nodo() : _siguientes(TAMANIO_ALFABETO, nullptr) {
            _palabraDefinida = false;
        }
    };

    Nodo* _raiz;
    Nat _longitudmax;

    void eliminar(Nodo* act);
};


#endif //TP_JUEGODEPALABRAS_CONJUNTO_TRIE_H
