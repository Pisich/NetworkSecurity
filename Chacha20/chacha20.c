#include <sodium.h>
#include <inttypes.h>

int main(void) {
    unsigned char message[] = "This is my message";
    int mlen = 20;

    unsigned char* key = (unsigned char *) malloc(crypto_stream_chacha20_KEYBYTES);
    randombytes_buf(key, crypto_stream_chacha20_KEYBYTES);
    unsigned char* nonce = (unsigned char *) malloc(crypto_stream_chacha20_ietf_NONCEBYTES);
    randombytes_buf(nonce, crypto_stream_chacha20_ietf_NONCEBYTES);

    unsigned char* encrypted_msg = (unsigned char *) malloc(crypto_stream_chacha20_ietf_NONCEBYTES);
    crypto_stream_chacha20_xor(encrypted_msg, message, mlen, nonce, key);
    printf("Key: %s\n", key);
    printf("Encrypted msg: ");
    for (int i=0; i < 19; i++) {
        printf("%c", encrypted_msg[i]);
    }

    crypto_stream_chacha20_xor(message, encrypted_msg, mlen, nonce, key);
    printf("\nDecrypted msg: ");
    for (int i=0; i < 19; i++) {
        printf("%c", message[i]);
    }

    printf("\n");

    return 0;
}
