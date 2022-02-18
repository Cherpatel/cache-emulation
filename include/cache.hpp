#pragma once
#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <cstdint>
#include <cstddef>

const uint8_t cacheCount = 2;

struct ram {
private:
    static const size_t _size = 1024 * 1024 * 6;

    double data[_size] {};
public:
    double* read(size_t addr) {
        addr = addr >> 3 << 3;

        return &(data[addr]);
    }

    void write(size_t addr, std::array<double, 8> line) {
        if (addr == -1) return;

        for (uint8_t i = 0; i < line.size(); i++)
            data[addr + i] = line[i];
    }
} RAM;

struct Stat {
    size_t hit = 0;  // Статистика попаданий
    size_t over = 0; // Статистика запросов
};

class CacheLine {
private:
    static const uint8_t lSize = 64; // Размер линии в байтах (8 double)

public:
    std::array<double, lSize / sizeof(double)> data {}; // Кэш линия

    ssize_t addrh; //Верхушка адреса, без 64 байт (без 8 double)

    CacheLine(ssize_t addr = -1 * 8, double* arr = nullptr) {
        addrh = addr >> 3;

        if (arr) {
            for (int i = 0; i < lSize / sizeof(double); i++)
                data[i] = arr[i];
        }
    }
};

class Unit {
private:
    uint8_t way;
public:
    Unit(uint8_t way) {
        way = way;
        unit.resize(way, CacheLine());
    }

    std::vector<CacheLine> unit; // Ячейка
};

class Cache {
private:
    size_t _size; // Размер всего кэша
    uint8_t _way;  // Кол-во линий в ячейке
    uint8_t _index; // Индекс (номер) кэша

    static const uint8_t lSize = 64; // размер линии в кэше

    std::vector<Unit> cache; // Кэш

    std::array<Cache*, cacheCount>* cachesArr; //Массив всех кэшей (указателей)

    Stat stat;

public:
    Cache(uint8_t index, size_t size, uint8_t way, std::array<Cache*, cacheCount>* arr) {
        _index = index;

        _size = size;
        _way = way;

        cachesArr = arr;

        cache.resize(size, Unit(way)); 
    }

    void printStat() {
        printf("hit: %zu\noverall: %zu\n", stat.hit, stat.over);
        std::cout << ((double)stat.hit / stat.over) * 100 << "%\n\n";
    }

    void lru(Unit& u, uint8_t index) {
        for (int i = index; i > 0; i--)
            std::swap(u.unit[i], u.unit[i - 1]);
    }

    CacheLine& get(size_t addr) {
        ++stat.over;
        
        size_t addrh = addr >> 3;

        for (uint8_t i = 0; i < _way; i++) {
            size_t curAddrh = cache[addrh % _size].unit[i].addrh;

            if (curAddrh == addrh) {
                ++stat.hit;

                lru(cache[addrh % _size], i);

                return cache[addrh % _size].unit[0];
            }
        }

        if (_index != cacheCount - 1) {
            CacheLine line = ((*cachesArr)[_index + 1] -> get(addr));
            std::swap(line, cache[addrh % _size].unit[_way - 1]);
            lru(cache[addrh % _size], _way - 1);

            return cache[addrh % _size].unit[0];
        } else {
            CacheLine line(addr, RAM.read(addr));
            std::swap(line, cache[addrh % _size].unit[_way - 1]);
            lru(cache[addrh % _size], _way - 1);

            RAM.write(line.addrh, line.data);
        }

        return cache[addrh % _size].unit[0];
    }
};