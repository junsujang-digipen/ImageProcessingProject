#pragma once
#include <string>
#include <vector>

class Filter {
	std::vector<float> filter{};
	int N{};
public:
	Filter(std::string str, int n);
	Filter(std::vector<float> f, int n);
	void save(std::string str);
	float Get(int i, int j);
	std::vector<float>& GetFilter() { return filter; }
};