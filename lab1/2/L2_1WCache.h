#ifndef L2_1WCACHE_H
#define L2_1WCACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Cache.h"

void resetTime();

uint32_t getTime();

/**************** RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t, uint8_t *, uint32_t);

/*********************** Cache *************************/

void initCache();
void accessL1(uint32_t, uint8_t *, uint32_t);
void accessL2(uint32_t, uint8_t *, uint32_t);

typedef struct CacheLine {
    uint8_t Valid;
    uint8_t Dirty;
    uint32_t Tag;
} CacheLine;

struct L1Cache {
    uint32_t init;
    CacheLine lines[L1_SIZE / BLOCK_SIZE];
};

struct L2Cache {
    uint32_t init;
    CacheLine lines[L2_SIZE / BLOCK_SIZE];
};

/*********************** Interfaces *************************/

void read(uint32_t, uint8_t *);

void write(uint32_t, uint8_t *);

#endif
