#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

pthread_t tid[24];
int matA[4][6];
int matB[4][6];

unsigned long long result[4][6];
int (*value)[6];

int func1 (int a)
{
    if (a >= 1)
    {
        return a*func1(a-1);
    }
    else
    {
        return 1;
    }
}

int func2 (int b, int a)
{
    int tot = 1;
    for (int i = b ; i > a ; i--)
    {
        tot = tot * i;
    }
    return tot;
}

void *operand (void *arg) 
{
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<6; j++) 
        {
            pthread_t id = pthread_self();
            int iter = 0;
            if (pthread_equal(id, tid[iter])) 
            {
                if (matA[i][j] == 0 || matB[i][j] == 0)
                { result[i][j] = 0; }

                if (matB[i][j] > matA[i][j])
                { result[i][j] = func1(matA[i][j]); }

                if (matA[i][j] >= matB[i][j])
                { result[i][j] = func2(matA[i][j], (matA[i][j] - matB[i][j])); }

                iter++;
            }
        }
    }
}

void main() {
    key_t key = 1234;
    
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    printf("Matrix 2A:\n");
    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 6; ++j) 
        {
            matA[i][j] = value[i][j];
            printf("%d\t", matA[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    int b[4][6] = 
    { 
        {14, 2, 3, 8, 8, 10},
        {7, 4, 8, 5, 14, 9},
        {9, 2, 13, 5, 11, 2},
        {8, 7, 10, 4, 10, 8}
    };

    printf("Matrix 2B:\n");
    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 6; ++j) 
        {
            matB[i][j] = b[i][j];
            printf("%d\t", matB[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 6; ++j) 
        {
            pthread_create(&tid[6*i + j], NULL, &operand, NULL);
        }
    }

    for (int i = 0; i < 4; ++i) 
    {
        for (int j = 0; j < 6; ++j) 
        {
            pthread_join(tid[6*i + j], NULL);
        }
    }
    
    printf("\nThe Result from Factorial Matrix are:\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++) 
        {
            printf("%llu\t", result[i][j]);
        }
        printf("\n");
    }

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}
