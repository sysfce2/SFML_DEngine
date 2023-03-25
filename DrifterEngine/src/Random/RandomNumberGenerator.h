#pragma once

namespace drft::rng
{
	unsigned int generateSeed();

	class RandomNumberGenerator
	{
	public:
		static void setSeed(unsigned int seed);
		static double realInRange(double min, double max);
		static int intInRange(int min, int max);

	private:
		static std::mt19937 _rng;
	};
}



