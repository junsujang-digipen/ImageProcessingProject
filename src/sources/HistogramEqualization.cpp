#include "HistogramEqualization.h"
#include <iostream>
#include "IntensityAnalysis.h"
#include "Common.h"

void HistogramEqualization::GenEqualizingTable(IntensityAnalysis& IA)
{
	const int max{ IA.GetL() + 1 };
	equalizingTable.resize(max, 0);
	int accumulate{};
	for (int i = 0; i < max;++i) {
		accumulate += intensityTable[i];
		int equalizedIntensity{ (accumulate * IA.GetL()) / IA.GetTotal() };
		equalizingTable[i] = equalizedIntensity;
	}
}

HistogramEqualization::HistogramEqualization(std::filesystem::path p, std::vector<std::string> detail):Operation(p)
{
	int size{ static_cast<int>(detail.size()) };
	for (int i = 0; i < size; ++i)
	{
		if (detail[i] == "-i") {
			input = detail[++i];
		}
		else if (detail[i] == "-o") {
			output = detail[++i];
		}
		else {
			std::cout << "unknown command" << std::endl;
		}
	}
	if (_strcmpi(input.c_str(), "curr") == false) {
		input = openedFile;
	}
	PPM inputImage{input};
	IntensityAnalysis IATemp{ inputImage };
	intensityTable = IATemp.GetIntensityTable();
	GenEqualizingTable(IATemp);

}

bool HistogramEqualization::Operate()
{
	PPM file{ input };
	int maxW{file.getW()};
	int maxH{file.getH()};
	int L{ file.getMax() };
	int maxL{ 255 };
	int size{ maxW * maxH * 3 };
	uint8_t* filebuffer = new uint8_t[size];
	uint8_t* inputfileBuffer = file.getImageHandler();
	for (int i = 0; i < size; ++i)
	{
		filebuffer[i] = equalizingTable[inputfileBuffer[i]];
	}
	PPM out{ filebuffer, maxH,maxW,maxL,"P3" };
	if (out.write(output) == 1)return false;
	delete[] filebuffer;
	return true;
}
