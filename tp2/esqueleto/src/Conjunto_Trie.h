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

    void copiar(vector<Nodo*>& v1, vector<Nodo*> v2){
        for(int i = 0 ; i < v2.size(); i++){
            if(v2[i] != nullptr){
                v1[i] = new Nodo();
                if(v2[i]->_palabraDefinida){
                    v1[i]->_palabraDefinida = true;
                }
                copiar(v1[i]->_siguientes, v2[i]->_siguientes);
            }
        }
    }


    Nodo* _raiz;
    Nat _longitudmax;

    void eliminar(Nodo* act);
};


#endif //TP_JUEGODEPALABRAS_CONJUNTO_TRIE_H

