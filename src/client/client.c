//
// Created by wenyu on 23-6-17.
//
#include "../lib/unp.h"
#include "glib-2.0/glib-object.h"
#include "protocol.h"


//void *pareseCotent(void *readStart, int cmd, int *flag, int len);

int main(int argc, char **argv) {
    registerCmdToFunc();
    int sockfd, n;
    struct sockaddr_in servaddr;
    if (argc != 2)
        err_quit("usage : a.out <IPaddress>");
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);
    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)//sockaddr
        err_sys("connect error");

    int len = 0;
    char *buff = malloc(RECIVEBUFF_LEN);
    char *temp = malloc(RECIVEBUFF_LEN);
    bzero(buff, RECIVEBUFF_LEN);
    bzero(temp, RECIVEBUFF_LEN);
    char *readStart = temp;
    char *orginPtr = temp;
    int flag = 0;
    int cmd = 0;
    while ((n = read(sockfd, buff, RECIVEBUFF_LEN)) > 0) {
        memcpy(temp, buff, n);
        temp = temp + n;//update pointer
        if ((temp - orginPtr) < HEADER_LEN) { //不足以解析头部
            continue;
        }
        for (;;) {
            if (flag == 0) {//解析头部
                readStart = pareseHeader(&len, &cmd, &flag, readStart);
                if (temp - readStart < len)
                    break;
            }
//            readStart = pareseCotent(readStart, cmd, &flag, len);
            readStart = parseExec(readStart, cmd, &flag, len);
            if (readStart == NULL)
                exit(0);
            int remBytes = (int) (temp - (char *) readStart);
            if (remBytes < HEADER_LEN)//rem bytes <
            {
                memcpy(orginPtr, readStart, remBytes);
                temp = orginPtr;
                readStart = temp;
                break;
            }
        }
    }
    if (n < 0)
        err_sys("read_error");
    exit(0);

}

void *pareseHeader(int *len, int *cmd, int *flag, void *readStart) {
    memcpy(len, readStart, PROTOCOL_LEN);
    readStart += PROTOCOL_LEN;
    memcpy(cmd, readStart, CMD_LEN);
    readStart += CMD_LEN;
    *flag = 1;
    return readStart;
}

void *pareseCotent(void *readStart, int cmd, int *flag, int len) {
    if (cmd == 100) {
        person p;
        memcpy(&p, readStart, len);
        readStart += len;
        *flag = 0;
        printf("person name=%s--age = %d----number = %d\n", p.name, p.age, p.number);
        return readStart;
    } else if (cmd == 101) {
        student s;
        memcpy(&s, readStart, len);
        readStart += len;
        *flag = 0;
        printf("student len=%d,address = %s salary = %.2f\n", s.len, s.address, s.salary);
        return readStart;
    } else {
        printf("exit\n");
        return NULL;
    }


}