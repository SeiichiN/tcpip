#define DEBUG
/***********************************************
 * TCPMessage.h -- ヘッダファイル
 * 
 * 関係するファイル
 *		TCPMessageClient.c
 *		TCPMessageServer.c
 *		TransactTCPClient.c
 * 
 * 作成日: 2017.01.25
 * 作成: Seiichi Nukayama
 * *********************************************/
#include <stdio.h>  /* printf() fprintf()に必要 */
#include <sys/socket.h> /* socket() connect() send() recv()に必要 */
#include <arpa/inet.h>  /* sockaddr_in inet_addr()に必要 */
#include <stdlib.h>  /* atoi()に必要 */
#include <string.h>  /* memset() に必要 */
#include <unistd.h>  /* close() に必要 */
#include <netinet/in.h> /* htonl() ntohl() */

/* 送信するデータ */
typedef struct {
	unsigned int dep;  /* deposits 預入金額 */
	unsigned int wd;   /* withdrawals 引出金額 */
} msgStruct;

/* 受信するデータ */
typedef struct {
	unsigned int centsDeposited;
	unsigned int centsWithdrawn;
	unsigned short numDeps;
	unsigned short numWds;
} msgBuf;

void DieWithError(char *errorMessage);  /* エラー処理関数 */
/*
 * 引数から金額を抜き出す関数
 * msgStruct *msg: 送信するデータ msgStruct構造体へのポインタ
 * char *str: 引数文字列を受け取る
 */

