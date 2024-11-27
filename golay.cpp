#include "golay.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>

using namespace std;

// constructor
Golay::Golay() {
    create_generator_matrix();
}

// creates a 12 x 23 matrix G = [I, B]
void Golay::create_generator_matrix() {
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            G[i][j] = I[i][j];
        }
    }
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 11; ++j) {
            G[i][j + 12] = B[i][j];
        }
    }
}

// prints G matrix
void Golay::print_generator_matrix() {
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 23; ++j) {
            cout << G[i][j] << " ";
        }
        cout << endl;
    }
}

void Golay::print_parity_check_matrix() {
    for (int i = 0; i < 24; ++i) {
        for (int j = 0; j < 12; ++j) {
            std::cout << H[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// binary addition (mod 2)
int Golay::bin_add(int a, int b) {
    return (a + b) % 2;
}

// binary multiplication (mod 2)
int Golay::bin_mult(int a, int b) {
    return (a * b) % 2;
}

// encodes a 12-bit message into a 23-bit codeword by matrix multiplication with the generator matrix G
int* Golay::encode(int* message) {
    static int encoded[23]; // Use static array to return the encoded message

    for (int i = 0; i < 23; ++i) {
        encoded[i] = 0;
    }

    for (int i = 0; i < 23; ++i) {
        for (int j = 0; j < 12; ++j) {
            encoded[i] = bin_add(encoded[i], bin_mult(message[j], G[j][i]));
        }
    }

    return encoded;
}

void Golay::print_encoded_message(const int* encoded_message) {
    for (int i = 0; i < 23; ++i) {
        cout << encoded_message[i];
    }
    cout << endl;
}

// prompts the user to input an error probability for the transmission channel
double Golay::get_input_probability() {
    string input;
    double p;

    while(true) {
        cout << "Please enter error probability as a floating point number (between 0 and 1, e.g., 0.0001): ";
        cin >> input;

        // replace ',' with '.'
        for (char& ch : input) {
            if (ch == ',') {
                ch = '.';
            }
        }

        try {
            p = stod(input);

            if (p >= 0.0 && p <= 1.0) {
                break;
            } else {
                cerr << "ERROR: Probability must be between 0 and 1." << endl;
            }
        }
        catch (...) {
            cerr << "ERROR: Invalid input. Please enter a valid floating-point number." << endl;
        }
    }

    return p;
}

// Simulates sending the encoded message through a noisy channel, introducing 
// errors based on the probability p. 
// Outputs the received message and error details
void Golay::send_through_channel(double p, const int* encoded_message, int* received_message, bool protect_last_bits) {
    int error_count = 0;
    vector<int> error_positions;

    for (int i = 0; i < 23; ++i) {
        if (protect_last_bits && i >= 8 && i <= 11) {
            received_message[i] = encoded_message[i];
            continue;
        }

        double a = (double)rand() / RAND_MAX;

        if (a < p) {
            received_message[i] = 1 - encoded_message[i];
            error_positions.push_back(i);
            error_count++;
        }
        else {
            received_message[i] = encoded_message[i];
        }
    }

    cout << "Received vector after passing through the channel: ";
    for (int i = 0; i < 23; ++i) {
        cout << received_message[i];
    }
    cout << endl;

    cout << "Number of errors: " << error_count << endl;
    if (error_count > 0) {
        cout << "Error positions: ";
        for (int pos : error_positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    else {
        cout << "No errors." << endl;
    }
}

void Golay::get_syndrome() {
    // get syndrome s = received * H
    for (int i = 0; i < 12; ++i) {
        syndrome[i] = 0;
    }

    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 24; ++j) {
            syndrome[i] = bin_add(syndrome[i], bin_mult(f_received[j], H[j][i])); 
        }
    }

    cout << "Syndrome: ";
    for (int i = 0; i < 12; ++i) {
        cout << syndrome[i];
    }
    cout << endl;
}

int Golay::get_weight(int *v, int size) {
    int weight = 0;
    for (int i = 0; i < size; ++i) {
        weight += v[i];
    }
    return weight;
}

// forms the 24-bit extended received vector by appending a parity bit to the 23-bit received message
void Golay::form_received(int* received_message) {
    for (int i = 0; i < 23; ++i) {
        f_received[i] = received_message[i];
    }

    int weight = get_weight(received_message, 23);

    if (weight % 2 == 0) {
        f_received[23] = 1;
    }
    else {
        f_received[23] = 0;
    }
}

// Attempts to decode the received vector by using error-correction techniques 
// based on the syndrome and predefined rules. 
// Outputs the decoded message or an error if the message is undecodable.
void Golay::decode(int* received_message, int* decoded_message) {
    int u[24] = {0};  // Error vector
    bool decodable = true;
    int sB[12] = {0};

    // Step 1: Form received vector with parity bit
    form_received(received_message);

    // Step 2: Compute syndrome
    get_syndrome();

    // Step 2: If weight of syndrome <= 3, use it as error pattern [s, 0]
    int syndrome_weight = get_weight(syndrome, 12);
    
    if (syndrome_weight <= 3) {
        for (int i = 0; i < 12; ++i) {
            u[i] = syndrome[i];
        }
        for (int i = 12; i < 24; ++i) {
            u[i] = 0;
        }
    }
    else {
        bool found = false;

        // Step 3: Check if weight(s + b_i) <= 2 for some row b_i of B
        for (int i = 0; i < 12; ++i) {
            int temp_syndrome[12];
            for (int j = 0; j < 12; ++j) {
                temp_syndrome[j] = bin_add(syndrome[j], B[i][j]);
            }

            int temp_weight = get_weight(temp_syndrome, 12);

            if (temp_weight <= 2) {
                // Set u = [s + b_i, e_i]
                for (int k = 0; k < 12; ++k) {
                    u[k] = temp_syndrome[k];
                }
                for (int k = 12; k < 24; ++k) {
                    u[k] = (k - 12 == i) ? 1 : 0;
                }
                found = true;
                break;
            }
        }

        if (!found) {
            // Step 4: Compute second syndrome sB = syndrome * B
            for (int i = 0; i < 12; ++i) {
                sB[i] = 0;
                for (int j = 0; j < 12; ++j) {
                    sB[i] = bin_add(sB[i], bin_mult(syndrome[j], B[j][i]));
                }
            }

            for (int i = 0; i < 12; ++i) {
                cout << sB[i];
            }
            cout << endl;

            // Step 5: If weight of sB <= 3, use [0, sB] as error pattern
            int sB_weight = get_weight(sB, 12);
            
            if (sB_weight <= 3) {
                for (int i = 0; i < 12; ++i) {
                    u[i] = 0;
                    u[i + 12] = sB[i];
                }
            }
            else {
                // Step 6: Check if weight(sB + b_i) <= 2 for some row b_i of B
                bool corrected = false;
                for (int i = 0; i < 12; ++i) {
                    int temp_sB[12];
                    for (int j = 0; j < 12; ++j) {
                        temp_sB[j] = bin_add(sB[j], B[i][j]);
                    }

                    int temp_sB_weight = get_weight(temp_sB, 12);

                    if (temp_sB_weight <= 2) {
                        // Set u = [e_i, sB + b_i]
                        for (int k = 0; k < 12; ++k) {
                            u[k] = (k == i) ? 1 : 0; // Set u[k] to 1 for error pattern
                            u[k + 12] = temp_sB[k];
                        }
                        corrected = true;
                        break;
                    }
                }

                if (!corrected) {
                    cout << "No correctable error pattern found. Marking as undecodable." << endl;
                    decodable = false;  // Unable to decode
                }
            }
        }
    }
 
    // Decode if possible
    if (decodable) {
        for (int i = 0; i < 23; ++i) {
            decoded_message[i] = bin_add(f_received[i], u[i]);
        }
    } else {
        cout << "ERROR: Message undecodable..." << endl;
        for (int i = 0; i < 23; ++i) {
            decoded_message[i] = 0;
        }
    }

    // clear f_received after decoding
    for (int i = 0; i < 24; ++i) {
        f_received[i] = 0;
    }
}

// prompts the user to input a 12-bit binary message. Validates the input for length and binary format
void Golay::get_message_input(int *message) {
    string input;
    bool valid = false;

    while (!valid) {
        cout << "Please enter a 12-bit binary message (e.g., 001001001001): ";
        cin >> input;

        if (input.length() != 12) {
            cerr << "Error: Input must be exactly 12 bits long." << endl;
            continue;
        }

        // we know for sure input is exactly 12 bits long
        valid = true;

        for (int i = 0; i < 12; ++i) {
            if (input[i] == '0') {
                message[i] = 0;
            }
            else if (input[i] == '1') {
                message[i] = 1;
            }
            else {
                cerr << "ERROR: Input must contain only binary digits (0 or 1)." << endl;
                valid = false;
                break;
            }
        }
    }
}

// allows the user to manually edit the received vector by 
// flipping specific bits at positions they specify. Outputs the updated message
void Golay::edit_received_message(int* received_message) {
    string response;
    cout << "Do you want to edit the received message? (yes/no): ";
    cin >> response;

    if (response != "yes") {
        cout << "Message was not edited." << endl;
        return;
    }

    cout << "Edit the received vector by specifying error positions (0-22), separated by spaces. Enter -1 when you are finished:" << endl;

    int pos;
    vector<int> positions;

    while(true) {
        cin >> pos;

        if (pos == -1) {
            break;
        }

        if (pos >= 0 && pos < 23) {
            positions.push_back(pos);
        }
        else {
            cout << "Position must be between 0 and 22. Please try again." << endl;
        }
    }

    // flipping bits
    for (int i : positions) {
        received_message[i] = 1 - received_message[i];
    }

    cout << "Edited message: ";
    for (int i = 0; i < 23; ++i) {
        cout << received_message[i];
    }
    cout << endl;
}
