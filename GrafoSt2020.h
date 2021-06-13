#ifndef _GRAFOST2020_H
#define _GRAFOST2020_H
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long int u32;

struct _VerticeSt
{
	u32 nombrev;
	u32 colorv;
	u32 gradov;
  struct _VerticeSt ** vecinosArr; //arreglo de punteros de vertices vecinos
};

typedef struct _VerticeSt * Vertice;


struct _AristaSt
{
	Vertice  extremo1;
	Vertice  extremo2;
};

typedef struct _AristaSt * Arista;


typedef struct _GrafoSt
{
	u32 nver; //cantidad de vertices
	u32 nlados; //canditdad de lados
	u32 ncolor; //cantidad de colores
	u32 delta; //grado max del grafo
	Vertice * verticesArr; //Arreglo de punteros de VerticeSt
	Vertice * verticesArr2;
	Arista * aristaArr; //Arreglo con punteros a estructuras AristaSt
} GrafoSt;

#endif //_GAFOST2020_H
