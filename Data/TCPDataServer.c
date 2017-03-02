/**************************************************
 * TCPMessageServer.c                             *
 *     TCPMessageClient のためのサービス          *
 *                                                *
 * 『TCP/IPソケットプログラミング Ｃ言語編』 p.41 *
 **************************************************/
#include "TCPMessage.h"

/* 同時にキュー可能な接続要求の最大数 */
#define MAXPENDING 5

/* TCPクライアント処理関数 */
void TransactTCPClient(int clntCocket);

int main(int argc, char *argv[])
{
  /* サーバのソケットディスクリプタ */
  int servSock;
  /* クライアントのソケットディスクリプタ */
  int clntSock;
  /* ローカルアドレス
     クライアントと同じアドレス構造体
   */
  struct sockaddr_in echoServAddr;
  /* クライアントアドレス */
  struct sockaddr_in echoClntAddr;
  /* サーバポート */
  unsigned short echoServPort;
  /* クライアントのアドレス構造体の長さ */
  unsigned int clntLen;
  /* 接続時のクライアントへの返答 */
  const char okmsg[] = "Connect OK!";
  /* そのサイズ */
  int okmsgsize;

  /* 引数が正しいか確認 */
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
    exit(1);
  }

  /* １つめの引数：ローカルポート */
  echoServPort = atoi(argv[1]);

  /*
   * 着信接続用のソケットを作成
   * クライアントで作成したのと同じ方法
   */
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithError("socket() failed");

  /*
   * ローカルのアドレス構造体を作成
   */
  /* 初期化 -- 構造体を 0 で埋める */
  memset(&echoServAddr, 0, sizeof(echoServAddr));
  /* インターネットアドレスファミリ */
  echoServAddr.sin_family = AF_INET;
  /* INADDR_ANY -- ワイルドカード
                 （サーバ自身のアドレス）
     htonl -- アドレスをネットワークのバイト順に変換 ??? */
  echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* ローカルポート
     htons -- ポート番号をネットワークのバイト順に変換 ??? */
  echoServAddr.sin_port = htons(echoServPort);

  /*
   * 着信接続用に作成したソケットとサーバのアドレスとを結びつける。
   * 成功：0     失敗：-1
   */
  if (bind(servSock, (struct sockaddr *) &echoServAddr,
           sizeof(echoServAddr)) < 0)
    DieWithError("bind() failed");

  /*
   * 「接続要求をリスン中」というマークをソケットにつける
   * MAXPENDING -- リスンの最大数（システムによって意味がちがう）???
   * 成功：0     失敗：-1
   */
  if (listen(servSock, MAXPENDING) < 0)
    DieWithError("listen() failed");

  for (;;) {
    /* 入出力パラメータのサイズをセット
       echoClntAddr -- クライアントのアドレス構造体 */
    clntLen = sizeof(echoClntAddr);

    /* クライアントからの接続要求を待機
       ソケットをクライアントのアドレスと結びつける。
       その際、アドレス（ポインタ）を指定しているのに注意。
       つまり、接続が成功した時点で、echoClntAddrには接続した
       クライアントのアドレスとポート番号が格納される。
       また、その長さはclntLenに格納される。
       処理が成功：新しいソケットディスクリプタを返す。 */
    if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,
                           &clntLen)) < 0)
      DieWithError("accept() failed");

    /* clntSockはクライアントに接続済み */

    printf("Handling client %s\n",
           inet_ntoa(echoClntAddr.sin_addr));
    /* inet_ntoa -- 32ビットの2進表記のアドレスを10進表記文字列に変換 */

    /* 接続したことをクライアントに伝える */
    okmsgsize = strlen(okmsg);
    if (send(clntSock, okmsg, okmsgsize, 0) != okmsgsize)
      DieWithError("okmsg send() failed");

    
    /* 受け取った文字列を処理してクライアントに送り返す */
    TransactTCPClient(clntSock);
  }
  /* この部分には到達しない */
}

