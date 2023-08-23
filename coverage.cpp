#include <cstdio>
#include <iostream>
#include <cstdint>
#include <cstring>

#include "crypto_aead.h"

using namespace std;

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

    // unsigned char decoded_text[24];
    // int result = crypto_aead_decrypt(decoded_text, &dlen, cipher_text, clen, NULL, 0, NULL, nonce, key);
    // if(result == 0) {
    //     cout << "Success decrypted!!!" << endl;
    // }  else {
    //     cout << "Incorrect MAC - cipher discarded!!!" << endl;
    // }
}

void print_binary(uint8_t number);
void print_binary_bytes(uint8_t *bytes);
void print_binary16(uint16_t number);

int main()
{
    uint8_t plaintext[8] = {'d', 'u', 'p', 'a', 5, 6, 7, 8};
    uint16_t cast[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
    {
        cast[i] = ((unsigned int *)plaintext)[i];
    }
    // for (uint16_t i = 0; i < 4096; i++) {
    //     uint16_t x = i & 0x0FFF;
    //     cout << x << endl;
    // }
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
    for (int i = 0; i < 4096; i++)
    {
        // cout << array[i] << endl;
        if(i<2572) counting[0]+=array[i];
        else if(i< 2584 && i>=2572) counting[1]+=array[i];
        else if(i<2594 && i>=2584) counting[2]+=array[i];
        else if(i<2606 && i>=2594) counting[3]+=array[i];
        else counting[4]+=array[i];
    }
    for(int i=0;i<5;i++) cout<<counting[i]<<" "<<endl;
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

void print_binary_bytes(uint8_t *bytes)
{
    for (int i = 0; i < 8; i++)
    {
        print_binary(bytes[i]);
        printf(" ");
    }
    printf("\n");
}