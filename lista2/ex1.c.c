#include <stdio.h>

void main() {
    int A[30], B[30], C[60];
    int i, j = 0;

    for (i = 0; i < 30; i++) {
        scanf("%d", &A[i]);
    }

    for (i = 0; i < 30; i++) {
        scanf("%d", &B[i]);
    }

    for (i = 0; i < 30; i++) {
        C[j] = A[i];
        j++;
        C[j] = B[i];
        j++;
    }

    for (i = 0; i < 60; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");
}