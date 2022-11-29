#ifndef JUEGO_H
#define JUEGO_H

#include <queue>
#include "Tipos.h"
#include "Variante.h"
#include "Tablero.h"


class Juego {
public:
    /**
     * Construye un juego a partir de la cantidad de jugadores k, la variante v y el repositorio r
     *
     * Complejidad: O(tamanoTab**2 + ALPHABET_SIZE*cantJugadores + cantFichas*cantJugadores)
     */
    Juego(Nat k, const Variante& v, const Repositorio& r);

    /**
     * Ubica una Ocurrencia o en el juego
     *
     * Complejidad: O(m)
     *   donde m es el numero de fichas que se ubican
     */
    void ubicar(const Ocurrencia& o);

    /**
     * Retorna el id del cliente al cual le toca jugar
     *
     * Complejidad: O(1)
     */
    IdCliente turno();

    /**
     * Retorna informacion sobre la variante del juego
     *
     * Complejidad: O(1)
     */
    const Variante& variante();

    /**
     * Retorna la cantidad de jugadores en el juego
     *
     * Complejidad: O(1)
     */
    Nat cantDeJugadores();

    /**
     * Retorna el repositorio del juego
     *
     * Complejidad: O(1)
     */
    const Repositorio& repositorio();

    /**
     * Retorna el tablero del juego
     *
     * Complejidad: O(1)
     */
    const Tablero& tablero();

    /**
     * Dada una posición del tablero, devuelve si está ocupada, la letra que lo ocupa y el turno en el que fue ubicada la ficha (si está vacía, devuelve la primera letra del alfabeto y el turno 0)
     *
     * Complejidad: O(1)
     */
    const Tablero& posDeTablero(Nat i, Nat j);

    /**
     * Retorna el jugador a quien le toca jugar
     *
     * Complejidad: O(1)
     */
    Nat turnoActual();

    /**
     * Retorna la cantidad de fichas de cada letra que tiene el jugador
     *
     * Complejidad: O(1)
     */
    const vector<Nat> fichasDeJugador(Nat i);

    /**
     * Dado un jugador id y una letra l, retorna cuantas fichas tiene el jugador de dicha letra.
     *
     * Complejidad: O(1)
     */
    Nat cantDeFichasLetra(IdCliente id, Letra l);

    /**
     * Determina si una jugada es valida o no
     *
     * Complejidad: O(Lmax ** 2)
     */
    bool jugadaValida(const Ocurrencia& o);

    /**
     * Retorna true sii la coordenada se encuentra ocupada.
     *
     * Complejidad: O(1)
     */
    bool hayLetra(Nat x, Nat y);

    /**
     * Obtiene el contenido del tablero en una coordenada.
     *
     * Complejidad: O(1)
     */
    Letra letra(Nat i, Nat j);

    /**
     * Retorna el puntaje del jugador id
     *
     * Complejidad: O(1 + m*Lmax)
     *   donde m es la cantidad de fichas que ubico el jugador desde la ultima vez que se preguntó por su puntaje.
     */
    Nat puntaje(IdCliente id);


    /**
     * Dado un jugador id y una letra l, retorna cuantas fichas tiene el jugador de dicha letra.
     *
     * Complejidad: O(1)
     */
    Nat cantFicha(IdCliente id, Letra l);

private:
    struct Jugador {
        vector<Nat> _fichasDelJugador;
        Nat _puntajeDelJugador;
        queue<Ocurrencia> _ocurrenciasDelJugador;
        Jugador(Variante v, Repositorio r) : _fichasDelJugador(CrearBolsaFichas(v.fichas(),r)), _puntajeDelJugador(0), _ocurrenciasDelJugador() {}
        // por fines practicos es un tupla de 3 elemntos
        const vector<Nat> CrearBolsaFichas(const Nat &f, Repositorio &r) {
            vector<Nat> res(f);
            for(Nat i = 0; i<f; i++){
                Letra l  = r.front();
                res[ord(l)]++;
                r.pop_front();
            }
            return res;
        }
    };
    const Variante& _variante;
    Repositorio _repositorio;
    Tablero _tablero;
    vector<Jugador> _jugadores;
    Nat _cantidadDeTurnos;
    Nat _turnoDe;

    /**
     * Definimos la funcion reponerFichas, que repone las fichas en la bolsa del jugador
     **/

    void reponerFichas(const Ocurrencia o);

    Nat calcularPuntos(queue<Repositorio> &palabras);

    tuple<bool, bool> HorizontalOVertical(Ocurrencia o);

    queue<Repositorio> palabrasFormadasTransversales(Ocurrencia o, bool sentido);

    Repositorio palabraFormadaHorizontal(Ficha f);

    Repositorio palabraFormadaVertical(Ficha f);

    Repositorio formarPalabraJugadaPrincial(Ocurrencia o, bool sentido);

    queue<Repositorio> palabrasFormadas(Ocurrencia o);

    Nat  calcularPuntosPalabrasJugadas(queue<Ocurrencia> &ocus);

};


#endif // JUEGO_H