#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "constants.h"

struct Vertice{

	u32 nombre; 		// nombre del vertice
	u32 color;			// color del vertice
	u32 grado;			// cantidad de vecinos
    u32 *list_v;       // vertices vecinos


};

struct GrafoSt {

	u32 n;       			    // numero de vertices
	u32 m;						// numero de aristas
    u32 delta;                  // maximo numero de vecinos
	struct Vertice * lista;			// Lista de vertices
	u32 *orden_dinamico;               //orden dinamico de los vertices
    u32 *orden_natural;


};


static const u32 ErrorGrafo = 4294967296-1;

struct _Nodo{
  u32 estado0;
  u32 estado1;
  u32 estado2;
  u32 estado3;
};

typedef struct _Nodo * Nodo;

struct _SetSt
{
	u32 length; //longitud de conjunto
	u32 * conj; //puntero de arreglo de u32
};

typedef struct _SetSt * Set;

struct _nodoCGBC
{
	u32 length;
	u32 * array;
};

typedef struct _nodoCGBC NodoBC;
