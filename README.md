# stegoBMP - BMP File Steganography Tool

## Overview
The stegoBMP command line tool is designed to encode and decode secret messages in BMP files using the least significant bit (LSB) technique. The program accepts command line arguments with flags such as -d, -e, -f, and -o, allowing users to specify the operation (encoding or decoding), input file, and output file.



## Digital Steganography
"Steganography is the practice of concealing a file, message, image, or video within another file, message, image, or video [Wikipedia](https://en.wikipedia.org/wiki/Steganography)". You can try stenography online through this [link](http://stylesuxx.github.io/steganography/).

![Steganography Example](https://miro.medium.com/max/519/0*rLECUN69yVKuX0nQ.png)
> Image Source: [The Threat of Digital Steganography-cloaked Malware to U.S. Critical infrastracture Systems](https://z3r0trust.medium.com/the-threat-of-digital-steganography-cloaked-malware-to-u-s-critical-infrastructure-systems-aff428db8ac7)

## Command Line Arguments
The stegoBMP tool supports the following command line flags:
-d: Decode mode.
-e: Encode mode.
-f <filename>: Specify the input BMP file.
-o <filename>: Specify the output BMP file.

## Implementation Details
The tool checks if the provided file is a valid 24-bit BMP file without encoding before performing any operations.
In encoding mode, the user is prompted to enter a message, which is then embedded into the image using the LSB technique.
The length of the message is stored in the metadata of the output file.
In decoding mode, the program reads the length of the hidden message from the metadata and displays the decoded message.
Additional extra credit options include specifying the number of bits used and supporting other non-compressed BMP file formats.
