#include <cstdio>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <random>
#include <chrono>

#include "crypto_aead.h"

#define COLS 10
#define ROWS 5

using namespace std;

void print_binary(uint8_t number);
void print_binary_bytes(uint8_t *bytes);
void print_binary16(uint16_t number);

default_random_engine dre (chrono::steady_clock::now().time_since_epoch().count());     // provide seed
int random (int lim)
{
    uniform_int_distribution<int> uid {0,lim};   // help dre to generate nos from 0 to lim (lim included);
    return uid(dre);    // pass dre as an argument to uid to generate the random no
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

uint8_t randomize_plaintext(uint8_t* plaintext) {
    srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < 8; ++i) {
        plaintext[i] = random(255);
    }

    return *plaintext;
}

int* single_instance(uint8_t* plaintext) {

    randomize_plaintext(plaintext);

    uint16_t cast[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
    {
        cast[i] = ((unsigned int *)plaintext)[i];
    }
    int array[4096];
    for (int i = 0; i < 4096; i++)
    {
        array[i] = 0;
    }

    for (int i = 0; i < 4096; i++)
    {
        uint16_t cast[4] = {0, 0, 0, 0};
        for (int i = 0; i < 4; i++)
        {
            cast[i] = ((unsigned int *)plaintext)[i];

        }
        cast[0] ^= i;
        uint8_t ciphertext[16];

        encrypt((unsigned char *)cast, ciphertext);

        uint16_t output = ((uint16_t *)ciphertext)[0];
        uint16_t o2 = output & 0x0FFF;
        array[o2]+=1;
    }
    int counting[5]={0,0,0,0,0};
    for (int j = 0; j < 4096; j++)
    {
        if(j<2572) counting[0]+=array[j];
        else if(j< 2584 && j>=2572) counting[1]+=array[j];
        else if(j<2594 && j>=2584) counting[2]+=array[j];
        else if(j<2606 && j>=2594) counting[3]+=array[j];
        else counting[4]+=array[j];
    }

    return counting;
}

int main()
{
    uint8_t plaintext[8];
    int* returned_counting[COLS];
    int to_return[COLS][ROWS];
    for(int i=0;i<COLS;i++) {
        returned_counting[i] = single_instance(plaintext);
        // cout<<returned_counting[i]<<endl;
    }
    for(int i = 0;i<10;i++){
        for(int j=0;j<5;j++){
            cout<<returned_counting[i]<<" ";

        }
        cout<<endl;
    }

}
//array [10][5] and put counting into each column
//expected [0][0] sum of row 0 * sum of col 0 / sum of all fields in array

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

void print_binary_bytes(uint8_t *bytes)
{
    for (int i = 0; i < 8; i++)
    {
        print_binary(bytes[i]);
        printf(" ");
    }
    printf("\n");
}