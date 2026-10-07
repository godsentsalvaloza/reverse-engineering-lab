#include <stdio.h>
#include <stdlib.h>

int main(){
    int t[36] = {5, 141, 43, 73, 114, 50, 197, 26, 166,
                 235, 37, 150, 75, 115, 4, 82, 35, 235,
                 201, 21, 198, 220, 162, 8, 184, 40, 239,
                 37, 49, 97, 177, 4, 175, 13, 197, 138};

    srand(2022); // initializes the pseudo-random number generator

    for(int i = 0; i < 36; i++){ 
        int nb = rand();  // generates the next pseudo-random number

        nb = nb & 0xff;  // keeps the lowest 8 bits
        nb = nb ^ t[i];  // XORs the value with the i-th element of array t

        printf("%c", nb); // prints the resulting value as a character
        // OUTPUT: Securinets{Ign0r3_th4T_jUnK_4nD_X0R}
    }
    return 0;
}