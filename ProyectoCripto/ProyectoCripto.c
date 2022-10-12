#include <openssl/aes.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma intrinsic(__rdtsc)
#define NTEST 100000

void measured_function(volatile int *var) { (*var) = 1; }

static const unsigned char key[] = {
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
  0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

int encrypt_file(char filename[]){
	FILE *f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	unsigned char *text = malloc(fsize + 1);
	fread(text, fsize, 1, f);
	fclose(f);
	text[fsize] = 0;

  unsigned char enc_out[80];
  unsigned char dec_out[80];

  AES_KEY enc_key, dec_key;

  int variable = 0;
  uint64_t start, end;

  printf("Calentamiento...\n");
  for (int i = 0; i < NTEST; i++)
    measured_function(&variable);

  AES_set_encrypt_key(key, 128, &enc_key);

  AES_encrypt(text, enc_out, &enc_key);

  printf("Original:\t");
  int stop = sizeof(text) - 1;
  for (int i = 0; *(text + i) != 0x00; i++)
    printf("%c ", *(text + i));
  printf("\nEncrypted:\t");
  FILE *fo = fopen("encripted_output.txt", "w");
  for (int i = 0; *(enc_out + i) != 0x00; i++){
    printf("%.2X ", *(enc_out + i));
    fprintf(fo,"%.2X ", enc_out[i]);
  }

	printf("\n");
  fclose(fo);
  printf("Se encriptó el archivo correctamente, el archivo resultante");
  printf(" es encripted_output.txt\n");
}

int decrypt_file(char filename[]){
	FILE *f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	unsigned char *text = malloc(fsize + 1);
	fread(text, fsize, 1, f);
	fclose(f);
	text[fsize] = 0;

  unsigned char enc_out[80];
  unsigned char dec_out[80];

	AES_KEY enc_key, dec_key;

  int variable = 0;
  uint64_t start, end;

	printf("Calentamiento...\n");
  for (int i = 0; i < NTEST; i++)
    measured_function(&variable);
	
	AES_set_decrypt_key(key, 128, &dec_key);
  AES_decrypt(enc_out, dec_out, &dec_key);

	printf("Original:\t");
  int stop = sizeof(text) - 1;
  for (int i = 0; *(text + i) != 0x00; i++)
    printf("%c ", *(text + i));
	printf("\nDecrypted:\t");
  FILE *fo = fopen("decripted_output.txt", "w");
  for (int i = 0; i < 14; i++){
    printf("%c ", *(dec_out + i));
    fprintf(fo,"%c ", dec_out[i]);
  }
  printf("\n");

  fclose(fo);
  printf("Se desencriptó el archivo correctamente, el archivo resultante");
  printf(" es decripted_output.txt\n");
}

int sign_file(char filename[]){
  printf("Looking for existing privatekey.pem...\n");
  FILE *fpk;
  if (fpk = fopen("privatekey.pem", "r")){
    printf("File is present, will use existing file\n");
  } else{
    printf("File is NOT present, will create new file\n");
    system("openssl genrsa -out privatekey.pem 2048");
    system("openssl rsa -in privatekey.pem -outform PEM -pubout -out publickey.pem");
  }
  printf("Signing file %s...\n", filename);
  char cmd[100] = "";
  sprintf(cmd, "openssl dgst -sha256 -sign privatekey.pem -out %s.signature %s", filename, filename);
  system(cmd);
  printf("Successfully signed file %s...\n", filename);
  printf("Signature file is: %s.signature\n", filename);
  return 0;
}

int verify_sign(char filename[]){
  printf("Looking for existing key files ...\n");
  FILE *fpk;
  if (fpk = fopen("privatekey.pem", "r")){
    printf("privatekey file is present, will use existing file\n");
    printf("publickey file is present, will use existing file\n");
  } else{
    printf("File is NOT present, cannot verify signature\n");
    return 1;
  }
  printf("Verifying the signed file %s...\n", filename);
  char cmd[100] = "";
  sprintf(cmd, "openssl dgst -sha256 -verify publickey.pem -signature %s.signature %s", filename, filename);
  system(cmd);
  printf("Successfully verified signed file %s...\n", filename);
  return 0;
}

int main() {
  fflush(stdin);
  int seleccion = 0;
  char filename[100] = "";

  FILE *fb = fopen("bitacora.txt", "a");
  fprintf(fb, "Se registró un acceso con tiempo epoch: %ld\n", (unsigned long)time(NULL));
  fclose(fb);


  printf("Bienvenid@, selecciona una opción para continuar:\n");
  printf("1. Encriptar archivo\n");
  printf("2. Desencriptar archivo\n");
  printf("3. Firmar archivo\n");
  printf("4. Verificar la firma de archivo\n");
  printf("Ingresa el número de la opción a ejecutar: \n");

  scanf("%d", &seleccion);
  fflush(stdin);
  if (seleccion == 1){
    printf("ENCRIPTAR ARCHIVO\n");
    printf("Ingresa el nombre del archivo a encriptar: ");

    scanf("%s", &filename);
    encrypt_file(filename);
  } else if (seleccion == 2){
    printf("DESENCRIPTAR ARCHIVO\n");
    printf("Ingresa el nombre del archivo a desencriptar: ");

    scanf("%s", &filename);
    decrypt_file(filename);
  } else if (seleccion == 3){
    printf("FIRMAR ARCHIVO\n");
    printf("Ingresa el nombre del archivo a firmar: ");

    scanf("%s", &filename);
    sign_file(filename);
  } else if (seleccion == 4){
    printf("VERIFICAR FIRMA DE ARCHIVO\n");
    printf("Ingresa el nombre del archivo a verificar (no ingreses el .signature): ");

    scanf("%s", &filename);
    verify_sign(filename);
  } else{
    printf("OPCION INVALIDA\n");
  }
  return 0;
}