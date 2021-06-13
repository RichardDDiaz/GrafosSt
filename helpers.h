#ifndef _HELPER_H
#define _HELPER_H
#include <stdbool.h>

#include "veinteveinte.h"
#include "GrafoSt2020.h"


static const u32 ErrorGrafo = 4294967296-1;

struct _Nodo{
  u32 estado0;
  u32 estado1;
  u32 estado2;
};

typedef struct _Nodo * Nodo;

struct _Nodo2{
  u32 estado0;
  u32 estado1;
  u32 estado2;
};

typedef struct _Nodo2 * Nodo2;

struct _nodoCGBC
{
	u32 length;
	u32 * array;
};

typedef struct _nodoCGBC NodoBC;


// Funciones de ordenación

// Brinda un orden de mayor a menor
int cmpN1(const void * a, const void * b);

// Brinda un orden de menor a mayor segun la cardinalidad de los colores
int cmpN2(const void * a, const void * b);

// Brinda un orden natural de punteros de Nodos
int cmpuestado0 (const void * a, const void * b);


// Funciones auxiliares

// Verifica que WelshPowell de un ordenamiento correcto
bool VerificadorWP(Grafo G);

// Copia datos de los vertices, devuelve un array de struct _Nodo
// ordenados naturalmente
Nodo OrdBC(Grafo G);

// Retorna la primera posición de la tabla con el valor solicitado
u32 BuscarNodo(u32 nombre, u32 NumVer, u32 valor, Nodo tabla);

// Posiciona el vertice con sus datos en la tabla hash
void ColocarNodo(u32 nombre, u32 index, u32 NumVer, Nodo tabla);

// Retorna la posición del orden interno del vertice
u32 ObtenerPosc(u32 nombre, u32 NumVer, Nodo tabla);

// Inicializa un array con el nombre y posición de cada vertice
Nodo InitTabla(u32 NumVer, Grafo G);

// Fijo el color del vertice index, viendo cual esta disponible, luego aplico
// BiDFS a cada vecino de index
bool BiDFS(u32 index, Grafo G);

Nodo2 *  inicializar_tabla(u32 NumVert, Grafo G);

void liberar_tabla(Nodo2 * hash,u32 NumVert);

bool vertice_fue_visitado( u32 nombrev, Nodo2 * hash,u32 NumVert);

u32 obtener_orden_interno( u32 nombrev, Nodo2 * hash,u32 NumVert);

void visitar_vertice (u32 nombrev, Nodo2 * visitados, u32 NumVert);

void visitar_vecinos(unsigned int i, Nodo2 * visitados, u32 NumVert,Grafo G);

// Cada lugar del arreglo conjunto: representa un color del coloreo greedy
// segun los colores de los vecinos de Nombre(index,G) pone 1 en esos lugares
void crear_cjto_color(u32 index,u32 * conjunto,u32 conjunto_length,Grafo G);

// Cada lugar del arreglo conjunto: representa un color del coloreo greedy
// busca el primer lugar donde no exista un 1: ese lugar es el minimo color
u32 buscar_min_color(u32 * conjunto, u32 conjunto_length);


#endif
