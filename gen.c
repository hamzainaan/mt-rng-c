#include <stdint.h>
#include <stdio.h>

static int amount = 10; //change this value.
static unsigned long seed = 5489UL; //seed, change it.

typedef struct {
    uint32_t mt[624];
    int index;
} RNG;

void Twist(RNG* rng) {
    for (int i = 0; i < 624; i++) {
        uint32_t y = (rng->mt[i] & 0x80000000UL) | (rng->mt[(i + 1) % 624] & 0x7fffffffUL);
        rng->mt[i] = rng->mt[(i + 397) % 624] ^ (y >> 1);
        if (y % 2 != 0) {
            rng->mt[i] ^= 0x9908b0dfUL;
        }
    }
    rng->index = 0;
}

uint32_t RandomUInt32(RNG* rng) {
    if (rng->index >= 624) {
        Twist(rng);
    }
    uint32_t y = rng->mt[rng->index++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);
    return y;
}

void SeedRNG(RNG* rng, uint32_t seed) {
    rng->mt[0] = seed;
    for (int i = 1; i < 624; i++) {
        rng->mt[i] = (1812433253UL * (rng->mt[i - 1] ^ (rng->mt[i - 1] >> 30)) + i);
    }
    rng->index = 624;
}

int main(int argc, char* argv[]) {
    RNG rng;
    SeedRNG(&rng, seed);

    for (int i = 0; i < amount; i++) {
        printf("%d. number: %u\n", i+1, RandomUInt32(&rng));
    }

    return 0;
}
