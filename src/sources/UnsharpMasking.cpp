#include "UnsharpMasking.h"
#include "Common.h"
#include <iostream>
#include "GaussianSmoothing.h"

UnsharpMasking::UnsharpMasking(std::filesystem::path p, std::vector<std::string> detail) :Operation(p)
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
}

bool UnsharpMasking::Operate()
{
	GaussianSmoothing blur{ currPath, {"-i",input,"-o",storage1, "-N", std::to_string(N), "-sigma", std::to_string(sigma)}};
	if (blur.Operate() == false) return false;
	SubOperation sub{ currPath,{"-i", input, storage1, "-o", output}};
	return sub.Operate();
}
