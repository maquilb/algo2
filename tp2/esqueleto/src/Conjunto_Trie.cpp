#include "Conjunto_Trie.h"

conjunto_Trie::conjunto_Trie() : _raiz(new Nodo), _longitudmax(0) {}

conjunto_Trie::~conjunto_Trie() {
    eliminar(_raiz);
}

void conjunto_Trie::eliminar(conjunto_Trie::Nodo* n) {
    if (n != nullptr){
        for(int i = 0; i < TAMANIO_ALFABETO; i++){
            eliminar(n->_siguientes[i]);
        }
        delete n;
    }
}

void conjunto_Trie::agregar(const Palabra &palabra) {
    Nat longitud = palabra.size();
    if (_longitudmax < longitud){
        _longitudmax = longitud;
    }
    Nodo* actual = _raiz;
    for (int i = 0; i < longitud; i++){
        int numLetra = ord(palabra[i]);
        if (actual->_siguientes[numLetra] == nullptr){
            actual->_siguientes[numLetra] = new Nodo;
        }
        actual = actual->_siguientes[numLetra];
    }
    actual->_palabraDefinida = true;
}

bool conjunto_Trie::esVacio() {
    bool res = _raiz == nullptr;
    int i =0;
    Nat longitud = _raiz->_siguientes.size();
    while (res && i<longitud){
        res = _raiz->_siguientes[i] == nullptr;
        i++;
    }
    return res;
}

bool conjunto_Trie::pertenece(const Palabra& palabra) const {
    bool res = true;
    Nodo* actual = _raiz;
    if (palabra.size() > _longitudmax){
        res = false;
    } else {
        for (int i = 0; i < palabra.size(); i++){
            int numLetra = ord(palabra[i]);
            if (actual->_siguientes[numLetra] == nullptr){
                return false;
            }
            actual = actual->_siguientes[numLetra];
        }
        res = actual->_palabraDefinida;
    }
    return res;
}

Nat conjunto_Trie::lmax() const {
    return _longitudmax;
}

void conjunto_Trie::agregarPalabras(set<Palabra>& palabras) {
    auto it = palabras.begin();
    while(it != palabras.end()){
        agregar(*it);
        it++;
    }
}
