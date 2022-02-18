#pragma once
#include <iostream>
#include <algorithm>
#include "../include/ram.hpp"

unsigned int size = 1024 * 1024;

// Массив кэшей
std::array<Cache*, cacheCount> caches;

void init_caches() {
    struct CacheParams {
        uint8_t ways;
        size_t size;
    };

    CacheParams cacheParams[cacheCount] = {{4, 128}, {8, 2048}}; //128 4  2048 8

    for (uint8_t i = 0; i < cacheCount; i++) {
        caches[i] = new Cache(i, cacheParams[i].size, cacheParams[i].ways, &caches);
    }
}

int caches_init = (init_caches(), 0);

// Вызываем конструктор обёртки
Wraper x(0, caches[0]);
Wraper y(1, caches[0]);
Wraper z(2, caches[0]);
Wraper xx(3, caches[0]);
Wraper yy(4, caches[0]);
Wraper zz(5, caches[0]);

void f(double w) {
    for (unsigned int i = 0; i < size; ++i) {
        x[i] = xx[i] * w + x[i];
    }

    for (unsigned int i = 0; i < size; ++i) {
        y[i] = yy[i] * w + y[i];
    }
    
    for (unsigned int i = 0; i < size; ++i) {
        z[i] = zz[i] * w + z[i];
    }
}

/*void f(double w) {
    for (unsigned int i=0; i < size; ++i) {
        x[i] = xx[i] * w + x[i];
        y[i] = yy[i] * w + y[i];
        z[i] = zz[i] * w + z[i];
    }
}*/