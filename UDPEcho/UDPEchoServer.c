/*******************************************************
 * UDPEchoServer.c                                     *
 *   UDPソケットを使った通信プログラム（サーバー）     *
 *                                                     *
 * 使用法：                                            *
 *     UDPEchoServer <ポート番号>                      *
 *              （ポート番号は、たとえば5000）         *
 *                                                     *
 * 『TCP/IPソケットプログラミングＣ言語編』p.67        *
 *******************************************************/
#include "UDPEcho.h"

int main(int argc, char *argv[])
{
    int sock;                         /* ソケットディスクリプタ */
	struct sockaddr_in echoServAddr;  /* ローカルサーバのアドレス */
	struct sockaddr_in echoClntAddr;  /* クライアントのアドレス */
	unsigned int cliAddrLen;          /* 着信メッセージの長さ */
	char echoBuffer[ECHOMAX];         /* エコー文字列の受信用バッファ */
	unsigned short echoServPort;      /* サーバのポート番号 */
	int recvMsgSize;                  /* 受信したメッセージの長さ */

	/* 引数の数が正しいか確認 */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <UDP SERVER PORT>\n", argv[0]);
		exit(1);
	}

	/* 1つ目の引数：ローカルポート番号 */
	echoServPort = atoi(argv[1]);

	/* データグラムの送受信に使うソケットを作成 */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		DieWithError("socket() failed");

	/*
	 * ローカルアドレス構造体を作成
	 */
    /* 構造体に0を埋める */
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	/* インターネットアドレスファミリ */
	echoServAddr.sin_family = AF_INET;
	/* ローカルのIPアドレス・ワイルドカードを使用 */
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	/* ローカルのポート番号 */
	echoServAddr.sin_port = htons(echoServPort);

	/*
	 * 着信接続用に作成したソケットとサーバのアドレスとを結びつける。
	 * 成功：0     失敗：-1
	 */
	if (bind(sock, (struct sockaddr *) &echoServAddr,
			 sizeof(echoServAddr)) < 0)
		DieWithError("bind() failed");
	
	/* 処理を繰り返し実行 */
	for (;;) {
		/* 入出力パラメータのサイズをセット */
		cliAddrLen = sizeof(echoClntAddr);
		
		/* クライアントからメッセージを受信するまでぶろっくする */
		if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0,
									(struct sockaddr *) &echoClntAddr,
									&cliAddrLen)) < 0)
			DieWithError("recvfrom() failed");
		
		printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
		/* inet_ntoa -- 32ビットの2進表記のアドレスを10進表記文字列に変換 */
		
		/* 受信したデータグラムをクライアントに返信する */
		if (sendto(sock, echoBuffer, recvMsgSize, 0,
				   (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr))
			!= recvMsgSize)
			DieWithError("sendto() sent a different number of bytes than expected");
	}
	/* ここには到達しない */
}

  
  
