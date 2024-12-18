#ifndef L2_2WCACHE_H
#define L2_2WCACHE_H

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

typedef struct L1CacheLine {
    uint8_t Valid;
    uint8_t Dirty;
    uint32_t Tag;
} L1CacheLine;

typedef struct L2CacheLine {
    uint8_t Valid;
    uint8_t Dirty;
    uint32_t Tag;
    uint32_t Time;
} L2CacheLine;

struct L1Cache {
    uint32_t init;
    L1CacheLine lines[L1_SIZE / BLOCK_SIZE];
};

struct L2Cache {
    uint32_t init;
    L2CacheLine lines[L2_SIZE / BLOCK_SIZE];
};

/*********************** Interfaces *************************/

void read(uint32_t, uint8_t *);

void write(uint32_t, uint8_t *);

#endif
