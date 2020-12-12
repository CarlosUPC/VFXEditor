#pragma once

#include "Globals.h"
#include "PCG/include/pcg_variants.h"
#include "PCG/extras/entropy.h"

class Random 
{
public:
	static void StartRNGSeed();
	static float GetRandom();
	static int GetRandomBetween(int min, int max);
	static uint GenerateUUID();

private:
	static pcg32_random_t rng;
};

//pcg32_random_t Random::rng;
