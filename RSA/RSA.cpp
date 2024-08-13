#include "RSA.h"
#include <iostream>

#define PRIME_LEN1 512
#define PRIME_LEN2 1024
Key RSA(int key_len) {
    ZZ p, q;
    int l;
    try {
        if (key_len != PRIME_LEN1 && key_len != PRIME_LEN2)
            throw std::string("Key length is not supported");
        else
            l = key_len;
    }
    catch (std::string e) {
        std::cout << e << std::endl;
    }

    p = GenPrime_ZZ(l);  // Generate a prime p with bit length l
    do {
        q = GenPrime_ZZ(l);  // Ensure q is not equal to p
    } while (p == q);

    ZZ phi = (p - 1) * (q - 1);

    ZZ a, b, d;
    do {
        RandomBnd(b, phi);
        GCD(d, b, phi);
    } while (b <= 1 || d != 1);

    InvMod(a, b, phi);
    ZZ n = p * q;
    Public_key pub = { n, b };
    Private_key pri = { p, q, a };
    Key res = { pub, pri };
    return res;
}

ZZ RSA_En(Public_key pub, ZZ x) {
    return PowerMod(x, pub.b, pub.n);
}

ZZ RSA_De(Private_key pri, Public_key pub, ZZ y) {
    return PowerMod(y, pri.a, pub.n);
}
