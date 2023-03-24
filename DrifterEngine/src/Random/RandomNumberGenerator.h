#pragma once

namespace drft::rng
{
	unsigned int generateSeed();

	class RandomNumberGenerator
	{
	public:
		static void setSeed(unsigned int seed);
		static double generateInRange(double min, double max);
		static long generateInRange(long min, long max);

	private:
		static std::mt19937 _rng;
	};
}



