#pragma once
#include <string>
#include "ImageProcessingOperation.h"

class SobelOperation : public Operation {
	std::string input{};
public:
	SobelOperation(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
};