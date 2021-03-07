//
// Created by Данил Федосеев on 07.03.2021.
//

#include "tiny_rsa.h"

//Расширенный алгоритм Евклида
int64_t tiny_rsa::gcdex(int64_t a, int64_t b, int64_t &x, int64_t &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    int64_t x1, y1;
    int64_t d = gcdex(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

//Обратное по модулю
int64_t tiny_rsa::invmod(int64_t a, int64_t m) {
    int64_t x, y;
    gcdex(a, m, x, y);
    x = (x % m + m) % m;
    return x;
}

//Квадрат числа
uint64_t tiny_rsa::sqr(uint64_t x) {
    return x * x;
}

//Быстрое возведение в степень
uint64_t tiny_rsa::binpow(uint64_t a, uint64_t e, uint64_t mod) {
    return e == 0 ? 1 : (e & 1U ? a * binpow(a, e - 1, mod) % mod : sqr(binpow(a, e / 2, mod)) % mod);
}

//Проверка на простое число
bool tiny_rsa::isPrime(int number) {
    for (int i = 2; i < number; i++)
        if (number % i == 0)
            return false;
    return true;
}

//Размер блока шифрования ключа
uint8_t tiny_rsa::get_chunk_size(key k) {
    return 32 - __builtin_clz(k.n);
}

//Резбиение/обьединение данных в блоки
std::vector<uint64_t> tiny_rsa::resize(const std::vector<uint64_t> &data, uint8_t in_size, uint8_t out_size) {
    std::vector<uint64_t> res;
    uint8_t done = 0;
    uint64_t cur = 0;
    for (uint64_t byte: data)
        for (uint8_t i = 0; i < in_size; i++) {
            cur = (cur << 1U) + (((uint64_t) byte & (1U << (uint64_t) (in_size - 1 - i))) != 0);
            done++;
            if (done == out_size) {
                done = 0;
                res.push_back(cur);
                cur = 0;
            }
        }
    //Дополнение нулями
    if (done != 0)
        res.push_back(cur << (uint64_t) (out_size - done));
    return res;
}

//(Де)шифрование
std::vector<uint8_t> tiny_rsa::process_bytes(const std::vector<uint8_t> &data, key k, bool encrypt) {
    std::vector<uint64_t> data_64(data.size());
    for (int i = 0; i < data.size(); i++)
        data_64[i] = (uint64_t) data[i];
    std::vector<uint64_t> resized_data = resize(data_64, 8, get_chunk_size(k) - encrypt); //Если мы шифруем, то размер блока K - 1, иначе K
    std::vector<uint64_t> encrypted_data(resized_data.size());
    for (int i = 0; i < resized_data.size(); i++)
        encrypted_data[i] = binpow(resized_data[i], k.e, k.n);
    std::vector<uint64_t> result_64 = resize(encrypted_data, get_chunk_size(k) - !encrypt, 8);
    std::vector<uint8_t> result(result_64.size());
    for (int i = 0; i < result_64.size(); i++)
        result[i] = (uint8_t) result_64[i];
    return result;
}

//Генерация пары ключей
std::pair<tiny_rsa::key, tiny_rsa::key> tiny_rsa::gen_keys(uint64_t p, uint64_t q) {
    uint64_t phi = (p - 1) * (q - 1);
    uint64_t n = p * q;
    //Простое число Мерсенна, используется в RSA для увеличения производительности
    uint64_t e = 65537;
    uint64_t d = invmod(e, phi);
    return {{e, n},
            {d, n}};
}

std::string tiny_rsa::vecToString(std::vector<uint8_t> vector){
    return std::string (vector.begin(), vector.end());
}

std::vector<uint8_t> tiny_rsa::encrypt(const std::string &data, key k){
    return process_bytes(std::vector<uint8_t> (data.begin(), data.end()), k, true);
}

std::vector<uint8_t> tiny_rsa::encrypt(const std::vector<uint8_t> &data, key k){
    return process_bytes(std::vector<uint8_t> (data.begin(), data.end()), k, true);
}

std::vector<uint8_t> tiny_rsa::decrypt(const std::vector<uint8_t> &data, key k){
    return process_bytes(std::vector<uint8_t> (data.begin(), data.end()), k, false);
}
