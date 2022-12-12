#include <sodium.h>
#include <inttypes.h>

int main(void)
{
    const uint32_t upper_bound = 500;
    for (int i = 0; i<100; i++){
      uint32_t value = randombytes_uniform(upper_bound);
      printf("%" PRIu32 "\n", value);
    }
    
    return 0;
}

//sudo gcc libsodium_is727366.c -o libsodium.exe -lsodium