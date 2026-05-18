#include "random.h"
#include <stdlib.h>
#include <time.h>

static int initialized = 0;

void init_rand() {
    if (initialized)
        return;
    srand((unsigned int)time(NULL));
    initialized = 1;
}

void init_rand_with_seed(unsigned int seed) {
    if (initialized)
        return;
    srand(seed);
    initialized = 1;
}

int randint(int min, int max) {
    if (!initialized)
        init_rand();
    double scale = (double)rand() / (double)RAND_MAX;
    return min + (int)(scale * ((double)max - (double)min));
}
