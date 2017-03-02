/****************************************************************************
 * UDPEchoClient.c                                                          *
 *   UDPソケットを使った通信プログラム（クライアント）                      *
 *                                                                          *
 * 使用法：                                                                 *
 *     UDPEchoClient <サーバーIPアドレス> <"文字列"> [<サーバーポート番号>] *
 *                                                                          *
 * 『TCP/IPソケットプログラミングＣ言語編』p.64                             *
 ****************************************************************************/
#include "UDPEcho.h"

int main(int argc, char *argv[])
{
	int sock;                         /* ソケットディスクリプタ */
	struct sockaddr_in echoServAddr;  /* エコーサーバのアドレス */
	struct sockaddr_in fromAddr;      /* エコー送信元のアドレス */
	unsigned short echoServPort;      /* エコーサーバのポート番号 */
	unsigned int fromSize;            /* recvfrom() のアドレスの入出力サイズ) */
	char *servIP;                     /* サーバのIPアドレス */
	char *echoString;                 /* エコーサーバへ送信する文字列 */
	char echoBuffer[ECHOMAX+1];      /* エコー文字列の受信用バッファ */
	int echoStringLen;                /* エコー文字列の長さ */
	int respStringLen;                /* 受信した応答の長さ */

    /* 引数の数が正しいか確認 */
	if ((argc < 3) || (argc > 4)) {
		fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n",
				argv[0]);
		exit(1);
	}

	/* 1つ目の引数：サーバのIPアドレス（ドット10進表記） */
	servIP = argv[1];
	/* 2つ目の引数：エコー文字列 */
	echoString = argv[2];

	/* 入力した文字列の長さをチェック */
	if ((echoStringLen = strlen(echoString)) > ECHOMAX)
		DieWithError("Echo word too long");

	if (argc == 4)
		/* 指定のポート番号があれば使用 */
		echoServPort = atoi(argv[3]);
	else
		/* 7は、エコーサービスのwell-knownポート番号 */
		echoServPort = 7;
	
	/* UDPデータグラムソケットの作成 */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		DieWithError("socket() failed");

	/*
	 * サーバのアドレス構造体の作成
	 */
	/* 構造体に0を埋める */
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	/* インターネットアドレスファミリ */
	echoServAddr.sin_family = AF_INET;
	/* サーバのIPアドレス */
	echoServAddr.sin_addr.s_addr = inet_addr(servIP);
	/* サーバのポート番号 */
	echoServAddr.sin_port = htons(echoServPort);

	/* 文字列をサーバに送信 */
	/* echoString -- 相手に渡す文字列（2つめの引数）
	   echoStringLen -- '\0'は入らない
	   成功：送った文字数を返す
       失敗：-1 */
	if (sendto(sock, echoString, echoStringLen, 0,
			   (struct sockaddr *) &echoServAddr,
			   sizeof(echoServAddr)) != echoStringLen)
		DieWithError("sendto() sent a different number of bytes than expected");

	/* 応答を受諾 */
	fromSize = sizeof(fromAddr);
	/* respStringLen -- 受け取った文字数
	   echoStringLen -- 送った文字数 */
	if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0,
								  (struct sockaddr *) &fromAddr, &fromSize))
		!= echoStringLen)
		DieWithError("recvfrom() failed");

	/* echoServAddr -- 送信先サーバアドレス（ポート番号も含む）
	   fromAddr -- 受信した送信元サーバアドレス（ポート番号も含む） */
	if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
		fprintf(stderr, "Error: received a packet from unknown source.\n");
		exit(1);
	}

	/* 受信データをNULL文字で終端させる */
	echoBuffer[respStringLen] = '\0';
	/* 引数のエコー文字列を表示 */
	printf("Received: %s\n", echoBuffer);

	close(sock);
	exit(0);
}
