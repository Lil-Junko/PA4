#ifndef COMMANDARGS_H
#define COMMANDARGS_H
#include <string>
using namespace std;

/*
-d flag stands for decoding, 
-e flag stands for encoding, 
-f specifies the name of the input file, 
-o specifies the name of the output file. 
-f flag and -o flag must be followed by the file name.
*/
struct CommandArgs
{
	bool hasDflag;
	bool hadEflag;
	bool hasFflag;
	bool hasOflag;
	string fFilename;
	string oFilename;
};
//my dad refused to let me put the two header files together

#endif