#include <openssl/aes.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <x86intrin.h>

#pragma intrinsic(__rdtsc)
#define NTEST 100000

void measured_function(volatile int *var) { (*var) = 1; }

/*static const unsigned char key[] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};*/

int main() {
  unsigned char text[] = "hola mundoooo";
  unsigned char enc_out[80];
  unsigned char dec_out[80];

  unsigned char key[32];
  int rc = RAND_bytes(key, sizeof(key));
  unsigned long err = ERR_get_error();
  if (rc != 1)
    return -1;

  AES_KEY enc_key, dec_key;

  int variable = 0;
  uint64_t start, end;

  printf("Calentamiento...\n");
  for (int i = 0; i < NTEST; i++)
    measured_function(&variable);

  AES_set_encrypt_key(key, 128, &enc_key);

  AES_encrypt(text, enc_out, &enc_key);

  AES_set_decrypt_key(key, 128, &dec_key);
  AES_decrypt(enc_out, dec_out, &dec_key);

  printf("Original:\t");
  int stop = sizeof(text) - 1;
  for (int i = 0; *(text + i) != 0x00; i++)
    printf("%c ", *(text + i));
  printf("\nEncrypted:\t");
  for (int i = 0; *(enc_out + i) != 0x00; i++)
    printf("%.2X ", *(enc_out + i));
  printf("\nDecrypted:\t");
  for (int i = 0; i < 14; i++)
    printf("%c ", *(dec_out + i));
  printf("\n");

  return 0;
}