#include "golay.h"
#include <iostream>

using namespace std;

int main() {
    Golay myGolay;

    // test message for encoding
    int message[12] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
    // int message[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    myGolay.print_generator_matrix();

    cout << endl;
    myGolay.print_parity_check_matrix();
    cout << endl;

    cout << "Message: ";
    for (int i = 0; i < 12; ++i) {
        cout << message[i] << " ";
    }
    cout << endl;

    cout << "Encoding..." << endl;
    myGolay.encode(message);

    cout << "Encoded message: ";
    myGolay.print_encoded_message();

    double p = myGolay.input_probability();

    cout << "Sending encoded message through a channel..." << endl;
    myGolay.send_through_channel(p);

    cout << "Received message: ";
    myGolay.print_received();

    cout << "Trying to decode the received message..." << endl;
    myGolay.decode();

    return 0;
}