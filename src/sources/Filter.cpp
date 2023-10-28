#include "Filter.h"
#include <fstream>
#include <iostream>


Filter::Filter(std::string str, int n):N(n)
{
	std::ifstream infile(str);

	if (!infile.is_open())
	{
		std::cout << "Failed to open " << str << std::endl;
	}
	
	std::string text{};
	while (infile >> text) {
		filter.push_back(std::stof(text));
	}
	infile.close();
}

Filter::Filter(std::vector<float> f,int n):filter(f),N(n)
{
}

void Filter::save(std::string str)
{
	std::ofstream file{str};
	for (auto& t : filter) {
		file << t << " ";
	}
	file.close();
}

float Filter::Get(int i, int j)
{
	const int filterSize{ 2 * N + 1 };
	const int filterW{ (i + N) * filterSize };
	const int filterI{ filterW + j+N };
	return filter[filterI];
}
