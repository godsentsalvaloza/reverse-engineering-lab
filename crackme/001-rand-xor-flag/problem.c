#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*###############

Authors => IronByte X t0m7r00z

##############*/

int main(int argc, char* argv[]) {

    int t[36] = {5, 141, 43, 73, 114, 50, 197, 26, 166,
                 235, 37, 150, 75, 115, 4, 82, 35, 235,
                 201, 21, 198, 220, 162, 8, 184, 40, 239,
                 37, 49, 97, 177, 4, 175, 13, 197, 138};

    char input[0x30];

    srand(2022);
    printf("Give me the flag please: ");
    scanf("%48s", input);
    if (strlen(input) != 36)
        printf("Wrong Length!");
    else {
        int i = 0;
        int flag = 1;
        while(i < 36 && flag) {
            int nb = rand();
            int x = (int)(nb >> 31) >> 24;      
            x = (nb + x & 0xff) - x;           
            x = x ^ t[i];                       
            if (input[i] != x) {                
                flag = 0;                       
                break;
            }

            i++;
        }

        if (!flag)
            printf("Wrong flag!");
        else
            printf("Nice you can validate with that flag!");
    }

    return 0;
}