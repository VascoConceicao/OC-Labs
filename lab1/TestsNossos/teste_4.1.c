#include "L1Cache.h"

void sameTagsameIndex() {

    int clock, value;

    printf("\n---------- Same Tag Same Index ----------\n");

    resetTime();
    initCache();

    for (int i = 0; i < 3; i++) {
        write(i * WORD_SIZE, (unsigned char *)(&i));
        clock = getTime();
        printf("Write; Address %d; Value %d; Time %d\n", i * WORD_SIZE, i, clock);
        read(i * WORD_SIZE, (unsigned char *)(&value));
        clock = getTime();
        printf("Read; Address %d; Value %d; Time %d\n", i * WORD_SIZE, value, clock);
    }
}

void diffTagsameIndex() {

    int clock, value;

    printf("\n---------- Different Tag Same Index ----------\n");

    resetTime();
    initCache();

    for (int i = 0; i < 3; i++) {
        write(i * L1_SIZE, (unsigned char *)(&i));
        clock = getTime();
        printf("Write; Address %d; Value %d; Time %d\n", i * L1_SIZE, i, clock);
        read(i * L1_SIZE, (unsigned char *)(&value));
        clock = getTime();
        printf("Read; Address %d; Value %d; Time %d\n", i * L1_SIZE, value, clock);
    }
}

void differentIndex() {

    int clock, value, address;

    printf("\n---------- Different Index ----------\n");

    resetTime();
    initCache();

    for (int i = 0; i < 3; i++) {
        address = rand() % DRAM_SIZE;
        write(address, (unsigned char *)(&i));
        clock = getTime();
        printf("Write; Address %d; Value %d; Time %d\n", address, i, clock);
        read(address, (unsigned char *)(&value));
        clock = getTime();
        printf("Read; Address %d; Value %d; Time %d\n", address, value, clock);
    }
}

void readMiss() {

    int clock, value;

    printf("\n---------- Read Miss ----------\n");

    resetTime();
    initCache();

    for (int i = 0; i < 3; i++) {
        write(i * L1_SIZE, (unsigned char *)(&i));
        clock = getTime();
        printf("Write; Address %d; Value %d; Time %d\n", i * L1_SIZE, i, clock);
    }

    for (int i = 0; i < 3; i++) {
        read(i * L1_SIZE, (unsigned char *)(&value));
        clock = getTime();
        printf("Read; Address %d; Value %d; Time %d\n", i * L1_SIZE, value, clock);
    }

}

void addressChecker() {

    int clock, value, address1, address2;

    printf("\n---------- Address Checker ----------\n");

    resetTime();
    initCache();

    address1 = L1_SIZE + BLOCK_SIZE;
    address2 =  2 * L1_SIZE + BLOCK_SIZE;

    value = rand() % 70;
    write(address1, (unsigned char *)(&value));
    clock = getTime();
    printf("Write; Address %d; Value %d; Time %d\n", address1, value, clock);

    value = rand() % 70;
    write(address2, (unsigned char *)(&value));
    clock = getTime();
    printf("Write; Address %d; Value %d; Time %d\n", address2, value, clock);

    read(address1, (unsigned char *)(&value));
    clock = getTime();
    printf("Read; Address %d; Value %d; Time %d\n", address1, value, clock);
}

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

    sameTagsameIndex();
    diffTagsameIndex();
    differentIndex();
    readMiss();
    addressChecker();
    pingPong1();

    randomTesting(10, 1000000);

    return 0;
}