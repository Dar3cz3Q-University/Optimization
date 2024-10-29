#pragma once

class RandomNumberGenerator
{
public:
	RandomNumberGenerator(const RandomNumberGenerator&) = delete;

	static RandomNumberGenerator& Get() { return s_Instance; }

	double Double(double min, double max)
	{
		std::uniform_real_distribution<> distribute(min, max);
		return distribute(m_Generator);
	}

private:
	RandomNumberGenerator() : m_Generator(m_RandomDevice()) {};

	static RandomNumberGenerator s_Instance;

	std::random_device m_RandomDevice;
	std::mt19937 m_Generator;
};

