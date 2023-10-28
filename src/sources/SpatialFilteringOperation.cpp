#include "SpatialFilteringOperation.h"
#include <iostream>
#include "Common.h"
#include "Filter.h"
#include <ppm_io/ppm_io.h>

int CutEdge(int value, int min, int max) {
	value = std::max(value, min);	
	value = std::min(value, max);
	return value;
}

CorrelationOperation::CorrelationOperation(std::filesystem::path p, std::vector<std::string> detail):Operation(p)
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
		else if (detail[i] == "-N") {
			N = std::stoi(detail[++i]);
		}
		else if (detail[i] == "-filter") {
			filter = detail[++i];
		}
		else {
			std::cout << "unknown command" << std::endl;
		}
	}
	if (_strcmpi(input.c_str(), "curr") == false) {
		input = openedFile;
	}
}

bool CorrelationOperation::Operate()
{
	Filter f{filter,N};
	PPM file{ input };
	int maxW{ file.getW() };
	int maxH{ file.getH() };
	int maxL{ file.getMax() };
	int size{ maxW * maxH * 3 };
	uint8_t* file1buffer = new uint8_t[size]();
	int width{3*maxW};
	for (int h = 0; h < maxH; ++h){
		int stPoint{ h * width };
		for (int w = 0; w < maxW; ++w) {
			int currPoint{ stPoint + w * 3 };
			for (int i = -N; i <= N; ++i)
			{
				int sh{ CutEdge(h+i,0,maxH-1)};
				int shStPoint{sh*width};
				for (int j = -N; j <= N; ++j)
				{
					int sw{ CutEdge(w + j,0,maxW-1) };
					int currIdx{ shStPoint + sw*3};
					float value{ f.Get(i, j) };
					file1buffer[currPoint] += value * file.getImageHandler()[currIdx];
					file1buffer[currPoint+1] += value * file.getImageHandler()[currIdx+1];
					file1buffer[currPoint+2] += value * file.getImageHandler()[currIdx+2];
				}
			}
			//file1buffer[currPoint] = CutEdge(file1buffer[currPoint],0, maxL);
			//file1buffer[currPoint + 1] = CutEdge(file1buffer[currPoint + 1], 0, maxL);
			//file1buffer[currPoint + 2] = CutEdge(file1buffer[currPoint + 2], 0, maxL);
		}
	}
	PPM out{ file1buffer, maxH,maxW,maxL,"P3" };
	if (out.write(output) == 1)return false;
	delete[] file1buffer;
	return true;
}

ConvolutionOperation::ConvolutionOperation(std::filesystem::path p, std::vector<std::string> detail) :Operation(p)
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
		else if (detail[i] == "-N") {
			N = std::stoi(detail[++i]);
		}
		else if (detail[i] == "-filter") {
			filter = detail[++i];
		}
		else {
			std::cout << "unknown command" << std::endl;
		}
	}
	if (_strcmpi(input.c_str(), "curr") == false) {
		input = openedFile;
	}


}

bool ConvolutionOperation::Operate()
{
	Filter f{ filter,N };
	PPM file{ input };
	int maxW{ file.getW() };
	int maxH{ file.getH() };
	int maxL{ file.getMax() };
	int size{ maxW * maxH * 3 };
	uint8_t* file1buffer = new uint8_t[size]();
	int width{ 3 * maxW };
	for (int h = 0; h < maxH; ++h) {
		int stPoint{ h * width };
		for (int w = 0; w < maxW; ++w) {
			int currPoint{ stPoint + w * 3 };
			for (int i = -N; i <= N; ++i)
			{
				int sh{ CutEdge(h - i,0,maxH - 1) };
				int shStPoint{ sh * width };
				for (int j = -N; j <= N; ++j)
				{
					int sw{ CutEdge(w - j,0,maxW - 1) };
					int currIdx{ shStPoint + sw * 3 };
					float value{ f.Get(i, j) };
					file1buffer[currPoint] += value * file.getImageHandler()[currIdx];
					file1buffer[currPoint + 1] += value * file.getImageHandler()[currIdx + 1];
					file1buffer[currPoint + 2] += value * file.getImageHandler()[currIdx + 2];
				}
			}
			//file1buffer[currPoint] = CutEdge(file1buffer[currPoint], 0, maxL);
			//file1buffer[currPoint + 1] = CutEdge(file1buffer[currPoint + 1], 0, maxL);
			//file1buffer[currPoint + 2] = CutEdge(file1buffer[currPoint + 2], 0, maxL);
		}
	}
	PPM out{ file1buffer, maxH,maxW,maxL,"P3" };
	if (out.write(output) == 1)return false;
	delete[] file1buffer;
	return true;
}
