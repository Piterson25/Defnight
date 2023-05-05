#include "Random.h"

std::mt19937 Random::s_RandomEngine;
std::uniform_real_distribution<float> Random::s_Distribution(0.0f, 1.0f);
