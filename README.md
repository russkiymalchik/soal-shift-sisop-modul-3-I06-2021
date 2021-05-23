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


 
