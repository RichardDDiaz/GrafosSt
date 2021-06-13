#include "veinteveinte.h"
#include "parser.h"
#include "queue.h"
#include "hash_table.h"
#include <time.h>
#include "comparadores.h"
//#include "GrafoStvalen.h"


Grafo grafo_init(u32 n, u32 m){
    Grafo G = NULL;
    G = calloc(1,sizeof(struct GrafoSt));
    G->n = n;
    G->m = m;
    G->lista = calloc(n,sizeof(struct Vertice));
    for(u32 i = 0 ; i < n ; i++)
        G->lista[i].list_v = calloc(0,sizeof(u32));
    G->orden_dinamico = calloc(n,sizeof(u32));
    G->orden_natural = calloc(n,sizeof(u32));

    for(u32 i = 0; i < n; i++){
        G->orden_dinamico[i] = i;
        //G->lista[i] = calloc(1,sizeof(struct Vertice));
        G->lista[i].nombre = -1;
        G->lista[i].grado = 0;
        G->lista[i].color = 0;
    }
    return G;
}


void crear_orden_natural(Grafo G){
    struct numgrad * par = NULL;

    u32 n = NumeroDeVertices(G);

    par = calloc(n,sizeof(struct numgrad));

    for(u32 i = 0 ; i < n ; i++){
        par[i].nombre = G->lista[i].nombre;
        par[i].valor = i;
    }
    qsort(par, n, sizeof(struct numgrad), cmpON);
    for(u32 i = 0; i < n; i++){
        G -> orden_natural[i] = par[i].valor;
    }

    free(par);

}

Grafo ConstruccionDelGrafo(){

    Grafo G = NULL;
    char *fst_line = NULL;
    char *fst_vertex = NULL,*snd_vertex = NULL;
    char *line = NULL;
    u32 fst = 0, snd = 0, n = 0, m = 1, m_cargados = 0 ,n_cargados = 0;
    u32 delta = 0;

    hash_t * hash_table = NULL;


    while(m_cargados < m){
        line = readline_from_stdin();
        fst_line = strtok(line, " ");

        if(strcmp(fst_line,"p")==0){
            fst_line = strtok(NULL, " "); //aca deberia decir edge;
            fst_vertex = strtok(NULL, " "); // Aca tengo vertices  en string
            snd_vertex = strtok(NULL, " "); // Aca tengo lados en string

            sscanf(fst_vertex, "%u", &fst);
            sscanf(snd_vertex, "%u", &snd);

            n = fst;
            m = snd;
            hash_table = hash_create(n);

            G = grafo_init(n,m);


        }
        else if(strcmp(fst_line,"e")==0){
            //bool es_nuevo_1 = true;
            //bool es_nuevo_2 = true;
            //u32 vx1 = 0,vx2=0;
            fst_vertex = strtok(NULL, " "); // Aca tengo el vertice 1  en string
            snd_vertex = strtok(NULL, "\n"); // Aca tengo el vertice 2 en string
            sscanf(fst_vertex, "%u", &fst);
            sscanf(snd_vertex, "%u", &snd);

            u32 grado_actual1;
            u32 grado_actual2;

            u32 n1 = search(hash_table,n,fst);
            if(n1 == (u32)-1){
                //no existe, hay que crearlo
                insert(hash_table,n,fst,n_cargados);
                n1 = n_cargados;
                G->lista[n_cargados].nombre = fst;
                G->lista[n_cargados].grado++;
                G->lista[n_cargados].color = 0;
                //FijarOrden(n_cargados,G,n_cargados);
                G->orden_dinamico[n_cargados] = n_cargados;


                grado_actual1 = G->lista[n_cargados].grado;
                n_cargados++;
            }
            else{
                //si existe, hay que actualizar su grado
                G->lista[n1].grado++;
                grado_actual1 = G->lista[n1].grado;
            }
            u32 n2 = search(hash_table,n,snd);
            if(n2 == (u32) -1){
                //no existe, hay que crearlo
                insert(hash_table,n,snd,n_cargados);
                n2 = n_cargados;
                G->lista[n_cargados].nombre = snd;
                G->lista[n_cargados].grado++;
                G->lista[n_cargados].color = 0;


                G->orden_dinamico[n_cargados] = n_cargados;
            //    G->edges[m_cargados].b = n_cargados;
                grado_actual2 = G->lista[n_cargados].grado;
                n_cargados++;
            }
            else{
                //si existe, hay que actualizar su grado
                G->lista[n2].grado++;
                grado_actual2 = G->lista[n2].grado;

            }

            G->lista[n1].list_v = realloc(G->lista[n1].list_v, grado_actual1*sizeof(u32));
            G->lista[n2].list_v = realloc(G->lista[n2].list_v, grado_actual2*sizeof(u32));

            G->lista[n1].list_v[grado_actual1-1] = n2;
            G->lista[n2].list_v[grado_actual2-1] = n1;


            m_cargados++;



        }
        else if(strcmp(fst_line,"c")==0){
            continue;
        }

        free(line);
    }
    for(u32 i = 0 ; i < NumeroDeVertices(G); i ++){
        if(delta < Grado(i,G)){
            delta = Grado(i,G);

        }

    }
    G->delta = delta;
    delete_ht(hash_table,n);

    crear_orden_natural(G);

    return G;


}



void DestruccionDelGrafo(Grafo G){

    for(u32 i = 0; i < NumeroDeVertices(G);i ++){
        free(G->lista[i].list_v);
    }

    free(G->lista);
    free(G->orden_dinamico);
    free(G->orden_natural);
    free(G);


}

Grafo CopiarGrafo(Grafo G){
    Grafo H = calloc(1,sizeof(struct GrafoSt));

    assert(H);

    H->n = NumeroDeVertices(G);

    H->m = NumeroDeLados(G);

    H->lista = calloc(H->n,sizeof(struct Vertice));

    H->orden_dinamico = calloc(H->n,sizeof(u32));

    assert(H->lista!= NULL);

    for(u32 i = 0 ; i < NumeroDeVertices(H) ; i++){
        u32 grado = Grado(i,G);
        H->lista[i].list_v = calloc(grado,sizeof(u32));
        for (u32 k = 0; k < grado; k++){
            H->lista[i].list_v[k] = -1;
        }

    }

    for(u32 i = 0; i < NumeroDeVertices(H); i ++){
        H->orden_dinamico[i] = G->orden_dinamico[i];
        H->lista[i].nombre = G->lista[i].nombre;
        H->lista[i].color = G->lista[i].color;
        H->lista[i].grado = G->lista[i].grado;

        for(u32 j = 0; j < H->lista[i].grado; j ++){
            H->lista[i].list_v[j] = G->lista[i].list_v[j];
        }
    }

    return H;



    /*
        Reasignacion de punteros

    */

}





u32 NumeroDeVertices(Grafo G){
    return G->n;
}


u32 NumeroDeLados(Grafo G){
    return G->m;
}

u32 Delta(Grafo G){
    return G->delta;
}



u32 Nombre(u32 i,Grafo G){
    u32 vertex = G->orden_dinamico[i];
    u32 i_name = (G->lista[vertex]).nombre;
    return i_name;
}

u32 Color(u32 i,Grafo G){
    u32 vertex = G->orden_dinamico[i];
    u32 i_color = G->lista[vertex].color;
    return i_color;
}

u32 Grado(u32 i,Grafo G){
    u32 vertex = G->orden_dinamico[i];

    return G->lista[vertex].grado;
}

u32 OrdenVecino(u32 j,u32 i,Grafo G){
    u32 vertex = G->orden_dinamico[i];
    u32 vecino = G->lista[vertex].list_v[j];

    return vecino;
}

u32 ColorVecino(u32 j,u32 i,Grafo G){

    u32 vertex = G->orden_dinamico[i];
    u32 vecino = G->lista[vertex].list_v[j];
    return G->lista[vecino].color;
    //return Color(vertex2,G);

    //return pow(2, 32)-1;
}

u32 NombreVecino(u32 j,u32 i,Grafo G){

    u32 vertex = G->orden_dinamico[i];
    u32 vecino = G->lista[vertex].list_v[j];
    return G->lista[vecino].nombre;

}

char FijarOrden(u32 i,Grafo G,u32 N){
    if(i<NumeroDeVertices(G) && N<NumeroDeVertices(G)){
        G->orden_dinamico[i] = G->orden_natural[N];
        return 0;
    }
    else{
        return 1;
    }

}

char FijarColor(u32 x,u32 i,Grafo G) {
    u32 vertex = G->orden_dinamico[i];
    G->lista[vertex].color = x;
    return 0;
}

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

  if (al.estado1 < bl.estado1)
  {
    return 1;
  }
  if (al.estado1 > bl.estado1)
  {
    return -1;

  }else{
    return 0;
  }
}


int cmpN3(const void * a, const void * b)
{
  struct _Nodo al = *(Nodo)a;
  struct _Nodo bl = *(Nodo)b;

  if (al.estado3 < bl.estado3)
  {
    return -1;
  }
  if (al.estado3 > bl.estado3)
  {
    return 1;

  }else{
		if(al.estado1 < bl.estado1)
		{
			return -1;
		}
		if(al.estado1 > bl.estado1)
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
* Se empleo array de struct _Nodo, llamado Nodo, de la manera,
* ∀i / 0 <= i < NumeroDeVertices(G):
* Nodo[i].estado0 = Nombre del vértice de G
* Nodo[i].estado1 = Color del vértice de G
* Nodo[i].estado2 = Posición natural del vértice de G
* Nodo[i].estado3 = La cantidad de repeticiones del color del vértice de G
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

	// Guardo datos de los vertices, calculo el maximo color y las repeticiones
  // de cada color
	for(u32 i=0; i<NumVer; i++)
	{
		Nodo[i].estado0 = Nombre(i,G);
		Nodo[i].estado1 = Color(i,G);

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

	// Ordenamos el array de forma natural
	qsort(Nodo, NumVer, sizeof(struct _Nodo), cmpN1);

	// Guardamos las posiciones naturales en los nodos y las veces que se repite
	// el color del vertice
	for(u32 i=0; i<NumVer; i++)
	{
		Nodo[i].estado2 = i;
		Nodo[i].estado3 = ColArray.array[Nodo[i].estado1];
	}
  free(ColArray.array);

	return Nodo;
}


/*-------------------------------BuscarNodo------------------------------------
* valor = Puede ser el nombre de un vértice u 2^32-1.
* Se emplea un struct _Nodo * llamado tabla, de la siguiente manera:
* tabla[indexTB].estado0 = Lo compararemos para obtener la indexación correcta.
*/
u32 BuscarNodo(u32 nombre, u32 NumVer, u32 valor, Nodo tabla)
{
  u32 indexTB = nombre % NumVer;
  bool vuelta = true;
  u32 indexTBaux = indexTB;
  while(true)
  {
    if(!vuelta && indexTB == indexTBaux)
    {
      printf("Error\n");
      exit(EXIT_FAILURE);
    }
    if(tabla[indexTB].estado0 == valor)
    {
      return indexTB;
    }
    else
    {
      indexTB++;
      if(indexTB == NumVer)
      {
        indexTB = 0;
        vuelta = false;
      }
    }
  }
}


/*-------------------------------ColocarNodo-----------------------------------
* Se emplea un struct _Nodo * llamado tabla, de la siguiente manera:
* tabla[libre].estado0 = Nombre del vértice.
* tabla[libre].estado1 = Posición del vértice en el orden interno.
*/
void ColocarNodo(u32 nombre, u32 index, u32 NumVer, Nodo tabla)
{
  u32 libre = BuscarNodo(nombre,NumVer,4294967296-1,tabla);
  tabla[libre].estado0 = nombre;
  tabla[libre].estado1 = index;
}


/*-------------------------------ObtenerPosc-----------------------------------
* Se emplea un struct _Nodo * llamado tabla, de la siguiente manera:
* tabla[nodo].estado1 = Posición en el orden interno del vértice.
*/
u32 ObtenerPosc(u32 nombre, u32 NumVer, Nodo tabla)
{
  u32 nodo = BuscarNodo(nombre,NumVer,nombre,tabla);
  return tabla[nodo].estado1;

}


/*-------------------------------InitTabla-------------------------------------
* Se emplea un struct _Nodo * llamado tabla, de la siguiente manera,
* ∀i / 0 <= i < NumeroDeVertices(G):
* tabla[i].estado0 = Valor error de nombre.
*/
Nodo InitTabla(u32 NumVer, Grafo G)
{
  Nodo tabla = calloc(NumVer, sizeof(struct _Nodo));
  for(u32 i=0; i<NumVer; i++)
  {
    tabla[i].estado0 = 4294967296-1;
  }
  // Coloco todos los vertices en la tabla con sus posiciones
  for(u32 i=0; i<NumVer; i++)
  {
    ColocarNodo(Nombre(i,G),i,NumVer,tabla);
  }
  return tabla;
}


/*----------------------------------BiDFS--------------------------------------
* Se emplea un struct _Nodo * llamado tabla, donde encontramos información de
* los vértices.
* Se usa la función auxiliar ObtenerPosc() para poder hacer recursión sobre un
* vértice del grafo.
*/
bool BiDFS(u32 color, u32 index, Grafo G, Nodo tabla)
{
  bool ret = true;
	color = 0;
  u32 disp0 = 0;
  u32 disp1 = 0;
  if(Color(index,G) == 4294967296-1)
  {
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

		// Itero sobre los vecinos
		for(u32 k=0; k<Grado(index,G); k++)
		{
      u32 posc = ObtenerPosc(NombreVecino(k,index,G),NumeroDeVertices(G),tabla);
			ret = BiDFS(color,posc,G,tabla);
      if(!ret)
			{
				return false;
			}
		}
  }
  return ret;
}

//estado0: nombrev, estado1: si esta visitado 1, si no 0
Nodo *  inicializar_tabla(u32 NumVert, Grafo G)
{
	Nodo * hash = malloc(sizeof(Nodo) * NumVert);
  if (hash != NULL)
  {
  	for (unsigned int i = 0; i < NumVert; i++)
  	{
  		hash[i] = malloc(sizeof(struct _Nodo ));
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

void liberar_tabla(Nodo * hash,u32 NumVert)
{
	for (unsigned int i = 0; i < NumVert; i++) {
		free(hash[i]);
	}
	free(hash);
}

bool vertice_fue_visitado( u32 nombrev, Nodo * hash,u32 NumVert)
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

u32 obtener_orden_interno( u32 nombrev, Nodo * hash,u32 NumVert)
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


void visitar_vertice (u32 nombrev, Nodo * visitados, u32 NumVert)
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


void visitar_vecinos(unsigned int i, Nodo * visitados, u32 NumVert,Grafo G)
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


/*Funciones de ordenacion*/

/*--------------------------Funciones de Ordenación--------------------------*/


/*-----------------------------WelshPowell-------------------------------------
* Se emplea un array de struct _Nodo * llamado Nodo de la siguiente manera,
* ∀i / 0 <= i < NumeroDeVertices(G):
* Nodo[i].estado0 = Nombre del vértice i de G.
* Nodo[i].estado1 = Grado del vértice i de G.
* Nodo[i].estado2 = Posición natural del vértice i de G para usar en
* FijarOrden.
*/
char WelshPowell(Grafo G)
{
  if( NumeroDeVertices(G) <= 1)
  {
    return '1';
  }else
  {
    u32 NumVer = NumeroDeVertices(G);
    Nodo Nodo = malloc(sizeof(struct _Nodo) * NumVer);
    if(Nodo == NULL)
    {
      printf("WelshPowell: Fallo al hacer calloc\n");
      return '1';
    }
    
    // Ordenamos de forma natural, guardamos los grados y el orden natural
    // de cada vertice
    for (u32 i=0; i<NumVer ; i++)
    {
      FijarOrden(i,G,i);
      Nodo[i].estado0 = Grado(i,G);
      Nodo[i].estado1 = i;
    }

    // Ordenamos de mayor a menor con respecto al grado de los vertices
    qsort(Nodo, NumVer, sizeof(struct _Nodo), cmpN1);

    // Ordenamos finalmente los vertices del Grafo
    for(u32 i = 0; i<NumeroDeVertices(G); i++)
    {
      FijarOrden(i, G, Nodo[i].estado1);
    }
    free(Nodo);
    return '0';
  }
}


/*--------------------------ChicoGrandeBC--------------------------------------
* Se hace empleo de un array de struct _Nodo * llamado Nodo de la siguiente
* manera, ∀i / 0 <= i < NumeroDeVertices(G):
* Nodo[i].estado2 = Posición natural del vértice i de G para usar en
* FijarOrden.
* Usamos la función auxiliar OrdBC(), carga todos los datos necesarios para
* luego ordenarlos por la cardinalidad de los bloques de colores.
*/
char ChicoGrandeBC(Grafo G)
{
	if( NumeroDeVertices(G) <= 1)
	{
		return '1';
	}else
	{
    Nodo Nodo = OrdBC(G);
		if(Nodo == NULL)
		{
			printf("ChicoGrandeBC: Fallo al hacer calloc\n");
			return '1';
		}
		// Ordenamos para obtener los bloques de colores segun su cardinalidad
		qsort(Nodo, NumeroDeVertices(G), sizeof(struct _Nodo), cmpN3);

		// Ordenamos finalmente los vertices del Grafo
		for(u32 i = 0; i<NumeroDeVertices(G); i++)
		{
			FijarOrden(i, G, Nodo[i].estado2);
		}
    free(Nodo);
    return '0';
	}
}


/*----------------------RevierteBC---------------------------------------------
* Se hace empleo de un array de struct _Nodo * llamado Nodo de la siguiente
* manera, ∀i / 0 <= i < NumeroDeVertices(G):
* Nodo[i].estado0 = Nombre del vértice i de G.
* Nodo[i].estado1 = Color del vértice i de G.
* Nodo[i].estado2 = Posición natural del vértice i de G para usar en
* FijarOrden.
*/
char RevierteBC(Grafo G)
{
	if( NumeroDeVertices(G) <= 1)
	{
		return '1';
	}else
	{
		u32 NumVer = NumeroDeVertices(G);
		Nodo Nodo = calloc(NumVer, sizeof(struct _Nodo));
		if(Nodo == NULL)
		{
			printf("RevierteBC: Fallo al hacer calloc\n");
			return '1';
		}

		// Guardo datos de los vertices
		for(u32 i=0; i<NumVer; i++)
		{
			Nodo[i].estado0 = Nombre(i,G);
			Nodo[i].estado1 = Color(i,G);
		}

		// Ordenamos el array de forma natural
		qsort(Nodo, NumVer, sizeof(struct _Nodo), cmpN1);

		// Guardamos las posiciones naturales en los nodos
		for(u32 i=0; i<NumVer; i++)
		{
			Nodo[i].estado2 = i;
		}

		// Ordenamos de mayor a menor los bloques de colores
		qsort(Nodo, NumVer, sizeof(struct _Nodo), cmpN2);


		// Ordenamos finalmente los vertices del Grafo
		for(u32 i = 0; i<NumeroDeVertices(G); i++)
		{
			FijarOrden(i, G, Nodo[i].estado2);
		}
    free(Nodo);
    return '0';
	}
}


/*----------------------Aleatorizar Vertices-----------------------------------
* Se hace empleo de un array de struct _Nodo * llamado nombrev_pos de la
* siguiente manera, ∀i / 0 <= i < NumeroDeVertices(G):
* nombrev_pos[i]->estado0 = Nombre del vertice de G.
* nombrev_pos[i]->estado1 = Posicion del vertice en el orden natural de G.
* NOTA: posteriormente se guarda en estado1 un numero ALEATORIO UNICO
*       para usar en FijarOrden.
*/
char AleatorizarVertices(Grafo G,u32 R)
{
  u32 aleatorio; // numero aleatorio usando R
  u32 aleatorio2; // numero aleatorio2 usano R
  u32 n_tope;
  u32 NumPrimoSheldon = 73;
  u32 NumVert = NumeroDeVertices(G);

  Nodo * nombrev_pos = malloc(sizeof(Nodo) * NumVert);
  // array o conjunto de numeros ALEATORIOS aun disponibles segun posicion
  // disponible_random[i] == 0 => numero i disponible, == 1 => i no disponible
  u32 * disponible_random = calloc(NumVert,sizeof(u32));
  //exepciones, por si no tenemos mas memoria para trabajar con aleatorizar
  if (nombrev_pos == NULL || disponible_random == NULL || NumVert == 0)
  {
    return '1';
  }
  for(u32 i=0;i<NumVert;i++)
  {
    nombrev_pos[i] = malloc(sizeof(struct _Nodo)); // se crean el struct
    nombrev_pos[i]->estado0 = Nombre(i,G); // se obtiene el nombre del vertice i
    nombrev_pos[i]->estado1 = i; // se guarda su posicion natural
  }
  //qsort ordena nombrev_pos segun el orden natural de cada estado0 =>
  //qsort ordena los nombres de los vertices segun el orden natural
  qsort(nombrev_pos,NumVert,sizeof(Nodo),cmpuestado0);
  srand(R); //utilizacion de la semilla para generar numeros aleatorios
  for(u32 i=0;i<NumVert;i++)
  {
    aleatorio = rand() % NumVert;
    aleatorio2 = (rand()*NumPrimoSheldon) % NumVert;
    if(disponible_random[aleatorio] == 0)// prueba con el primer aleatorio
    {
      disponible_random[aleatorio]=1;
      nombrev_pos[i]->estado1=aleatorio;
    }else if(disponible_random[aleatorio2] == 0)// prueba con el segundo
    {
      disponible_random[aleatorio2]=1;
      nombrev_pos[i]->estado1=aleatorio2;
    }else
    {
      n_tope = aleatorio;
      aleatorio++;
      //se hace una busqueda circular para encontrar un numero disponible
      while(aleatorio !=n_tope)
      {
        if(aleatorio >= NumVert)
        {
          aleatorio = 0;
        }else if(disponible_random[aleatorio] == 0)
        {
          disponible_random[aleatorio]=1;
          nombrev_pos[i]->estado1=aleatorio;
          aleatorio=n_tope;
        }else
        {
          aleatorio++;
        }
      }
    }
  }
  // fijar vertices de forma lineal mientras libera memoria que ya no utiliza
  for(u32 j=0;j<NumVert;j++){
    FijarOrden(j,G,nombrev_pos[j]->estado1);
    free(nombrev_pos[j]);
  }
  free(nombrev_pos);
  free(disponible_random);
  return '0';
}


/*-----------------------------Funciones de Coloreo--------------------------*/


/*--------------------------------Greedy---------------------------------------
* Se hace empleo de un struct _Set * llamado cjto_color de la siguente manera:
* cjto_color->length_color = Cantidad de colores utlizados actualmente.
* cjto_color->conj = array/conjunto de tamaño length_color, donde
* las posiciones i representan los colores usados por los vecinos
* de un vertice, se crean con calloc, usamos que se inicializan con cero como:
* cjto_color->conj[i] == 0: color i disponible, == 1: color i no disponible.
*/
u32 Greedy(Grafo G)
{
  //verificamos que existe la memoria disponible para usar greedy
  //para evitar quedar coloreado a medias
  u32 * check_max_memory = malloc(sizeof(u32) * Delta(G));
  Set cjto_color = malloc(sizeof(struct _SetSt));
  if (cjto_color == NULL || check_max_memory == NULL)
  {
    free(check_max_memory);
    check_max_memory = NULL;
    free(cjto_color);
    cjto_color = NULL;
    return ErrorGrafo;
  }
  free(check_max_memory);
  check_max_memory = NULL;

  u32 NumVert = NumeroDeVertices(G);
  /*como no diferencia de los colores que esta usando para el nuevo coloreo
  * de los que ya venian en G antes de llamar a la funcion, se los inicializa
  * con un color que por definicion es imposible, 4294967296-1*/
  for (u32 i = 0; i < NumVert; i++)
  {
    FijarColor(ErrorGrafo,i,G);
  }
  u32 length_color = 1;
  FijarColor(0,0,G);//primer vertice de cualquier orden pinta con 0

  cjto_color->length = length_color; //cantidad de colores usados hasta ahora

  //para cada vertice de G salvo el primero
  for (u32 i = 1; i < NumVert ; i++)
  {
    //creacion del array/conjunto usado para colorear el vertice i
    cjto_color->conj = calloc(cjto_color->length,sizeof(u32));

    for (u32 j = 0; j < (Grado(i,G)) ; j++)//para cada vecino j de i
    {
      u32 colorvecino = ColorVecino(j,i,G);

      /*si el color del vecino j es mayor que los manejados actualmente
        (solo puede ser el ErrorGrafo) => no fue coloreado
        si es menor, entonces tiene un color de los usados actualmente*/
      if(colorvecino < cjto_color->length)
      {
        cjto_color->conj[colorvecino] = 1;
      }
    }

    u32 min_color_disp = 0;
    //obtener minimo color disponible no usado por los vecinos del vertice i
    while (min_color_disp < cjto_color->length)
    {
      if (cjto_color->conj[min_color_disp] == 0)
      {
        FijarColor(min_color_disp,i,G);
        break;
      }else{
        min_color_disp++;
      }

    }
    //si todos los colores que manejamos actualmente son usados en los vecinos
    //de i, entonces hay que crear un color nuevo.
    if(min_color_disp == cjto_color->length)
    {
      length_color++;
      FijarColor(min_color_disp,i,G);

      cjto_color->length = length_color;
    }
      //se libera el arreglo para el proximo vertice o fin de la funcion
      free(cjto_color->conj);
      cjto_color->conj = NULL;
  }

  free(cjto_color);
  cjto_color = NULL;
  return length_color;
}


/*--------------------------------Bipartito------------------------------------
* Se emplea un struct _Nodo * llamado tabla para la ubicación de los vertices
* en el Grafo.
* Se usa la función auxiliar InitTabla() para llenar la tabla con los datos, la
* función BiDFS para colorear.
* en caso de error se coloreara con Greedy el grafo G.
*/
char Bipartito(Grafo G)
{
  bool ret = true;
  char value = '1';
  u32 NumVer = NumeroDeVertices(G);
	u32 color = 0;
  for(u32 i=0; i<NumVer; i++)
  {
    FijarColor(4294967296-1,i,G);
  }
  // Creamos una tabla con los nombres y posiciones de los vertices
  Nodo tabla = InitTabla(NumVer, G);
  // Coloreamos mediante DFS
  for(u32 j=0; j<NumVer; j++)
  {
    ret = BiDFS(color,j,G,tabla);
    if(!ret)
    {
      value = '0';
      break;
    }
  }

  if (value == '0')
  {
    Greedy(G);
  }
  free(tabla);
  return value;
}


char SwitchColores(Grafo G,u32 i,u32 j)
{
	u32 NumVert = NumeroDeVertices(G);
  u32 colormax = 0;
	for (u32 v = 0; v < NumVert; v++) {
		u32 colorv = Color(v,G);
    if (colorv > colormax)
		{
      colormax = colorv;
    }
	}
	//verifico que el color i y j esten en el intervalo correcto.
	if ( colormax < i || colormax < j )
	{
		return '1' ;
	}
	//como i y j tienen sentido,entonces empiezo a hacer lo que hay que hacer
	//voy a recorrer ahora el grafo, vertice por vertice,
	//a los que tienen color i les asigno j, a los que tienen color j les asigno i
	else
	{
    u32 nvert = NumeroDeVertices(G);
	  for (u32 k = 0; k < nvert; k++) {
		  if (Color(k,G) == i) {
        FijarColor(j,k,G);
		  }
		  else if (Color(k,G) == j) {
			  FijarColor(i,k,G);
		  }
	}
	  return '0';
	}
}

u32 NumCCs(Grafo G)
{
u32 NumVert = NumeroDeVertices(G);
//aca podria pasarle solo el grafo y calcular NumVert adentro
Nodo * visitados = inicializar_tabla(NumVert,G);
u32 numcc = 0;
for (unsigned int i = 0; i < NumVert; i++)
{
	u32 nombrei = Nombre(i,G);
	if(!vertice_fue_visitado(nombrei,visitados,NumVert))
	{
	  numcc++;
		visitar_vertice(nombrei,visitados,NumVert);
		visitar_vecinos(i,visitados,NumVert,G);
	}
}

liberar_tabla(visitados,NumVert);

return numcc;
}
