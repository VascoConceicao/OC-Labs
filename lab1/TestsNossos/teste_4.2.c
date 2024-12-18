#include "L2_1WCache.h"

void pingPong1() {

    int clock, value;

    printf("\n---------- Ping Pong 1 ----------\n");

    resetTime();
    initCache();

    for (int i = 0; i < 2; i++) {
        write(i * L1_SIZE, (unsigned char *)(&i));
        clock = getTime();
        printf("Write; Address %d; Value %d; Time %d\n", i * L1_SIZE, i, clock);
    }

    for (int i = 0; i < 3; i++) 
        for (int j = 0; j < 2; j++) {
            read(j * L1_SIZE, (unsigned char *)(&value));
            clock = getTime();
            printf("Read; Address %d; Value %d; Time %d\n", j * L1_SIZE, value, clock);
        }
}

void pingPong2() {

    int clock, value;

    printf("\n---------- Ping Pong 2 ----------\n");

    resetTime();
    initCache();

    for (int i = 0; i < 2; i++) {
        write(i * L2_SIZE, (unsigned char *)(&i));
        clock = getTime();
        printf("Write; Address %d; Value %d; Time %d\n", i * L2_SIZE, i, clock);
    }

    for (int i = 0; i < 3; i++) 
        for (int j = 0; j < 2; j++) {
            read(j * L2_SIZE, (unsigned char *)(&value));
            clock = getTime();
            printf("Read; Address %d; Value %d; Time %d\n", j * L2_SIZE, value, clock);
        }
}

void randomTesting(int m, int n) {
    int sum = 0;
    for (int j = 0; j < m; j++) {
        resetTime();
        initCache();
        for (int i = 0; i < n; i++) {
            int j = rand() % 100;
            if (j < 50) {
                uint32_t address = rand() % DRAM_SIZE, value;
                read(address, (unsigned char *)(&value));
            } else {
                uint32_t address = rand() % DRAM_SIZE, value = 0;
                write(address, (unsigned char *)(&value));
            }
        }
        sum += getTime(); 
    }
    printf("\nAverage Time: %d\n", sum / m);
}

int main() {

    pingPong1();
    pingPong2();

    randomTesting(10, 1000000);

    return 0;
}