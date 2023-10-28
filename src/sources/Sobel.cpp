#include "Sobel.h"
#include <iostream>
#include "Common.h"
#include <ppm_io/ppm_io.h>
#include "SpatialFilteringOperation.h"

SobelOperation::SobelOperation(std::filesystem::path p, std::vector<std::string> detail):Operation(p)
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
}

bool SobelOperation::Operate()
{
	ConvolutionOperation vert_cro{ currPath,{"-i",input,"-o",storage1, "-N","1","-filter","./assets/Filter/VerticalSobelFilter.txt"}};
	if (vert_cro.Operate() == false) return false;
	ConvolutionOperation horizontal_cro{ currPath,{"-i",input,"-o",storage2, "-N","1","-filter","./assets/Filter/HorizontalSobelFilter.txt"}};
	if (horizontal_cro.Operate() == false) return false;
	AddOperation add{ currPath,{"-i",storage1,storage2,"-o",output} };
	return add.Operate();
	//ConvolutionOperation vert_cro{ currPath,{"-i",input,"-o",output, "-N","1","-filter","./assets/Filter/MeanFilter.txt"} };
	//return vert_cro.Operate();
}
