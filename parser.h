#ifndef _PARSER_H
#define _PARSER_H
#include "veinteveinte.h"

struct _Vnodo{
  Vertice vertice;
  u32 estado;
};

typedef struct _Vnodo * Vnodo;


Vertice crearVertice(u32 vname);
Arista crearArista();
void parser(Grafo g );
int cmpfunc (const void * a, const void * b);


#endif //_PARSER_H
