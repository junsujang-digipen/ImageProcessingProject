#pragma once
#include <vector>
#include <string>
#include <filesystem>

void Resize(int currW, int currH, int targetW, int targetH, uint8_t* currBuffer, uint8_t* targetBuffer);

class Operation {
protected:
public:
	Operation(std::filesystem::path p) :currPath(p) {}
	std::filesystem::path currPath{};
	std::string output{"output.ppm"};
	virtual bool Operate() { return true; }
	std::string OutputFile() { return output; }
};

class ResizeOperation : public Operation {
	int newW{255};
	int newH{255};
	std::string input{};
public:
	ResizeOperation(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
};
class AddOperation : public Operation{
	std::string input1{};
	std::string input2{};
public:
	AddOperation(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
};

class SubOperation : public Operation {
	std::string input1{};
	std::string input2{};
public:
	SubOperation(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
}; 

class MulOperation : public Operation {
	std::string input1{};
	std::string input2{};
public:
	MulOperation(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
}; 

class InvOperation : public Operation {
	std::string input{};
public:
	InvOperation(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
};

class LogOperation : public Operation {
	float c{1.f};
	float b{10.f};
	std::string input{};
public:
	LogOperation(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
};

class PowOperation : public Operation {
	float c{1.f};
	float gamma{1.f};
	std::string input{};
public:
	PowOperation(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
};

class CCLOperation : public Operation {
	float threshold{0.5f};
	std::string input{};
	void CCL(uint8_t* buffer,int w,int h);
public:
	CCLOperation(std::filesystem::path p, std::vector<std::string> detail);
	bool Operate()override;
};