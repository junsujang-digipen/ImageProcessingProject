#include "ImageProcessing.h"
#include <ppm_io/ppm_io.h>
#include <iostream>

void Resize(int currW, int currH, int targetW, int targetH, uint8_t* currBuffer, uint8_t* targetBuffer)
{
	float ratioH{ (float)currH / targetH };
	float ratioW{ (float)currW / targetW };
	for (int i = 0; i < targetH; ++i) {
		for (int j = 0; j < targetW; ++j) {
			int curri = i * ratioH;
			int currj = j * ratioW;
			targetBuffer[i * targetW * 3 + j * 3] = currBuffer[curri * currW * 3 + currj * 3];
			targetBuffer[i * targetW * 3 + j * 3 + 1] = currBuffer[curri * currW * 3 + currj * 3 + 1];
			targetBuffer[i * targetW * 3 + j * 3 + 2] = currBuffer[curri * currW * 3 + currj * 3 + 2];
		}
	}
}

float log_a_to_base_b(float a, float b)
{
	return log2f(a) / log2f(b);
}

ResizeOperation::ResizeOperation(std::filesystem::path p, std::vector<std::string> detail):Operation(p)
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
		else if (detail[i] == "-w") {
			newW = std::stoi(detail[++i]);
		}
		else if (detail[i] == "-h") {
			newH = std::stoi(detail[++i]);
		}
		else {
			std::cout << "unknown command" << std::endl;
		}
	}
}

bool ResizeOperation::Operate()
{
	PPM file1{ input };
	int size{ newW * newH * 3 };
	uint8_t* file1buffer = new uint8_t[size];
	Resize(file1.getW(), file1.getH(), newW, newH, file1.getImageHandler(), file1buffer);
	PPM out{ file1buffer, newW,newH,file1.getMax(),"P3"};
	if (out.write(output) == 1)return false;
	delete[] file1buffer;
	return true;
}

AddOperation::AddOperation(std::filesystem::path p, std::vector<std::string> detail) :Operation(p)
{
	int size{static_cast<int>(detail.size())};
	for (int i = 0; i < size; ++i)
	{
		if (detail[i] == "-i") {
			input1 = detail[++i];
			input2 = detail[++i];
		}
		else if(detail[i] == "-o") {
			output = detail[++i];
		}
		else {
			std::cout << "unknown command" << std::endl;
		}
	}
}

bool AddOperation::Operate()
{
	PPM file1{input1};
	PPM file2{input2};
	int maxW{std::max(file1.getW(),file2.getW())};
	int maxH{ std::max(file1.getH(),file2.getH())};
	int L{ file1.getMax() + file2.getMax()};
	int maxL{255};
	int size{ maxW * maxH * 3 };
	uint8_t* file1buffer = new uint8_t[size];
	uint8_t* file2buffer = new uint8_t[size];
	Resize(file1.getW(), file1.getH(), maxW,maxH, file1.getImageHandler(), file1buffer);
	Resize(file2.getW(), file2.getH(), maxW,maxH, file2.getImageHandler(), file2buffer);

	for (int i = 0; i < size; ++i)
	{
		file1buffer[i] =  (file1buffer[i] + file2buffer[i])* ((float)maxL / L);
	}
	PPM out{ file1buffer, maxH,maxW,maxL,"P3" };
	if (out.write(output) == 1)return false;
	delete[] file1buffer;
	delete[] file2buffer;
	return true;
}


SubOperation::SubOperation(std::filesystem::path p, std::vector<std::string> detail) :Operation(p)
{
	int size{ static_cast<int>(detail.size()) };
	for (int i = 0; i < size; ++i)
	{
		if (detail[i] == "-i") {
			input1 = detail[++i];
			input2 = detail[++i];
		}
		else if (detail[i] == "-o") {
			output = detail[++i];
		}
		else {
			std::cout << "unknown command" << std::endl;
		}
	}
}

bool SubOperation::Operate()
{
	PPM file1{ input1 };
	PPM file2{ input2 };
	int maxW{ std::max(file1.getW(),file2.getW()) };
	int maxH{ std::max(file1.getH(),file2.getH()) };
	int maxL{ std::max(file1.getMax(),file2.getMax()) };
	int size{ maxW * maxH * 3 };
	uint8_t* file1buffer = new uint8_t[size];
	uint8_t* file2buffer = new uint8_t[size];
	Resize(file1.getW(), file1.getH(), maxW, maxH, file1.getImageHandler(), file1buffer);
	Resize(file2.getW(), file2.getH(), maxW, maxH, file2.getImageHandler(), file2buffer);

	for (int i = 0; i < size; ++i)
	{
		file1buffer[i] = static_cast<uint8_t>(std::max((file1buffer[i] - file2buffer[i]),0));
	}
	PPM out{ file1buffer, maxH,maxW,maxL,"P3" };
	if (out.write(output) == 1)return false;
	delete[] file1buffer;
	delete[] file2buffer;
	return true;
}

MulOperation::MulOperation(std::filesystem::path p, std::vector<std::string> detail) :Operation(p)
{
	int size{ static_cast<int>(detail.size()) };
	for (int i = 0; i < size; ++i)
	{
		if (detail[i] == "-i") {
			input1 = detail[++i];
			input2 = detail[++i];
		}
		else if (detail[i] == "-o") {
			output = detail[++i];
		}
		else {
			std::cout << "unknown command" << std::endl;
		}
	}
}

bool MulOperation::Operate()
{
	PPM file1{ input1 };
	PPM file2{ input2 };
	int maxW{ std::max(file1.getW(),file2.getW()) };
	int maxH{ std::max(file1.getH(),file2.getH()) };
	int L{ file1.getMax() * file2.getMax() };
	int maxL{ 255 };
	int size{ maxW * maxH * 3 };
	uint8_t* file1buffer = new uint8_t[size];
	uint8_t* file2buffer = new uint8_t[size];
	Resize(file1.getW(), file1.getH(), maxW, maxH, file1.getImageHandler(), file1buffer);
	Resize(file2.getW(), file2.getH(), maxW, maxH, file2.getImageHandler(), file2buffer);

	for (int i = 0; i < size; ++i)
	{
		file1buffer[i] = static_cast<uint8_t>(((file1buffer[i]*file2buffer[i]) * ((float)maxL / L)));
	}
	PPM out{ file1buffer, maxH,maxW,maxL,"P3" };
	if (out.write(output) == 1)return false;
	delete[] file1buffer;
	delete[] file2buffer;
	return true;
}

InvOperation::InvOperation(std::filesystem::path p, std::vector<std::string> detail) :Operation(p)
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
}

bool InvOperation::Operate()
{
	PPM file1{ input };
	int maxW{ file1.getW() };
	int maxH{ file1.getH() };
	int maxL{ file1.getMax()};
	int size{ maxW * maxH * 3 };
	uint8_t* file1buffer = new uint8_t[size];

	for (int i = 0; i < size; ++i)
	{
		file1buffer[i] = (maxL - file1.getImageHandler()[i]);
	}
	PPM out{ file1buffer, maxH,maxW,maxL,"P3" };
	if (out.write(output) == 1)return false;
	delete[] file1buffer;
	return true;
}


LogOperation::LogOperation(std::filesystem::path p, std::vector<std::string> detail) :Operation(p)
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
		else if (detail[i] == "-c") {
			c = std::stof(detail[++i]);
		}
		else if (detail[i] == "-b") {
			b = std::stof(detail[++i]);
		}
		else {
			std::cout << "unknown command" << std::endl;
		}
	}
}

bool LogOperation::Operate() // ??
{
	PPM file1{ input };
	int maxW{ file1.getW() };
	int maxH{ file1.getH() };
	int maxL{ file1.getMax() };
	float loggedMax{ (float)file1.getMax() /(log_a_to_base_b(1.f + file1.getMax(),10.f)) };
	int size{ maxW * maxH * 3 };
	uint8_t* file1buffer = new uint8_t[size];
	for (int i = 0; i < size; ++i)
	{
		float logged{ (log_a_to_base_b(1.f + ((float)file1.getImageHandler()[i]),b)) };
		file1buffer[i] = (uint8_t)std::min((c * (logged)*loggedMax),(float)maxL);
	}
	PPM out{ file1buffer, maxH,maxW,maxL,"P3" };
	if (out.write(output) == 1)return false;
	delete[] file1buffer;
	return true;
}


PowOperation::PowOperation(std::filesystem::path p, std::vector<std::string> detail) :Operation(p)
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
		else if (detail[i] == "-c") {
			c = std::stof(detail[++i]);
		}
		else if (detail[i] == "-gamma") {
			gamma = std::stof(detail[++i]);
		}
		else {
			std::cout << "unknown command" << std::endl;
		}
	}
}

bool PowOperation::Operate()
{
	PPM file1{ input };
	int maxW{ file1.getW() };
	int maxH{ file1.getH() };
	int maxL{ file1.getMax() };
	int size{ maxW * maxH * 3 };
	uint8_t* file1buffer = new uint8_t[size];

	for (int i = 0; i < size; ++i)
	{
		float powed{ (c * powf(FLT_EPSILON + ((float)file1.getImageHandler()[i] / maxL),gamma)) };
		file1buffer[i] = (uint8_t)std::min((powed) *maxL, (float)maxL);
	}
	PPM out{ file1buffer, maxH,maxW,maxL,"P3" };
	if (out.write(output) == 1)return false;
	delete[] file1buffer;
	return true;
}

void CCLOperation::CCL(uint8_t* buffer, int w, int h)
{
}

CCLOperation::CCLOperation(std::filesystem::path p, std::vector<std::string> detail) :Operation(p)
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
		else if (detail[i] == "-t") {
			threshold = std::stof(detail[++i]);
		}
		else {
			std::cout << "unknown command" << std::endl;
		}
	}
}

bool CCLOperation::Operate()
{
	PPM file1{ input };
	int maxW{ file1.getW() };
	int maxH{ file1.getH() };
	int maxL{ file1.getMax() };
	int size{ maxW * maxH * 3 };
	uint8_t* file1buffer = new uint8_t[size];

	threshold *= maxL*3;

	for (int i = 0; i < size; i+=3)
	{
		int value{ file1.getImageHandler()[i] + file1.getImageHandler()[i + 1] + file1.getImageHandler()[i + 2] };
		int to_binary{ ((float)value <= threshold ? 0:1 ) };
		file1buffer[i] = (uint8_t)to_binary;
	}

	CCL(file1buffer,maxW,maxH);

	PPM out{ file1buffer, maxH,maxW,maxL,"P3" };
	if (out.write(output) == 1)return false;
	delete[] file1buffer;
	return true;
}
