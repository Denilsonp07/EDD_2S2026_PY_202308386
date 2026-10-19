#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

namespace {
const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b,
    0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01,
    0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7,
    0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152,
    0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc,
    0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819,
    0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08,
    0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f,
    0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

uint32_t rotar(uint32_t valor, uint32_t bits) {
    return (valor >> bits) | (valor << (32 - bits));
}
uint32_t elegir(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}
uint32_t mayoritaria(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}
}

string sha256(const string& entrada) {
    uint64_t bits = static_cast<uint64_t>(entrada.size()) * 8;
    size_t longitud = entrada.size() + 1;
    while ((longitud % 64) != 56) ++longitud;
    string datos(longitud + 8, '\0');
    for (size_t i = 0; i < entrada.size(); ++i) datos[i] = entrada[i];
    datos[entrada.size()] = static_cast<char>(0x80);
    for (int i = 0; i < 8; ++i) {
        datos[longitud + i] =
            static_cast<char>((bits >> (56 - i * 8)) & 0xff);
    }

    uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                     0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    for (size_t bloque = 0; bloque < datos.size(); bloque += 64) {
        uint32_t w[64];
        for (int i = 0; i < 16; ++i) {
            size_t p = bloque + static_cast<size_t>(i) * 4;
            w[i] = (static_cast<uint32_t>(
                        static_cast<unsigned char>(datos[p])) << 24) |
                   (static_cast<uint32_t>(
                        static_cast<unsigned char>(datos[p + 1])) << 16) |
                   (static_cast<uint32_t>(
                        static_cast<unsigned char>(datos[p + 2])) << 8) |
                   static_cast<uint32_t>(
                       static_cast<unsigned char>(datos[p + 3]));
        }
        for (int i = 16; i < 64; ++i) {
            uint32_t s0 = rotar(w[i - 15], 7) ^ rotar(w[i - 15], 18) ^
                          (w[i - 15] >> 3);
            uint32_t s1 = rotar(w[i - 2], 17) ^ rotar(w[i - 2], 19) ^
                          (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
        uint32_t e = h[4], f = h[5], g = h[6], x = h[7];
        for (int i = 0; i < 64; ++i) {
            uint32_t s1 = rotar(e, 6) ^ rotar(e, 11) ^ rotar(e, 25);
            uint32_t temporal1 = x + s1 + elegir(e, f, g) + k[i] + w[i];
            uint32_t s0 = rotar(a, 2) ^ rotar(a, 13) ^ rotar(a, 22);
            uint32_t temporal2 = s0 + mayoritaria(a, b, c);
            x = g; g = f; f = e; e = d + temporal1;
            d = c; c = b; b = a; a = temporal1 + temporal2;
        }
        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += x;
    }
    ostringstream salida;
    salida << hex << setfill('0');
    for (uint32_t valor : h) salida << setw(8) << valor;
    return salida.str();
}
