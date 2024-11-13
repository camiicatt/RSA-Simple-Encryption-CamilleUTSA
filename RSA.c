#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int isPrime(unsigned long num) {
    if (num <= 1) return 0;
    for (unsigned long i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

// calculate gcd(a, b) using the Euclidean algorithm
unsigned long gcd(unsigned long a, unsigned long b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}
// find a suitable e
unsigned long findE(unsigned long phiN) {
    for (unsigned long e = 2; e < phiN; e++) { // start from 2, since e cannot be 1
        if (gcd(e, phiN) == 1) {
            return e;
        }
    }
    return 0; // no valid e found
}
// calculate a^b mod m using modular exponentiation
unsigned long modpow(unsigned long a, unsigned long b, unsigned long m) {
    unsigned long result = 1;
    a %= m; // Reduce a modulo m initially
    while (b > 0) {
        if (b & 1) {
            result = (result * a) % m;
        }
        a = (a * a) % m;
        b >>= 1;
    }
    return result;
}

unsigned long modInverse(unsigned long e, unsigned long phiN) {
    long t = 0, newT = 1;
    long r = phiN, newR = e;

    while (newR != 0) {
        unsigned long quotient = r / newR;
        long tempT = t;
        t = newT;
        newT = tempT - quotient * newT;

        long tempR = r;
        r = newR;
        newR = tempR - quotient * newR;
    }

    if (r > 1) {
        return 0; // there's no modular inverse for this
    }
    if (t < 0) {
        t += phiN;
    }
    return t;
}

int getPrimes(unsigned long *p, unsigned long *q) {
    while (1) {
        printf("Enter two prime numbers p and q ≽^•⩊•^≼ or (2 2 to exit): ");
        scanf("%lu %lu", p, q);

        if (*p == 2 && *q == 2) {
            printf("Exiting :p\n");
            return 1; 
        }

        // Validate that both p and q are prime
        if (!isPrime(*p) || !isPrime(*q) || *p <= 1 || *q <= 1 || *p == *q) {
            printf("Both numbers must be prime and/or different. Please try again if you want!!\n");
        } else {
            return 0; 
        }
    }
}

int generateKeys(unsigned long p, unsigned long q, unsigned long *n, unsigned long *phiN, unsigned long *e, unsigned long *d) {
    unsigned long userInput;

    *n = p * q;
    *phiN = (p - 1) * (q - 1);

    printf("-----------------------------------------\n");
    printf("If you want to choose e then enter value now (if you want it to be 7 (or any number) make sure it fits the numbers you choose or else the function won't work, also the prime numbers chosen might not work),\n OR type 0 and e will be calculated for you always working :> : ");
    scanf("%lu", &userInput);

    if (userInput == 0) {
        *e = findE(*phiN); // Automatically find a valid e
    } else {
        if (userInput > 1 && userInput < *phiN && gcd(userInput, *phiN) == 1) {
            *e = userInput; // Use user input if valid
        } else {
            printf("Invalid value of e provided. Please try again.\n");
            return 1; 
        }
    }

    // Calculate d (modular inverse of e)
    *d = modInverse(*e, *phiN);
    if (*d == 0) {
        printf("Error: Modular inverse for e=%lu could not be calculated. Ensure valid primes are chosen.\n", *e);
        return 1;
    }

    // Ensure e and d are not the same
    if (*e == *d) {
        printf("Error: e (%lu) and d (%lu) cannot be the same. If you did not choose e & d to be the same then d was equal to e after attemp to generate keys. Choose new values...\n", *e, *d);
        return 1;
    }

    return 0;
}

int main() {
    unsigned long p, q, e, n, d, phiN = 0;

    if (getPrimes(&p, &q) == 1) {
        exit(0); 
    }

    while (generateKeys(p, q, &n, &phiN, &e, &d) == 1) {
        if (getPrimes(&p, &q) == 1) {
            exit(0);
        }
    }

    printf("Public key (e, n): (%lu, %lu)\n", e, n);
    printf("Private key (d, n): (%lu, %lu)\n", d, n);

    unsigned long plaintext;

    do {
        printf("Enter an integer between 1 and %lu as plaintext to encrypt: ", n - 1);
        scanf("%lu", &plaintext);

        if (plaintext <= 0 || plaintext >= n) {
            printf("Error: Plaintext must be greater than 0 and less than %lu. Please try again please.\n", n);
        }
    } while (plaintext <= 0 || plaintext >= n);

    unsigned long ciphertext = modpow(plaintext, e, n); // modpow used for Decryption and Encryption 
    printf("Encrypted ciphertext ૮ ˶ᵔ ᵕ ᵔ˶ ა : %lu\n", ciphertext);

    unsigned long decrypted = modpow(ciphertext, d, n);
    printf("Decrypted plaintext (˵ •̀ ᴗ - ˵ ) ✧: %lu\n\n", decrypted);
    printf("˚　　　　✦　　　.　　. 　 ˚　.　　　　　 . ✦　　　 　˚　　　　 . ★⋆.\n");
    printf("　　　.   　　˚　✭　 　　*　　 　　✦　　　.　　.　　　✦　˚ 　　　　 ˚　.˚　　　　　✭　.　　. 　 ˚　.　　　　 　　 　　　　 ✦\n");

    printf("  ∧,,,∧\n( • · • )\n/ づ♡ all done thank you!\n");

    return 0;
}