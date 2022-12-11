
#include "Juego.h"

Juego::Juego(Nat k, const Variante &v, const Repositorio &r): _variante(v), _repositorio(r), _tablero(Tablero(_variante.tamanoTablero())), _jugadores(k, Jugador()), _cantidadDeTurnos(0), _turnoDe(0), _siguiente_letra(r.begin()) {
    for (int i = 0; i < _jugadores.size(); i++) darFichas(i,v.fichas());
}

void Juego::ubicar(const Ocurrencia &o) {
    _tablero.ponerLetras(o, _cantidadDeTurnos);
    _cantidadDeTurnos ++;
    _jugadores[_turnoDe]._ocurrenciasDelJugador.push(o);
    reponerFichas(o);
    _turnoDe ++;
    if (_turnoDe == _jugadores.size()){
        _turnoDe = 0;
    }
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
    Nat puntosACalcular;
    puntosACalcular = calcularPuntosPalabrasJugadas(_jugadores[id]._ocurrenciasDelJugador);
    _jugadores[id]._puntajeDelJugador += puntosACalcular;
    return _jugadores[id]._puntajeDelJugador;
}

Nat Juego::cantFicha(IdCliente id, Letra l) {
    return _jugadores[id]._fichasDelJugador[ord(l)];
}

void Juego :: reponerFichas(const Ocurrencia &o){
    auto it = o.begin();
    while (it != o.end()){
        tuple<Nat, Nat, Letra> ficha = *it;
        Letra l = get<2>(ficha);
        _jugadores[_turnoDe]._fichasDelJugador[ord(l)]--;
        it ++;
    }
    darFichas(_turnoDe,o.size());
}
void Juego :: darFichas(const Nat &id, const Nat& cant){
    for(Nat i = 0;i < cant;i++){
        Letra nueva_l = *_siguiente_letra;
        _jugadores[id]._fichasDelJugador[ord(nueva_l)]++;
        _siguiente_letra++;
    }
}

Nat Juego::calcularPuntos(queue<list<Letra>> &palabras){
    Nat res = 0;
    while(!palabras.empty()){
        list<Letra> p = palabras.front();
        auto it = p.begin();
        while(it != p.end()){
            Letra l = *it;
            res += _variante.puntajeLetra(l);
            it++;
        }
        palabras.pop();
    }
    return res;
}

tuple<bool, bool> Juego::HorizontalOVertical(Ocurrencia o){
    bool horizontal = true, vertical = true;

    if (o.size()>1) {
        auto it = o.begin();
        Nat fila0 = get<0>(*it);
        Nat col0 = get<1>(*it);
        it++;
        while(it != o.end() && (horizontal || vertical)){
            Nat fila1 = get<0>(*it);
            Nat col1 = get<1>(*it);
            if (horizontal) horizontal = fila1 == fila0;
            if (vertical) vertical = col1 == col0;
            fila0 = fila1;
            col0 = col1;
            it++;
        }
    }
    return make_tuple(horizontal,vertical);
}

Nat  Juego::calcularPuntosPalabrasJugadas(queue<Ocurrencia> &ocus){
    Nat res = 0;
    while(not ocus.empty()){
        Ocurrencia p = ocus.front();
        queue<list<Letra>> palabras = palabrasFormadas(p);
        ocus.pop();
        res += calcularPuntos(palabras);
    }
    return res;
}

queue<list<Letra>> Juego::palabrasFormadas(Ocurrencia &o){
    if (not o.empty()){
        tuple<bool,bool> esHorizontalOVertical = HorizontalOVertical(o);
        bool horizontal = get<0>(esHorizontalOVertical);
        bool vertical = get<1>(esHorizontalOVertical);
        bool sentido = true;
        if(vertical){
            sentido = false;
        }
        queue<list<Letra>> res = palabrasFormadasTransversales(o, sentido);
        list<Letra> palabraPrincipal = formarPalabraEnSentido(*o.begin(),sentido);
        res.push(palabraPrincipal);
        return res;
    } else {
        return {};
    }
}


queue<list<Letra>> Juego::palabrasFormadasTransversales(Ocurrencia &o, bool sentido) {
    queue<list<Letra>> res;
    auto it = o.begin();
    while(it != o.end()){
        Ficha f= *it;
        list<Letra> palabra = formarPalabraEnSentido(f, not sentido);
        res.push(palabra);
        it++;
    }
    return res;
}


list<Letra> Juego::formarPalabraEnSentido(Ficha f,bool sentido) const {
    list<Letra> res = {get<2>(f)};

    Nat fila = get<0>(f);
    Nat col = get<1>(f);

    bool jugada = _tablero.estaOcupada(fila, col);
    // jugada == false -> estoy formando transversales en jugadaValida

    int turno_jug = (-1);
    if(jugada) turno_jug = _tablero.turnoApoyado(fila,col);

    signed int ant_fil = fila ,ant_col = col, sig_fil = fila, sig_col = col;

    (sentido)? ant_col-=1: ant_fil-=1;
    (sentido)? sig_col+=1: sig_fil+=1;

    while( _tablero.estaOcupada(ant_fil,ant_col) &&
            (not jugada || (jugada && turno_jug >= _tablero.turnoApoyado(ant_fil,ant_col))) ){

        Letra letraAdelante = _tablero.letraEnPos(ant_fil, ant_col);
        res.push_front(letraAdelante);
        (sentido)? ant_col-=1 : ant_fil-=1;
    }

    while( _tablero.estaOcupada(sig_fil,sig_col) &&
            (not jugada || (jugada && turno_jug >= _tablero.turnoApoyado(sig_fil, sig_col))) ){

        Letra letraSiguiente = _tablero.letraEnPos(sig_fil, sig_col);
        res.push_back(letraSiguiente);
        (sentido)? sig_col+=1 : sig_fil+=1;
    }

    return res;
}

bool Juego::jugadaValida(const Ocurrencia &o) {
    bool res = true;
    if (o.empty()) {
        return true;
    } else {
        const Variante &varJ = _variante;
        Nat largoMax = _variante.lmax();

        if (res && (varJ.fichas() < o.size() or largoMax < o.size())) res = false;

        vector<Nat> &fichasJugador = (_jugadores[_turnoDe])._fichasDelJugador;

        if (res && not tieneLasFichas(fichasJugador, o)) res = false;

        if (res && not _tablero.sonCeldasLibres(o)) res = false;

        bool esHorizontal = false;
        bool esVertical = false;

        if (res) {
            tuple<bool,bool> esHorizontalOVertical = HorizontalOVertical(o);
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
            list<Letra> palabraPrincipal = FormarPalabraPrincipal(_tablero, o, sentido);
            res = varJ.palabraLegitima(palabraPrincipal);
            if(res){
                list<list<Letra>> palabras = PalabrasTransversales(_tablero,o,sentido);
                res = varJ.palabrasLegitimas(palabras);
            }
        }
    }

    return res;
}

bool Juego::tieneLasFichas(vector<Nat> &fichas, const Ocurrencia &o) {
    bool res = true;
    vector<tuple<Letra,Nat>>apariciones;
    auto it = o.begin();
    while (it != o.end()){
        Letra letra0 = get<2>(*it);
        SumarAparicion(apariciones,letra0);
        it++;
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

void Juego::SumarAparicion(vector<tuple<Letra, Nat>> &apariciones, Letra l) {
    bool noEsta = true;
    int i = 0;
    while( noEsta && i < apariciones.size()){
        if(get<0>(apariciones[i]) == l){
            noEsta = false;
            get<1>(apariciones[i])++;
        }
        i++;
    }
    if(noEsta) apariciones.push_back(make_tuple(l,1));
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
        it++;
    }
    return res;
}

Ficha Juego::masChico(Ficha &ficha1, Ficha &ficha2, bool sentido) {
    Ficha res = ficha1;
    if(sentido){
        if(get<0>(ficha2) < get<0>(ficha1)) res = ficha2;
    }else{
        if(get<1>(ficha2) < get<1>(ficha1)) res = ficha2;
    }
    return res;
}

Ficha Juego::masGrande(Ficha &ficha1, Ficha &ficha2, bool sentido) {
    Ficha res = ficha1;
    if(sentido){
        if(get<0>(ficha2) > get<0>(ficha1)) res = ficha2;
    }else{
        if(get<1>(ficha2) > get<1>(ficha1)) res = ficha2;
    }
    return res;}

Nat Juego::distanciaEntreFichas(Ficha &min, Ficha &max, bool sentido) {
    return (sentido)? get<0>(max) - get<0>(min) : get<1>(max) - get<1>(min);
}

list<list<Letra>> Juego::PalabrasTransversales(const Tablero &tab, const Ocurrencia &o, bool sentido){
    list<list<Letra>> res;
    auto it = o.begin();
    while(it != o.end()){
        Ficha f= *it;
        list<Letra> palabra;
/*        if (sentido){
            palabra =  palabraTransversalVertical(tab, f);
        } else {
            palabra =  palabraTransversalHorizontal(tab, f);
        }*/
        palabra = formarPalabraEnSentido(f,not sentido);
        res.push_back(palabra);
        it++;
    }
    return res;
}


vector<Ficha> Juego::ocurrenciaAVector(const Ocurrencia &o) {
    vector<Ficha> res;
    auto it = o.begin();
    while( it != o.end()){
        Ficha fich = *it;
        res.push_back(fich);
        it++;
    }
    return res;
}

void Juego::ordenarVectorDeFichas(vector<Ficha> &vect_fichas, bool sentido) {
    bool swapped = true;
    int i = vect_fichas.size();
    while(swapped) {
        swapped = false;
        int j = 0;
        while (j < i-1) {
            bool mayorFila = get<0>(vect_fichas[j]) > get<0>(vect_fichas[j+1]);
            bool mayorColumna = get<1>(vect_fichas[j]) > get<1>(vect_fichas[j+1]);
            if((mayorFila and sentido) or ((not sentido) and mayorColumna)){
                Ficha temp = vect_fichas[j];
                vect_fichas[j] = vect_fichas[j+1];
                vect_fichas[j+1] = temp;
                swapped = true;
            }
            j++;
        }
        i--;
    }
}


list<Letra> Juego::FormarPalabraPrincipal( const Tablero& tab, const Ocurrencia& o ,bool sentido) {
    list<Letra> res;
    // pasa a vector la ocurrencia y la ordena,
    vector<Ficha> fichas = ocurrenciaAVector(o);
    ordenarVectorDeFichas(fichas, sentido);

    // pone la primer letra en el resultado e inicializa una variable para iterar sobre el vector fichas
    res.push_back(get<2>(fichas[0]));
    signed int fila = get<0>(fichas[0]);
    signed int col = get<1>(fichas[0]);
    int i = 1;

    // iteradores para recorrer el tablero en base a la primer ficha del vector.
    signed int ant_fil = fila ,ant_col = col, sig_fil = fila, sig_col = col;
    (sentido)? ant_col-=1: ant_fil-=1;
    (sentido)? sig_col+=1: sig_fil+=1;

    // condiciones de guarda de los ciclos
    bool esContigua = true;
    bool termino = false;

    while( _tablero.enTablero(ant_fil, ant_col) && _tablero.hayUnaLetraEnPos(ant_fil, ant_col)){
        Letra letraAnterior = _tablero.letraEnPos(ant_fil, ant_col);
        res.push_front(letraAnterior);
        (sentido)? ant_col-=1: ant_fil-=1;
    }
    while(_tablero.enTablero(sig_fil, sig_col) && not termino && esContigua){
        if(_tablero.hayUnaLetraEnPos(sig_fil, sig_col)){
            Letra letraPosterior = _tablero.letraEnPos(sig_fil, sig_col);
            res.push_back(letraPosterior);
        }else{
            if(i == fichas.size()){
                termino = true;
            }else{
                Nat ficha_i_fil = get<0>(fichas[i]);
                Nat ficha_i_col = get<1>(fichas[i]);

                if(ficha_i_fil == sig_fil && ficha_i_col == sig_col){
                    Letra LetraNueva = get<2>(fichas[i]);
                    res.push_back(LetraNueva);
                    i++;
                }else{
                    res = list<Letra>();
                    esContigua = false;
                }
            }
        }
        (sentido)? sig_col+=1: sig_fil+=1;
    }
    return res;
}

