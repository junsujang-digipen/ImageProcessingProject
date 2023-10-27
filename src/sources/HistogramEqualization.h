#pragma once
#include "ImageProcessingOperation.h"
class IntensityAnalysis;
class HistogramEqualization : public Operation {
	std::string input{};
	std::vector<int> intensityTable{};
	std::vector<int> equalizingTable{};

	void GenEqualizingTable(IntensityAnalysis& IA);

public:
	HistogramEqualization(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
	std::vector<int> GetIntensityTable() { return intensityTable; }
	std::vector<int> GetEqualizingTable() { return equalizingTable; }
};