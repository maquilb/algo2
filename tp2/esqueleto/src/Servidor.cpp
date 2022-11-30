#include "Servidor.h"

Servidor::Servidor(Nat cantJugadores, const Variante &variante, const Repositorio &r) : _cantJugadoresConectados(0), _cantJugadoresEsperados(cantJugadores), _notificacionesPersonales(cantJugadores), _cantVistosNotificacionesParaTodos(cantJugadores), _notificacionesParaTodos(), _cantDeNotificaciones(0), _juego(cantJugadores, variante, r) {}

void Servidor::conectarCliente() {
    enviarNotiPers(_cantJugadoresConectados, Notificacion::nuevaIdCliente(_cantJugadoresConectados));
    _cantJugadoresConectados ++;
    if (_cantJugadoresConectados == _cantJugadoresEsperados){
        Nat tamTablero = _juego.variante().tamanoTablero();
        enviarNotiTodos(Notificacion::nuevaEmpezar(tamTablero));
        enviarNotiTodos(Notificacion::nuevaTurnoDe(0));
        for(Nat i=0; i<_cantJugadoresEsperados; i++){
            enviarNotiPers(i, Notificacion::nuevaReponer(fichasIniciales(i)));
        }
    }
}

void Servidor::recibirMensaje(IdCliente id, const Ocurrencia &o) {
    bool esValida = ((empezoJuego()) && (id == _juego.turno()));
    if(esValida){
        bool jugValida = _juego.jugadaValida(o);

        if(jugValida){
            vector<Nat> fichasAnteriores = _juego.fichasDeJugador(id);
            Nat puntajePrevio = _juego.puntaje(id);
            _juego.ubicar(o);
            const vector<Nat> fichasPostJugada = _juego.fichasDeJugador(id);
            enviarNotiTodos(Notificacion::nuevaUbicar(id,o));
            Nat puntajePostJugada = _juego.puntaje(id);
            enviarNotiTodos(Notificacion::nuevaSumaPuntos(id, puntajePostJugada - puntajePrevio));
            enviarNotiTodos(Notificacion::nuevaTurnoDe(_juego.turno()));
            enviarNotiPers(id, Notificacion::nuevaReponer(fichasRepuestas(id, fichasAnteriores, fichasPostJugada)));
        } else {
            enviarNotiPers(id, Notificacion::nuevaMal());
        }
    }
}

Nat Servidor::jugadoresEsperados() {
    return _cantJugadoresEsperados;
}

Nat Servidor::jugadoresConectados() {
    return _cantJugadoresConectados;
}

std::list<Notificacion> Servidor::notificaciones(IdCliente id) {
    return ordenarNotificaciones(id);
    queue<tuple<Notificacion, Nat>> notisPers{};
    //Puede ser que este paso no sea necesario, porque vacio la cola cuando llamo a la funcion colaAVector
    _notificacionesPersonales[id] = notisPers;
    Nat cant = _notificacionesParaTodos.size();
    _cantVistosNotificacionesParaTodos[id] +=cant;
}

const Juego &Servidor::juegoDelServidor() {
    return _juego;
}

bool Servidor::empezoJuego() {
    return _cantJugadoresEsperados == _cantJugadoresConectados;
}

void Servidor::enviarNotiTodos(Notificacion notificacion) {
    Notificacion notif = notificacion;
    Nat cant = _cantDeNotificaciones;
    tuple<Notificacion, Nat> noti = make_tuple(notif, cant);
    _notificacionesParaTodos.push_back(noti);
    _cantDeNotificaciones++;
}

void Servidor::enviarNotiPers(Nat i, Notificacion notificacion) {
    Notificacion notif = notificacion;
    Nat cant = _cantDeNotificaciones;
    tuple<Notificacion, Nat> noti = make_tuple(notif, cant);
    _notificacionesPersonales[i].push(noti);
    _cantDeNotificaciones++;
}

std::list<Notificacion> Servidor::ordenarNotificaciones(IdCliente id) {
    vector<tuple<Notificacion, Nat>> notisPers;
    notisPers = colaAVector(_notificacionesPersonales[id]);
    Nat prim = _cantVistosNotificacionesParaTodos[id];
    list<Notificacion> res;
    if (prim < _notificacionesParaTodos.size()){
        vector<tuple<Notificacion, Nat>> notisTodos;
        for(Nat i = prim; i<_notificacionesParaTodos.size(); i++){
            notisTodos.push_back(_notificacionesParaTodos[i]);
        }
        Merge(res, notisPers, notisTodos);
    } else {
        for (Nat i = 0; i<notisPers.size(); i++){
            res.push_back(get<0>(notisPers[i]));
        }
    }
    return res;
}

multiset<Letra> Servidor::fichasIniciales(Nat id) {
    multiset<Letra> res;
    for (int i=0; i < TAMANIO_ALFABETO; i++) {
        Nat k = _juego.cantFicha(id, inversaDeOrd(i));
        for (int j=0; j < k; j++) {
            res.insert(inversaDeOrd(i));
        }
    }
    return res;
}

multiset<Letra> Servidor::fichasRepuestas(Nat id, vector<Nat> fichasAnteriores, const vector<Nat> fichasPostJugada) {
    multiset<Letra> res;
    for(Nat i=0; i < fichasPostJugada.size() - 1 ;i++ ){
        if(fichasAnteriores[i] < fichasPostJugada[i]){
            Nat cant =  fichasPostJugada[i] - fichasAnteriores[i];
            while(cant > 0){
                res.insert(inversaDeOrd(i));
                cant--;
            }
        }
    }
    return res;
}

vector<tuple<Notificacion, Nat>> Servidor::colaAVector(queue<tuple<Notificacion, Nat>> &notisPers) {
    vector<tuple<Notificacion, Nat>> res;
    while (!notisPers.empty()){
        res.push_back(notisPers.front());
        notisPers.pop();
    }
    return res;
}

void Servidor::Merge(list<Notificacion> res, vector<tuple<Notificacion, Nat>> notisPers,
                     vector<tuple<Notificacion, Nat>> notisTodos) {
    Nat iP = 0;
    Nat iT = 0;
    Nat tam = notisPers.size() + notisTodos.size();
    for(Nat i=0; i<tam; i++){
        if (iP<notisPers.size() && (iT>= notisTodos.size() || get<1>(notisPers[iP])<get<1>(notisTodos[iT]))){
            res.push_back(get<0>(notisPers[iP]));
            iP++;
        } else {
            res.push_back(get<0>(notisTodos[iT]));
            iT++;
        }
    }
}

