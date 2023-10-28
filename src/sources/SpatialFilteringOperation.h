#pragma once
#include "ImageProcessingOperation.h"

class CorrelationOperation:public Operation {
	std::string input{};
	int N{};
	std::string filter{};
public:
	CorrelationOperation(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
};

class ConvolutionOperation: public Operation {
	std::string input{};
	int N{};
	std::string filter{};
public:
	ConvolutionOperation(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
};