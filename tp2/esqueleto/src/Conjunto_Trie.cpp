#include "Conjunto_Trie.h"

conjunto_Trie::conjunto_Trie() : _raiz(nullptr), _longitudmax(0) {
    _raiz = new Nodo();
}

conjunto_Trie::~conjunto_Trie() {
    eliminar(_raiz);
}

void conjunto_Trie::eliminar(conjunto_Trie::Nodo* n) {
    /*if (n != nullptr){
        for(int i = 0; i < TAMANIO_ALFABETO; i++){
            eliminar(n->_siguientes[i]);
        }
        n = nullptr;
        delete n;
    }
    delete n; */
}

void conjunto_Trie::agregar(Repositorio &palabra) {
    Nat longitud = palabra.size();
    if (_longitudmax < longitud){
        _longitudmax = longitud;
    }
    Nodo* actual = _raiz;
    auto it = palabra.begin();
    while(it != palabra.end()){
        Nat numLetra = ord(*it);
        if (actual->_siguientes[numLetra] == nullptr){
            actual->_siguientes[numLetra] = new Nodo;
        }
        actual = actual->_siguientes[numLetra];
        it++;
    }
    actual->_palabraDefinida = true;
}

bool conjunto_Trie::esVacio() const{
    bool res = _raiz == nullptr;
    int i =0;
    Nat longitud = _raiz->_siguientes.size();
    while (res && i<longitud){
        res = _raiz->_siguientes[i] == nullptr;
        i++;
    }
    return res;
}

bool conjunto_Trie::pertenece(const Repositorio& palabra) const {
    bool res = true;
    Nodo* actual = _raiz;
    if (palabra.size() > _longitudmax){
        res = false;
    } else {
        auto it = palabra.begin();
        while(it != palabra.end()){
            Nat numLetra = ord(*it);
            if (actual->_siguientes[numLetra] == nullptr){
                return false;
            }
            actual = actual->_siguientes[numLetra];
            it++;
        }
        res = actual->_palabraDefinida;
    }
    return res;
}

Nat conjunto_Trie::lmax() const {
    return _longitudmax;
}

void conjunto_Trie::agregarPalabras(set<Repositorio> &palabras) {
    auto it = palabras.begin();
    while(it != palabras.end()){
        agregar(const_cast<Repositorio &>(*it));
        it++;
    }
}

conjunto_Trie::conjunto_Trie(const conjunto_Trie &aCopiar) {
    *this = aCopiar;
}

conjunto_Trie &conjunto_Trie::operator=(const conjunto_Trie &d) {
    eliminar(_raiz);
    this->_raiz = new Nodo();
    if(not d.esVacio()){
        copiar(this->_raiz->_siguientes, d._raiz->_siguientes);
    }
    return *this;
}
