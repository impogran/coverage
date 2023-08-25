#include <cstdio>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <sstream>

#include "grain128aead-v2_opt.h"

#define COLS 10
#define ROWS 5

using namespace std;

void print_binary(uint8_t number);
void print_binary_bytes(uint8_t *bytes);
void print_binary16(uint16_t number);

void generate_input(uint8_t* input) {    
    for(int i = 0; i < 8; i++) {
        input[i] = rand() % 256;    
    }
    //print_binary_bytes(input);
}

void encrypt(uint8_t *plain_text, uint8_t *cipher_text)
{
    unsigned char key[16] = {84, 101, 115, 116,
                             64, 49, 50, 51,
                             116, 51, 54, 63,
                             69, 88, 100, 49};
    unsigned char nonce[12] = {84, 101, 115, 116,
                               64, 49, 50, 51,
                               116, 51, 54, 63};
    unsigned long long clen, dlen;
    unsigned long long mlen = strlen((char *)plain_text);
    crypto_aead_encrypt(cipher_text, &clen, plain_text, mlen, NULL, 0, NULL, nonce, key);
}

int *single_instance(uint8_t *plaintext)
{

    //randomize_plaintext(plaintext);
    generate_input(plaintext);

    uint16_t cast[4] = {0, 0, 0, 0};
    memcpy(&cast, plaintext, sizeof(plaintext));
        
    int array[4096];
    for (int i = 0; i < 4096; i++)
    {
        array[i] = 0;
    }

    for (int i = 0; i < 4096; i++)
    {
        uint16_t cast[4] = {0, 0, 0, 0};
        generate_input(plaintext);
        memcpy(&cast, plaintext, sizeof(plaintext));
        cast[0] ^= i;
        uint8_t ciphertext[16];

        encrypt((unsigned char *)cast, ciphertext);

        uint16_t output = ((uint16_t *)ciphertext)[0];
        uint16_t o2 = output & 0x0FFF;
        array[o2] += 1;
    }
    static int counting[5] = {0, 0, 0, 0, 0};
    for (int j = 0; j < 4096; j++)
    {
        if (j < 2572)
            counting[0] += array[j];
        else if (j < 2584 && j >= 2572)
            counting[1] += array[j];
        else if (j < 2594 && j >= 2584)
            counting[2] += array[j];
        else if (j < 2606 && j >= 2594)
            counting[3] += array[j];
        else
            counting[4] += array[j];
    }

    return counting;
}

int main()
{
    ofstream file;
    string file_name = "data.txt";
    file.open(file_name);
    for (int i = 0; i < COLS; i++)
    {
        uint8_t plaintext[8];
        int *returned_counting;
        returned_counting = single_instance(plaintext);
        for (int j = 0; j < ROWS; j++)
        {
            file << returned_counting[j] << ",";
            returned_counting[j] = 0;
        }
    }
    file.close();
}

    void print_binary(uint8_t number)
    {
        int a[8], i;
        for (int x = 0; x < 8; x++)
            a[x] = 0;
        for (i = 0; number > 0; i++)
        {
            a[i] = number % 2;
            number = number / 2;
        }
        for (i = 7; i >= 0; i--)
        {
            printf("%d", a[i]);
        }
    }
    void print_binary16(uint16_t number)
    {
        int a[16], i;
        for (int x = 0; x < 16; x++)
            a[x] = 0;
        for (i = 0; number > 0; i++)
        {
            a[i] = number % 2;
            number = number / 2;
        }
        for (i = 15; i >= 0; i--)
        {
            printf("%d", a[i]);
        }
    }

    void print_binary_bytes(uint8_t * bytes)
    {
        for (int i = 0; i < 8; i++)
        {
            print_binary(bytes[i]);
            printf(" ");
        }
        printf("\n");
    }