#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{ 
    char buff[4096];
    struct sockaddr_in servaddr, cliaddr;
    int servfd, connfd;
    FILE *sh;

    memset(&buff, 0, sizeof(buff));
    memset(&servaddr, 0, sizeof(struct sockaddr_in));

    servfd = socket(AF_INET, SOCK_STREAM, 0);
    if(servfd < 0){
        printf("%s\n", "couldnt create socket\n");
        exit(0);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = 8080;

    if(bind(servfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in)) == -1){
        printf("%s\n", "couldnt bind a socket\n");
    }

    if(listen(servfd, 10) == -1){
        printf("%s\n", "couldnt listen\n");
    }

    while(1){
        memset(&buff, 0, sizeof(buff));
        sh = popen("find . -maxdepth 1 -mtime -1 -printf \"%f:%m\\n\" | tail -n+2", "r");
        fread(buff, 1, 4096, sh);
        pclose(sh);

        socklen_t cliaddr_size = sizeof(struct sockaddr_in);
        connfd = accept(servfd, (struct sockaddr*)&cliaddr, &cliaddr_size);
        if(connfd == -1){
            printf("%s\n", "couldnt accept\n");
            continue;
        }

        size_t nsent;
        nsent = send(connfd, buff, strlen(buff)+1, 0);

        printf("bytes sent: %ld\n", nsent);
        printf("data sent:\n%s\n", buff);

        memset(buff, 0, sizeof(buff));
        recv(connfd, buff, 4096, 0);

        if(!strcmp(buff, "closecon")){
            close(connfd);
            break;
        }

        printf("feedback:\n%s\n", buff);

        close(connfd);
    }
    close(servfd);
    return 0;
}
