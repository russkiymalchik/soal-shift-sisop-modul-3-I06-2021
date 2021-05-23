#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#define gas 512-212*1
#define hadeh 2

int socketawal = -1;
bool socketakhir = false;
const int ceksize = sizeof(int) * gas;
const int input = sizeof(int)* gas * 1;
int create_socket()
{
    struct sockaddr_in saddr;
    int fd, ret_val;
    int opt = 1;
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        fprintf(stderr, "socket failed [%s]\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7000);
    saddr.sin_addr.s_addr = INADDR_ANY;
    ret_val = bind(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (ret_val != 0) {
        fprintf(stderr, "bind failed [%s]\n", strerror(errno));
        close(fd);
        exit(EXIT_FAILURE);
    }
    ret_val = listen(fd, 5);
    if (ret_val != 0) {
        fprintf(stderr, "listen failed [%s]\n", strerror(errno));
        close(fd);
        exit(EXIT_FAILURE);
    }
    return fd;
}
char validator[hadeh][300]; 
const int SIZE_BUFFER = sizeof(char) * 300;
void *utama(void *argv);
void login(char *buf, int fd);
void daftar(char *buf, int fd);
void add(char *buf, int fd);
void download(char *filename, int fd);
void hapus(char *filename, int fd);
void see(char *buf, int fd, bool isFind);
void runninglog(char *cmd, char *filepath);
int ambilinput(int fd, char *prompt, char *cursor);
int validasi(int fd, char *id, char *password);
int masukkanfile(int fd, char *dirname, char *targetFileName);
int kirim(int fd, char *filename);
char *ceknamafile(char *filePath);
bool loginberhasil(FILE *fp, char *id, char *password);
bool sudahregister(FILE *fp, char *id);
bool sudahdownload(FILE *fp, char *filename);
void pemisahfile(char *filepath, char *namafile, char *ext);

int main(int argc ,char const *argv1[])
{
    socklen_t addrlen;
    struct sockaddr_in new_addr;
    pthread_t tid;
    char buf[300];
    char argv[300 + hadeh];
    int new_fd, ret_val;
    int server_fd = create_socket();

    while (1) {
        new_fd = accept(server_fd, (struct sockaddr *)&new_addr, &addrlen);
        if (new_fd >= 0) {
            printf("Connected to port: %d\n", new_fd-3);
            pthread_create(&tid, NULL, &utama, (void *) &new_fd);
        } else {
            fprintf(stderr, "Connection failed. %s\n", strerror(errno));
        }
    }
    return 0;
}

void *utama(void *argv)
{
    int fd = *(int *) argv;
    char cmd[300];
    chdir("/home/hilmyhnf/modul3/soal1/Server");

    while (recv(fd, cmd, 300, MSG_PEEK | MSG_DONTWAIT) != 0) {
        if (fd != socketawal) {
            if (ambilinput(fd, "\nWelcome. Please login / register\n = ", cmd) == 0) break;
            write(fd, "\n", SIZE_BUFFER);
            if (strcmp(cmd, "login") == 0) 
            {
                login(cmd, fd);
            } 
            else if (strcmp(cmd, "register") == 0) 
            {
                daftar(cmd, fd);
            } 
            else 
            {
                send(fd, "Error: Invalid command\n", SIZE_BUFFER, 0);
            }
        } else 
        { 
            char prompt[300];
            strcpy(prompt, "\nAvailable commands(add,downlod,delete,see,find)\n = ");
            if (ambilinput(fd, prompt, cmd) == 0) break;
            write(fd, "\n", SIZE_BUFFER);
            if (strcmp(cmd, "add") == 0) 
            {
                add(cmd, fd);
            } 
            else if (strcmp(cmd, "see") == 0) 
            {
                see(cmd, fd, false);
            }
            else 
            {
                //cekking 
                char *tmp = strtok(cmd, " ");
                char *tmp2 = strtok(NULL, " ");
                if (!tmp2) {
                    send(fd, "Error : Invalid command\n", SIZE_BUFFER, 0);
                } 
                else if (strcasecmp(tmp, "download") == 0) {
                    download(tmp2, fd);
                } 
                else if (strcasecmp(tmp, "delete") == 0) {
                    hapus(tmp2, fd);
                }
                else if (strcasecmp(tmp, "find") == 0) {
                    see(tmp2, fd, true);
                }
                else {
                    send(fd, "Error: Invalid command\n", SIZE_BUFFER, 0);
                }
            }
        }
        sleep(0.001);
    }
    if (fd == socketawal) 
    {
        socketawal = -1;
    }
    printf("Disconnected at port: %d\n", fd-3);
    close(fd);
}
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

void runninglog(char *cmd, char *filename)
{
    FILE *fp = fopen("running.log", "a+");
    cmd = (strcmp(cmd, "add") == 0) ? "Tambah" : "Hapus";
    fprintf(fp, "%s : %s (%s:%s)\n", cmd, filename, validator[0], validator[1]);
    fclose(fp);
}

void pemisahfile(char *filepath, char *namafile, char *ext)
{
    char *temp;
    if (temp = strrchr(filepath, '.')) strcpy(ext, temp + 1);
    else strcpy(ext, "-");
    strcpy(namafile, ceknamafile(filepath));
    strtok(namafile, ".");
}
//download
int kirim(int fd, char *filename)
{
    char buf[300] = {0};
    int ret_val;
    printf("Sending file %s to client ...\n", filename);
    strcpy(buf, filename);
    //mengirimkan ke FILES
    sprintf(filename, "FILES/%s", buf);
    FILE *fp = fopen(filename, "r");

    if (!fp) {
        //cek apabila file tidak ada
        printf("File does not exist.\n");
        send(fd, "File does not exist.\n", SIZE_BUFFER, 0);
        return -1;
    }
    send(fd, "Sending file ...\n", SIZE_BUFFER, 0);
    send(fd, buf, SIZE_BUFFER, 0);

    // Transfer size
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    rewind(fp);
    sprintf(buf, "%d", size);
    send(fd, buf, SIZE_BUFFER, 0);

    while ((ret_val = fread(buf, 1, 300, fp)) > 0) {
        send(fd, buf, ret_val, 0);
    }
    recv(fd, buf, 300, 0);
    printf("File sent.\n");
    fclose(fp);
    return 0;
}

char *ceknamafile(char *filePath)
{
    char *ret = strrchr(filePath, '/');
    if (ret) return ret + 1;
    else return filePath;
}

int masukkanfile(int fd, char *dirname, char *targetFileName)
{
    int ret_val, size;
    char buf[300] = {0};
    char in[1];

    // Make sure that client has the file
    ret_val = recv(fd, buf, 300, 0);
    if (ret_val == 0 || strcmp(buf, "File found.") != 0) {
        if (ret_val == 0) printf("Disconnected from client.\n");
        else puts(buf);
        return -1;
    }
    recv(fd, buf, SIZE_BUFFER, 0);
    size = atoi(buf);

    printf("Sending file %s to server ...\n", targetFileName);
    sprintf(buf, "%s/%s", dirname,targetFileName);
    FILE *fp = fopen(buf, "w+");

    while (size-- > 0) {
        if ((ret_val = recv(fd, in, 1, 0)) < 0)
            return ret_val;
        fwrite(in, 1, 1, fp);
    }
    ret_val = 0;
    printf("File sent to server.\n");
    fclose(fp);
    return ret_val;
}

int validasi(int fd, char *id, char *password)
{
    if (ambilinput(fd, "Enter Username = ", id) == 0) return 0;
    if (ambilinput(fd, "Enter Password = ", password) == 0) return 0;
    return 1;
}

int ambilinput(int fd, char *prompt, char *cursor)
{
    send(fd, prompt, SIZE_BUFFER, 0);

    // Get input
    int count, ret_val;
    ioctl(fd, FIONREAD, &count);
    count /= 300;
    for (int i = 0; i <= count; i++) {
        ret_val = recv(fd, cursor, 300, 0);
        if (ret_val == 0) return ret_val;
    }
    while (strcmp(cursor, "") == 0) {
        ret_val = recv(fd, cursor, 300, 0);
        if (ret_val == 0) return ret_val;
    }
    printf("Command Client = %s\n", cursor);
    return ret_val;
}

//login dimasukkan ke file
bool loginberhasil(FILE *fp, char *id, char *password)
{
    char db[300], input[300];
    sprintf(input, "%s:%s", id, password);
    while (fscanf(fp, "%s", db) != EOF) {
        if (strcmp(db, input) == 0) return true;
    }
    return false;
}

//cek sudah regist apa belum
bool sudahregister(FILE *fp, char *id)
{
    char db[300], *tmp;
    while (fscanf(fp, "%s", db) != EOF) {
        tmp = strtok(db, ":");
        if (strcmp(tmp, id) == 0) return true;
    }
    return false;
}

bool sudahdownload(FILE *fp, char *filename)
{
    char db[300], *tmp;
    while (fscanf(fp, "%s", db) != EOF) {
        tmp = ceknamafile(strtok(db, "\t"));
        if (strcmp(tmp, filename) == 0) return true;
    }
    return false;
}
