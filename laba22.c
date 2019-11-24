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

sigjmp_buf obl;
int interupt_counter = 0;
char term_names[BUF_SIZE];


void handler(){
    printf("\nInteruption!\n");
    interupt_counter++;
    if(interupt_counter % 5 == 0) {
        // print names of terminals received the message
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
    char username[80], ttyname[80];

    if(fork() == 0){
        fd = creat(FILE_NAME, 0666);
        dup2(fd, 1);
        execl("/usr/bin/who", "who", NULL);
        close(fd);
    } else {
        wait(0);
        fd = open(FILE_NAME, O_RDONLY);
        int readed_size = read(fd, buf, BUF_SIZE);

        int j = 0;
        int nameflag = 1;
        int ttyflag = 0;
        char message[80];

        sigsetjmp(obl, 1);
        printf("Type your message:\n");
        fgets(message, 80, stdin);
        message[strlen(message) - 1] = '\0'; 
        

        for(int i = 0; i < readed_size; ++i) {
            if(nameflag) {
                if(buf[i] != ' ') {
                    username[j] = buf[i];
                    j++;
                } else {
                    nameflag = 0;
                    username[j] = '\0';
                    j = 0;
                    ttyflag = 1;
                    i += 2;
                }
            } 
            if(ttyflag) {
                if(buf[i] != ' ') {
                    ttyname[j] = buf[i];
                    j++;
                } else {
                    char for_buf[80];
                    strcpy(for_buf, ttyname);
                    for_buf[j] = '\n';
                    strcat(term_names, for_buf);

                    ttyname[j] = '\0';
                    j = 0;
                    ttyflag = 0;
                    char cmd[128];
                    strcpy(cmd, "echo ");
                    strcat(cmd, message);
                    strcat(cmd, " | write ");
                    strcat(cmd, username);
                    strcat(cmd, " ");
                    strcat(cmd, ttyname);
                    system(cmd);

                }
            }
            if(buf[i] == '\n') {
                nameflag = 1;
            }

        }


    }
    sigsetjmp(obl, 1);
    sleep(1);
    sleep(1);
    sleep(1);
    printf("Farewell!\n");
}
