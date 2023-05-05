#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

class Random
{
private:
    static std::mt19937 s_RandomEngine;
    static std::uniform_real_distribution<float> s_Distribution;
public:
    static void Init()
    {
        s_RandomEngine.seed(std::random_device()());
    }

    static float Float()
    {
        return s_Distribution(s_RandomEngine);
    }
};

#endif