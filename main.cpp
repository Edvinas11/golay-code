#include "golay.h"
#include <iostream>
#include <fstream>

using namespace std;

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
    }

    return 0;
}