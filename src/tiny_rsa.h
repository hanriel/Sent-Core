//
// Created by Данил Федосеев on 07.03.2021.
//

#ifndef SENT_CORE_TINY_RSA_H
#define SENT_CORE_TINY_RSA_H


#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <stack>

class tiny_rsa {
    //Структура ключа
    typedef struct { uint64_t e; uint64_t n; } key;
private:
    //===Недмного дополнительного math-а===
    //Расширенный алгоритм Евклида
    static int64_t gcdex(int64_t a, int64_t b, int64_t &x, int64_t &y);
    //Обратное по модулю
    static int64_t invmod(int64_t a, int64_t m);
    //Квадрат числа
    static uint64_t sqr(uint64_t x);
    //Быстрое возведение в степень
    static uint64_t binpow(uint64_t a, uint64_t e, uint64_t mod = LLONG_MAX);
    //Проверка на простое число
    bool isPrime(int number);

    //Размер блока шифрования ключа
    static uint8_t get_chunk_size(key k);
    //Резбиение/обьединение данных в блоки
    static std::vector<uint64_t> resize(const std::vector<uint64_t> &data, uint8_t in_size, uint8_t out_size);
    //Де/шифрование
    static std::vector<uint8_t> process_bytes(const std::vector<uint8_t> &data, key k, bool encrypt);

public:
    //Генерация пары ключей
    static std::pair<key, key> gen_keys(uint64_t p, uint64_t q);
    static std::string vecToString(std::vector<uint8_t> vector);
    static std::vector<uint8_t> encrypt(const std::string &data, key k);
    static std::vector<uint8_t> encrypt(const std::vector<uint8_t> &data, key k);
    static std::vector<uint8_t> decrypt(const std::vector<uint8_t> &data, key k);
};


#endif //SENT_CORE_TINY_RSA_H
