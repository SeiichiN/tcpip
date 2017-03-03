/****************************************
 * ChatTCP.c                  *
 *                                      *
 * p44                                  *
 ****************************************/
#include "TCPChat.h"

void ChatTCP(int clntSocket)
{
  /* クライアントから受け取るデータ */
	char ClientData[MAXWORD];
	/* クライアントからの受信バッファ */
    //	void *ClientBuffer = (void *) &ClientData;
    void *ClientBuffer = NULL;
	/* 返答データ */
	char HentoData[MAXWORD];
	/* 受信データ */
	int totalBytesRcvd, bytesRcvd;
	/* 預入回数 */
//	static unsigned short countDeps = 0;
	/*引出回数 */
//	static unsigned short countWds = 0;
	/* 送信データの長さ */
	int HentoDLen;

//	static unsigned int deposits;
//	static unsigned int withdrawals;

	/* 
	 * クライアントから受信
	 */
	/* クライアントからのデータを表示するための準備 */
	totalBytesRcvd = 0;
	printf("Received: \n");

#ifdef DEBUG
	printf("sizeof(ClientData)=> %ld\n", sizeof(ClientData));
#endif  /* DEBUG */
	
	for (totalBytesRcvd = 0; totalBytesRcvd < sizeof(ClientData); totalBytesRcvd += bytesRcvd) {
    /* バッファサイズに達するまで(NULL文字用の1バイトを除く)
       サーバからのデータを受信する
       recv -- -1:エラー
                0:受信バイト数がゼロ(通信先がコネクションをクローズ)
       ClientBuffer -- 受信データを格納するバッファへのポインタ
	   totalBytesRcvd -- 受信したデータの全バイト数
	   sizeof(ClientData) -- 受信データ用変数のサイズ
	   sizeof(ClientData) - totalBytesRcvd -- 残りのデータ
       0 -- ソケットが呼び出されたときの振る舞い（デフォルト）
    */
#ifdef DEBUG
		printf("totalBytesRcvd=> %d\n", totalBytesRcvd);
		printf("bytesRcvd => %d\n", bytesRcvd);
#endif  /* DEBUG */
		
//		if ((bytesRcvd = recv(clntSocket, ClientBuffer + totalBytesRcvd,
//							sizeof(ClientData) - totalBytesRcvd, 0)) <= 0)
		if ((bytesRcvd = recv(clntSocket, ClientBuffer + totalBytesRcvd,
							sizeof(ClientData) - totalBytesRcvd, 0)) <= 0)
			DieWithError("recv() failed or connection closed prematurely");
	}

#ifdef DEBUG
	printf("sizeof(ClientData)= %ld\n", sizeof(ClientData));
	printf("totalBytesRcvd= %d\n", totalBytesRcvd);
	printf("bytesRcvd= %d\n", bytesRcvd);
#endif  /* DEBUG */
	

	/* クライアントからのデータを変数に入れる */
//	deposits = ntohl(ClientData.dep);
//	withdrawals = ntohl(ClientData.wd);
	
	/* データがあれば、預入回数が増える。 */
//	if (deposits > 0)
//		++countDeps;
	/* データがあれば、引出回数が増える。 */
//	if (withdrawals > 0)
//		++countWds;

	/* クライアントから受け取ったデータを表示する */
	printf("クライアントからのメッセージ> %s\n", ClientData);
//	printf("回数= %d  引出金額= %d\n", countWds, withdrawals);

	fgets(HentoData, sizeof(HentoData), stdin);
	HentoData[strlen(HentoData)-1] = '\0';
	
/* クライアントにデータを送る。 */
	HentoDLen = sizeof(HentoData);
	if (send(clntSocket, &HentoData, HentoDLen, 0) != HentoDLen)
		DieWithError("send() failed");

	/* クライアントソケットをクローズする */
	close(clntSocket);
}

   
