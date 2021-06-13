#include "comparadores.h"

int cmpON(const void *a, const void *b) {

    struct numgrad *par1;
    struct numgrad *par2;

    par1 = (struct numgrad *)a;
    par2 = (struct numgrad *)b;

    if (par1->nombre < par2->nombre){
        return 1;
    }
    else if (par1->nombre > par2->nombre){
        return -1;
    }
    else{
        return 0;
    }

}

int WelshP_y_RevierteBC_Ord(const void *a, const void *b){

    struct numgrad *par1;
    struct numgrad *par2;

    par1 = (struct numgrad *)a;
    par2 = (struct numgrad *)b;

    if (par1->valor < par2->valor){
        return 1;
    }
    else if (par1->valor > par2->valor){
        return -1;
    }
    else{
        return 0;
    }

}

int ChicoGrandeBCOrd(const void *a, const void *b){


    struct BC * bloque1;
    struct BC * bloque2;


    bloque1 = (struct BC*)a;
    bloque2 = (struct BC*)b;


    if(bloque1->size > bloque2->size){
        return 1;
    }
    else if(bloque1->size < bloque2->size){
        return -1;
    }

    else{
        return 0;
    }


}

int cmpChicoGrandeBC(const void *a,const void *b){
    struct numgrad *par1;
    struct numgrad *par2;

    par1 = (struct numgrad *)a;
    par2 = (struct numgrad *)b;

    if (par1->valor < par2->valor){
        return -1;
    }
    else if (par1->valor > par2->valor){
        return 1;
    }
    else{
        return 0;
    }
}

int AleatorizarVerticesOrd(const void *a,const void *b){
    int k = rand()%3;
    (int*)a ++;
    (int*)b++;
    //printf("%i ",k);
    if (k==0)
    return 0;
    else if (k==1)
    return 1;
    else
    return -1;

}
