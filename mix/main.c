#include <stdio.h>
#include "veinteveinte.h"
#include <time.h>

int main(){

  clock_t t_ini, t_fin;
  double secs;
  //printf("ChicoGrandeBC Y Greedy 1100 iteraciones\n");
  printf("empeze a construir el grafo\n");
  Grafo g = ConstruccionDelGrafo();
  printf("termine de construir el grafo\n");
  t_ini = clock();
  //u32 temp = ErrorGrafo;
  //for (u32 i = 0; i < 1000; i++) {
    WelshPowell(g);
    u32 color = Greedy(g);
    //if (color < temp ) {
    //  temp = color ;
    //}
    printf("%u\n",color);
    //char check = ChicoGrandeBC(g);
    //if (check != '0'){
    //printf("error\n");
    //exit(EXIT_FAILURE);
    //}

  
  t_fin = clock();
  secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
//printf("menor coloreo obtenido -> %lu \n",temp);
  printf("%.16g segundos\n", secs);
  DestruccionDelGrafo(g);

return 0;
}
