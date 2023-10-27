#include "HistogramMatching.h"
#include <iostream>
#include "Common.h"
#include "HistogramEqualization.h"
#include <ppm_io/ppm_io.h>

void HistogramMatching::BuildMatchingTable(HistogramEqualization& in, HistogramEqualization& spec)
{
	std::vector<int> input_equalT{ in.GetEqualizingTable() };
	std::vector<int> spec_equalT{ spec.GetEqualizingTable() };
	int size = input_equalT.size();
	matchingTable.resize(size);
	int j{};
	for (int i = 0; i < size;++i) {
		int value = input_equalT[i];
		for (j; j < size;++j) {
			if (spec_equalT[j] == value) break;
			if (spec_equalT[j] > value) {
				if (j == 0) break;
				int diff{ spec_equalT[j]  - value};
				if (diff >= (value - spec_equalT[j - 1])) {
					--j;
				}
				break;
			}
		}
		matchingTable[i] = j;
	}
}

HistogramMatching::HistogramMatching(std::filesystem::path p, std::vector<std::string> detail):Operation(p)
{
	int size{ static_cast<int>(detail.size()) };
	for (int i = 0; i < size; ++i)
	{
		if (detail[i] == "-i") {
			input = detail[++i];
		}
		else if (detail[i] == "-s") {
			specification_image = detail[++i];
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
	HistogramEqualization input_histogram{ p, {"-i", input}};
	HistogramEqualization specification_histogram{ p, {"-i", specification_image}};
	BuildMatchingTable(input_histogram,specification_histogram);
}

bool HistogramMatching::Operate()
{
	PPM file{ input };
	int maxW{ file.getW() };
	int maxH{ file.getH() };
	int L{ file.getMax() };
	int maxL{ 255 };
	int size{ maxW * maxH * 3 };
	uint8_t* filebuffer = new uint8_t[size];
	uint8_t* inputfileBuffer = file.getImageHandler();
	for (int i = 0; i < size; ++i)
	{
		filebuffer[i] = matchingTable[inputfileBuffer[i]];
	}
	PPM out{ filebuffer, maxH,maxW,maxL,"P3" };
	if (out.write(output) == 1)return false;
	delete[] filebuffer;
	return true;
}
