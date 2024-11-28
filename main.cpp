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

void string_to_message_list(const string& input_text, int message_list[][12], int& message_count) {
    message_count = input_text.length(); // Number of 12-bit messages corresponds to the string length

    for (int i = 0; i < message_count; ++i) {
        char_to_binary_with_padding(input_text[i], message_list[i]);
    }
}

void process_messages(Golay& golay, int message_list[][12], int message_count, bool protect_last_bits, bool allow_edit, int decoded_messages[][23]) {
    double p = golay.get_input_probability();

    for (int i = 0; i < message_count; ++i) {
        cout << "Encoding..." << endl;
        int* encoded_message = golay.encode(message_list[i]);

        cout << "Encoded message: ";
        golay.print_encoded_message(encoded_message);

        int received_message[23];
        cout << "Sending encoded message through untrusted channel with error probability " << p << "..." << endl;
        golay.send_through_channel(p, encoded_message, received_message, protect_last_bits);

        if (allow_edit) {
            golay.edit_received_message(received_message);
        }

        cout << "Trying to decode the received message..." << endl;
        golay.decode(received_message, decoded_messages[i]);

        cout << "Decoded message: ";
        for (int j = 0; j < 23; ++j) {
            cout << decoded_messages[i][j];
        }

        cout << endl;
    }
}

int main() {
    srand(time(0));

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

        int decoded_messages[1][23];
        process_messages(myGolay, &message, 1, false, true, decoded_messages);
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

        int decoded_messages[100][23];
        process_messages(myGolay, messageList, messageCount, true, false, decoded_messages);

        // reconstruct the decoded text
        string decoded_text = "";
        for (int i = 0; i < messageCount; ++i) {
            char decoded_char = 0;
            for (int j = 0; j < 8; ++j) { // Convert first 8 bits of each decoded message to a character
                decoded_char |= (decoded_messages[i][j] << (7 - j));
            }
            decoded_text += decoded_char;
        }

        cout << "Reconstructed text after decoding: " << decoded_text << endl;
    }
    else {
        cout << "Invalid choice. Exiting program." << endl;
    }

    return 0;
}