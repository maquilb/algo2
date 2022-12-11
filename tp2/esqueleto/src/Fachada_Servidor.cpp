#include "Fachada_Servidor.h"

Fachada_Servidor::Fachada_Servidor(
        Nat cantJugadores,
const Fachada_Variante& variante,
Repositorio& r
):  _servidor(cantJugadores, variante.variante(), r){}

IdCliente Fachada_Servidor::conectarCliente(){
    IdCliente res = _servidor.jugadoresConectados();
    _servidor.conectarCliente();
    return res;
}

void Fachada_Servidor::recibirMensaje(IdCliente id, const Ocurrencia& o){
    _servidor.recibirMensaje(id, o);
}

Nat Fachada_Servidor::jugadoresEsperados(){
    return _servidor.jugadoresEsperados();
}

Nat Fachada_Servidor::jugadoresConectados(){
    return _servidor.jugadoresConectados();
}

std::list<Notificacion> Fachada_Servidor::notificaciones(IdCliente id){
    return _servidor.notificaciones(id);
}