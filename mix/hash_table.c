#include "hash_table.h"


struct DataItem {
    bool in_use;
    u32 key;
    u32 data;
};


hash_t* hash_create(u32 size){
    hash_t *htable = NULL;
    htable = calloc(size, sizeof(hash_t));

    for (u32 i = 0; i < size; i++) {
        htable[i] = calloc(1, sizeof(struct DataItem));
        htable[i]->in_use = false;
        htable[i]->key = 0;
        htable[i]->data = 0;
    }
    return htable;
}

u32 hashCode(u32 size, u32 key) {
    return key % size;
}


u32 search(hash_t *hashArray, u32 size, u32 key) {
    // get the hash
    u32 hashIndex = hashCode(size, key);
    u32 contador = 0;

    // moverme en el arreglo hasta encontrar un elemento in_use.
    bool cut = true;
    while (contador < size && cut) {
        if (hashArray[hashIndex]->in_use){
            if (hashArray[hashIndex]->key == key){
                return hashArray[hashIndex]->data;
            }
        }
        else {
            cut = false;
            // si encontramos un elemento no usado quiere decir que no se
            // encuentra el que buscamos. Sino en el peor de los casos se
            // hubiera cargado en éste elemento.
        }
        // go to next cell
        hashIndex++;
        // wrap around the table
        hashIndex %= size;
        contador++;
    }
    return -1;
}

u32 update(hash_t *hashArray,u32 size,u32 key, u32 new_data){
    u32 hashIndex = hashCode(size, key);
    u32 contador = 0;

    // moverme en el arreglo hasta encontrar un elemento in_use.
    bool cut = true;
    while (contador < size && cut) {
        if (hashArray[hashIndex]->in_use){
            if (hashArray[hashIndex]->key == key){
                hashArray[hashIndex]->data = new_data;
                return 0;
            }
        }
        else {
            cut = false;
            // si encontramos un elemento no usado quiere decir que no se
            // encuentra el que buscamos. Sino en el peor de los casos se
            // hubiera cargado en éste elemento.
        }
        // go to next cell
        hashIndex++;
        // wrap around the table
        hashIndex %= size;
        contador++;
    }
    return -1;
}

void insert(hash_t *hashArray,u32 size, u32 key, u32 data) {
    // get the hash
    u32 hashIndex = hashCode(size, key);
    // move in array until an empty cell
    while (hashArray[hashIndex]->in_use) {
        // go to next cell
        hashIndex++;
        // wrap around the table
        hashIndex %= size;
    }
    hashArray[hashIndex]->key = key;
    hashArray[hashIndex]->data = data;
    hashArray[hashIndex]->in_use = true;
}

void delete_ht(hash_t *hashArray, u32 size) {
    for (u32 i = 0; i < size; i++) {
        free(hashArray[i]);
        hashArray[i] = NULL;
    }
    free(hashArray);
    hashArray = NULL;
}

void display(hash_t *hashArray, u32 size) {
    for (u32 i = 0; i < size; i++) {
        if (hashArray[i] != NULL)
            printf(" (%u,%u)", hashArray[i]->key, hashArray[i]->data);
        else
            printf(" ~~ ");
    }
    printf("\n");
}
