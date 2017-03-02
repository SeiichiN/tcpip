/***************************************************
 * testStruct.c -- structの検討用
 *                                                 *
 ***************************************************/
#include <stdio.h>  /* printf() fprintf()に必要 */
#include <sys/socket.h> /* socket() connect() send() recv()に必要 */
#include <arpa/inet.h>  /* sockaddr_in inet_addr()に必要 */
#include <stdlib.h>  /* atoi()に必要 */
#include <string.h>  /* memset() に必要 */
#include <unistd.h>  /* close() に必要 */

/* 送信するデータ */
typedef struct {
	int dep;  /* deposits 預入金額 */
	int wd;   /* withdrawals 引出金額 */
} msgStruct;

/* 受信するデータ */
typedef struct {
	int centsDeposited;
	int centsWithdrawn;
	unsigned short numDeps;
	unsigned short numWds;
} msgBuf;

void setdata(msgStruct *msg, char *str);

int deposits;
int withdrawals;

int main(int argc, char *argv[])
{
	msgStruct msgdata;  /* データ用変数 */

    msgBuf msgrvdata;  /* 受信データ用変数 */
    
  /* 引数の数が正しいか確認 */
  if (argc != 2) {   
    fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n",
            argv[0]);
    exit(1);
  }

  /* 1つめの引数：データを読み取る */
  setdata(&msgdata, argv[1]);

  printf("預入金額 = %d\n", msgdata.dep);
  printf("引出金額 = %d\n", msgdata.wd);
  

  /* 受信データの表示 */
  /*
  printf("預入金額 = %d\n", msgrvdata.centsDeposited);
  printf("預入回数 = %d\n", msgrvdata.numDeps);
  printf("引出金額 = %d\n", msgrvdata.centsWithdrawn);
  printf("引出回数 = %d\n", msgrvdata.numWds);
  */
  
  /* 最後の改行を出力 */
  printf("\n");
  
  exit(0);
}

void setdata(msgStruct *msg, char *str)
{
	if (*str == '+') {
		++str;
		deposits = atoi(str);
		msg->dep = deposits;
	} else if (*str == '-') {
		++str;
		withdrawals = atoi(str);
		msg->wd = withdrawals;
	} else {
		fprintf(stderr, "Usage: +<number> or -<number>\n");
		exit (1);
	}
}

