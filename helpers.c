#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>
#include <stdbool.h>
#include "helpers.h"


int cmpN1(const void * a, const void * b)
{
  struct _Nodo al = *(Nodo)a;
  struct _Nodo bl = *(Nodo)b;

  if (al.estado0 < bl.estado0)
  {
    return 1;
  }
  if (al.estado0 > bl.estado0)
  {
    return -1;

  }else{
    return 1;
  }
}


int cmpN2(const void * a, const void * b)
{
  struct _Nodo al = *(Nodo)a;
  struct _Nodo bl = *(Nodo)b;

  if (al.estado2 < bl.estado2)
  {
    return -1;
  }
  if (al.estado2 > bl.estado2)
  {
    return 1;

  }else{
		if(al.estado0 < bl.estado0)
		{
			return -1;
		}
		if(al.estado0 > bl.estado0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
  }
}


int cmpuestado0 (const void * a, const void * b)
{
  Nodo al =*((Nodo*)a);
  Nodo bl =*((Nodo*)b);

  if (bl->estado0 > al->estado0)
  {
    return -1;
  }
  if (bl->estado0 < al->estado0)
  {
    return 1;
  }else{
    return 0;
  }
}


bool VerificadorWP(Grafo G)
{
	u32 NumVer = NumeroDeVertices(G);
	for(u32 i=0; i<NumVer; i++)
	{
		for(u32 j=i; j<NumVer; j++)
		{
  		if(Grado(i,G) < Grado(j,G))
  		{
  			return false;
  		}
		}
	}
	return true;
}


/*----------------------------------OrdBC--------------------------------------
* Se empleo array de struct _Nodo *, llamado Nodo, de la manera,
* ∀i / 0 <= i < NumeroDeVertices(G):
* Nodo[i].estado0 = Color del vértice i del grafo G.
* Nodo[i].estado1 = Posición natural del vértice i del grafo G.
* Nodo[i].estado2 = La cantidad de repeticiones del color del vértice i del 
* grafo G.
* Se empleo NodoBC, llamado ColArray, de la manera:
* ColArray.length = cantidad de colores
* ColArray.array = array de u32, la posición i determina las repeticiones del
* color i en el grafo G.
*/
Nodo OrdBC(Grafo G)
{
	u32 NumVer = NumeroDeVertices(G);
	Nodo Nodo = calloc(NumVer, sizeof(struct _Nodo));
	if(Nodo == NULL)
	{
		return NULL;
	}

	NodoBC ColArray;
	ColArray.length = 0;
  ColArray.array = NULL;
  u32 ColTem;

	// Ordeno de forma natural, guardo datos de los vertices, calculo el maximo
	// color y las repeticiones de cada color
	for(u32 i=0; i<NumVer; i++)
	{
		FijarOrden(i,G,i);
		Nodo[i].estado0 = Color(i,G);
		Nodo[i].estado1 = i;

		if(ColArray.length <= Color(i,G))
		{
      ColTem = ColArray.length;
			ColArray.length = Color(i,G) + 1;
      ColArray.array = realloc(ColArray.array,ColArray.length*sizeof(u32));
      if(ColArray.array == NULL)
      {
        free(ColArray.array);
        ColArray.array = NULL;
        return NULL;
      }
      // Inicializamos las celdas en 0
      for(u32 i=ColTem; i<ColArray.length;i++)
      {
        ColArray.array[i] = 0;
      }
		}
    // Incrementamos las repeticiones del color
    ColArray.array[Color(i,G)] += 1;
	}

	// Guardamos las veces que se repite el color del vertice
	for(u32 i=0; i<NumVer; i++)
	{
		Nodo[i].estado2 = ColArray.array[Nodo[i].estado0];
	}

  free(ColArray.array);
	return Nodo;
}


/*----------------------------------BiDFS--------------------------------------
* Mediante OrdenVecino() obetenmos la posición del vecino para poder hacer 
* recurción sobre BiDFS.
*/
bool BiDFS(u32 index, Grafo G)
{
  bool ret = true;
	u32 color = 0;
  u32 disp0 = 0;
  u32 disp1 = 0;
  if(Color(index,G) == 4294967296-1)
  {
  	// Buscamos el color disponible para el vertice en la posición index
		for(u32 k=0; k<Grado(index,G);k++)
		{
			if(ColorVecino(k,index,G) == 0 && disp0 == 0)
			{
				disp0 = 1;
			}
			if(ColorVecino(k,index,G) == 1 && disp1 == 0)
			{
				disp1 = 1;
			}
      if (disp1 == 1 && disp0 == 1)
      {
        return false;
      }else if(disp0 == 0){
        color = 0;
      }else if(disp1 == 0){
        color = 1;
      }
		}

    FijarColor(color,index,G);

		// Hago recurción sobre los vecinos llamando BiDFS
		for(u32 k=0; k<Grado(index,G); k++)
		{
      u32 posc = OrdenVecino(k,index,G);
			ret = BiDFS(posc,G);
      if(!ret)
			{
				return false;
			}
		}
  }
  return ret;
}

//estado0: nombrev, estado1: si esta visitado 1, si no 0
Nodo2 *  inicializar_tabla(u32 NumVert, Grafo G)
{
	Nodo2 * hash = malloc(sizeof(Nodo) * NumVert);
  if (hash != NULL)
  {
  	for (unsigned int i = 0; i < NumVert; i++)
  	{
  		hash[i] = malloc(sizeof(struct _Nodo2 ));
      if (hash[i] != NULL) {
    		hash[i]->estado0 = ErrorGrafo;
    		hash[i]->estado1 = 0;
      }
  	}
  }
	for (u32 j = 0; j < NumVert; j++)
	{
		u32 index = Nombre(j,G) % NumVert;
		//en la posicion index en hash esta vacia
		if(hash[index]->estado0 == ErrorGrafo)
		{
			hash[index]->estado0 = Nombre(j,G);
      hash[index]->estado2 = j;
		}else
		//tenemos que buscar una celda vacia
		{
			u32 tope = index;
			index++;
			while(index != tope)
			{
				if(index == NumVert) {
					index = 0;
				}else if(hash[index]->estado0 == ErrorGrafo)
				{
					hash[index]->estado0 = Nombre(j,G);
          hash[index]->estado2 = j;
          break;
				}else
				{
					index++;
				}
			}
		}
	}
	return hash;
}

void liberar_tabla(Nodo2 * hash,u32 NumVert)
{
	for (unsigned int i = 0; i < NumVert; i++) {
		free(hash[i]);
	}
	free(hash);
}

bool vertice_fue_visitado( u32 nombrev, Nodo2 * hash,u32 NumVert)
{
	bool result = false;
	u32 index = nombrev % NumVert;
	if (hash[index]->estado0 == nombrev)
	{
		result = hash[index]->estado1;
	}
	else
	{
    u32 tope = index;
		index++;
    while (tope != index)
		{
			if (index == NumVert) {
				index = 0;
			}
			if (hash[index]->estado0 == nombrev)
			{
			   	result = hash[index]->estado1;
					break;
			}
			else
			{
				index++;
			}
		}

	}
 return result;
}

u32 obtener_orden_interno( u32 nombrev, Nodo2 * hash,u32 NumVert)
{
	u32 result = 0;
	u32 index = nombrev % NumVert;
	if (hash[index]->estado0 == nombrev)
	{
		result = hash[index]->estado2;
	}

	else
	{
    u32 tope = index;
		index++;
    while (tope != index)
		{
			if (index == NumVert) {
				index = 0;
			}
			if (hash[index]->estado0 == nombrev)
			{
			   	result = hash[index]->estado2;
					break;
			}
			else
			{
				index++;
			}
		}

	}
 return result;
}


void visitar_vertice (u32 nombrev, Nodo2 * visitados, u32 NumVert)
{
	u32 index = nombrev % NumVert;
	if (visitados[index]->estado0 == nombrev)
	{
		visitados[index]->estado1 = 1;
	}

	else
	{
    u32 tope = index;
		index++;
    while (tope != index)
		{
			if (index == NumVert) {
				index = 0;
			}
			if (visitados[index]->estado0 == nombrev)
			{
			   	visitados[index]->estado1 = 1;
					break;
			}
			else
			{
				index++;
			}
		}

	}
}


void visitar_vecinos(unsigned int i, Nodo2 * visitados, u32 NumVert,Grafo G)
{
	u32 numVecinos = Grado(i,G);
	for (unsigned int j = 0; j < numVecinos; j++) {
    u32 nombrevecino = NombreVecino(j,i,G);
		if (!vertice_fue_visitado(nombrevecino,visitados,NumVert))
		{
			u32 ordeninterno = obtener_orden_interno(nombrevecino,visitados,NumVert);
			visitar_vertice(nombrevecino,visitados,NumVert);
			visitar_vecinos(ordeninterno,visitados,NumVert,G);
		}
	}
}


void crear_cjto_color(u32 index,u32 * conjunto,u32 conjunto_length,Grafo G)
{
  for (u32 j = 0; j < (Grado(index,G)) ; j++)//para cada vecino j de index
    {
      u32 colorvecino = ColorVecino(j,index,G);

      /*si el color del vecino j es mayor que los manejados actualmente
        (solo puede ser el ErrorGrafo) => no fue coloreado
        si es menor, entonces tiene un color de los usados actualmente*/
      if(colorvecino < conjunto_length)
      {
        conjunto[colorvecino] = 1;
      }
    }

}


u32 buscar_min_color(u32 * conjunto, u32 conjunto_length)
{
  u32 index = 0;
  while (index < conjunto_length)
  {
    if(conjunto[index] == 0)
    {
      return index;
    }else
   {
      index++;
    }
  }
  return index;
}