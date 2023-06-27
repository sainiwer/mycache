//
// Created by wenyu on 23-6-23.
//

#ifndef C_LANGUAGE_PROTOCOL_H
#define C_LANGUAGE_PROTOCOL_H
typedef struct {
    char name[10];
    int age;
    int number;
}__attribute__((packed)) person;

typedef struct {
    int len;
    char address[20];
    float salary;
}__attribute__((packed)) student;

#define HEADER_LEN 8
#define CMD_LEN 4
#define PROTOCOL_LEN 4
#define RECIVEBUFF_LEN 1024
/*
 * term candidate’s term
*  candidateId candidate requesting vote
    lastLogIndex index of candidate’s last log entry (§5.4)
   lastLogTerm term of candidate’s last log entry (§5.4)
 */
typedef struct {
    unsigned long long termID;
    unsigned char candidateId;
    unsigned int lastLogIndex;
    unsigned long long lastLogTerm;
}__attribute__((packed)) voteRpc;



typedef struct {
    void *readStart;
    void *BodyInfo;
}__attribute__((packed)) parse_return;

/*
 * Arguments:
term leader’s term
leaderId so follower can redirect clients
prevLogIndex index of log entry immediately preceding
new ones
prevLogTerm term of prevLogIndex entry
entries[] log entries to store (empty for heartbeat;
may send more than one for efficiency)
leaderCommit leader’s commitIndex
Results:
term currentTerm, for leader to update itself
success true if follower contained entry matching
prevLogIndex and prevLogTerm

 */
typedef struct {

}__attribute__((packed)) AppendOnlyLog;
extern void *parseExec(void *pReadStart, int cmd, int *flag, int len);

extern void registerCmdToFunc();

void *pareseHeader(int *len, int *cmd, int *flag, void *readStart);

typedef void (*pFUNC)(void *readStart, parse_return *, int *flag, int len);

extern void parsevoteRpc(void *readStart, parse_return *, int *flag, int len);

extern void parseAppenLog(void *readStart, parse_return *, int *flag, int len);

extern void deal(void *p, void *readStart, parse_return *ret, int *flag, int len);

#define PARES_VOTE_RPC 100
#define PARES_APPEND_LOG 101

#endif //C_LANGUAGE_PROTOCOL_H
