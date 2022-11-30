
#include "Juego.h"

Juego::Juego(Nat k, const Variante &v, const Repositorio &r): _variante(v), _repositorio(r), _tablero(Tablero(_variante.tamanoTablero())), _jugadores(k, Jugador(v, r)), _cantidadDeTurnos(0), _turnoDe(0) {}

void Juego::ubicar(const Ocurrencia &o) {
    _tablero.ponerLetras(o, _cantidadDeTurnos);
    _cantidadDeTurnos ++;
    _turnoDe ++;
    if (_turnoDe == _jugadores.size()){
        _turnoDe = 0;
    }
    _jugadores[_turnoDe]._ocurrenciasDelJugador.push(o);
    reponerFichas(o);
}

IdCliente Juego::turno() {
    return _turnoDe;
}

const Variante &Juego::variante() {
    return _variante;
}

Nat Juego::cantDeJugadores() {
    return _jugadores.size();
}

const Repositorio &Juego::repositorio() {
    return _repositorio;
}

const Tablero &Juego::tablero() {
    return _tablero;
}

const tuple<bool, Letra, Nat> Juego::posDeTablero(Nat i, Nat j) {
    bool hayLetra = _tablero.hayUnaLetraEnPos(i, j);
    Letra letra = _tablero.letraEnPos(i, j);
    Nat turno = _tablero.turnoApoyado(i,j);
    const tuple<bool, Letra, Nat> res = make_tuple(hayLetra, letra, turno);
    return res;
}

Nat Juego::turnoActual() {
    return _turnoDe;
}

const vector<Nat> Juego::fichasDeJugador(Nat i){
    return _jugadores[i]._fichasDelJugador;
}

Nat Juego::cantDeFichasLetra(IdCliente id, Letra l) {
    return _jugadores[id]._fichasDelJugador[ord(l)];
}

bool Juego::hayLetra(Nat x, Nat y) {
    return _tablero.hayUnaLetraEnPos(x, y);
}

Letra Juego::letra(Nat i, Nat j) {
    return _tablero.letraEnPos(i, j);
}

Nat Juego::puntaje(IdCliente id) {
    queue <Ocurrencia> ocus = _jugadores[id]._ocurrenciasDelJugador;
    Nat puntosACalcular;
    puntosACalcular = calcularPuntosPalabrasJugadas(ocus);
    _jugadores[id]._puntajeDelJugador += puntosACalcular;
    return _jugadores[id]._puntajeDelJugador;
}

Nat Juego::cantFicha(IdCliente id, Letra l) {
    return 0;
}

void Juego :: reponerFichas(const Ocurrencia o){
    auto it = o.begin();
    while (it != o.end()){
        tuple<Nat, Nat, Letra> ficha = *it;
        Letra l = get<2>(ficha);
        _jugadores[_turnoDe]._fichasDelJugador[ord(l)]--;
        Letra nueva_l = _repositorio.front();
        _jugadores[_turnoDe]._fichasDelJugador[ord(nueva_l)]++;
        _repositorio.pop_front();
        it ++;
    }
}

Nat Juego::calcularPuntos(queue<Repositorio> &palabras){
    Nat res = 0;
    while(!palabras.empty()){
        Repositorio p = palabras.front();
        auto it = p.front();
        while(it != p.back()){
            Letra l = p.front();
            res += _variante.puntajeLetra(l);
            it++;
        }
        palabras.pop();
    }
    return res;
}

tuple<bool, bool> Juego::HorizontalOVertical(Ocurrencia o){
    tuple<bool, bool> res;
    bool horizontal = get<0>(res);
    bool vertical = get<1>(res);
    horizontal = true;
    vertical = true;
    if (o.size()>0){
        auto it = o.begin();
        Nat fila0 = get<0>(*it);
        Nat col0 = get<1>(*it);
        it++;
        while(it != o.end() && (horizontal || vertical)){
            Nat fila1 = get<0>(*it);
            Nat col1 = get<1>(*it);
            if(horizontal){
                horizontal = fila1 == fila0;
            }
            if (vertical){
                vertical = col1 != col0;
            }
            fila0 = fila1;
            col0 = col1;
            it++;
        }
    }
    return res;
}

Nat  Juego::calcularPuntosPalabrasJugadas(queue<Ocurrencia> &ocus){
    Nat res = 0;
    while(!ocus.empty()){
        Ocurrencia p = ocus.front();
        queue<Repositorio> palabras = palabrasFormadas(p);
        ocus.pop();
        res += calcularPuntos(palabras);
    }
    return res;
}

queue<Repositorio> Juego::palabrasFormadas(Ocurrencia o){
    if (o.size()>0){
        tuple<bool,bool> esHorizontalOVertical = HorizontalOVertical(o);
        bool horizontal = get<0>(esHorizontalOVertical);
        bool vertical = get<1>(esHorizontalOVertical);
        bool sentido = true;
        if(vertical){
            sentido = false;
        }
        queue<Repositorio> res = palabrasFormadasTransversales(o, sentido);
        Repositorio palabraPrincipal = formarPalabraJugadaPrincial(o,sentido);
        res.push(palabraPrincipal);
        return res;
    } else {
        return queue<Repositorio>();
    }
}

Repositorio Juego::palabraFormadaVertical(Ficha f) {
    Repositorio res;
    Letra l = get<2>(f);
    res.push_front(l);
    Nat fila = get<0>(f);
    Nat columna = get<1>(f);
    Nat arriba = fila - 1;
    Nat abajo = fila + 1;
    Nat turno = _tablero.turnoApoyado(fila, columna);
    while (0<= arriba && _tablero.estaOcupada(arriba, columna) && turno>= _tablero.turnoApoyado(arriba, columna)){
        Letra l0 = _tablero.letraEnPos(arriba, columna);
        res.push_front(l0);
        arriba--;
    }
    while (abajo <= _tablero.tamanio() && _tablero.estaOcupada(abajo, columna) && turno>= _tablero.turnoApoyado(abajo, columna)){
        Letra l0 = _tablero.letraEnPos(abajo, columna);
        res.push_back(l0);
        abajo++;
    }
    return res;
}

Repositorio Juego::palabraFormadaHorizontal(Ficha f) {
    Repositorio res;
    Letra l = get<2>(f);
    res.push_front(l);
    Nat fila = get<0>(f);
    Nat columna = get<1>(f);
    Nat izquierda = columna - 1;
    Nat derecha = columna + 1;
    Nat turno = _tablero.turnoApoyado(fila, columna);
    while (derecha <=_tablero.tamanio() && _tablero.estaOcupada(fila, derecha) && turno>= _tablero.turnoApoyado(fila, derecha)){
        Letra l0 = _tablero.letraEnPos(fila, derecha);
        res.push_back(l0);
        derecha++;
    }
    while (0<= izquierda && _tablero.estaOcupada(fila, izquierda) && turno>= _tablero.turnoApoyado(fila, derecha)){
        Letra l0 = _tablero.letraEnPos(fila, izquierda);
        res.push_front(l0);
        izquierda--;
    }
    return res;
}

queue<Repositorio> Juego::palabrasFormadasTransversales(Ocurrencia o, bool sentido) {
    queue<Repositorio> res;
    auto it = o.begin();
    while(it != o.end()){
        Ficha f= *it;
        Repositorio palabra;
        if (sentido){
            palabra =  palabraFormadaVertical(f);
        } else {
            palabra =  palabraFormadaHorizontal(f);
        }
        res.push(palabra);
        it++;
    }
    return res;
}

Repositorio Juego::formarPalabraJugadaPrincial(Ocurrencia o, bool sentido) {
    auto it = o.begin();
    Ficha f = *it;
    Repositorio res;
    if(sentido){
        res = palabraFormadaHorizontal(f);
    } else {
        res = palabraFormadaVertical(f);
    }
    return res;
}

bool Juego::jugadaValida(const Ocurrencia& o) {
    bool res = true;
    if (o.empty()) {
        return true;
    } else {
        const Variante &varJ = _variante;
        Nat largoMax = _variante.lmax();

        if (res && (varJ.fichas() < o.size() or largoMax < o.size())) res = false;

        vector<Nat> &fichasJugador = (this->_jugadores[this->_turnoDe])._fichasDelJugador;

        if (res && not tieneLasFichas(fichasJugador, o)) res = false;

        if (res && not _tablero.sonCeldasLibres(o)) res = false; // �porque me pone mal _tablero?

        bool esHorizontal = false;
        bool esVertical = false;

        if (res) {
            tuple<bool,bool> esHorizontalOVertical = HorizontalOVertical(o); // hay que crear metodo const
            esHorizontal = get<0>(esHorizontalOVertical);
            esVertical = get<1>(esHorizontalOVertical);
            if (not(esHorizontal or esVertical)) {
                res = false;
            }
        }

        if (res && not ocurrenciaSinPosicionesRepetidas(o)) res = false;

        bool sentido = true;
        if (res && esVertical) sentido = false;

        if (res && 2 <= o.size() && DistanciaMayorAlongMax(o, sentido, largoMax)) res = false;

        if (res) {
            Repositorio palabraPrincipal = FormarPalabraPrincipal(this->_tablero, o, sentido);
            if ( not varJ.palabraLegitima(palabraPrincipal)) res = false;
            if(res){
                list<Repositorio> palabras = PalabrasTransversales(this->_tablero,o,sentido);
                if(not varJ.palabrasLegitimas(palabras)) res = false;

            }
        }
    }
    return res;
}

bool Juego::tieneLasFichas(vector<Nat> &fichas, const Ocurrencia &set) {
    bool res = true;
    vector<tuple<Letra,Nat>>apariciones;
    for ( auto const & f : set) {
        Letra letra0 = get<2>(f);
        SumarAparicion(apariciones,letra0);
    }
    int i = 0;
    while(res && i < apariciones.size()){
        Letra letra0 = get<0>(apariciones[i]);
        Nat cant = get<1>(apariciones[i]);
        if(fichas[ord(letra0)] < cant) res = false;
        i++;
    }

    return res;
}

void Juego::SumarAparicion(vector<tuple<Letra, Nat>> apariciones, Letra l) {
    bool noEsta = true;
    int i = 0;
    while( noEsta && i < apariciones.size()){
        if(get<0>(apariciones[i]) == l){
            noEsta = false;
            get<1>(apariciones[i])++;
        }
    }
    if( noEsta ) apariciones.push_back(make_tuple(l,1));
}

bool Juego::ocurrenciaSinPosicionesRepetidas(const Ocurrencia &o) {
    bool res = true;
    auto it0 = o.begin();
    while (res && it0 != o.end()){
        Ficha ficha0 = *it0;
        auto it1 = it0;
        it1++;
        while(res && it1 != o.end()){
            Ficha ficha1 = *it1;
            if(get<0>(ficha0) == get<0>(ficha1) && get<1>(ficha0) == get<1>(ficha1)){
                res = false;
            }
            it1++;
        }
        it0++;
    }
    return res;
}

bool Juego::DistanciaMayorAlongMax(const Ocurrencia &o, bool sentido, Nat largoMax) {
    bool res = false;
    auto it = o.begin();
    Ficha min = *it, max = *it;
    it++;
    Nat distancia = 0;
    while(not res && it != o.end()){
        Ficha ficha0 = *it;
        min = masChico(min,ficha0, sentido);
        max = masGrande(max, ficha0, sentido);
        distancia = distanciaEntreFichas(min,max,sentido);
        if(largoMax < distancia) res = true;
    }
    return res;
}

Ficha Juego::masChico(Ficha ficha1, Ficha ficha2, bool sentido) {
    Ficha res = ficha1;
    if(sentido){
        if(get<0>(ficha2) < get<0>(ficha1)) res = ficha2;
    }else{
        if(get<1>(ficha2) < get<1>(ficha1)) res = ficha2;
    }
    return res;
}

Ficha Juego::masGrande(Ficha ficha1, Ficha ficha2, bool sentido) {
    Ficha res = ficha1;
    if(sentido){
        if(get<0>(ficha2) > get<0>(ficha1)) res = ficha2;
    }else{
        if(get<1>(ficha2) > get<1>(ficha1)) res = ficha2;
    }
    return res;}

Nat Juego::distanciaEntreFichas(Ficha min, Ficha max, bool sentido) {
    return (sentido)? get<0>(max) - get<0>(min) : get<1>(max) - get<1>(min);
}

list<Repositorio> Juego::PalabrasTransversales(Tablero tab, const Ocurrencia &o, bool sentido){
    list<Repositorio> res;
    auto it = o.begin();
    while(it != o.end()){
        Ficha f= *it;
        Repositorio palabra;
        if (sentido){
            palabra =  palabraTransversalVertical(tab, f);
        } else {
            palabra =  palabraTransversalHorizontal(tab, f);
        }
        res.push_back(palabra);
        it++;
    }
    return res;
}

Repositorio Juego::palabraTransversalHorizontal(const Tablero& tab, const Ficha &f){
    Repositorio res;
    Nat fila = get<0>(f);
    Nat Columna = get<1>(f);
    Nat izquierda = Columna - 1;
    Nat derecha = Columna + 1;
    res.push_back(get<2>(f));
    while( _tablero.estaOcupada(fila,derecha)){
        Letra letra0 = _tablero.letraEnPos(fila,derecha);
        res.push_front(letra0);
        derecha++;
    }
    while( _tablero.estaOcupada(fila,izquierda)){
        Letra letra0 = _tablero.letraEnPos(fila,izquierda);
        res.push_back(letra0);
        izquierda++;
    }
    return res;
}

Repositorio Juego::palabraTransversalVertical(const Tablero& tab, const Ficha &f){
    Repositorio res;
    Nat fila = get<0>(f);
    Nat Columna = get<1>(f);
    Nat Arriba = fila - 1;
    Nat Abajo = fila + 1;
    res.push_back(get<2>(f));
    while( _tablero.estaOcupada(Arriba,Columna)){
        Letra letra0 = _tablero.letraEnPos(Arriba,Columna);
        res.push_front(letra0);
        Arriba--;
    }
    while( _tablero.estaOcupada(Abajo,Columna)){
        Letra letra0 = _tablero.letraEnPos(Abajo,Columna);
        res.push_back(letra0);
        Abajo++;
    }
    return res;
}

vector<Ficha> Juego::ocurrenciaAVector(const Ocurrencia &o) {
    vector<Ficha> res;
    auto it = o.begin();
    while( it != o.end()){
        Ficha fich = *it;
        res.push_back(fich);
    }
    return res;
}

void Juego::ordenarVectorDeFichas(vector<Ficha> vect_fichas, bool sentido) {
    bool swapped = true;
    int i = vect_fichas.size();
    while(swapped) {
        swapped = false;
        int j = 0;
        while (j < i) {
            bool mayorFila = get<0>(vect_fichas[j]) > get<0>(vect_fichas[j+1]);
            bool mayorColumna = get<1>(vect_fichas[j]) > get<1>(vect_fichas[j+1]);
            if((mayorFila and sentido) or ((not sentido) and mayorColumna)){
                Ficha temp = vect_fichas[j];
                vect_fichas[j] = vect_fichas[j+1];
                vect_fichas[j+1] = temp;
                swapped = true;
                j++;
            }
        }
        i++;
    }
}


Repositorio Juego::FormarPalabraPrincipal( const Tablero& tab, const Ocurrencia& o ,bool sentido) {
    vector<Ficha> fichas = ocurrenciaAVector(o);
    ordenarVectorDeFichas(fichas, sentido);
    Repositorio res;
    if (sentido) {
        res = principalHorizontal(_tablero, fichas);
    } else {
        res = principalVertical(_tablero, fichas);
    }
    return res;
}

Repositorio Juego::principalHorizontal(const Tablero &tablero, const vector<Ficha> &fichas) {
    Repositorio res;
    res.push_back(get<2>(fichas[0]));
    Nat fila = get<0>(fichas[0]);
    Nat columna = get<1>(fichas[0]);
    Nat izquierda = columna - 1;
    Nat derecha = columna + 1;
    int i = 1;
    bool esContigua = true;
    bool termino = false;
    while( 0 <= izquierda && _tablero.hayUnaLetraEnPos(fila,izquierda)){
        Letra letraIzquierda = _tablero.letraEnPos(fila,izquierda);
        res.push_front(letraIzquierda);
        izquierda--;
    }
    while(derecha < _tablero.tamanio() && not termino && esContigua){
        if(_tablero.hayUnaLetraEnPos(fila,derecha)){
            Letra letraDerecha = _tablero.letraEnPos(fila,derecha);
            res.push_back(letraDerecha);
        }else{
            if(i == fichas.size()){
                termino = true;
            }else{
                if(get<1>(fichas[i]) == derecha){
                    Letra LetraNueva = get<2>(fichas[i]);
                    res.push_back(LetraNueva);
                    i++;
                }else{
                    res = Repositorio();
                    esContigua = false;
                }
            }
        }
        derecha++;
    }
    return res;
}


Repositorio Juego::principalVertical(const Tablero &tablero, const vector<Ficha> &fichas) {
    Repositorio res;
    res.push_back(get<2>(fichas[0]));
    Nat fila = get<0>(fichas[0]);
    Nat columna = get<1>(fichas[0]);
    Nat arriba = fila - 1;
    Nat abajo = fila + 1;
    int i = 1;
    bool esContigua = true;
    bool termino = false;
    while( 0 <= arriba && _tablero.hayUnaLetraEnPos(arriba,columna)){
        Letra letraArriba = _tablero.letraEnPos(arriba,columna);
        res.push_front(letraArriba);
        arriba--;
    }
    while(abajo < _tablero.tamanio() && not termino && esContigua){
        if(_tablero.hayUnaLetraEnPos(abajo,columna)){
            Letra letraAbajo = _tablero.letraEnPos(abajo,columna);
            res.push_back(letraAbajo);
        }else{
            if(i == fichas.size()){
                termino = true;
            }else{
                if(get<0>(fichas[i]) == abajo){
                    Letra LetraNueva = get<2>(fichas[i]);
                    res.push_back(LetraNueva);
                    i++;
                }else{
                    res = Repositorio();
                    esContigua = false;
                }
            }
        }
        abajo++;
    }
    return res;
}


