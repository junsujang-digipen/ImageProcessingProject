#include "GaussianSmoothing.h"
#include <iostream>
#include "Common.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Filter.h"
#include "SpatialFilteringOperation.h"

GaussianSmoothing::GaussianSmoothing(std::filesystem::path p, std::vector<std::string> detail) :Operation(p)
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
		else if (detail[i] == "-sigma") {
			sigma = std::stof(detail[++i]);
		}
		else {
			std::cout << "unknown command" << std::endl;
		}
	}
	if (_strcmpi(input.c_str(), "curr") == false) {
		input = openedFile;
	}
	std::vector<float> filter{};
	for (int i = -N; i <= N; ++i)
	{
		for (int j = -N; j <= N; ++j)
		{
			float powVal{ -(i * i + j * j) / (2 * sigma * sigma) };
			float powed{std::powf((float)M_E, powVal)};
			float basic{ (1 / (2 * (float)M_PI * sigma * sigma)) };
			filter.push_back(basic * powed);
		}
	}
	Filter f{filter,N};
	f.save(filterName);
}

bool GaussianSmoothing::Operate()
{
	ConvolutionOperation gaussian{ currPath,{"-i",input,"-o",output, "-N", std::to_string(N) ,"-filter",filterName} };
	return gaussian.Operate();
}
