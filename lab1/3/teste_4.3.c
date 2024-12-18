#include "L2_2WCache.h"

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
            uint32_t address = rand() % DRAM_SIZE, value = 0;
            int j = rand() % 100;
            if (j < 50)
                read(address, (unsigned char *)(&value));
            else
                write(address, (unsigned char *)(&value));
        }
        sum += getTime(); 
    }
    printf("\nAverage Time: %d\n", sum / m);
}

int main() {

    pingPong2();

    for (int i = 10; i <= 1000000; i *= 10) {
        randomTesting(10, i);
    }

    return 0;
}