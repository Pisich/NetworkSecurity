#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>


#define AES_BLOCK_SIZE 16
#define AES_KEY_SIZE 32

typedef struct _AES_DATA 
{
    unsigned char *key;
    unsigned char *iv;
} AES_DATA;

typedef struct Message_Struct
{
    unsigned char *body;
    int *length;
    AES_DATA *aes_settings;
    
} Message;

void hex_print(unsigned char *in, size_t len) {
    for(int i = 0; i < len; i++) {
        printf("%02x", *(in + i));
    }
    printf("\n\n");
}

int aes256_init(Message * input) {
    AES_DATA *aes_info = malloc(sizeof(AES_DATA));
    aes_info -> key = malloc(sizeof(char) * AES_KEY_SIZE);
    aes_info -> iv = malloc(sizeof(char) * AES_KEY_SIZE);

    input -> aes_settings = aes_info;

    //Crear llave y IV
    RAND_bytes(input -> aes_settings -> key, AES_KEY_SIZE);
    RAND_bytes(input -> aes_settings -> iv, AES_KEY_SIZE);

    return 0;
}

Message * message_init(int length) {
    Message *ret = malloc(sizeof(Message));
    ret -> body = malloc(length);
    ret -> length = malloc(sizeof(int));
    *ret -> length = length;

    aes256_init(ret);
    return ret;
}

Message *aes256_encrypt(Message * msg) {
    EVP_CIPHER_CTX *enc_ctx;
    Message * encrypted_message;
    int enc_length = *(msg -> length) + (AES_BLOCK_SIZE - *(msg -> length) % AES_BLOCK_SIZE);

    encrypted_message = message_init(enc_length);

    enc_ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit(enc_ctx, EVP_aes_256_cbc(), msg -> aes_settings -> key, msg -> aes_settings -> iv);
    //Encriptar todos los bloques menos el último
    EVP_EncryptUpdate(enc_ctx, encrypted_message -> body, &enc_length, msg -> body, *msg -> length);
    //Len = cantidad bloques encriptados
    *(encrypted_message -> length) = enc_length;

    //Encriptar el último bloque
    EVP_EncryptFinal_ex(enc_ctx, enc_length + encrypted_message -> body, &enc_length);

    //Len += último bloque
    *(encrypted_message -> length) += enc_length;


    memcpy(encrypted_message -> aes_settings -> key, msg -> aes_settings -> key, AES_KEY_SIZE);
    memcpy(encrypted_message -> aes_settings -> iv, msg -> aes_settings -> iv, AES_KEY_SIZE);

    return encrypted_message;
}

int main() {
    //Buffer de entrada de 16 bytes
    char input[1024] = {0};

    Message *msg, *msg_encriptado;
    printf("Ingresa el mensaje a encriptar: \n");
    fgets(input, 1024, stdin);

    msg = message_init(strlen(input));
    strcpy((char *) msg -> body, input);

    aes256_init(msg);

    printf("Llave: ");
    hex_print(msg -> aes_settings -> key, AES_KEY_SIZE);

    printf("IV: ");
    hex_print(msg -> aes_settings -> iv, AES_KEY_SIZE);

    msg_encriptado = aes256_encrypt(msg);

    printf("Mensaje encriptado: ");
    hex_print(msg_encriptado -> body, *msg_encriptado -> length);

    return 0;
}