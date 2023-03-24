#include "pch.h"
#include "RandomNumberGenerator.h"

using namespace drft::rng;

std::mt19937 RandomNumberGenerator::_rng(std::random_device{}());

void RandomNumberGenerator::setSeed(unsigned int seed)
{
	_rng.seed(seed);
}

double RandomNumberGenerator::generateInRange(double min, double max)
{
	static std::uniform_real_distribution<double> dist(min, max);
	return dist(_rng);
}

long drft::rng::RandomNumberGenerator::generateInRange(long min, long max)
{
	static std::uniform_int_distribution<long> dist(min, max);
	return dist(_rng);
}

unsigned int drft::rng::generateSeed()
{
	return static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}
