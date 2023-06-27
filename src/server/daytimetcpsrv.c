//
// Created by wenyu on 23-6-20.
//
#include "../lib/unp.h"
#include "../client/protocol.h"

int main(void) {
    person p = {
            .name="wenyu",
            .age=32,
            .number=34,
    };
    student student = {
            .len=500,
            .address="fgelin",
            .salary=37.1f,
    };

    int alen = sizeof(p);
    int a2 = sizeof(student);
    int cmd = 100;
    int cmd2 = 101;
    char *p2 = malloc((alen + HEADER_LEN+a2+HEADER_LEN) );//
    bzero(p2, (alen + HEADER_LEN+a2+HEADER_LEN));


    char *writePoint = p2;
    memcpy(writePoint, &alen, PROTOCOL_LEN);//4bytes
    writePoint += PROTOCOL_LEN;
    memcpy(writePoint,&cmd,CMD_LEN);
    writePoint+=CMD_LEN;
    memcpy(writePoint, &p, alen);
    writePoint += alen;

    memcpy(writePoint, &a2, PROTOCOL_LEN);
    writePoint += PROTOCOL_LEN;
    memcpy(writePoint,&cmd2,CMD_LEN);
    writePoint+=CMD_LEN;
    memcpy(writePoint, &student, a2);
    writePoint+=a2;
    printf("sizeof p = %d\n", alen);
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    //open  a  socket and return a FD
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);
    //bind address
    printf("listenfd =%d\n",listenfd);
//    Close(listenfd);
    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

    for (;;) {

        connfd = Accept(listenfd, (SA *) NULL, NULL);

        Write(connfd, p2, writePoint-p2);
        Close(connfd);
    }
}