#undef DEBUG
/****************************************
 * HandleTCPClient.c                    *
 *                                      *
 * p44                                  *
 ****************************************/
#include <stdio.h>  /* printf() fprintf() */
#include <sys/socket.h>  /* recv() send() */
#include <unistd.h>  /* close() */
#include <string.h>  /* strlen */

/* 受信バッファのサイズ */
#define RCVBUFSIZE 100
#define MAXWORD 100

/* エラー処理関数 */
void DieWithError(char *errorMessage);
void initBuffer(char echoBuffer[]);

void HandleTCPClient(int clntSocket)
{
	/* エコー文字列のバッファ */
	// char echoBuffer[RCVBUFSIZE];
	char echoBuffer[MAXWORD];
	/* 受信メッセージのサイズ */
	int recvMsgSize;

	
	for(;;) {

		initBuffer(echoBuffer);

        /* クライアントからの受信メッセージ */
		if ((recvMsgSize = recv(clntSocket, echoBuffer, MAXWORD - 1, 0)) < 0)
			DieWithError("recv() failed");

#ifdef DEBUG
		printf("strlen(echoBuffer)=> %ld\n", strlen(echoBuffer));
		printf("recvMsgSize=> %d\n", recvMsgSize);
#endif /* DEBUG */
	
		echoBuffer[strlen(echoBuffer)] = '\0';
	
		/* クライアントからのメッセージを表示 */
		printf("受信> %s\n", echoBuffer);
  
		/* メッセージをクライアントにエコーバックする */
		if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
			DieWithError("send() failed");

#ifdef DEBUG
		printf("1 strlen(echoBuffer)=> %ld\n", strlen(echoBuffer));
		printf("1 recvMsgSize=> %d\n", recvMsgSize);
#endif /* DEBUG */

		/* quit が送られてきたら、エコーバックした後、ループを抜ける */
		if (strcmp(echoBuffer, "quit") == 0)
			break;
	}
	/* クライアントソケットをクローズする */
	close(clntSocket);
}

void initBuffer(char buffer[])
{
	int i;
	
	for(i = 0; i <= MAXWORD; i++) 
		buffer[i] = '\0';
}




