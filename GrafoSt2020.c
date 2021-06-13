#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "GrafoSt2020.h"
#include "helpers.h"
#include "veinteveinte.h"
#include "parser.h"


Grafo ConstruccionDelGrafo()
{
	//creo un grafo y le reservo memoria
	Grafo G = malloc(sizeof(GrafoSt));
	//chequeo que malloc haya terminado exitosamente
  if (G == NULL) {
  	printf("No hay suficiente memoria para el grafo :( )\n");
  }
	//inicializo los datos
	G->nver = 0;
	G->nlados = 0 ;
	G->ncolor = 0;
	G->delta  = 0;
	G->verticesArr = NULL;
  G->aristaArr = NULL;
	//ahora empieza el parseo amigos

	parser(G);
	return G;
}

void DestruccionDelGrafo(Grafo G){
  for(u32 i=0 ; i<G->nlados; i++)
  {
    G->aristaArr[i]->extremo1 = NULL;
    G->aristaArr[i]->extremo2 = NULL;
    free(G->aristaArr[i]);
    G->aristaArr[i] = NULL;
  }
  free(G->aristaArr);
  G->aristaArr = NULL;

  for(u32 j=0 ; j<G->nver; j++)
  {
    for(u32 k=0 ; k<G->verticesArr[j]->gradov; k++)
    {
      G->verticesArr[j]->vecinosArr[k] = NULL;
    }
    free(G->verticesArr[j]->vecinosArr);
  }

  for(u32 l=0 ; l<G->nver; l++)
  {
    free(G->verticesArr[l]);
    G->verticesArr[l] = NULL;
  }
  free(G->verticesArr);
  G->verticesArr = NULL;
  free(G);
  G = NULL;
}


/*0 si todo esta bien, 1 si se encontro un lado con los los mismos
colores en sus vertices, usar con cuidado*/
char VerificadorGready(Grafo G)
{
	for (u32 i = 0; i < G->nver; i++) { //vertice
		for (u32 j = 0; j < G->verticesArr[i]->gradov; j++) { //vecino
			if (G->verticesArr[i]->colorv == G->verticesArr[i]->vecinosArr[j]->colorv) {
        printf("(%lu,%lu)",G->verticesArr[i]->colorv,G->verticesArr[i]->vecinosArr[j]->colorv);
				printf("se coloreo mal, vertice: %lu con vecino su vecino: %lu\n",G->verticesArr[i]->nombrev,
                                                          G->verticesArr[i]->vecinosArr[j]->nombrev);

        exit(EXIT_FAILURE);

			}
		}
	}
	return '0'; //excelente coloreo
}


/* Funciones para extraer informacion de datos del grafo */

u32 NumeroDeVertices(Grafo G)
{
  return (G->nver);
}


u32 NumeroDeLados(Grafo G)
{
  return (G->nlados);
}


u32 Delta(Grafo G)
{
  return (G->delta);
}


/* Funciones para extraer informacion de los vertices */

u32 Nombre(u32 i,Grafo G)
{
  return (G->verticesArr[i]->nombrev);
}


u32 Color(u32 i,Grafo G)
{
  if (i >= G->nver)
  {
  	return (ErrorGrafo);
  }
  else
  {
  	return (G->verticesArr[i]->colorv);
  }
}


u32 Grado(u32 i,Grafo G)
{
  if (i >= G->nver)
  {
  	return (ErrorGrafo);
  }
  else
  {
  	return (G->verticesArr[i]->gradov);
  }
}


/* Funciones para extraer informacion de los vecinos de un vertice */

//enunciado: debe usarse con cuidado
u32 ColorVecino(u32 j, u32 i, Grafo G)
{
  if (i >= G->nver || j >= G->verticesArr[i]->gradov)
  {
  	return(ErrorGrafo);
  }else
  {
    return G->verticesArr[i]->vecinosArr[j]->colorv;
  }
}


u32 NombreVecino(u32 j, u32 i, Grafo G)
{
  return G->verticesArr[i]->vecinosArr[j]->nombrev;
}


u32 OrdenVecino(u32 j,u32 i,Grafo G)
{
	u32 nombre = NombreVecino(j,i,G);
	u32 NumVer = NumeroDeVertices(G);
  u32 index = nombre % NumVer;
  bool vuelta = true;
  u32 indexAux = index;
  while(true)
  {
    if(!vuelta && index == indexAux)
    {
      printf("Error\n");
      exit(EXIT_FAILURE);
    }
    if(Nombre(index,G) == nombre)
    {
      return index;
    }
    else
    {
      index++;
      if(index == NumVer)
      {
        index = 0;
        vuelta = false;
      }
    }
  }
}

/* Funciones para modificar datos de los vertices */

char FijarColor(u32 color,u32 iver,Grafo G)
{
  if (iver >= G->nver)
  {
  	return '1';
  }
  else
  {
  	G->verticesArr[iver]->colorv = color;
  	return '0';
  }
}


char FijarOrden(u32 i,Grafo G,u32 N)
{
  if(i > G->nver || N > G->nver)
  {
    return '1';
  }
  G->verticesArr[i] = G->verticesArr2[N];

  return '0';
}

/*Funciones de ordenacion*/

void OrdenNatural(Grafo G)
{
  u32 NumVert = NumeroDeVertices(G);
  qsort(G->verticesArr,NumVert,sizeof(Vertice),cmpfunc);
}

/*--------------------------Funciones de Ordenación--------------------------*/


/*-----------------------------WelshPowell-------------------------------------
* Se emplea un array de struct _Nodo * llamado Nodo de la siguiente manera,
* ∀i / 0 <= i < NumeroDeVertices(G):
* Nodo[i].estado0 = Grado del vértice i del grafo G.
* Nodo[i].estado1 = Posición natural del vértice i del grafo G para usar en
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
* Se hace empleo de un array de struct _Nodo *, llamado Nodo, de la siguiente
* manera, ∀i / 0 <= i < NumeroDeVertices(G):
* Nodo[i].estado1 = Posición natural del vértice i de G para usar en
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
		qsort(Nodo, NumeroDeVertices(G), sizeof(struct _Nodo), cmpN2);

		// Ordenamos finalmente los vertices del Grafo
		for(u32 i = 0; i<NumeroDeVertices(G); i++)
		{
			FijarOrden(i, G, Nodo[i].estado1);
		}
    free(Nodo);
    return '0';
	}
}


/*----------------------RevierteBC---------------------------------------------
* Se hace empleo de un array de struct _Nodo * llamado Nodo de la siguiente
* manera, ∀i / 0 <= i < NumeroDeVertices(G):
* Nodo[i].estado0 = Color del vértice i del grafo G.
* Nodo[i].estado1 = Posición natural del vértice i del grafo G para usar en
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
    
    // Ordenamos naturalmente, guardamos los colores y el orden natural de
    // cada vertice
    for (u32 i=0; i<NumVer ; i++)
    {
      FijarOrden(i,G,i);
      Nodo[i].estado0 = Color(i,G);
      Nodo[i].estado1 = i;
    }

		// Ordenamos de mayor a menor los bloques de colores
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


/*----------------------Aleatorizar Vertices-----------------------------------
* array_random: se crea un arreglo auxiliar con las posiciones existente de los vertices
* de forma [0,1,3,4,5,6,....,NumeroDeVertices-1], para cada numero del arreglo
* se lo swapea con otro numero en posicion pseudoaleatorio desde 0 hasta
* NumeroDeVertices-1, esto nos permite tener numeros aleatorios unicos para
* usar en fijar orden y que no se pierdan vertices 
*/
char AleatorizarVertices(Grafo G,u32 R)
{
  u32 NumVert = NumeroDeVertices(G);
  //se crea/inicializa un array con numeros no repetidos
  u32 * array_random = malloc(sizeof(u32)*NumVert);
  //condicional por si no existen vertices o no hay suficiente memoria 
  //para el arreglo axuiliar
  if (array_random == NULL || NumVert == 0)
  {
    return '1';
  }
  u32 count;
  u32 random_num; //numero pseudoaleatorio 
  u32 numero_sheldon = 73;
  //se ordenan los vertices en orden natural
  for (count = 0; count < NumVert ; count++)
  {
    FijarOrden(count,G,count);
  }

  for (u32 i = 0; i < NumVert; i++)
  {
    array_random[i]=i;
  }

  srand(R);//semilla para aleatorizar
  for (count = 0; count < NumVert; count++)
  {
    random_num = (rand()*numero_sheldon) % NumVert; //genera numero pseudoaleatorio
    //swap para cada numero de array_random
    u32 temp = array_random[count];
    array_random[count] = array_random[random_num];
    array_random[random_num] = temp;
  }
  //fijar nuevo orden en los vertices
  for(u32 j=0;j<NumVert;j++){
    FijarOrden(j,G,array_random[j]);
  }
  free(array_random);
  return '0';
}


/*-----------------------------Funciones de Coloreo--------------------------*/


/*--------------------------------Greedy---------------------------------------
* Utiliza un arreglo llamado conjunto: cada lugar de este conjunto representa
* un color de los cuales se estan usando actualmente para colorear
* Para cada vertice(menos el primero) se crea el conjunto y se lo modifica para
* que quede como conjunto de los colores no usados en los vecinos de x vertice
* luego se obtiene el menor color no usado, y se colorea x
*/
u32 Greedy(Grafo G)
{ //se comprueba que existira suficiente memoria para el conjunto
  u32 * check_max_memory = malloc(sizeof(u32) * Delta(G));
  if (check_max_memory == NULL)
  {
    free(check_max_memory);
    check_max_memory = NULL;
    return ErrorGrafo;
  }
  free(check_max_memory);
  check_max_memory = NULL;

  //se inicia el coloreo 
  u32 NumVert = NumeroDeVertices(G);
  for (u32 i = 0; i < NumVert; i++) //reseteo del color de los vertices
  {
    FijarColor(ErrorGrafo,i,G);
  }

  u32 * conjunto = NULL; //conjunto
  u32 length_color = 1; //cantidad de colores
  FijarColor(0,0,G); //coloreo del primer vertice
  u32 conjunto_length = length_color; //longitud del conjunto

  //se recorren todos los vertices para colorear
  for (u32 i = 1; i < NumVert ; i++)
  {
    conjunto = calloc(conjunto_length,sizeof(u32));
    //se crea el conjunto de los colores no usados en los vecinos de i
    crear_cjto_color(i,conjunto,conjunto_length,G);
    //se busca el minimo color no usado
    u32 min_color_disp = buscar_min_color(conjunto,conjunto_length);

    //si no superamos los colores que manejamos actualmente
    if(!(min_color_disp == conjunto_length)){ 
      FijarColor(min_color_disp,i,G);

    }else //creamos un nuevo color 
    {
      length_color++;
      FijarColor(min_color_disp,i,G);
      conjunto_length = length_color;
    }
    //se libera el conjunto para crear otro para el sig vertice i
    free(conjunto);
    conjunto = NULL;
  }
  return length_color;
}


/*--------------------------------Bipartito------------------------------------
* Se usa la función auxiliar BiDFS para colorear.
* En caso de error se colorea con Greedy el grafo G.
*/
char Bipartito(Grafo G)
{
  bool ret = true;
  char value = '1';
  u32 NumVer = NumeroDeVertices(G);

  // Coloremoas todos los vertices 
  for(u32 i=0; i<NumVer; i++)
  {
    FijarColor(4294967296-1,i,G);
  }

  // Coloreamos mediante DFS
  for(u32 j=0; j<NumVer; j++)
  {
    ret = BiDFS(j,G);
    if(!ret)
    {
      value = '0';
      break;
    }
  }

  // Si no es bipartito, le damos un color propio
  if (value == '0')
  {
    Greedy(G);
  }

  return value;
}



/*---------------------------SwitchColores----------------------------------
Primero veo cual es el maximo color que se usa en el Grafo, luego me fijo 
si los valores i y j tienen sentido, de ser asi
a los vertices que tienen color i, les asign0 el color j y viceversa
*/
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


/*---------------------------NumCCs----------------------------------
Utilizamos el algoritmo DFS para contar las componentes conexas.
Con este proposito utilizamos una tabla hash donde almacenamos los nombres de los vertices
si fueron visitados aun o no, y la posicion en el grafo.
Empezamos desde el primer vertice y visitamos todos sus vecinos recursivamente,
terminada la recursion, si aun quedan vecinos sin visitar,
incrementamos la cantidad de componentes conexas y aplicamos recursion nuevamente. 
*/
u32 NumCCs(Grafo G)
{
u32 NumVert = NumeroDeVertices(G);
//aca podria pasarle solo el grafo y calcular NumVert adentro
Nodo2 * visitados = inicializar_tabla(NumVert,G);
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
