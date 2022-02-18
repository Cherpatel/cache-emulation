#pragma once
#include "../include/cache.hpp"

class Wraper {
private:
    uint8_t _index; // Номер массива
    Cache* _L1; // Указатель на первый кэш

    const size_t _size = 1024 * 1024; // Размер массива

    size_t _addr;   // Адрес
    size_t _addrh;  // Верхушка адреса
    uint8_t _addrt; // Хвост адреса

public:
    Wraper(uint8_t index, Cache* cache) {
        _index = index;
        _L1 = cache;
    }
 
    double& operator[](size_t index) { 
        _addr = _size * _index + index;
        _addrt = _addr & 0x7;

        return (_L1 -> get(_addr)).data[_addrt];
    }
};