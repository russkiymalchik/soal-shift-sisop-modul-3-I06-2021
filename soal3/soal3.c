
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>

//check the existance of a file
int check(const char *fname){
  struct stat buffer;
  int exist = stat(fname, &buffer);
  
  if(exist == 0) return 1;
  else return 0;
}

//move file
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

//List files (recursive)
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

int main(int argc, char *argv[]){
  
  char cwd[1000];

  //3a -f
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
  }
