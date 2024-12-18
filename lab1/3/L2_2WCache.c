#include "L2_2WCache.h"

uint8_t L1Cache[L1_SIZE];
uint8_t L2Cache[L2_SIZE];
uint8_t DRAM[DRAM_SIZE];
uint32_t time;
struct L1Cache L1SimpleCache;
struct L2Cache L2SimpleCache;

/**************** Time Manipulation ***************/
void resetTime() { time = 0; }

uint32_t getTime() { return time; }

/**************** RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {

    if (address >= DRAM_SIZE - WORD_SIZE + 1)
        exit(-1);

    if (mode == MODE_READ) {
        memcpy(data, &(DRAM[address]), BLOCK_SIZE);
        time += DRAM_READ_TIME;
    }

    if (mode == MODE_WRITE) {
        memcpy(&(DRAM[address]), data, BLOCK_SIZE);
        time += DRAM_WRITE_TIME;
    }
}

/*********************** L1 + L2 cache *************************/

void initCache() {
    L1SimpleCache.init = 0;
    L2SimpleCache.init = 0;
}

void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {

    uint32_t index, offset, Tag, MemAddress;
    uint8_t TempBlock[BLOCK_SIZE];

    /* init cache */
    if (L1SimpleCache.init == 0) {
        for (int i = 0; i < L1_SIZE / BLOCK_SIZE; i++)
            L1SimpleCache.lines[i].Valid = 0;
        L1SimpleCache.init = 1;
    }

    address = address / WORD_SIZE * WORD_SIZE;
    
    index = address % L1_SIZE / BLOCK_SIZE;

    offset = address % BLOCK_SIZE;

    L1CacheLine *Line = &L1SimpleCache.lines[index];

    Tag = address / L1_SIZE;

    MemAddress = address / BLOCK_SIZE * BLOCK_SIZE; // address of the block in memory

    /* access Cache */

    if (!Line->Valid || Line->Tag != Tag) {         // if block not present - miss
        accessL2(MemAddress, TempBlock, MODE_READ); // get new block from DRAM

        if ((Line->Valid) && (Line->Dirty)) { // line has dirty block
            MemAddress = Line->Tag * L1_SIZE + index * BLOCK_SIZE; // get address of the block in memory
            accessL2(MemAddress, &(L1Cache[index * BLOCK_SIZE]), MODE_WRITE); // then write back old block
        }

        memcpy(&(L1Cache[index * BLOCK_SIZE]), TempBlock, BLOCK_SIZE); // copy new block to cache line
        Line->Valid = 1;
        Line->Tag = Tag;
        Line->Dirty = 0;
    } // if miss, then replaced with the correct block

    if (mode == MODE_READ) { // read data from cache line
        memcpy(data, &(L1Cache[index * BLOCK_SIZE + offset]), WORD_SIZE);
        time += L1_READ_TIME;
    }

    if (mode == MODE_WRITE) { // write data from cache line
        memcpy(&(L1Cache[index * BLOCK_SIZE + offset]), data, WORD_SIZE);
        time += L1_WRITE_TIME;
        Line->Dirty = 1;
    }
}

void accessL2(uint32_t MemAddress, uint8_t *data, uint32_t mode) {

    uint32_t index, Tag;
    uint8_t TempBlock[BLOCK_SIZE];

    /* init cache */
    if (L2SimpleCache.init == 0) {
        for (int i = 0; i < L2_SIZE / BLOCK_SIZE; i++)
            L2SimpleCache.lines[i].Valid = 0;
        L2SimpleCache.init = 1;
    }

    index = MemAddress % (L2_SIZE / N_WAYS) / BLOCK_SIZE;

    Tag = MemAddress / (L2_SIZE / N_WAYS);

    int iFound = -1;
    int iInvalid = -1;
    int iLRU = -1;
    int LRU = time;
    for (int i = 0; i < N_WAYS; i++) {
        if (!L2SimpleCache.lines[index * N_WAYS + i].Valid) {
            iInvalid = i;
        } else if (L2SimpleCache.lines[index * N_WAYS + i].Tag != Tag) {
            if (L2SimpleCache.lines[index * N_WAYS + i].Time < (uint32_t) LRU) {
                iLRU = i;
                LRU = L2SimpleCache.lines[index * N_WAYS + i].Time;
            }
        } else {
            iFound = i;
        }
    }

    int i;
    if (iFound != -1)
        i = iFound;
    else if (iInvalid != -1)
        i = iInvalid;
    else
        i = iLRU;

    L2CacheLine *Line = &L2SimpleCache.lines[index * N_WAYS + i];

    /* access Cache */

    if (!Line->Valid || Line->Tag != Tag) {           // if block not present - miss
        accessDRAM(MemAddress, TempBlock, MODE_READ); // get new block from DRAM

        if ((Line->Valid) && (Line->Dirty)) { // line has dirty block
            MemAddress = Line->Tag * (L2_SIZE / N_WAYS) + index * BLOCK_SIZE; // get address of the block in memory
            accessDRAM(MemAddress, &(L2Cache[(index * N_WAYS + i) * BLOCK_SIZE]), MODE_WRITE); // then write back old block
        }

        memcpy(&(L2Cache[(index * N_WAYS + i) * BLOCK_SIZE]), TempBlock, BLOCK_SIZE); // copy new block to cache line
        Line->Valid = 1;
        Line->Tag = Tag;
        Line->Dirty = 0;
    } // if miss, then replaced with the correct block

    if (mode == MODE_READ) { // read data from cache line
        memcpy(data, &(L2Cache[(index * N_WAYS + i) * BLOCK_SIZE]), BLOCK_SIZE);
        Line->Time = time;
        time += L2_READ_TIME;
    }

    if (mode == MODE_WRITE) { // write data from cache line
        memcpy(&(L2Cache[(index * N_WAYS + i) * BLOCK_SIZE]), data, BLOCK_SIZE);
        Line->Time = time;
        time += L2_WRITE_TIME;
        Line->Dirty = 1;
    }
}

void read(uint32_t address, uint8_t *data) {
    accessL1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t *data) {
    accessL1(address, data, MODE_WRITE);
}
