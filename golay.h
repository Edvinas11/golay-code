#ifndef GOLAY_H
#define GOLAY_H

class Golay {
private:
    int I[12][12] = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    };
    int B[12][11] = {
        {1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0},
        {1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1},
        {0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1},
        {1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0},
        {1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1},
        {0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1},
        {0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0},
        {0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0},
        {1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0},
        {0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    int G[12][23];
    int H[12][24];
    int encoded[23] = {0};
    int received[23];
    int f_received[24];
    int syndrome[12] = {};

    void create_generator_matrix();
    void create_parity_check_matrix();
    void get_syndrome();
    void form_received();
    int bin_add(int a, int b);
    int bin_mult(int a, int b);
    int get_weight(int *v, int size);

public:
    Golay();
    void print_generator_matrix();
    void print_parity_check_matrix();
    void encode(int *message);
    void print_encoded_message();
    void send_through_channel(double p);
    void print_received();
    void decode();
    double input_probability();
};

#endif