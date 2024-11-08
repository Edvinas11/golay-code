#include "golay.h"
#include <iostream>

using namespace std;

int main() {
    Golay myGolay;
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

    return 0;
}