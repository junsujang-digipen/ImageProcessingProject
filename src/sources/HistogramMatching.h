#pragma once
#include "ImageProcessingOperation.h"
class HistogramEqualization;
class HistogramMatching : public Operation {
	std::string input{};
	std::string specification_image{};
	std::vector<int> matchingTable{};
	void BuildMatchingTable(HistogramEqualization& in, HistogramEqualization& spec);
public:
	HistogramMatching(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
};