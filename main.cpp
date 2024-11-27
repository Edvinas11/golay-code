#include "golay.h"
#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

void char_to_binary_with_padding(char ch, int binary[12]) {
    bitset<8> bits(ch); // Convert character to 8-bit binary

    // Fill the first 8 bits of the array with the binary representation of the character
    for (int i = 0; i < 8; ++i) {
        binary[i] = bits[7 - i]; // Reverse bitset order for proper binary representation
    }

    // Add 4 padding bits
    for (int i = 8; i < 12; ++i) {
        binary[i] = 0;
    }
}

void string_to_message_list(const string& inputText, int messageList[][12], int& messageCount) {
    messageCount = inputText.length(); // Number of 12-bit messages corresponds to the string length

    for (int i = 0; i < messageCount; ++i) {
        char_to_binary_with_padding(inputText[i], messageList[i]);
    }
}

int main() {
    Golay myGolay;
    int scenarioNr;

    cout << "Choose simulation mode:" << endl;
    cout << "1. Binary input" << endl;
    cout << "2. Text input" << endl;
    cout << "Enter your choice (1 or 2): ";
    cin >> scenarioNr;

    if (scenarioNr == 1) {
        cout << "Binary input selected." << endl;
        int message[12];

        myGolay.get_message_input(message);
        cout << "Message: ";
        for (int i = 0; i < 12; ++i) {
            cout << message[i];
        }
        cout << endl;

        cout << "Encoding..." << endl;
        myGolay.encode(message);

        cout << "Encoded message: ";
        myGolay.print_encoded_message();

        double p = myGolay.get_input_probability();
        cout << "Sending encoded message through untrusted channel with error probability " << p << "..." << endl;
        myGolay.send_through_channel(p);

        myGolay.edit_received_message();

        cout << "Trying to decode the received message..." << endl;
        myGolay.decode();
    }
    else if (scenarioNr == 2) {
        int messageList[100][12];
        int messageCount = 0;
        cout << "Text input selected." << endl;

        string filename;
        cout << "Enter the filename containing the text: ";
        cin >> filename;

        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open the file." << endl;
            return 1;
        }

        string inputText((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

        cout << "Original text from file: " << endl << inputText << endl;

        string_to_message_list(inputText, messageList, messageCount);

        cout << "Converted 12-bit messages:" << endl;
        for (int i = 0; i < messageCount; ++i) {
            cout << "Message " << i + 1 << ": ";
            for (int j = 0; j < 12; ++j) {
                cout << messageList[i][j];
            }
            cout << endl;
        }
    }

    return 0;
}