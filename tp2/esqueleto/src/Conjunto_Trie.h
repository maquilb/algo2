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

    /**
    CONSTRUCTOR POR COPIA
    * Construye un diccionario por copia.
    **/
    conjunto_Trie(const conjunto_Trie& aCopiar);

    /**
    OPERADOR ASIGNACION
     */
    conjunto_Trie& operator=(const conjunto_Trie& d);


    void agregar(Repositorio &palabra);

    bool esVacio() const;

    bool pertenece(const Repositorio &palabra) const;

    /* Longitud de la palabra mas larga */
    Nat lmax() const;

    void agregarPalabras(set<Repositorio> palabras);

private:
    struct Nodo{
        vector<Nodo*> _siguientes;
        bool _palabraDefinida;
        Nodo() : _palabraDefinida(false), _siguientes(TAMANIO_ALFABETO, nullptr) {
        }
    };

    Nodo* copiar(Nodo* d){
        Nodo* n = new Nodo;
        n->_palabraDefinida = d->_palabraDefinida;
        n->_siguientes = d->_siguientes;
        for(int i = 0 ; i < TAMANIO_ALFABETO; i++){
            if(n->_siguientes[i] != nullptr){
                n->_siguientes[i] = copiar(n->_siguientes[i]);
            }
        }
        return n;
    }


    Nodo* _raiz;
    Nat _longitudmax;

    void eliminar(Nodo* act);
};


#endif //TP_JUEGODEPALABRAS_CONJUNTO_TRIE_H

