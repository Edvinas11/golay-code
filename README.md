# Golay code

## Task Summary
This project implements the Golay code for encoding, transmitting, and decoding messages through a noisy channel. It supports both binary input and text input scenarios, demonstrating error detection and correction capabilities of the Golay code.

## Implemented Features
Implemented:

 - Binary message encoding and decoding
 - Text message encoding and decoding
 - Handling of noisy channels with adjustable error probabilities
 - Display of error statistics

Not implemented:
 - Image encoding and decoding

## Libraries Used
Standart C++ Libraries:
 - ```<iostream>```: for input and output.
 - ```<fstream>```: for file handling.
 - ```<bitset>```: for binary conversions.
 - ```<time.h>```: for random seed.
 - ```<string>```: for string handling.
 - ```<vector>```: for storing error positions.

No additional thirt-party libraries were used.

## Program Execution
The executable is ```golay.exe```

Usage:
```cmd
make golay.exe
./golay.exe
```

## Source Files Description
| File  | Description  |
|---|---|
| ```golay.h```  | Header file containing the class definition for Golay code.  |
| ```golay.cpp```  | Implementation of Golay encoding, transmission, and decoding.  |
| ```main.cpp```  | Main program logic, user interface, and simulation handling.  |