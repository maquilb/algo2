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
    Juego(Nat k, const Variante &v, const Repositorio &r);

    /**
     * Ubica una Ocurrencia o en el juego
     *
     * Complejidad: O(m)
     *   donde m es el numero de fichas que se ubican
     */
    void ubicar(const Ocurrencia &o);

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
    const Variante &variante();

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
    const Repositorio &repositorio();

    /**
     * Retorna el tablero del juego
     *
     * Complejidad: O(1)
     */
    const Tablero &tablero();

    /**
     * Dada una posición del tablero, devuelve si está ocupada, la letra que lo ocupa y el turno en el que fue ubicada la ficha (si está vacía, devuelve la primera letra del alfabeto y el turno 0)
     *
     * Complejidad: O(1)
     */
    const tuple<bool, Letra, Nat> posDeTablero(Nat i, Nat j);

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
    bool jugadaValida(const Ocurrencia &o);

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

        Jugador() : _fichasDelJugador(TAMANIO_ALFABETO), _puntajeDelJugador(0), _ocurrenciasDelJugador() {}
    };

    const Variante &_variante;
    const Repositorio &_repositorio;
    Repositorio::const_iterator _siguiente_letra;
    Tablero _tablero;
    vector<Jugador> _jugadores;
    Nat _cantidadDeTurnos;
    Nat _turnoDe;

    /**
     * Definimos la funcion reponerFichas, que actualiza las fichas  del jugador del turno actual
     * para esto quita las fichas de la ocurrencia o las repone con las siguientes fichas en el Repositorio.
     **/

    void reponerFichas(const Ocurrencia &o);

    /* para un id válido y un natural cant repone un numero cant de fichas en el repositorio
        del jugador id.
     */
    void darFichas(const Nat &id, const Nat &cant);

    Nat calcularPuntos(queue<list<Letra>> &palabras);

    tuple<bool, bool> HorizontalOVertical(Ocurrencia o);

    queue<list<Letra>> palabrasFormadasTransversales(Ocurrencia &o, bool sentido);

    queue<list<Letra>> palabrasFormadas(Ocurrencia &o);

    Nat calcularPuntosPalabrasJugadas(queue<Ocurrencia> &ocus);

    list<list<Letra>> PalabrasTransversales(const Tablero &tablero, const Ocurrencia &set, bool sentido);

    bool tieneLasFichas(vector<Nat> &vector, const Ocurrencia &set);

    void SumarAparicion(vector<tuple<Letra, Nat>> &apariciones, Letra l);

    bool ocurrenciaSinPosicionesRepetidas(const Ocurrencia &set);

    bool DistanciaMayorAlongMax(const Ocurrencia &o, bool sentido, Nat largoMax);

    Nat distanciaEntreFichas(Ficha &min, Ficha &max, bool sentido);

    Ficha masChico(Ficha &ficha1, Ficha &ficha2, bool sentido);

    Ficha masGrande(Ficha &ficha1, Ficha &ficha2, bool sentido);

    vector<Ficha> ocurrenciaAVector(const Ocurrencia &o);

    void ordenarVectorDeFichas(vector<Ficha> &vect_fichas, bool sentido);

    /* PRE: @o es una ocurrencia que no tiene fichas con misma coordenada y distinta letra,
     *      tiene un largo menor a la longitud maxima de la palabra valida mas larga, y todas las fichas
     *      poseen la misma coordenada de fila o de la columna (es horizontal o vertical)
     *
     * POST: @RETURN un lista de letras (palabra) con una longitud acotada por la palabra valida mas larga,
     *       la palabra esta formada por todas las letras de las fichas de la ocurrencia parametro y por las
     *       fichas que estan entre la ficha "mas chica" y la ficha "mas grande", ademas de todas las que estan
     *       continuamente despues y antes de estas respectivamente en el tablero.
     *       En caso de no existir una palabra con todos estos requisitos devuelve una lista vacia.

     * Comentario: el juego usa esta funcion para armar una palabra entre la ocurrencia y las fichas que han sido
     *              jugadas previamente. De no ser posible formar una palabra con todas estas devuelve una lista vacia.
     * */

    list<Letra> FormarPalabraPrincipal(const Tablero &tab, const Ocurrencia &o, bool sentido);

    /* Esta funcion recibe una ficha y un sentido. Recorre el tablero del juego en forma horizontal (vertical) si sentido
     * es de valor true (false) tomando las fichas del mismo hasta que no haya alguna o bien si la ficha F esta en el tablero
     * hasta que el turno de las fichas sea mayor al turno de f.
     * Esta funcion permite formar palabras transversales de una jugada o bien "leer" la palabra que ha sido jugada
     * cuando se jugo la ficha f.
     * */
    list<Letra> formarPalabraEnSentido(Ficha f, bool sentido) const;

};


#endif // JUEGO_H