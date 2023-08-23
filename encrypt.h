#define CRYPTO_KEYBYTES     16
#define CRYPTO_NSECBYTES    0
#define CRYPTO_NPUBBYTES    16
#define CRYPTO_ABYTES       16
#define CRYPTO_NOOVERLAP    1

#include "gift128.h"
#include "crypto_aead.h"


typedef unsigned char block[16];
typedef unsigned char half_block[8];

static void padding(block d, block s, unsigned no_of_bytes);
static void xor_block(block d, block s1, block s2, unsigned no_of_bytes);
static void xor_topbar_block(block d, block s1, half_block s2);
static void double_half_block(half_block d, half_block s);
static void triple_half_block(half_block d, half_block s);
static void G(block d, block s);
static void pho1(block d, block Y, block M, int no_of_bytes);
static void pho(block Y, block M, block X, block C, int no_of_bytes);
static void phoprime(block Y, block C, block X, block M, int no_of_bytes);
static int cofb_crypt(unsigned char *out, unsigned char *k, unsigned char *n,
                     unsigned char *a, unsigned alen,
                     unsigned char *in, unsigned inlen, int encrypting);
void cofb_encrypt(unsigned char *c, unsigned char *k, unsigned char *n,
                 unsigned char *a, unsigned abytes,
                 unsigned char *p, unsigned pbytes);
int cofb_decrypt(unsigned char *p, unsigned char *k, unsigned char *n,
                unsigned char *a, unsigned abytes,
                unsigned char *c, unsigned cbytes);

int crypto_aead_encrypt(
	unsigned char *c, unsigned long long *clen,
	const unsigned char *m,unsigned long long mlen,
	const unsigned char *ad,unsigned long long adlen,
	const unsigned char *nsec,
	const unsigned char *npub,
	const unsigned char *k
	);

int crypto_aead_decrypt(
	unsigned char *m,unsigned long long *mlen,
	const unsigned char *c,unsigned long long clen,
	const unsigned char *ad,unsigned long long adlen,
	const unsigned char *nsec,
	const unsigned char *npub,
	const unsigned char *k
	);