/****************************************
 * TransactTCPChat.c                  *
 *                                      *
 * p44                                  *
 ****************************************/
#include "TCPChat.h"

void TransactTCPChatt(int clntSocket)
{
  /* クライアントから受け取るデータ */
	msgStruct ClientData;
	/* クライアントからの受信バッファ */
	void *ClientBuffer = (void *) &ClientData;
	/* 返答データ */
	msgBuf HentoData;
	/* 受信データ */
	int totalBytesRcvd, bytesRcvd;
	/* 預入回数 */
	static unsigned short countDeps = 0;
	/*引出回数 */
	static unsigned short countWds = 0;
	/* 送信データの長さ */
	int HentoDLen;

	static unsigned int deposits;
	static unsigned int withdrawals;

	/* 
	 * クライアントから受信
	 */
	/* クライアントからのデータを表示するための準備 */
	totalBytesRcvd = 0;
	printf("Received: \n");

	for (totalBytesRcvd = 0; totalBytesRcvd < sizeof(ClientData); totalBytesRcvd += bytesRcvd) {
    /* バッファサイズに達するまで(NULL文字用の1バイトを除く)
       サーバからのデータを受信する
       recv -- -1:エラー
                0:受信バイト数がゼロ(通信先がコネクションをクローズ)
       ClientBuffer -- 受信データを格納するバッファへのポインタ
	   totalBytesRcvd -- 受信したデータの全バイト数
	   sizeof(ClientData) -- 受信データ用変数のサイズ
	   sizeof(DlientData) - totalBytesRcvd -- 残りのデータ
       0 -- ソケットが呼び出されたときの振る舞い（デフォルト）
    */
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
	deposits = ntohl(ClientData.dep);
	withdrawals = ntohl(ClientData.wd);
	
	/* データがあれば、預入回数が増える。 */
	if (deposits > 0)
		++countDeps;
	/* データがあれば、引出回数が増える。 */
	if (withdrawals > 0)
		++countWds;

	/* クライアントから受け取ったデータを表示する */
	printf("回数= %d  預入金額= %d\n", countDeps, deposits);
	printf("回数= %d  引出金額= %d\n", countWds, withdrawals);

#ifdef DEBUG
	unsigned int depoTest = htonl(deposits);
	unsigned short countTest = htons(countDeps);
	char *chosa = (char *)&depoTest;
	printf("%x %x %x %x\n", chosa[0], chosa[1], chosa[2], chosa[3]);
	char *chosa2 = (char *)&countTest;
	printf("%x %x\n", chosa2[0], chosa2[1]);
#endif /* DEBUG */

  
	/* クライアントの端末に表示するためのデータを作成する。 */
#ifdef DEBUG
 	printf("htonl(deposits) = %d\n", htonl(deposits));
	printf("htonl(withdrawals) = %d\n", htonl(withdrawals));
	printf("htons(countDeps) = %d\n", htons(countDeps));
	printf("htons(countWds) = %d\n", htons(countWds));
#endif /* DEBUG */

	HentoData.centsDeposited = htonl(deposits);
	HentoData.centsWithdrawn = htonl(withdrawals);
	HentoData.numDeps = htons(countDeps);
	HentoData.numWds = htons(countWds);
	
	
#ifdef DEBUG
	printf("HentoData.centsDeposited = %d\n", HentoData.centsDeposited);
	printf("HentoData.centsWithdrawn = %d\n", HentoData.centsWithdrawn);
	printf("HentoData.numDeps = %d\n", HentoData.numDeps);
	printf("HentoData.numWds = %d\n", HentoData.numWds);
#endif /* DEBUG */

#ifdef DEBUG
	char *hento = (char *)&HentoData.centsDeposited;
	printf("sizeof(HentoData)= %ld\n", sizeof(HentoData));
	printf("HentoData Address= %p\n", &HentoData);
	printf("HentoData.centsDeposited= %p\n", &HentoData.centsDeposited);
	printf("HentoData.centsDeposited= %x %x %x %x\n",
		   hento[0], hento[1], hento[2], hento[3]);
#endif  /* DEBUG */

/* クライアントにデータを送る。 */
	HentoDLen = sizeof(HentoDLen);
	if (send(clntSocket, &HentoData, HentoDLen, 0) != HentoDLen)
		DieWithError("send() failed");

	/* クライアントソケットをクローズする */
	close(clntSocket);
}

   
