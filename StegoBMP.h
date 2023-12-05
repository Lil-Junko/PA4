#ifndef STEGOBMP_H
#define STEGOBMP_H
#include "CommandArgs.h"

class StegoBMP
{
public:
	bool ValidateArgs(int argc, char** argv);
	void PrintArgs();
	void GetArgs(CommandArgs *args, int argc, char** argv);
	void encrypt (string outputFile, string inputFile);
	void decrypt(string outputFile);
};
//my dad refused to let me put the two header files together

#endif