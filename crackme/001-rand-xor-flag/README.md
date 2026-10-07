# 001-rand-xor-flag

Sample problem from **IronByte**'s Reverse Engineering Learning Series. 

**Source**: [IronByte | Reverse Engineering - Fundamentals](https://www.youtube.com/watch?v=hSfI7izOvr0&list=PL-A03qCBcinTiuCUtfhWGy1HMpnr0rOdh&index=2)

---

## The Problem
Trace and reverse the piece of code to retrieve the flag.

```c
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
```

---

## The Analysis

On the first three statements, a declared array of integers with a size of **36** named **t**, which appears to contain random numbers.

```c
int t[36] = {5, 141, 43, 73, 114, 50, 197, 26, 166,
                 235, 37, 150, 75, 115, 4, 82, 35, 235,
                 201, 21, 198, 220, 162, 8, 184, 40, 239,
                 37, 49, 97, 177, 4, 175, 13, 197, 138};
```

We can also see an array of characters named **input**, which has a size of **0x30**, which when converted to decimal, gives us **48**. This means that the `input` variable has a capacity of **48 bytes**.

```c
char input[0x30];
```

Interestingly, we have an **srand()** function with the parameter of **2022**. The purpose of the **srand()** function is to initialize the pseudo-random number generator with a seed.

```c
srand(2022);
```

Skipping the **scanf()** function for the flag input, we can observe a conditional statement checking the length of our input. We can now confirm that the required flag length is **exactly 36 characters**.

```c
if (strlen(input) != 36)
        printf("Wrong Length!");
```

For our else block, this is where the algorithm happens. Let's analyse it line-by-line.

An integer **i** is declared for counting the iterations of the while loop, which will run up to 36 times. The integer **flag** acts as the check for the loop whether the contents of the input are still consistent with the expected flag.

```c 
int i = 0;
int flag = 1;       
```

So what is happening here? 

In this block of code, a while loop will run while the variable **i** is less than 36, **AND** the variable **flag** remains **1**.

The **rand()** function returns a non-negative value to the variable **nb**.

```c
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
```

At this point, this is where the concept of **bitwise operators** comes in handy. We can simplify the given process to better understand how the algorithm works.

Just to give a bit of background, on the system used for this challenge, an `int` is 32 bits, which is something like this:

```text
0000 0000 0000 0000 0000 0000 0000 0000
```

On the other hand, the concept of **right shift (`>>`)** means moving the binary equivalent of the integer to the right depending on the value of the shift.

For instance, if we have a variable **val** with a value of 128, we convert it to binary. It goes something like this:

```text
val = 1 0 0 0 0 0 0 0   // 128
```

Assume we want to right shift **val** by 3. We can do something like this:

```c
val = val >> 3;
```

This line of code indicates that the variable `val` should be right shifted by 3 positions. As a result, we move the binary values 3 positions to the right, which ultimately gives us the result of **16**.

```text
val = 0 0 0 1 0 0 0 0    // 16
```

Going back to the problem, we can see that this piece of code tells variable **nb** to right shift by **31** positions, and then right shift the result by another **24** positions.

Since **rand()** returns a non-negative value, the most significant bit of `nb` is **0**. Therefore, shifting `nb` right by 31 positions produces **0**. Shifting that result by another 24 positions still produces **0**.

```c
int x = (int)(nb >> 31) >> 24;
```

We can simplify the code like so:

```c
int x = 0;
```

Moving forward, we can see here that the variable **x** is being reassigned once again. But this time, since we know that `x` is currently assigned as 0, we can simplify the code from this:

```c
x = (nb + x & 0xff) - x;
```

To this:

```c
x = nb & 0xff;
```

As such, we are now required to execute a bitwise **AND** operation. In this operation, we can only get the value **1** if both corresponding bits are **1**.

Since we don't know the value of variable **nb**, let's focus our attention on **0xff**. Converting this hexadecimal value to binary gives us:

```text
1111 1111
```

Knowing that we will do an **AND** operation with this value, we can see that it acts as a mask that keeps only the **lowest 8 bits** of `nb`.

Example:

```text
     0000 0000 1111 1111
     1011 0111 1010 0010
AND
-------------------------
     0000 0000 1010 0010
```

Thus, what this line of code does is reduce the random value to its **lowest 8 bits**.

Lastly, this part of the algorithm performs a bitwise **exclusive OR (XOR)**:

```c
x = x ^ t[i]; 
```

What bitwise XOR does is compare each bit and return **1 if the values are different**. In this line of code, we can see that variable **x** is being XORed with the **i-th value** of the integer array **t**.

Let's try doing an XOR with the values 13 and 7:

```text
     1101
     0111
XOR
---------
     1010
```

Now that we finally understand how the algorithm works, let's write its simplified form:

```c
while(i < 36 && flag) {
    int nb = rand();       // generates a deterministic pseudo-random number from seed 2022.
    int x = 0;
    x = nb & 0xff;         // keeps the lowest 8 bits of nb.
    x = x ^ t[i];          // XORs x with the i-th element of array t.

    if (input[i] != x) {   // breaks the loop if x is not equal to the i-th character of input.
        flag = 0;
        break;
    }

    i++;                   // increments loop by 1.
}
```
---
### Capturing the flag

Now that we know the inner workings of the algorithm, let's now focus on how to actually extract the flag from it.

One of the notable lines of code here is the **srand()** function used for the random number generation.

```c
srand(2022);
```

Since the random seed in this case is constant, we can actually predict the "randomness" of the pseudo-random number generator.

To prove that, I wrote a simple code that prints the output of the random numbers:

```c
#include <stdio.h>
#include <stdlib.h>

int main(){
    srand(2022);

    for(int i = 0; i < 36; i++){
        int nb = rand();

        printf("%d\n", nb);
    }
    return 0;
}
```

When executed, this will give us the same values every time. I only included the first and last two integers for simplicity:

> **Note:** You should use the same C compiler/runtime environment (**gcc**) used for the original challenge, as different implementations can produce different `rand()` sequences.

```text
1341262422
470018536
...
1279441815
1891646455
```

This shows that the output of `rand()` is deterministic when the same seed and implementation are used. This becomes a weakness when a pseudo-random number generator is used in a situation where unpredictable values are required.

Now that we understand how to deal with the **random** values, let's incorporate everything to extract the flag from the algorithm.

```c
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
    }
    return 0;
}
```

And with that, we finally get the output:

```text
Securinets{Ign0r3_th4T_jUnK_4nD_X0R}
```
---

## Final Thoughts

This challenge gives us great introductory knowledge about reverse engineering.

It also shows the importance of understanding fundamental Computer Science concepts, such as bitwise operators.

On the other hand, it demonstrates the difficulty of reverse engineering a piece of code, while also showing the challenges involved in learning the field of reverse engineering.