// StateMachine.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include "StateMachineClass.h"

void LoadText(const std::string& fileName, std::string& text)
{
	std::cout << "Loading text ..." << std::endl;

	std::ifstream file(fileName.c_str());
	if (!file)
	{
		std::cerr << "Can't open file " << fileName << '\n';
		std::cin.get();
		exit(-1);
	}

	std::copy(std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>(), std::back_inserter(text));

	std::cout << "Text length: " << text.length() << " bytes" << std::endl;
	std::cout << "" << std::endl;
}

int main(int argc, char* argv[])
{
	if ((argc < 2) || (argc < 3 && argv[1] == "-f" || (argc > 3)))
	{
		std::cerr << "Usage: " << argv[0] << " TEXT" << std::endl;
		std::cerr << "or: " << argv[0] << " -f <filename>" << std::endl;
		std::cin.get();
		return 1;
	}
	StateMachineClass NewMachine;
	std::string text;
	std::string par = argv[1];
	if (par == "-f")
	{
		LoadText(argv[2], text);
	}
	else
	{
		text = argv[1];
	}
	NewMachine.Process(text);
	std::cout << "Words count: " << NewMachine.Words() << std::endl;
	std::cout << "Spaces count: " << NewMachine.Spaces() << std::endl;
	std::cout << "Charecters count: " << NewMachine.Charecters() << std::endl;
	std::cout << "Numbers count: " << NewMachine.Numbers() << std::endl;
	std::cin.get();
	return 0;
}

