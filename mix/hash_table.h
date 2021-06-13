#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>

typedef uint32_t u32;

typedef struct DataItem *hash_t;

hash_t* hash_create(u32 size);

// return -1 si no lo encuentra.
u32 search(hash_t *htable, u32 size, u32 key);

void insert(hash_t *htable, u32 size, u32 key, u32 data);

void delete_ht(hash_t *htable, u32 size);

u32 update(hash_t *htable, u32 size, u32 key, u32 new_data);

void display(hash_t *htable, u32 size);
