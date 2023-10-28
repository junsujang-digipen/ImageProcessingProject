#pragma once
#include <string>
#include "ImageProcessingOperation.h"

class GaussianSmoothing : public Operation {
	std::string input{};
	int N{2};
	float sigma{1.f};
	std::string filterName{ "./Assets/Filter/GaussianSmoothFilter.txt" };
public:
	GaussianSmoothing(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
};