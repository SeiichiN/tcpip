/***************************************************
 * TCPEchoClient.c                                 *
 *     TCPを使用したエコーサーバと通信をおこなう   *
 *     クライアント・プログラム                    *
 *                                                 *
 * 使用法：                                        *
 *     エコーさせる文字列は、引数で指定する。      *
 *     $ TCPEchoClient <エコー文字列>              *
 *                                                 *
 * 出典：                                          *
 *     『TCP/IPソケットプログラミング Ｃ言語編』   *
 *      p.33                                       *
 ***************************************************/
#include <stdio.h>  /* printf() fprintf()に必要 */
#include <sys/socket.h> /* socket() connect() send() recv()に必要 */
#include <arpa/inet.h>  /* sockaddr_in inet_addr()に必要 */
#include <stdlib.h>  /* atoi()に必要 */
#include <string.h>  /* memset() に必要 */
#include <unistd.h>  /* close() に必要 */

#define RCVBUFSIZE 32  /* 受信バッファのサイズ */

void DieWithError(char *errorMessage);  /* エラー処理関数 */

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
  char echoString[100];                 /* エコーサーバに送信する文字列 */
  char echoBuffer[RCVBUFSIZE];     /* エコー文字列用バッファ */
  unsigned int echoStringLen;      /* エコーする文字列のサイズ */
  int bytesRcvd, totalBytesRcvd;   /* 1回のrecv() で読み取られる
                                      バイト数と全バイト数 */

  /* 引数の数が正しいか確認 */
  if ((argc < 2) || (argc > 3)) {   
    fprintf(stderr, "Usage: %s <Server IP>  [<Echo Port>]\n",
            argv[0]);
    exit(1);
  }

  /* 1つめの引数：サーバのIPアドレス（ドット10進表記） */
  servIP = argv[1];
  /* 文字を入力する */
  fgets(echoString, sizeof(echoString), stdin);
  echoString[strlen(echoString)-1] = '\0';

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
  printf("sock= %d  adress= %p  size= %ld\n", sock, &sock, sizeof(sock));
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
     htons -- バイト順の問題 ??? */
  echoServAddr.sin_port = htons(echoServPort);

  /*
   * エコーサーバへの接続を確立
   * エコーサーバのアドレスを表す構造体へのアドレスを
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
  echoStringLen = strlen(echoString);
  
  /*
   * 文字列をサーバに送信
   * 0 -- ソケットが呼び出されたときの振る舞い（デフォルト）???
   * send -- 送信成功：送信したバイト数を返す
   *         送信失敗：-1
   * （注意）-- 終了文字('\0')は送信しない。
   */
  if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
    DieWithError("send() sent a different number os bytes than expected");

  /* 
   * 同じ文字列をサーバから受信
   */
  /* エコーされた文字列を表示するための準備 */
  totalBytesRcvd = 0;
  printf("Received: ");

  while (totalBytesRcvd < echoStringLen) {
    /* バッファサイズに達するまで(NULL文字用の1バイトを除く)
       サーバからのデータを受信する
       recv -- -1:エラー
                0:受信バイト数がゼロ(通信先がコネクションをクローズ)
       echoBuffer -- 受信データを格納するバッファへのポインタ
       RCVBUFSIZE -- 一度に受信可能なバイト数の最大値を指定
                     終了文字を付加するため、1バイトをマイナス。
       0 -- ソケットが呼び出されたときの振る舞い（デフォルト）
    */
    if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
      DieWithError("recv() failed or connection closed prematurely");
    /* 総バイト数の集計 */
    totalBytesRcvd += bytesRcvd;
    /* 文字列の終了 */
    echoBuffer[bytesRcvd] = '\0';
    /* エコーバッファの表示 */
    printf("%s\n", echoBuffer);
  }

  /* 最後の改行を出力 */
  printf("\n");
  
  close(sock);
  exit(0);
}
