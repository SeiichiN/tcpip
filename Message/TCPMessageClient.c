/***************************************************
 * TCPMessageClient.c                              *
 *     TCPを使用してサーバと通信をおこなう         *
 *     クライアント・プログラム                    *
 *                                                 *
 * 使用法：                                        *
 *     金額は引数で指定する。                      *
 *     預入金額: +3000（例）                       *
 *     引出金額: -2500（例）                       *
 * $ TCPEchoClient <ip-address> <金額> <port>      *
 *                                                 *
 * 預入金額 = int deposits                         *
 * 引出金額 = int withdrawals                      *
 * TCPソケットディスクリプタ = int sock            *
 * sizeof(int) = 4   sizeof(short) = 2             *
 *                                                 *
 * 出典：                                          *
 *     『TCP/IPソケットプログラミング Ｃ言語編』   *
 *      p.49                                       *
 ***************************************************/
#include "TCPMessage.h"

void setdata(msgStruct *msg, char *str);

int main(int argc, char *argv[])
{


	int sock;                        /* ソケットディスクリプタ */
	struct sockaddr_in echoServAddr; /* エコーサーバのアドレス */
	/*
	 * struct sockaddr_in {
	 *     unsigned short sin_family;  // TCP/IP(AF_INET)
	 *     unsigned short sin_port;    // アドレスポート(16ビット)
	 *     struct in_addr sin_addr;    // IPアドレス(32ビット)
	 *     char sin_zero[8];           // 不使用
	 * };
	 * struct in_addr {
	 *     unsigned long s_addr;       // IPアドレス(32ビット)
	 * };
	 */
	unsigned short echoServPort;     /* エコーサーバのポート番号 */
	char *servIP;                    /* サーバのIPアドレス(ドット10進表記) */

	static msgStruct msgdata;  /* データ用変数 */
	msgdata.dep = 0;   /* 初期化 */
	msgdata.wd = 0;    /* 初期化 */

	static msgBuf msgrvdata;  /* 受信データ用変数 */
	msgrvdata.centsDeposited = 0;
	msgrvdata.centsWithdrawn = 0;
    
	unsigned int deposits;
	unsigned int withdrawals;
	unsigned short countDeps;
	unsigned short countWds;
  
	void *msgBuffer = (void *) &msgrvdata;  /* データ用バッファ */
	unsigned int msgLen;           /* データのサイズ */
	int bytesRcvd, totalBytesRcvd;   /* 1回のrecv() で読み取られる
                                      バイト数と全バイト数 */

	/* 引数の数が正しいか確認 */
	if ((argc < 3) || (argc > 4)) {   
		fprintf(stderr, "Usage: %s <Server IP> <+/-Number> [<Sever Port>]\n",
				argv[0]);
		exit(1);
	}

	/* 1つめの引数：サーバのIPアドレス（ドット10進表記） */
	servIP = argv[1];
	/* ２つめの引数：データを読み取る */
	setdata(&msgdata, argv[2]);

	/* 指定のポート番号があれば使用 */
	if (argc == 4)
		echoServPort = atoi(argv[3]);
	else
		echoServPort = 7;  /* 7はエコーサービスのwell-knownポート番号 */

	/* TCPによる信頼性の高いストリームソケットを作成 */
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError("socket() failed");
	/* PF_INET -- TCP/IPプロトコルファミリを使う
	 * SOCK_STREAM -- 信頼性の高いストリームソケットを作成する
	 * IPPROTO_TCP -- TCPを使うことを明示。（0 を指定しても同じ）
	 * 戻り値：成功時、負でない整数。失敗時、-1
	 */

	/*
	 * サーバのアドレス構造体を作成
	 */
	/* 構造体に0を埋める */
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	/* インターネットアドレスファミリー */
	echoServAddr.sin_family = AF_INET;
	/* サーバのIPアドレス
	   inet_addr -- 10進表記を32ビットの2進表記に変換 */
	echoServAddr.sin_addr.s_addr = inet_addr(servIP);
	/* サーバのポート番号
	   htons -- バイト順の問題 */
	echoServAddr.sin_port = htons(echoServPort);
	
	/*
	 * サーバへの接続を確立
	 * サーバのアドレスを表す構造体へのアドレスを
	 * sockaddrという汎用のデータ型にキャストする.
	 * その上でsockと結びつける。サイズも必要。
	 */
	if (connect(sock, (struct sockaddr *)&echoServAddr,
				sizeof(echoServAddr)) < 0)
		DieWithError("connect() failed");

	/*
	 * 入力データの長さを調べる
	 * 終了文字('\0')は含まない
	 */
	msgLen = sizeof(msgdata);
#ifdef DEBUG
	printf("msgLen= %d\n", msgLen);
#endif  /* DEBUG */
  
	/*
	 * 文字列をサーバに送信
	 * 0 -- ソケットが呼び出されたときの振る舞い（デフォルト）???
	 * send -- 送信成功：送信したバイト数を返す
	 *         送信失敗：-1
	 * （注意）-- 終了文字('\0')は送信しない。
	 */
	if (send(sock, &msgdata, msgLen, 0) != msgLen)
		DieWithError("send() sent a different number os bytes than expected");

	/* 
	 * サーバから受信
	 */
	/* サーバからのデータを表示するための準備 */
	totalBytesRcvd = 0;
	printf("Received: \n");
	
#ifdef DEBUG
	printf("初期値 ------------------------------\n");
	printf("msgBuffer=  %s  %p\n", (char *)msgBuffer, msgBuffer);
	printf("msgrvdata=  %p\n", &msgrvdata);
	printf("msgrvdata.centsDeposited= %d  %p\n", msgrvdata.centsDeposited,
		   &msgrvdata.centsDeposited);
	printf("msgrvdata.centsWthdrawn= %d\n", msgrvdata.centsWithdrawn);
	printf("msgrvdata.numDeps= %d\n", msgrvdata.numDeps);
	printf("msgrvdata.numWds= %d\n", msgrvdata.numWds);
	printf("-------------------------------------\n");
#endif /* DEBUG */
	
	
	for (totalBytesRcvd = 0; totalBytesRcvd < sizeof(msgrvdata); totalBytesRcvd += bytesRcvd) {
		/* バッファサイズに達するまで(NULL文字用の1バイトを除く)
		   サーバからのデータを受信する
		   recv -- -1:エラー
		   0:受信バイト数がゼロ(通信先がコネクションをクローズ)
		   msgBuffer -- 受信データを格納するバッファへのポインタ
		   totalBytesRcvd -- 受信したデータの全バイト数
		   sizeof(msgrvdata) -- 受信データ用変数のサイズ
		   sizeof(msgrvdata) - totalBytesRcvd -- 残りのデータ
		   0 -- ソケットが呼び出されたときの振る舞い（デフォルト）
		*/
		if ((bytesRcvd = recv(sock, msgBuffer + totalBytesRcvd, sizeof(msgrvdata) - totalBytesRcvd, 0)) <= 0)
			DieWithError("recv() failed or connection closed prematurely");
	}		  
	
#ifdef DEBUG
	printf("sizeof(msgrvdata)= %ld\n", sizeof(msgrvdata));
	printf("totalBytesRcvd= %d\n", totalBytesRcvd);
	printf("bytesRcvd= %d\n", bytesRcvd);
#endif  /* DEBUG */

#ifdef DEBUG
	printf("受信後\n");
	printf("msgBuffer=  %s  %p\n", (char *)msgBuffer, msgBuffer);
	printf("msgrvdata=  %p\n", &msgrvdata);
	printf("msgrvdata.centsDeposited= %s\n", (char *)&msgrvdata.centsDeposited);
	printf("msgrvdata.centsWthdrawn= %s\n", (char *)&msgrvdata.centsWithdrawn);
	printf("msgrvdata.numDeps= %s\n", (char *)&msgrvdata.numDeps);
	printf("msgrvdata.numWds= %s\n", (char *)&msgrvdata.numWds);
#endif /* DEBUG */



/* バイト順を変換 */
	
	deposits = ntohl(msgrvdata.centsDeposited);
	withdrawals = ntohl(msgrvdata.centsWithdrawn);
	countDeps = ntohs(msgrvdata.numDeps);
	countWds = ntohs(msgrvdata.numWds);
  
#ifdef DEBUG
	printf("バイト順変換後\n");
	printf("msgBuffer=  %s  %p\n", (char *)msgBuffer, msgBuffer);
	printf("msgrvdata=  %p\n", &msgrvdata);
	printf("msgrvdata.centsDeposited= %d\n", msgrvdata.centsDeposited);
	printf("msgrvdata.centsWthdrawn= %d\n", msgrvdata.centsWithdrawn);
	printf("msgrvdata.numDeps= %d\n", msgrvdata.numDeps);
	printf("msgrvdata.numWds= %d\n", msgrvdata.numWds);
#endif /* DEBUG */


	/* 受信データの表示 */
	printf("預入金額 = %d\n", deposits);
	printf("預入回数 = %d\n", countDeps);
	printf("引出金額 = %d\n", withdrawals);
	printf("引出回数 = %d\n", countWds);
	
  /* 最後の改行を出力 */
	printf("\n");
  
	close(sock);
	exit(0);
}

void setdata(msgStruct *msg, char *str)
{
	unsigned int deposits;
	unsigned int withdrawals;

	if (*str == '+') {
		++str;
		deposits = atoi(str);
		msg->dep = htonl(deposits);
	} else if (*str == '-') {
		++str;
		withdrawals = atoi(str);
		msg->wd = htonl(withdrawals);
	} else {
		DieWithError("Usage: +<number> or -<number>\n");
		exit (1);
	}
}

