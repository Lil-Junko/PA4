#include "StegoBMP.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <math.h>
#include <bitset>

using namespace std;

bool StegoBMP::ValidateArgs(int argc, char** argv)
{
    bool returnval = false;

    // Verify -d, -e, -f, and -o called flags.
    for (int counter = 0; counter < argc; counter++)
    {
        string temp = argv[counter];
        string delimiter = " ";
        string arg = temp.substr(0, temp.find(delimiter));

        if ((arg == "-d") ||
            (arg == "-e") ||
            (arg == "-f") ||
            (arg == "-o"))
        {
            returnval = true;
        }
    }

    return returnval;
}

void StegoBMP::PrintArgs()
{
    cout << "- d flag stands for decoding\n";
    cout << "- e flag stands for encoding\n";
    cout << "- f specifies the name of the input file\n";
    cout << "- o specifies the name of the output file\n";
    cout << "- f flag and -o flag must be followed by the file name.\n";
}

void StegoBMP::GetArgs(CommandArgs* args, int argc, char** argv)
{
    for (int counter = 0; counter < argc; counter++)
    {
        string temp = argv[counter];
        string delimiter = " ";
        size_t blank = temp.find(delimiter);
        string arg = temp.substr(0, blank);

        if (arg == "-d")
        {
            args->hasDflag = true;
        }
        if (arg == "-e")
        {
            args->hadEflag = true;
        }
        if (arg == "-f")
        {
            args->hasFflag = true;
            // grab next arg
            args->fFilename = argv[counter + 1];
            counter++;
        }
        if (arg == "-o")
        {
            args->hasOflag = true;
            // grab next arg
            args->oFilename = argv[counter + 1];
            counter++;
        }
    }
}

//reads messege
void StegoBMP::decrypt(string outputFile)
{
    fstream outfile(outputFile, ios::binary | ios::in | ios::out);
    if (!outfile)
    {
        cout << "Error opening the file.\n";
        exit(3);
    }

    // Verify file has a message
    int test;
    outfile.seekp(6, ios::beg);
    outfile.read(reinterpret_cast<char*>(&test), sizeof(test));
    if (test == 0)
    {
        cout << "file already has no encryption in it" << endl;
        exit(6);
    }

    // Get size of message  from 6 position
    outfile.seekg(6, ios::beg);
    uint8_t size;    
    outfile.read(reinterpret_cast<char*>(&size), 1);

    //starts at the first pixel to read
    outfile.seekg(54, ios::beg); 
    
    uint8_t pixel;
    char messege[size];
    // decoding pixel
    uint8_t decodePixelMask = 1;
    char decodedCharacter = 0;
    
    for (int i = 0; i < size; i++)
    {
        for(int i = 0; i < 8; i++)
        {
            // rebuilds characters from first bit to 8th bit
            outfile.read(reinterpret_cast<char *>(&pixel), 1);
            if (int(pixel & decodePixelMask) == 1)
            {
                decodedCharacter += (pixel & decodePixelMask) * (pow(2, i));
            }
        }
        // confirms char
        char temp = static_cast<char>(abs(int(decodedCharacter) + 1));
        // position character in the message
        messege[i] = temp;
        decodedCharacter = 0;
    }
    //outputs messege
    cout << "The encrypted messege is: ";
    for (int j = 0; j < size; j++)
    {
        cout << messege[j];
    }
    cout << endl;

    outfile.close();
}

void StegoBMP::encrypt (string outputFile, string inputFile)
{
    
    fstream file(inputFile, ios::binary | ios::in | ios::out);
    if (!file)
    {
        cout << "Error opening the input file.\n";
        exit(3);
    }

    fstream outfile(outputFile, ios::binary | ios::in | ios::out | ios::trunc);
    if (!outfile)
    {
        cout << "Error opening the output file.\n";
        exit(3);
    }

    uint8_t encodeMask = 254;
    // Copy file to output file
    std::ofstream(outputFile, std::ios::binary) << std::ifstream(inputFile, std::ios::binary).rdbuf();

    // check if first two byes is "BM"
    uint8_t magic[2];
    outfile.read(reinterpret_cast<char*>(magic), sizeof(magic));
    if (magic[0] != 0x42 || magic[1] != 0x4D)
    {
        cout << "It is not a bmp file. Please use the correct file\n";
        exit(5);
    }
    else
    {
        // check if no compression is used
        // jump to position 30 =0x1E
        outfile.seekg(0x1E, ios::beg);
        uint32_t compression;
        outfile.read(reinterpret_cast<char*>(&compression), 4);
        if (compression != 0)
        {
            cout << "I can't use compressed BMP file. Please choose non compressed\n";
            exit(6);
        }
    }

    int beginOfPixelArray;
    int fileSize;
    // jump to the 10th byte
    outfile.seekg(10, ios::beg);
    outfile.read(reinterpret_cast<char*>(&beginOfPixelArray), 4);
    outfile.seekg(2, ios::beg);
    outfile.read(reinterpret_cast<char*>(&fileSize), 4);
    //cout << "The pixel array begins at byte " << beginOfPixelArray
    //<< " = 0x" << hex << beginOfPixelArray << endl;

    int test;
    file.seekp(6, ios::beg);
    file.read(reinterpret_cast<char*>(&test), sizeof(test));
    if (test != 0)
    {
        cout << "file already has a encryption in it" << endl;
        exit(6);
    }

    uint32_t numberOfCharacters = (fileSize - beginOfPixelArray) / 8;
    cout << "the total number of letters you can enter is: " << numberOfCharacters << endl;

        // Get user input
    string message = "";
    cout << "enter a messege you want to encrypt: ";
    getline(cin,message);
    if (message.length() == 0 || message.length() > numberOfCharacters)
    {
        cout << "You did not enter a proper messege";
        exit(7);
    }

    uint16_t lengthTemp = message.length();
    outfile.seekp(6, ios::beg);
    outfile.write(reinterpret_cast<char*>(&lengthTemp), sizeof(lengthTemp));

    uint8_t temp;
    int lsb = 0;
    file.seekp(54, ios::beg);
    outfile.seekp(54, ios::beg); //starts at the first pixel to change
    for (int i = 0; i <int(message.size()); i++)
    {
        char msgChar = message[i]; //each letter of the message
        for (int i = 0; i < 8; i++)
        {
            file.read(reinterpret_cast<char*>(&temp), sizeof(temp));
            lsb = msgChar & 1; //gets last bit of messege
            //tricky part
            temp = temp&encodeMask;
            if (lsb == 0)
            {
            temp += 1;
            }
            //goes back 1 space to teplace the temp just grabbed
            outfile.write(reinterpret_cast<char*>(&temp), sizeof(temp));
            msgChar = msgChar >> 1; //shifts message 1 then repeats
        }
    }
    outfile.close();
    file.close();
}