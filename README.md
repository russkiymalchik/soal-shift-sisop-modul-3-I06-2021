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

![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal2/result2a.png)

### 2b
Create a program using the output matrix of the previous program (program soal2a.c) (Note!: Use shared memory). Then the matrix will be calculated with the new matrix. As follows an example of calculation for the existing matrix. The calculation is that each cell originating from matrix A becomes a number for factorial, then cells from matrix B become the maximum factorial limit (from largest to smallest) (Note!: Use threads for calculations in each cell).

### Problem Solving

First, we make two function:
- func1 is used when receive only one parameter
- func2 is used when receive two parameters
```
pthread_t tid[24];
int matA[4][6];
int matB[4][6];

unsigned long long result[4][6];
int (*value)[6];

int func1(int a)
{
    if(a >= 1)
    {
        return a*func1(a-1);
    }
    else
    {
        return 1;
    }
}

int func2(int b, int a)
{
    int tot = 1;
    for(int i = b ; i > a ; i--)
    {
        tot = tot * i;
    }
    return tot;
}
```
Second, we make another function to calculate the factorial operation. In this source code it's named as void *operand.

According to the problem, it has 3 condition:
1. If 0 -> 0
```
if(matA[i][j] == 0 || matB[i][j] == 0)
	{ result[i][j] = 0; }
```

2. If b > a -> a!

It will called func1
```
if(matB[i][j] > matA[i][j])
	{ result[i][j] = func1(matA[i][j]); }
```

3. If a >= b  -> a!/(a-b)!

It will called func2
```
if(matA[i][j] >= matB[i][j])
	{ result[i][j] = func2(matA[i][j], (matA[i][j] - matB[i][j])); }
```
Next, called the result from problem 2a when they (program 2a) sleep.
```
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
```
After that, declare the new matrix that will be the length of factorial matrix from problem 2a and print it.
```
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
```
Create and join the thread. At last, print the result of problem 2b.
```
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
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 6; j++) 
        {
            printf("%llu\t", result[i][j]);
        }
        printf("\n");
    }

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}
```
Below is the result of 2b

![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal2/result2b.png)

### 2c
For fear of lags in the process of helping Loba, Crypto also created a program (soal2c.c) to check the top 5 processes consuming computer resources with the command  “ps aux | sort -nrk 3,3 | head -5” (Note !: You must use IPC Pipes).

### Problem Solving

First, we make two pipes:
- fd1 is a pipe to connect the result from ps aux -> sort -nrk 3,3
- fd2 is a pipe to connect the result from sort -nrk 3,3 -> head -5
```
int main() 
{ 
	int fd1[2]; // Used to store two ends of first pipe 
	int fd2[2]; // Used to store two ends of second pipe 

	char input_str[100]; 
	pid_t p; 

	if (pipe(fd1)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	if (pipe(fd2)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
```
Second, make a fork that  contain parent process and child process. In parent process, we operate the "ps" "aux" command.
```
	p = fork(); 
	if (p < 0) 
	{ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	} 
		// Parent process 
		if (p == 0) 
		{
		close(fd1[0]); // Close reading end of first pipe 
	
	        dup2(fd1[1], STDOUT_FILENO);
       		char *argv[] = {"ps", "aux", NULL};
       		execv("/bin/ps", argv);
       		exit(EXIT_SUCCESS);
		} 
```
Next, in the child process, use the pipe to connect the 3 command (ps aux | sort -nrk 3,3 | head -5).
```
    	else 
    	{
		wait(NULL);
		p = fork();
		if (p < 0) 
        	{
			exit(EXIT_FAILURE);
	    	}

	    	if (p == 0)
        	{
			close(fd1[1]); // Close writing end of first pipe 
            		dup2(fd1[0], STDIN_FILENO);
		    
            		close(fd2[0]); // Close reading end of second pipe 
		    	dup2(fd2[1], STDOUT_FILENO);
		    
            		char *argv[] = {"sort", "-nrk 3,3", NULL};
		    	execv("/usr/bin/sort", argv);
            		exit(EXIT_SUCCESS);
	    	}

		else
		{
			close(fd2[1]); // Close writing end of second pipe 
            		close(fd1[1]); // Close writing end of first pipe
            		close(fd1[0]); // Close reading end of first pipe
		    
            		wait(NULL);
		    
            		dup2(fd2[0], STDIN_FILENO);
			char *argv[] = {"head", "-5", NULL};
			execv("/usr/bin/head", argv);
            		exit(EXIT_SUCCESS);
	  	}
	}
```

Below is the result of 2c

![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal2/result2c.png)

## Problem 3
During his time of inactivity, a student named Alex. He had an idea to tidy up a number of files on his laptop. Because there are too many files, Alex asked Ayub for advice.
Ayub suggests creating a C program to categorize the files. This program will move files according to their extension into a folder according to the extension. The folder is in the working directory in which the categorization program is run.

### 3a
Program accepts -f option like stated above, with this option the user may add file
arguments to be categorized as much as they want.

Output:

```
File 1 : Berhasil Dikategorikan (jika berhasil)
File 2 : Sad, gagal :( (jika gagal)
File 3 : Berhasil Dikategorikan
```

### 3b
Program may accept -d option to categorize a directory instead. With this option,
the user may only input 1 directory as it's arguments, unlike the -f option where
the user may input multiple file arguments. 

Example:

```
$ ./kategori -d /path/to/directory/
```

The command above will categorize the files in /path/to/directory, the
categorization result will be saved in the working directory where the C program
is called (categorization result is not located at /path/to/directory).

Output:

```
Jika berhasil, print “Direktori sukses disimpan!”
Jika gagal, print “Yah, gagal disimpan :(“
```

### 3c
Other than the above options, this program accepts the * option. Example:

```
$ ./kategori \*
```

This option categorizes all the file in the working directory when the C program is
run

### 3d
All files must be moved into a folder. Files without extensions will be moved to a
folder named "Unknown". Hidden files will be moved to a folder named "Hidden".

### 3e
Each file to be categorized will be processed by a thread to make the program
run in parallel to make it quicker.

### Answer
There are 3 functions + 1 main function in this code.

The ```check(const char *fname)``` is made to check the existence of a file

```
int check(const char *fname){
  struct stat buffer;
  int exist = stat(fname, &buffer);
  
  if(exist == 0) return 1;
  else return 0;
}
```

The ```*move(void *fname)``` play big role in moving all of these file into respective directory

```
void *move(void *fname)
{
  int i;
  char cwd[1000];
  char h[1000]; 
  char h_name[1000];
  char dname[1000];  
  char file[1000]; 
  char fileexist[1000];

  strcpy(fileexist, fname);
  strcpy(h_name, fname);
  char *hname = strrchr(h_name, '/');
  strcpy(h, h_name);

  //3d. hidden
  if(h[1] == '.'){
    strcpy(dname, "h");
  }

  //regular files
  else if(strstr(fname, ".") != NULL){
    strcpy(file, fname);
    strtok(file, ".");
    char *token = strtok(NULL, "");

    //insensitive case
    for(i = 0; token[i]; i++){
      token[i] = tolower(token[i]);
    }

    strcpy(dname, token);
  }

  //3d. files w/o extension
  else{
    strcpy(dname, "Unknown");
  }

  int exist = check(fileexist);

  if(exist){
    mkdir(dname, 0755);
  }
    
  //get the name of the file
  if (getcwd(cwd, sizeof(cwd)) != NULL){
    char *nama = strrchr(fname, '/');
    char namafile[200];
    strcpy(namafile, cwd);
    strcat(namafile, "/");
    strcat(namafile, dname);
    strcat(namafile, nama);

    //Function to move the file
    rename(fname, namafile);
  }
}
```

The last is ```list(char *basePath)``` which made to list the files in a recursive way

```
void list(char *basePath){
  int n = 0;
  char path[1000];
  struct dirent *dp;
  struct stat buffer;
  DIR *dir = opendir(basePath);

  if(!dir){
    return;
  }

  while((dp = readdir(dir)) != NULL){
    
    if(strcmp(dp -> d_name, ".") != 0 && strcmp(dp -> d_name, "..") != 0){
      //Construct new path from our base path
      strcpy(path, basePath);
      strcat(path, "/");
      strcat(path, dp -> d_name);

      if(stat(path, &buffer) == 0 && S_ISREG(buffer.st_mode)){
        //Make thread to check move function
        pthread_t thread;
        int err = pthread_create(&thread, NULL, move, (void *)path);
        pthread_join(thread, NULL);
      }

      list(path);
    }
  }

  closedir(dir);
}
```

in the main function, for answering the question 3a here are the answer.

```
if (strcmp(argv[1], "-f") == 0){
    pthread_t thread;
    int i;

    //thread and output
    for (i = 1; i < argc; i++){
        char message[1000];
        int exist = check(argv[i]);
        if (exist) sprintf(message, "File %d : Berhasil Dikategorikan", i);
        else sprintf(message, "File %d : Sad, gagal :(", i);
            
        printf("%s\n", message);
        int err = pthread_create(&thread, NULL, move, (void *)argv[i]);
    }

    //Join thread
    pthread_join(thread, NULL);
  }
```
using thread to make the process become paralel, ended by joining the thread.

for 3b and 3c, the folowing code is the answer.

```
else{
    //3c. *
     if (strcmp(argv[1], "*") == 0){
      if (getcwd(cwd, sizeof(cwd)) != NULL){
        //Open the working directory
        list(cwd);
      }
    }

    //3b. -d
    else if (strcmp(argv[1], "-d") == 0){
      //Open directory according to the second argument
      list(argv[2]);
      struct stat buffer;
      int err = stat(argv[2], &buffer);

      if(err == -1)printf("Yah, gagal disimpan :(\n");
      else printf("Direktori sukses disimpan!\n");
      
      
      }

      else
      {
        printf("Direktori sukses disimpan!\n");
      }
    }
 ```
 
 here, for the 3b and 3c, we utilise the ```list(char *basePath)``` function which is a requirement of the question to solve this in recursive way.
 
 ### Obstacle
 In testing for 3a, even though I trying 2 file as the test case, the output written that there is 3 file that have been checked which is wrong.

![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal3ss/lab%203a.jpg)

### Question 3 Screenshot

Before 3b testcase was run
![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal3ss/before%203b.jpg)

after 3b testcase was run
![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal3ss/after%203b.jpg)

before 3c testcase was run
![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal3ss/before%203c.jpg)

after 3c testcase was run
![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal3ss/after%203c.jpg)

 
