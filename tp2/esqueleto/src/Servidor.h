#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "Tipos.h"
#include "Notificacion.h"
#include "Juego.h"
#include <queue>


class Servidor {
public:
    /**
     * El constructor de Servidor debe inicializar un nuevo servidor, y todas sus dependencias (como el juego,
     * la variante, el tablero, etc)
     *
     * Complejidad: O(tamanoTab**2 + TAMANIO_ALFABETO*cantJugadores + cantFichas*cantJugadores)
     */
    Servidor(
            Nat cantJugadores,
            const Variante& variante,
            const Repositorio& r
    );

    /**
     * Conecta un cliente al servidor y retorna su id de cliente
     *
     * Complejidad: O(1)
     */
    void conectarCliente();

    /**
     * Recibe un mensaje o del cliente id
     *
     * Complejidad: No se impone una cota explícita. La complejidad no debe depender de
     * N ni de K. Puede depender de |Σ|, F, Lmáx y del número de fichas que el jugador pretenda ubicar al
     * enviar este mensaje.
     */
    void recibirMensaje(IdCliente id, const Ocurrencia& o);

    /**
     * Retorna la cantidad de jugadores necesarios para poder empezar el juego
     *
     * Complejidad: O(1)
     */
    Nat jugadoresEsperados();

    /**
     * Retorna la cantidad de jugadores necesarios para poder empezar el juego
     *
     * Complejidad: O(1)
     */

    Nat jugadoresConectados();
    /**
    * Retorna el juego del servidor
    *
    * Complejidad: O(1)
    */
    const Juego& juegoDelServidor();

    /**
     * Indica si el juego de un servido empezó
     *
     * Complejidad: O(1)
     */
    bool empezoJuego();

    /**
     * Consulta y vacia la cola de notificaciones del cliente id
     *
     * Complejidad: O(n*F)
     *   donde n es la cantidad de mensajes sin consultar de dicho cliente
     *   y F es la cantidad de fichas por jugador de la variante.
     */
    std::list<Notificacion> notificaciones(IdCliente id);

private:
    Nat _cantJugadoresConectados;
    const Nat _cantJugadoresEsperados;
    vector<queue<tuple<Notificacion, Nat>>> _notificacionesPersonales;
    vector<Nat> _cantVistosNotificacionesParaTodos;
    vector<tuple<Notificacion, Nat>> _notificacionesParaTodos;
    Nat _cantDeNotificaciones;
    Juego _juego;


    void enviarNotiTodos(Notificacion notificacion);

    void enviarNotiPers(Nat i, Notificacion notificacion);

    std::list<Notificacion> ordenarNotificaciones(IdCliente id);

    multiset<Letra> fichasIniciales(Nat id);

    multiset<Letra> fichasRepuestas(Nat id, vector<Nat> fichasAnteriores, const vector<Nat> fichasPostJugada);

    vector<tuple<Notificacion, Nat>> colaAVector(queue<tuple<Notificacion, Nat>> &queue);

    void
    Merge(list<Notificacion> res, vector<tuple<Notificacion, Nat>> notisPers, vector<tuple<Notificacion, Nat>> notisTodos);
};

#endif // SERVIDOR_H