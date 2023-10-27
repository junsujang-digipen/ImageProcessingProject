#include "IntensityAnalysis.h"

IntensityAnalysis::IntensityAnalysis(PPM& image)
{
	L = image.getMax();
	intensityTable.resize(L+1,0);
	const int H = image.getH();
	const int W = image.getW();
	totalIntensities = H * W * 3;
	uint8_t* bytes = image.getImageHandler();
	for (int i = 0; i < totalIntensities; ++i) {
		// HSV?
		// RGB? currently
		++intensityTable[bytes[i]];
	}

}

std::vector<int> IntensityAnalysis::GetIntensityTable()
{
	return intensityTable;
}
