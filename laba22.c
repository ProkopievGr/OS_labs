#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 2048
#define FILE_NAME "users.txt"
#define PATH "/dev/"

sigjmp_buf obl;
int interupt_counter = 0;
char term_names[BUF_SIZE];

void handler(){
    printf("\nInteruption!\n");
    interupt_counter++;
    if(interupt_counter % 5 == 0) {
        printf("%s", term_names);
    }
    siglongjmp(obl, 1);
}

void main(int argc, char **argv){
    struct sigaction interupt;
    interupt.sa_handler = handler;
    sigaction(SIGINT, &interupt, 0);


    int fd;
    char buf[BUF_SIZE];
    char ttyname[80];
    

    if(fork() == 0){
        fd = creat(FILE_NAME, 0666);
        dup2(fd, 1);
        close(fd);
        execl("/usr/bin/who", "who", NULL);
    } else {
        wait(0);
        fd = open(FILE_NAME, O_RDONLY);
        int readed_size = read(fd, buf, BUF_SIZE);

        int j = 0;
        int nameflag = 1;
        int ttyflag = 0;
        char message[80];
        char for_buf[80];
        char ttypath[80];

        sigsetjmp(obl, 1);
        printf("Type your message:\n");
        fgets(message, 80, stdin);
        message[strlen(message) - 1] = '\n';

        char *p1, *p2;
        char *saveptr1, *saveptr2;
        int i = 0;
        int tty_d;
        for(p1 = strtok_r(buf, "\n", &saveptr1); p1; p1 = strtok_r(NULL, "\n", &saveptr1)) {
            for(p2 = strtok_r(p1, " ", &saveptr2), i = 0; p2; p2 = strtok_r(NULL, " ", &saveptr2), ++i) {
                if(i) {
                    strcpy(ttypath, PATH);
                    strcat(ttypath, p2);
                    tty_d = open(ttypath, O_WRONLY);
                    write(tty_d, message, strlen(message));
                    close(tty_d);

                    strcpy(for_buf, p2);
                    for_buf[strlen(for_buf)] = '\n';
                    strcat(term_names, for_buf);
                    break;
                }
            } 
        }


    }
    
    sigsetjmp(obl, 1);
    sleep(1);
    sleep(1);
    sleep(1);
    sleep(1);
    sleep(1);
    printf("Farewell!\n");
}
