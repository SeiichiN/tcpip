/***************************************************
 * TCPChatClient.c                              *
 *     TCPを使用してサーバと通信をおこなう         *
 *     クライアント・プログラム                    *
 *                                                 *
 * 使用法：                                        *
 *     チャットをおこなうプログラム
 * $ TCPChatClient <ip-address> <port>
 *                                                 *
 * ことば = word[140]
 * 最大文字数を140字とする。
 *
 * TCPソケットディスクリプタ = int sock            *
 * sizeof(int) = 4   sizeof(short) = 2             *
 *                                                 *
 * 出典：                                          *
 *     『TCP/IPソケットプログラミング Ｃ言語編』   *
 *      p.49                                       *
 ***************************************************/
#include "TCPChat.h"

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

	char sendword[MAXWORD];  /* チャットデータ入力用 */

    /* 受信するデータ */
	char recvword[MAXWORD];
	char *msgdata = NULL;
	char *msgrvdata = NULL;
	void *msgBuffer = (void *) recvword;  /* データ用バッファ */
	unsigned int msgLen;           /* データのサイズ */
	int bytesRcvd, totalBytesRcvd;   /* 1回のrecv() で読み取られる
                                      バイト数と全バイト数 */

	/* 引数の数が正しいか確認 */
	if (argc != 3) {   
		fprintf(stderr, "Usage: %s <Server IP> [<Sever Port>]\n",
				argv[0]);
		exit(1);
	}

	/* 1つめの引数：サーバのIPアドレス（ドット10進表記） */
	servIP = argv[1];

	/* 指定のポート番号があれば使用 */
	if (argc == 3)
		echoServPort = atoi(argv[2]);
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

	/* チャットデータを入力する */
	printf("入力 > ");
	fgets(sendword, sizeof(sendword), stdin);
	/* 入力の末尾に\0を入れる */
	sendword[strlen(sendword) - 1] = '\0';

#ifdef DEBUG
	printf("sendword=> %s\n", sendword);
#endif /* DEBUG */	

	/* msgdata にコピー */
	// msgdata = sendword;
	

#ifdef DEBUG
	printf("msgdata=> %s\n", msgdata);
#endif /* DEBUG */	

	/*
	 * 入力データの長さを調べる
	 * 終了文字('\0')は含まない
	 */
	// msgLen = sizeof(msgdata);
	msgLen = strlen(sendword);

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
//	if (send(sock, &msgdata, msgLen, 0) != msgLen)
	if (send(sock, sendword, msgLen, 0) != msgLen)
		DieWithError("send() sent a different number os bytes than expected");

	/* 
	 * サーバから受信
	 */
	/* サーバからのデータを表示するための準備 */
	totalBytesRcvd = 0;
	printf("Received: \n");
	
	
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


	/* 受信データの表示 */
	printf("メッセージ= %s\n", msgrvdata);
	
  /* 最後の改行を出力 */
	printf("\n");
  
	close(sock);
	exit(0);
}

