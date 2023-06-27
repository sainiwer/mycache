//
// Created by wenyu on 23-6-23.
//
#include "../lib/unp.h"
#include "protocol.h"

pFUNC funcSet[102400];

void registerCmdToFunc() {
    funcSet[PARES_VOTE_RPC] = parsevoteRpc;
    funcSet[PARES_APPEND_LOG] = parseAppenLog;
}
void parseAppenLog(void *pReadStart, parse_return *ret, int *flag, int len) {
    AppendOnlyLog *p = malloc(sizeof(*p));
    deal(p, pReadStart, ret, flag, len);
}
void parsevoteRpc(void *pReadStart, parse_return *ret, int *flag, int len) {
    voteRpc *p = malloc(sizeof(*p));
    memcpy(p, pReadStart, len);
    deal(p, pReadStart, ret, flag, len);
}



void deal(void *p, void *pReadStart, parse_return *ret, int *flag, int len) {
    memcpy(p, pReadStart, len);
    pReadStart += len;
    *flag = 0;
    ret->readStart = pReadStart;
    ret->BodyInfo = p;
}

void *parseExec(void *readStart, int cmd, int *flag, int len) {
    pFUNC pSpecificFunc = funcSet[cmd];
    parse_return *result = malloc(sizeof(*result));
    (*pSpecificFunc)(readStart, result, flag, len);
    switch (cmd) {
        case PARES_VOTE_RPC: {
            voteRpc *p;
            p = (voteRpc *) result->BodyInfo;
            printf("termID= %llu\n", p->termID);
            break;
        }
        case PARES_APPEND_LOG: {

            student *p;
            p = (student *) result->BodyInfo;
            printf(">>>>>>>>>>>>name= %d\n", p->len);
            break;
        }
    }
    free(result->BodyInfo);
    return result->readStart;
}

//void *pareseCotent(void *readStart, int cmd, int *flag, int len) {
//    if (cmd == 100) {
//        person p;
//        memcpy(&p, readStart, len);
//        readStart += len;
//        *flag = 0;
//        printf("person name=%s--age = %d----number = %d\n", p.name, p.age, p.number);
//        return readStart;
//    } else if (cmd == 101) {
//        student s;
//        memcpy(&s, readStart, len);
//        readStart += len;
//        *flag = 0;
//        printf("student len=%d,address = %s salary = %.2f\n", s.len, s.address, s.salary);
//        return readStart;
//    } else {
//        printf("exit\n");
//        return NULL;
//    }
//}