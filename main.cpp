#include "golay.h"
#include <iostream>

using namespace std;

int main() {
    Golay myGolay;

    // test message for encoding
    int message[12] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};

    cout << "Message: ";
    for (int i = 0; i < 12; ++i) {
        cout << message[i] << " ";
    }
    cout << endl;

    cout << "Encoding..." << endl;
    myGolay.encode(message);

    cout << "Encoded message: ";
    myGolay.print_encoded_message();

    return 0;
}