#include <stdio.h>
#include <stdbool.h>
#include "parser.h"


Vnodo obtener_nodo(u32 nombre, u32 index,Vnodo * clonev,u32 lenght)
{
  Vnodo nodo = NULL;
  if(clonev[index]->vertice->nombrev == nombre)
  {
    nodo = clonev[index];
  }else
  {
    u32 finish_clonev = index;
    index++;
    while(index != finish_clonev)
    {
      if(index >= lenght)
      {
        index = 0;
      }else if(clonev[index]->vertice->nombrev == nombre)
      {
        nodo = clonev[index];
        index = finish_clonev;
      }else
      {
        index++;
      }
    }
    if(nodo == NULL){
      fprintf(stderr, "algo salio mal, no existe el nodo\n");
      exit(EXIT_FAILURE);
    }
  }

  return nodo;
}

void guardar_en_clonev(Vertice verticeaux,Vnodo * clonev,u32 lenght)
{
  u32 index = verticeaux->nombrev % lenght;

  //caso donde justo la celda esta vacia
  if(clonev[index]->estado == 1)
  {
    clonev[index]->vertice = verticeaux;
    clonev[index]->estado = 0;
  //caso donde no, entonces empesamos a buscar en las demas
  }else{
    u32 finish_clonev = index;
    index++;
    while(index != finish_clonev){
      //si nos pasamos con el index de la longitud del los vertices en clonev
      //vamos con los que estaban detras de index.
      if(index >= lenght)
      {
        index = 0;
      //si no nos pasamos y justo encontramos uno vacio, asignamos y
      //cambiamos el estado.
      }else if(clonev[index]->estado == 1)
      {
          clonev[index]->vertice = verticeaux;
          clonev[index]->estado = 0;
          index = finish_clonev;

      }else
      {
        index++;
      }
    }
  }
}


bool existeElVertice(Grafo g , u32 vname, Arista aristaaux,Vnodo * clonev)
{
 bool ret = false;

  //depende el nver es que tantas colisiones va a haber, lo lei en una pagina web xd
  u32 index = vname % g->nver;

  //caso donde como no existe ningun vertice => este tampoco
  if(clonev[index]->estado == 1)
  {
    return ret;

  //caso donde lo encontramos justo en su celda correspondiente en hash
  }else if(clonev[index]->vertice->nombrev == vname)
  {
    //el vertice existe, sumo al grado ya que voy a ponerlo en una arista
    clonev[index]->vertice->gradov++;
    if (clonev[index]->vertice->gradov > g->delta)
    {
      g->delta = clonev[index]->vertice->gradov;
    }
    if (aristaaux->extremo1 == NULL)
    { //digo que el extremo1 o 2 tengan el espacio de memoria del vertice
      aristaaux->extremo1 = clonev[index]->vertice;
    }else if(aristaaux->extremo2 == NULL){
      aristaaux->extremo2 = clonev[index]->vertice;
    }

    ret = true;
    return ret;

  //no esta en su celda correspondiente en hash, realizar busqueda quasilineal
  }else
  {

    u32 finish_clonev = index;
    index++;
    while(index != finish_clonev){
      //empezamos desde una posicion n, si nos pasamos seguimos
      //buscando en las posiciones anteriores a n.
      if(index >= g->nver){
        index = 0;
      //pero no deberia chequear en finish_clonev tambien, o sea puede que ser que el vertice este justo antes de index
      if(index == finish_clonev){
        fprintf(stderr, "diste la vuelta y no asignaste\n");
        //exit(EXIT_FAILURE);
        return false;
      }
      //caso donde encontramos en una celda aleatoria no vacia
      }else if(clonev[index]->estado == 0)
      {
          //caso donde es justo el vertice
          if(clonev[index]->vertice->nombrev == vname){

            //el vertice existe, sumo al grado ya que voy a ponerlo en una arista
            clonev[index]->vertice->gradov++;
            if (clonev[index]->vertice->gradov > g->delta)
            {
              g->delta = clonev[index]->vertice->gradov;
            }
            if (aristaaux->extremo1 == NULL)
            { //digo que el extremo1 o 2 tengan el espacio de memoria del vertice
              aristaaux->extremo1 = clonev[index]->vertice;
            }else if(aristaaux->extremo2 == NULL){
              aristaaux->extremo2 = clonev[index]->vertice;
            }
            index = finish_clonev;
            ret = true;
          }else
          {
            index++;
          }
      //aun no se encontro nada, buscamos en el siguente
      }else
      {
        index++;
      }
    }
  }

  return ret;

  /*
  bool ret = false ;
  unsigned int i = 0;
  while( i < vcounter){

    if(g->verticesArr[i]->nombrev == vname){
      ret = true;
      //sumar 1 al grado
      g->verticesArr[i]->gradov++;

      if (g->verticesArr[i]->gradov > g->delta)
      {
      //si el nuevo valor de grado es mayor
        //que el delta => es el nuevo delta
      g->delta = g->verticesArr[i]->gradov;
      }

      if (aristaaux->extremo1 == NULL)
      { //digo que el extremo1 o 2 apunte al espacio de memoria lo que esta
         //almacenado en la posicion i del arreglo verticesArr
        aristaaux->extremo1 = g->verticesArr[i];
      }else if(aristaaux->extremo2 == NULL){
        aristaaux->extremo2 = g->verticesArr[i];
      }

      i = vcounter;
    }else{
      i++;
    }
  }
  return ret;
  */
}

/*todos los vertices que creamos los creamos cuando existe una arista entre ellos
y no existe otro vertice con el mismo nombre => nacen con grado 1*/
Vertice crearVertice(u32 vname)
{
  Vertice v = malloc(sizeof(struct _VerticeSt));
  v->nombrev = vname;
  //se le pone este numero especialmente para que funcione greedy, pero esta mal :/
  v->colorv = 4294967296-1;
  v->gradov = 0;

  return v;
}

Arista crearArista()
{
  Arista a = malloc(sizeof(struct _AristaSt));
  a->extremo1 = NULL;
  a->extremo2 = NULL;

  return a;
}

// Ordena de forma natural los vertices
int cmpfunc (const void * a, const void * b) {

  //fijense en este truquito
  Vertice al =*((Vertice*)a);
  Vertice bl =*((Vertice*)b);


  if (al->nombrev < bl->nombrev)
  {
    return -1;
  }
  if (al->nombrev > bl->nombrev)
  {
    return 1;

  }else{
    return 0;
  }
}

void parser(Grafo g )
{

  char pathname[100];
  int scan;
  printf("indique la ruta de su archivo\n");
  scan = scanf("%s", pathname);
  if (scan <= 0)
  {
    exit(EXIT_FAILURE);
    printf("pathname vacio\n");
  }

  FILE *file = NULL;
  file = fopen(pathname,"r");
  if(file == NULL)
  {
    fprintf(stderr, "Problema al abrir el archivo\n");
    exit(EXIT_FAILURE);
  }

  printf("Exito al abrir el archivo\n");

  char firstchar;
  u32 nvert;
  u32 naris;
  Vertice verticeaux = NULL;
  Arista aristaaux = NULL;
  u32 vertice1;
  u32 vertice2;
  u32 vcounter = 0;
  u32 acounter = 0;
  static Vnodo *clonev;
  int check;

  while(!feof(file))
  {
    check = fscanf(file,"%c", &firstchar);

    if (check >= 0) {

     if(firstchar == 'c')
     {
       while (firstchar != '\n')
       {
        check = fscanf(file,"%c", &firstchar);
        if (check < 0)
        {
          exit(EXIT_FAILURE);
          printf("error al leer el primer caracter\n");
        }
       }
     }

     else if(firstchar == 'p')
     {
         check = fscanf(file,"%*s %lu %lu", &nvert, &naris);
         if (check <= 0)
         {
           exit(EXIT_FAILURE);
           printf("error al leer los parametros de p\n");
         }
         printf("hay %lu vertices y %lu aristas declaradas\n", nvert, naris );

         //relleno los datos
         g -> nver = nvert;
         g -> nlados = naris;
         //guardo memoria para mi arreglo de punteros a vertices
         g -> verticesArr = malloc(sizeof(Vertice) * nvert);
         clonev = malloc(sizeof(Vnodo) * nvert);
         //clonev[0] = malloc(sizeof(struct _Vnodo));
         //clonev[0]->vertice = crearVertice(7);
         //clonev[0]->estado = 0;
         //clonev[0]->clave = 7;
         //printf("estado: %lu",clonev[0]->estado);
         //printf("nombre por vertice: %lu",clonev[0]->vertice->nombrev);

         for(u32 i=0;i < nvert;i++){
           //recordar luego mandar free lineal en el arreglo
           clonev[i] = malloc(sizeof(struct _Vnodo));
           //no existe el vertice 0 y esto es para las hash
           clonev[i]->vertice = NULL;
           //1 es vacio, 0 es usado
           clonev[i]->estado = 1;
         }
         printf("se guardo espacio para %lu vertices\n", nvert);
         g -> aristaArr = malloc(sizeof(Arista) * naris);
         printf("se guardo espacio para %lu aristas\n", naris);

     }

     else if(firstchar == 'e')
     {
      /*crea los vertices de derecha a izquierda*/
        check = fscanf(file,"%lu" "%lu", &vertice1, &vertice2);
        if (check <= 0)
        {
          exit(EXIT_FAILURE);
          printf("error al leer una linea de vertices\n");
        }
        aristaaux = crearArista();

        if (!existeElVertice(g,vertice1,aristaaux,clonev)) {
          verticeaux = crearVertice(vertice1);
          verticeaux->gradov++;
          //printf("v: %lu, ", vertice1);
          g->verticesArr[vcounter] = verticeaux;
          guardar_en_clonev(verticeaux,clonev,g->nver);
          vcounter = vcounter + 1;
            //apunte al espacio de memoria del vertice creado
            if (aristaaux->extremo1 == NULL)
            {
              aristaaux->extremo1 = verticeaux;
            }else if (aristaaux->extremo1 == NULL){
              aristaaux->extremo2 = verticeaux;
            }
        }

        if (!existeElVertice(g,vertice2,aristaaux,clonev)) {
          verticeaux = crearVertice(vertice2);
          verticeaux->gradov++;
          //printf("v: %lu, ", vertice2);
          g->verticesArr[vcounter] = verticeaux;
          guardar_en_clonev(verticeaux,clonev,g->nver);
          vcounter = vcounter + 1 ;
            /*apunte al espacio de memoria del vertice creado*/
            if (aristaaux->extremo1 == NULL)
            {
              aristaaux->extremo1 = verticeaux;
            }else if(aristaaux->extremo2 == NULL){
              aristaaux->extremo2 = verticeaux;
            }
        }


        g->aristaArr[acounter] = aristaaux;
        acounter++;

        //printf("hay una arista que va del vertice %lu al vertice %lu\n",  vertice1, vertice2);

     }
   }
  }
  verticeaux = NULL;
  aristaaux = NULL;

  fclose(file);
  for(u32 i=0; i < g->nver ;i++)
  {
    g->verticesArr[i]->vecinosArr = malloc(sizeof(Vertice) * g->verticesArr[i]->gradov);
  }
  //para extraer cada extremo en las aristas,recordar liberar

  Vertice vertic1;
  Vertice vertic2;
  u32 index_1;
  u32 index_2;
  Vnodo nodo_vertic1;
  Vnodo nodo_vertic2;
  //apartir de ahora. como en clone v tenemos el puntero a un vertice y su estado en u32
  //usaremos ese estado para almancenar el contador de cuantos vecinos vamos almacenando
  for(u32 j=0; j < g->nlados ;j++)
  {
    vertic1 = g->aristaArr[j]->extremo1;
    vertic2 = g->aristaArr[j]->extremo2;

    //posiciones en las cuales podrian estar en clonev
    index_1 = vertic1->nombrev % g->nver;
    index_2 = vertic2->nombrev % g->nver;

    nodo_vertic1 = obtener_nodo(vertic1->nombrev,index_1,clonev,g->nver);
    nodo_vertic2 = obtener_nodo(vertic2->nombrev,index_2,clonev,g->nver);

    vertic1->vecinosArr[nodo_vertic1->estado] = vertic2;
    nodo_vertic1->estado++;
    vertic2->vecinosArr[nodo_vertic2->estado] = vertic1;
    nodo_vertic2->estado++;

  }

  u32 delta = 0;
  for(u32 i = 0; i < g->nver; i++)
  {
    if(delta < Grado(i,g))
    {
      delta = Grado(i,g);
    }
  }
  g->delta = delta;
  vertic1 = NULL;
  vertic2 = NULL;
  nodo_vertic1 = NULL;
  nodo_vertic1 = NULL;

  for(u32 i=0;i<g->nver;i++)
  {
    clonev[i]->vertice = NULL;
    free(clonev[i]);
    clonev[i] = NULL;
  }
  free(clonev);
  clonev = NULL;

  // Copiamos vertices
  g -> verticesArr2 = malloc(sizeof(Vertice) * nvert);
  for(u32 i=0; i<g->nver; i++)
  {
    g->verticesArr2[i] = g->verticesArr[i];
  }
  // Los ordenamos de forma natural segun el nombre
  qsort(g->verticesArr2, g->nver, sizeof(Vertice), cmpfunc);

/////////////////////////////////////////////////////////////////////////////////////////////////////////

  //busqueda de vecinos para cada vertice, medio lerda
  /*
  for (unsigned int i = 0; i < vcounter; i++)
  {
    //neighborsv, creacion de arreglo para los vecinos tipo puntero _VerticeSt
    g->verticesArr[i]->vecinosArr = calloc(g->verticesArr[i]->gradov, sizeof(Vertice));

    u32 counter = 0;
     // para cada arista
    for (u32 j = 0; j < acounter; j++)
    {
      //si alguno de los extremos es del mismo nombre que del del vertice
      if (g->aristaArr[j]->extremo1->nombrev == g->verticesArr[i]->nombrev)
      {
        g->verticesArr[i]->vecinosArr[counter] = g->aristaArr[j]->extremo2;
        counter++;
      }
      else if (g->aristaArr[j]->extremo2->nombrev == g->verticesArr[i]->nombrev)
      {
        g->verticesArr[i]->vecinosArr[counter] = g->aristaArr[j]->extremo1;
        counter++;
      }
    }
  }
  */
  //printf("vecinos de %lu:\n",g->verticesArr[3]->nombrev);
  //for (u32 j = 0; j < g->verticesArr[3]->gradov; j++){
  //  printf("%lu, ",g->verticesArr[3]->vecinosArr[j]->nombrev);
  //}
  //printf("\n");
  //RECORDAR LIBERAR EN CLONEV
}
