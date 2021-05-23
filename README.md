# soal-shift-sisop-modul-3-I06-2021
## Group Member Name:
1. Hilmy Hanif Ibrahim                  (05111942000005)
2. Juan Carlos Tepanus Pardosi          (05111942000017)
3. Khairi Wiryawan    				          (05111942000023)
## Problem 1
Keverk is a famous ambitious person in his colleagues. Before he became the head of department in HTMC, he has done a project which tells him to make a book server database. This project is required, so that it could be used by the app owner and is expected from the users. 

In this project, Keverk is asked to: 
### 1a
When a client is connected to the server, there are two main options. They are register and login. If the user has chosen the register option, the client will prompt an id and password to be sent to the server. Users can also login to the server. Login is successful if the id and the password sent to the server matched with one of the accounts listed on the server application. This system can accepts multiple connections at once. A connection is counted when a client application is connected to the server. If there are 2 connections or more then we must wait for the first client to exit the application to be able to login and use the application. Keverk wanted the file that stores the id and password named account.txt with the following format :
**akun.txt**
```
id:password
id2:password2
```
Answer :
```
//client
int main(int argc, char const *argv[])
{
    pthread_t tid[2];
    int fdc = create_socket();
    //bisa menghandle client banyak
    //minta masukka username
    pthread_create(&(tid[0]), NULL, &cekoutput, (void *) &fdc);
    //minta masukkan email
    pthread_create(&(tid[1]), NULL, &cekinput, (void *) &fdc);
    //joinkan
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    close(fdc);
    return 0;
}
```

```
//register
void daftar(char *messages, int fd)
{
    char id[300], password[300];
    FILE *fp = fopen("akun.txt", "a+");
    if (validasi(fd, id, password) != 0) {
        if (sudahregister(fp, id)) {
            send(fd, "Username already exist.\n", SIZE_BUFFER, 0);
        } else {
            fprintf(fp, "%s:%s\n", id, password);
            send(fd, "Account registration successful.\n", SIZE_BUFFER, 0);
        }
    }
    fclose(fp);
}
//login
void login(char *messages, int fd)
{
    if (socketawal != -1) {
        send(fd, "Server is busy. Please wait ...\n", SIZE_BUFFER, 0);
        return;
    }
    //buka akun
    char id[300], password[300];
    FILE *fp = fopen("akun.txt", "a+");
    //cek apakah berhasil
    if (validasi(fd, id, password) != 0) {
        if (loginberhasil(fp, id, password)) {
            send(fd, "Login successful.\n", SIZE_BUFFER, 0);
            socketawal = fd;
            strcpy(validator[0], id);
            strcpy(validator[1], password);
        } else {
            send(fd, "Incorrect username / password\n", SIZE_BUFFER, 0);
        }
    }
    fclose(fp);
}
```
Result :
![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal1/result1a.png)

### 1b
The system has a database on a file named files.tsv. The content of the file is the file path on the server, publisher, and the year of publication. For every insertion and removal of a file in the folder FILES on the server, the file files.tsv will be affected. Folder FILES are made, automatically when the server is started

Answer :
```
void add(char *messages, int fd)
{
    char *dirName = "FILES";
    char publisher[300], year[300], client_path[300];
    sleep(0.001);
    if (ambilinput(fd, "Publisher: ", publisher) == 0) return;
    if (ambilinput(fd, "Publication Year: ", year) == 0) return;
    if (ambilinput(fd, "Filepath: ", client_path) == 0) return;
    FILE *fp = fopen("files.tsv", "a+");
    char *fileName = ceknamafile(client_path);
    if (sudahdownload(fp, fileName)) {
        send(fd, "File uploaded already exist.\n", SIZE_BUFFER, 0);
    } else {
        send(fd, "Sending file ...\n", SIZE_BUFFER, 0);
        mkdir(dirName, 0777);
        if (masukkanfile(fd, dirName, fileName) == 0) {
            fprintf(fp, "%s\t%s\t%s\n", client_path, publisher, year);
            printf("File sent.\n");
            runninglog("add", fileName);
        } else {
            printf("Error occured when receiving file\n");
        }
    }
    fclose(fp);
}
```
Result :
![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal1/result1b.png)

### 1c
Not only that, Keverk is asked to make a feature such that clients are able to insert new files to the server. FILES directory have a structure as follows : 
Direktori **FILES**
```
File1.extension
File2.extension
```
First, the client send input to the server with the structure as follows:
Client command example :
```
add
```
Client console output :
```
Publisher :
Publication Year :
Filepath :
```
Then, we can fill the book’s data (please note that Filepath is the path to the file that will be sent to the server). Then the client will send the file to the server via socket. When a file is received at the server, a new row containing new data of the book is then added to the files.tsv.

Answer :
```
void add(char *messages, int fd)
{
    char *dirName = "FILES";
    char publisher[300], year[300], client_path[300];
    sleep(0.001);
    if (ambilinput(fd, "Publisher: ", publisher) == 0) return;
    if (ambilinput(fd, "Publication Year: ", year) == 0) return;
    if (ambilinput(fd, "Filepath: ", client_path) == 0) return;
    FILE *fp = fopen("files.tsv", "a+");
    char *fileName = ceknamafile(client_path);
    if (sudahdownload(fp, fileName)) {
        send(fd, "File uploaded already exist.\n", SIZE_BUFFER, 0);
    } else {
        send(fd, "Sending file ...\n", SIZE_BUFFER, 0);
        mkdir(dirName, 0777);
        if (masukkanfile(fd, dirName, fileName) == 0) {
            fprintf(fp, "%s\t%s\t%s\n", client_path, publisher, year);
            printf("File sent.\n");
            runninglog("add", fileName);
        } else {
            printf("Error occured when receiving file\n");
        }
    }
    fclose(fp);
}
```
result :
![alt text}(https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal1/result1c.png)

### 1d
Clients can also download files that exist in the FILES folder in the server, then the system must send the following file to the client. Server must check from files.tsv to check if the file requested is valid. If it’s not valid, the system sends an error message back to the client. If valid, the following file is then sent to the client in the client folder.

Client command example :
```
delete TEMPfile.pdf
```

Answer :
```
void download(char *filename, int fd)
{
    FILE *fp = fopen("files.tsv", "a+");
    if (sudahdownload(fp, filename)) {
        kirim(fd, filename);
    } else {
        send(fd, "File does not exist.\n", SIZE_BUFFER, 0);
    }
    fclose(fp);
}
```
result :
![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal1/result1d.png)

### 1e
After that, the client can also delete file that is stored on the server. But keverk is afraid that the file that is going to be deleted is an important file, so instead of deleting the file, the program just changes its name to ‘old-FileName.extension’. When the filename is changed, the row of the file in files.tsv will be deleted.

Client command example :
```
delete TEMPfile.pdf
```

Answer : 
```
void hapus(char *filename, int fd)
{
    //buka file
    FILE *fp = fopen("files.tsv", "a+");
    char db[300], currFilePath[300], publisher[300], year[300];
    if (sudahdownload(fp, filename)) {
        rewind(fp);
        FILE *tmp_fp = fopen("temp.tsv", "a+");
        //buat sebuah temp supaya pada saat pertukaran data tidak berubah2
        while (fgets(db, SIZE_BUFFER, fp)) {
            sscanf(db, "%s\t%s\t%s", currFilePath, publisher, year);
            if (strcmp(ceknamafile(currFilePath), filename) != 0) { 
                fprintf(tmp_fp, "%s", db);
            }
            memset(db, 0, SIZE_BUFFER);
        }
        fclose(tmp_fp);
        fclose(fp);
        remove("files.tsv");
        rename("temp.tsv", "files.tsv");
        char deletedFileName[300];
        sprintf(deletedFileName, "FILES/%s", filename);
        char newFileName[300];
        sprintf(newFileName, "FILES/old-%s", filename);
        rename(deletedFileName, newFileName);
        send(fd, "File successfully deleted.\n", SIZE_BUFFER, 0);
        runninglog("delete", filename);
    } 
    else {
        send(fd, "File download failed.\n", SIZE_BUFFER, 0);
        fclose(fp);
    }
}
```
result :
![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal1/result1e.png)

### 1f
Clients can see all the contents of files.tsv by calling the see command. Output of the command will follow this format. 
Client command example :
```
see
```
Client console output :
```
Name :
Publisher:
Publication year:
File extension : 
Filepath : 
Name:
Publisher:
Publication Year:
File extension: 
Filepath : 
```

Answer :
```
void see(char *buf, int fd, bool isFind)
{
    int counter = 0;
    FILE *src = fopen("files.tsv", "r");
    if (!src) {
        write(fd, "Files.tsv not found\n", SIZE_BUFFER);
        return;
    }
    char temp[300 + 85], namafile[300/3], ext[5],
        filepath[300/3], publisher[300/3], year[10];
        
    while (fscanf(src, "%s\t%s\t%s", filepath, publisher, year) != EOF) {
        pemisahfile(filepath, namafile, ext);
        if (isFind && strstr(namafile, buf) == NULL) continue;
        counter++;
        sprintf(temp, 
            "Name: %s\nPublisher: %s\nPublication Year: %s\nFile Extension: %s\nFilepath: %s\n\n",
            namafile, publisher, year, ext, filepath
        );
        write(fd, temp, SIZE_BUFFER);
        sleep(0.001);
    }
    if(counter == 0) {
        if (isFind) write(fd, "Command not existed in files.tsv\n", SIZE_BUFFER);
        else write(fd, "Data not existed in files.tsv database\n", SIZE_BUFFER);
    } 
    fclose(src);
}
```
result :
![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal1/result1f.png)

### 1g
The client application can also do search by using the find command and a string. It will output the same format as number 6.
Client command example :
```
find TEMP
```

Answer :
```
void see(char *buf, int fd, bool isFind)
{
    int counter = 0;
    FILE *src = fopen("files.tsv", "r");
    if (!src) {
        write(fd, "Files.tsv not found\n", SIZE_BUFFER);
        return;
    }
    char temp[300 + 85], namafile[300/3], ext[5],
        filepath[300/3], publisher[300/3], year[10];
        
    while (fscanf(src, "%s\t%s\t%s", filepath, publisher, year) != EOF) {
        pemisahfile(filepath, namafile, ext);
        if (isFind && strstr(namafile, buf) == NULL) continue;
        counter++;
        sprintf(temp, 
            "Name: %s\nPublisher: %s\nPublication Year: %s\nFile Extension: %s\nFilepath: %s\n\n",
            namafile, publisher, year, ext, filepath
        );
        write(fd, temp, SIZE_BUFFER);
        sleep(0.001);
    }
    if(counter == 0) {
        if (isFind) write(fd, "Command not existed in files.tsv\n", SIZE_BUFFER);
        else write(fd, "Data not existed in files.tsv database\n", SIZE_BUFFER);
    } 
    fclose(src);
}
```
result :
![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal1/result1g.png)

### 1h
Keverk is cautious on insertion and removal of files on the server, so he made a log file for the server named running.log. The content of the log file is following this format
**running.log**
```
Tambah : File1.extension (id:pass)
Hapus : File2.extension (id:pass)
```

Answer :
```
void runninglog(char *cmd, char *filename)
{
    FILE *fp = fopen("running.log", "a+");
    cmd = (strcmp(cmd, "add") == 0) ? "Tambah" : "Hapus";
    fprintf(fp, "%s : %s (%s:%s)\n", cmd, filename, validator[0], validator[1]);
    fclose(fp);
}
```
result :
![alt text](https://github.com/russkiymalchik/soal-shift-sisop-modul-3-I06-2021/blob/main/screenshots/soal1/result1h.png)

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

### Error When Solving Problem 2
1. In problem 2b, the constraint of the result can't use "int" in some testcase. Thus, we use "unsigned long long".
2. In problem 2c, there is a minor error in the pipe before, but now we already fix it.

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
 
