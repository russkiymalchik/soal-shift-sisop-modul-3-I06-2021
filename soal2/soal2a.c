#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

void main() {
    key_t key = 1234;
    int *value;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int matrix1[4][3];
    int matrix2[3][6];

	int i;
	int j;
	int k;

    printf("Input Matrix 4x3:\n");
    for (i = 0; i < 4; ++i) 
    {
        for (j = 0; j < 3; ++j) 
        {
            scanf("%d", &matrix1[i][j]);
        }
    }
    
    printf("Input Matrix 3x6:\n");
    for ( i = 0; i < 3; ++i) 
    {
        for ( j = 0; j < 6; ++j) 
        {
            scanf("%d", &matrix2[i][j]);
        }
    }

    int result = 0;
    for ( i = 0; i < 4; ++i) 
    {
        for ( j = 0; j < 6; ++j) 
        {
            for ( k = 0; k < 3; ++k) 
            {
                result = result + matrix1[i][k] * matrix2[k][j];
            }
            *(value + 6*i + j) = result;
            result = 0;
        }
    }

    printf("Your Matrix 4x3 is:\n");

    for ( i = 0; i < 4; ++i) 
    {
        for ( j = 0; j < 3; ++j) 
        {
            printf("%d\t", matrix1[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("Your Matrix 3x6 is:\n");

    for ( i = 0; i < 3; ++i) 
    {
        for ( j = 0; j < 6; ++j) 
        {
            printf("%d\t", matrix2[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("Multiplication Matrix:\n");
    
    for ( i = 0; i < 4; ++i) 
    {
        for ( j = 0; j < 6; ++j) 
        {
            printf("%d\t", *(value + 6*i + j));
        }
        printf("\n");
    }

    sleep(10);

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}
