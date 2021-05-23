# soal-shift-sisop-modul-3-I06-2021
## Group Member Name:
1. Hilmy Hanif Ibrahim                  (05111942000005)
2. Juan Carlos Tepanus Pardosi          (05111942000017)
3. Khairi Wiryawan    				          (05111942000023)
## Problem 1

## Problem 2

Crypto (you) is Loba's friend. One morning, Crypto saw Loba who was overwhelmed with his boss's assignment. Since Crypto is such a challenging person, he wanted to help Loba with his job. The details of the assignment are:

### 2a
Create a matrix multiplication program (4x3 and 3x6) and then display the results. The matrix will contain the numbers 1-20 (no need to create a number filter).

### Problem Solving
First, we need to scan all the input from both of two matrix. 

- For matrix 1, it is in the form of M1(4x3).

- For matrix 2, it is in the form of M2(3x6).

```
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
```
Second, we make a calculation to multiply both of two matrix. Because M1(4x3) and M2(3x6), so the result will be in the form of Mr(4x6).

- Mr = Matrix result

```
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
```
Last, we print the result of multiplication matrix. At the bottom, `sleep(10)` is used to pending the program to copy the result into problem 2b.
```
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
```
Below is the result of 2a

### 2b
Create a program using the output matrix of the previous program (program soal2a.c) (Note!: Use shared memory). Then the matrix will be calculated with the new matrix. As follows an example of calculation for the existing matrix. The calculation is that each cell originating from matrix A becomes a number for factorial, then cells from matrix B become the maximum factorial limit (from largest to smallest) (Note!: Use threads for calculations in each cell).

Below is the result of 2b

### 2c
For fear of lags in the process of helping Loba, Crypto also created a program (soal2c.c) to check the top 5 processes consuming computer resources with the command  “ps aux | sort -nrk 3,3 | head -5” (Note !: You must use IPC Pipes).

Below is the result of 2c

