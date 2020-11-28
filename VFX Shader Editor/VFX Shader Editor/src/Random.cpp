#include "Random.h"
#include <time.h>
#include <math.h>

void Random::StartRNGSeed()
{
	pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
}

float Random::GetRandom()
{
	return ldexp(pcg32_random_r(&rng), -32);
}

int Random::GetRandomBetween(int min, int max)
{
	return (min < max) ? pcg32_boundedrand_r(&rng, (max - min)) + min : (min == max) ? max : 0;
}

uint Random::GenerateUUID()
{
	return pcg32_random_r(&rng);
}
