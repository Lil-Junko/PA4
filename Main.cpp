#include <iostream>
#include "CommandArgs.h"
#include "StegoBMP.h"

using namespace std;

int main(int argc, char* argv[])
{
    CommandArgs* _args = new CommandArgs();
    StegoBMP stegoBMP;

    // Check if args are correct
    if (argc >= 2)
    {
        // Check if we have valid command line args
        if (!stegoBMP.ValidateArgs(argc, argv))
        {
            cout << "Invalid arguments:\n" << endl;
            stegoBMP.PrintArgs();
            exit(1);
        }
        else
        {
            // Get the args from the command line
            stegoBMP.GetArgs(_args, argc, argv);
        }
    }
    else
    {
        cout << "Must provide one of the following command line options:\n" << endl;
        stegoBMP.PrintArgs();
        exit(2);
    }
    
    if (_args->hasFflag)
    {
        if(_args->fFilename.length() == 0)
        {
            cout << "Please enter a file for input";
            exit(4);
        }
    }

    if (_args->hasOflag)
    {
        if(_args->oFilename.length() == 0)
        {
            cout << "Please enter a file for output";
            exit(4);
        }
    }

    if (_args->hadEflag)
    {
        stegoBMP.encrypt (_args->oFilename, _args->fFilename);
    }
    // Decrypt file
    if (_args->hasDflag)
    {
        stegoBMP.decrypt(_args->oFilename);
    }

    return 0;
}