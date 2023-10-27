#pragma once
#include <ppm_io/ppm_io.h>
#include <vector>


class IntensityAnalysis {
	int totalIntensities{};
	int L{};
	std::vector<int> intensityTable{};
public:
	IntensityAnalysis(PPM& image);
	std::vector<int> GetIntensityTable();
	int GetL() { return L; }
	int GetTotal() { return totalIntensities; }
};