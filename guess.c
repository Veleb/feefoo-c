#include <stdio.h>
#include <stdlib.h>

#define RANGE 100

int main(void) {

    int guess;
    int target = rand() % RANGE;

    printf("Range is: %d\n", RANGE);
    while (1) {
        printf("Guess: ");
        scanf("%d", &guess);

        if (guess == target) {
            printf("CORRECT!");
            break;
        } else {
            printf("Wrong try again!\n");
            continue;
        }
    }

    return 0;
}