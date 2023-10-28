#pragma once
#include "ImageProcessingOperation.h"

class UnsharpMasking : public Operation {
	std::string input{};
	int N{2};
	float sigma{1.f};
public:
	UnsharpMasking(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
};