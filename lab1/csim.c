#include "cachelab.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CacheLine {
    int valid;
    unsigned int tag;
    int count;
} CacheLine;

int hit_count, miss_count, eviction_count;
int opt, s, E, b, t;
int S, B, T;

CacheLine **Cache;

int searchCache(unsigned addr) {
    unsigned tag = (addr >> (s + b)) & (T - 1);
    unsigned group = (addr >> b) & (S - 1);
    int flag = 0;
    for (int j = 0; j < E; j++) {
        if (Cache[group][j].valid == 1) {
            Cache[group][j].count++;
        }
    }
    for (int i = 0; i < E; i++) {
        if (Cache[group][i].valid == 1 && Cache[group][i].tag == tag) {
            Cache[group][i].count = 0;
            hit_count++;
            return 0;
        }
    }
    miss_count++;
    for (int i = 0; i < E; i++) {
        if (Cache[group][i].valid == 0) {
            Cache[group][i].tag = tag;
            Cache[group][i].valid = 1;
            Cache[group][i].count = 0;
            flag = 1;
            break;
        }
    }
    if (flag) return 1;
    eviction_count++;
    int max_count = -1;
    int eindex = -1;
    for (int i = 0; i < E; i++) {
        if (Cache[group][i].count > max_count) {
            max_count = Cache[group][i].count;
            eindex = i;
        }
    }
    Cache[group][eindex].tag = tag;
    Cache[group][eindex].valid = 1;
    Cache[group][eindex].count = 0;
    return 2;
}


int main(int argc, char **argv) {
    int i, j;
    char trace[30];
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {//处理参数
        switch (opt) {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                strcpy(trace, optarg);
            default:
                break;
        }
    }
    S = 1 << s;
    B = 1 << b;
    t = 32 - s - b;
    T = 1 << t;
    Cache = (CacheLine **) malloc(sizeof(CacheLine *) * S);
    for (i = 0; i < S; i++) {
        Cache[i] = (CacheLine *) malloc(sizeof(CacheLine) * E);
    }
    for (i = 0; i < S; i++) {
        for (j = 0; j < E; j++) {
            Cache[i][j].valid = 0;
            Cache[i][j].tag = -1;
            Cache[i][j].count = 0;
        }
    }
    FILE *fp = fopen(trace, "r");
    char operation;
    unsigned address;
    int size;
    while (fscanf(fp, " %c %x,%u", &operation, &address, &size) > 0) {
        if (operation == 'L') {
            searchCache(address);
        } else if (operation == 'S') {
            searchCache(address);
        } else if (operation == 'M') {
            searchCache(address);
            searchCache(address);
        }
    }
    printSummary(hit_count, miss_count, eviction_count);
    fclose(fp);
    for (int i = 0; i < S; i++) free(Cache[i]);
    free(Cache);
    return 0;
}