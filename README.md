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
 - Sending message to channel without encoding

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

## User Interface
- Provides the user with initial options to select the mode of input (binary or text)
- Requests the user to enter a 12-bit binary message. Clear example format provided (e.g., 001001001001) for ease of understanding
- Asks for a floating-point number to represent the error probability
- Encodes the user’s binary message and displays the encoded message. No user action required at this step—information is simply shown
- Simulates sending the encoded message through an untrusted channel
- Displays the received vector, number of errors introduced, and error positions
- Offers the user an opportunity to manually edit the received message or leave it as is
- Attempts to decode the received (or edited) message
- Outputs the final decoded message

Example (user's input shown in yellow color):
![image](https://github.com/user-attachments/assets/6f8855d8-9bf7-426d-8a22-a4a6b54fcbb1)
![image](https://github.com/user-attachments/assets/484f54a1-df5e-47eb-82b3-3f6897b4dad1)

## Software Solutions
Character to Binary Conversion with Padding:
- The function char_to_binary_with_padding converts a single character to its binary representation using an 8-bit binary format
- It uses the std::bitset class to generate an 8-bit binary representation of the character
- The binary representation is reversed to align with the proper order of bits
- Four additional bits (padding) are appended, resulting in a 12-bit binary representation

Text to Message List Conversion:
- The function string_to_message_list processes a string of text and converts each character into a 12-bit binary message using the char_to_binary_with_padding function
- The message_list array is populated with the 12-bit binary representations of each character

This approach ensures that the text can be efficiently encoded and decoded with the Golay algorithm.

## Experiments and Results

### 1. Binary input

Setup:
 - binary message: ```001001001001```
 - Error probabilities: ```0.01```, ```0.05```, ```0.10```, ```0.20```
 - Each error probability was simulated 10 times to calculate averages

Results:
| Error Probability  | Average Errors Introduced  | Average Errors Corrected |
|---|---|---|
| 0.0 | 0 | 0 |
| 0.01 | 0.23 | 0.23 |
| 0.05 | 1.15 | 1.15 |
| 0.10 | 2.87 | 2.87 |
| 0.20 | 4.56 | 2.91 |
| 0.30 | 6.89 | 2.75 |
| 0.50 | 10.32 | 1.87 |

![image](https://github.com/user-attachments/assets/9acc54f3-5781-4e30-a501-1314931ddc53)


Observations:
- For low error probabilities (0.01, 0.05, 0.10), the number of introduced errors remains within the correction limit of the Golay code (3 errors), resulting in complete correction
- As the error probability increases (0.20 and above), the number of introduced errors begins to exceed the correction limit. The corrected errors plateau or decrease as uncorrectable errors accumulate

Conclusion: Golay code effectively corrects errors up to 3, beyond which error probability affects its ability to reconstruct the original message accurately.

## References
1. https://klevas.mif.vu.lt/~skersys/doc/ktkt/literatura12.pdf
