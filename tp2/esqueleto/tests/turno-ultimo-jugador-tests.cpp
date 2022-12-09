#include "gtest-1.8.1/gtest.h"
#include "../src/Fachada_Servidor.h"

#include "utils/PalabrasComunes.h"
#include "utils/NotificacionesUtils.h"
#include "utils/Repositorio.h"
#include "utils/Puntajes.h"

class TurnoUltimoJugador : public testing::Test {
protected:
    Nat cantJugadores = 2;
    Nat tamanoTab = 100;
    Nat cantFichas = 100;

    map<Letra, Nat> puntajes;
    set<vector<Letra> > palabrasLegitimas;
    Repositorio repo;

    Fachada_Variante variante;
    Fachada_Servidor fachada;

    TurnoUltimoJugador():
            puntajes(puntajesIngles),
            palabrasLegitimas(begin(palabrasComunes), end(palabrasComunes)),
            repo(RepositorioGrande::repositorioGrande()),
            variante(tamanoTab, cantFichas, puntajes, palabrasLegitimas),
            fachada(cantJugadores, variante, repo)
    {
        for (Nat i = 0; i < cantJugadores; i++){
            fachada.conectarCliente();
        }
    }

};

TEST_F(TurnoUltimoJugador, jugadores_reciben_notificaciones_ante_jugada_valida) {

    fachada.recibirMensaje(0, {{0, 0, 'p'}, {0, 1, 'o'}, {0, 2, 'r'}});

    fachada.recibirMensaje(1, {{1, 0, 'a'}, {2, 0, 'r'}, {3, 0, 'a'}});

    fachada.notificaciones(0);
    fachada.notificaciones(1);

    Ocurrencia o = {{4, 0, 'r'}};
    fachada.recibirMensaje(0, o);


    compareNotificaciones(fachada.notificaciones(0), {
            Notificacion::nuevaUbicar(0, o),
            Notificacion::nuevaSumaPuntos(0, 8),
            Notificacion::nuevaReponer({'e'}),
            Notificacion::nuevaTurnoDe(1)
    });

    compareNotificaciones(fachada.notificaciones(1), {
            Notificacion::nuevaUbicar(0, o),
            Notificacion::nuevaSumaPuntos(0, 8),
            Notificacion::nuevaTurnoDe(1)
    });


};



