#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>


#include "veinteveinte.h"
#include "GrafoSt2020.h"
#include "parser.h"
#include "helpers.h"

int main()
{
  char welsh[3] = "-w";
  char aleat[3] = "-a";
  char cg[3] = "-c";
  char rbc[3] = "-r";
  char numcc[3] = "-n";
  char bip[3] = "-b";
  char gr[3] = "-g";
  char nat[4] = "-nt";
  char q[4] = "-q";
  char wg[4] = "-wg";
  char rc[4] = "-rc" ;
  char cc[4] = "-cc";
  clock_t t_ini, t_fin;
  double secs;
  char op[4]="";
  printf("empeze a construir el grafo\n");
  Grafo g = ConstruccionDelGrafo();
  printf("termine de construir el grafo\n");
while(true)
{
  printf("modo de uso:\n -g : Greedy n veces \n -w : WelshPowell y Greedy \n"
  " -a : AleatorizarVertices y Greedy\n -c : ChicoGrandeBC y Greedy \n -r :"
  "RevierteBC y Greedy \n -n : NumCCs \n -b : Bipartito\n -nt : Orden Natural \n" 
  " -q : break \n -wg : WelshPowell y sus grados\n -rc : RevierteBC y sus colores\n"
  "-cc : ChicoGrandeBC y sus colores\n");

  printf("Opción:\n");
  scanf("%s",*(&op));
  printf("opcion elegida %s\n",op);
  if (0 == strcmp(op,welsh))
  {
    printf("WelshPowell Y Greedy 1100 iteraciones\n");
    t_ini = clock();
    for (u32 i = 0; i < 1000; i++) {
      char check = WelshPowell(g);
      if (check != '0'){
      printf("error\n");
      exit(EXIT_FAILURE);
      }
      u32 color = Greedy(g);
      printf("%lu \n",color);
    }
    t_fin = clock();
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("\n%.16g segundos\n", secs);


  }else if(0 == strcmp(op ,aleat))
  {
    printf("AleatorizarVertices Y Greedy 1100 iteraciones\n");
    /*printf("empeze a construir el grafo\n");
    Grafo g = ConstruccionDelGrafo();
    printf("termine de construir el grafo\n");*/
    t_ini = clock();
    for (u32 i = 0; i < 1000; i++) {
      char check = AleatorizarVertices(g,i);
      if (check != '0'){
      printf("error\n");
      exit(EXIT_FAILURE);
      }
      u32 color = Greedy(g);
      printf("%lu \n",color);
    }
    t_fin = clock();
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("\n%.16g segundos\n", secs);


  }else if (0 == strcmp(op,cg))
  {
    printf("ChicoGrandeBC Y Greedy 1100 iteraciones\n");
    /*printf("empeze a construir el grafo\n");
    Grafo g = ConstruccionDelGrafo();
    printf("termine de construir el grafo\n");*/
    t_ini = clock();
    for (u32 i = 0; i < 1000; i++) {
      u32 color = Greedy(g);
      printf("%lu \n",color);
      char check = ChicoGrandeBC(g);
      if (check != '0'){
      printf("error\n");
      exit(EXIT_FAILURE);
      }

    }
    t_fin = clock();
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("\n%.16g segundos\n", secs);

  }else if (0 == strcmp(op,rbc))
  {
    printf("RevierteBC Y Greedy 1100 iteraciones\n");
    /*printf("empeze a construir el grafo\n");
    Grafo g = ConstruccionDelGrafo();
    printf("termine de construir el grafo\n");*/
    t_ini = clock();
    for (u32 i = 0; i < 1000; i++) {
      char check = RevierteBC(g);
      if (check != '0'){
      printf("error\n");
      exit(EXIT_FAILURE);
      }
      u32 color = Greedy(g);
      printf("%lu \n",color);
    }
    t_fin = clock();
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("\n%.16g segundos\n", secs);

  }else if (0 == strcmp(op,numcc))
  {
    printf("NumCCs 1100 iteraciones\n");
    /*printf("empeze a construir el grafo\n");
    Grafo g = ConstruccionDelGrafo();
    printf("termine de construir el grafo\n");*/
    t_ini = clock();
    for (u32 i = 0; i < 1000; i++) {
      u32 numcc = NumCCs(g);
      if (numcc == 0){
      printf("error\n");
      exit(EXIT_FAILURE);
      }
      printf("numcc -> %lu \n",numcc);
    }
    t_fin = clock();
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("\n%.16g segundos\n", secs);

  }else if (0 == strcmp(op,bip))
  {
    printf("Bipartito 1100 iteraciones\n");
    /*printf("empeze a construir el grafo\n");
    Grafo g = ConstruccionDelGrafo();
    printf("termine de construir el grafo\n");*/
    t_ini = clock();
    for (u32 i = 0; i < 1000; i++) {
      char bipartito = Bipartito(g);
      printf("bipartito -> %c \n",bipartito);
    }
    t_fin = clock();
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("\n%.16g segundos\n", secs);
  }else if(0 == strcmp(op,gr))
  {
    printf("Indique n: \n");
    u32 n = 0;
    scanf("%lu",&n);
    if (n > 0){
      printf("Greedy %lu veces\n",n);
      /*printf("empeze a construir el grafo\n");
      Grafo g = ConstruccionDelGrafo();
      printf("termine de construir el grafo\n");*/
      t_ini = clock();
      for (u32 i = 0; i < n; i++) {
        u32 color = Greedy(g);
        printf("%lu \n",color);
      }
      t_fin = clock();
      secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
      printf("\n%.16g segundos\n", secs);
  }
}else if(0 == strcmp(op,nat)){
  printf("Orden natural\n");
  t_ini = clock();
  OrdenNatural(g);
  t_fin = clock();
  secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
  printf("\n%.16g segundos\n", secs);
}
else if(0 == strcmp(op,q))
  {
    printf("Break\n");
    break;
  }


  else if(0 == strcmp(op,wg)){
  printf("WelshPowell con sus grados\n");
  t_ini = clock();
  WelshPowell(g);
  for(u32 i=0; i<NumeroDeVertices(g);i++)
  {
    printf("Nombre %lu;   Grado %lu\n",Nombre(i,g), Grado(i,g));
  }
  printf("Grado Grafo %lu\n",Delta(g));
  t_fin = clock();
  secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
  printf("\n%.16g segundos\n", secs);
}

  else if(0 == strcmp(op,rc)){
  printf("RevierteBC con sus colores\n");
  t_ini = clock();
  Greedy(g);
  RevierteBC(g);
  for(u32 i=0; i<NumeroDeVertices(g);i++)
  {
    printf("Nombre %lu;   Color %lu\n",Nombre(i,g), Color(i,g));
  }
  t_fin = clock();
  secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
  printf("\n%.16g segundos\n", secs);
}

  else if(0 == strcmp(op,cc)){
  printf("ChicoGrandeBC con sus colores\n");
  t_ini = clock();
  Greedy(g);
  ChicoGrandeBC(g);
  for(u32 i=0; i<NumeroDeVertices(g);i++)
  {
    printf("Nombre %lu;   Color %lu\n",Nombre(i,g), Color(i,g));
  }
  t_fin = clock();
  secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
  printf("\n%.16g segundos\n", secs);
}


  else {
    printf("Argumento Invalido\n");
  }
}
  DestruccionDelGrafo(g);
  return 0;
}
