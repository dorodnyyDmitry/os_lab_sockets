#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

void input_handle(char* buff, int sockfd){
    /*char *found, *temp;
    char parse_result[4096];

    while( (found = strsep(&buff, "\n")) != NULL){
        temp = strsep(&found, ":");

        if(strsep(&found, ":")[0] != '7'){
            strncpy(parse_result + strlen(parse_result), temp, strlen(temp));
            parse_result[strlen(parse_result)] = '\n';
        }
        if(strlen(buff) == 0){
            break;
        }
    }*/

    char parse_result[4096];

    FILE* sh = popen("ps", "r");
    fread(parse_result, 1, 4096, sh);

    send(sockfd, parse_result, strlen(parse_result), 0);
}

int main(){
    int sockfd;
    char buff[4096];
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("%s\n", "couldnt create socket\n");
        exit(0);
    }

    memset(&servaddr, 0, sizeof(struct sockaddr_in));
    memset(&buff, 0, sizeof(buff));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = 8080;

    while(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in)) == -1){
        sleep(1);
    }

    recv(sockfd, buff, 4096, 0);

    input_handle(buff, sockfd);

    close(sockfd);
}

