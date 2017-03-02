/****************************************
 * HandleTCPClient.c                    *
 *                                      *
 * p44                                  *
 ****************************************/
#include <stdio.h>  /* printf() fprintf() */
#include <sys/socket.h>  /* recv() send() */
#include <unistd.h>  /* close() */

/* 受信バッファのサイズ */
#define RCVBUFSIZE 32

/* エラー処理関数 */
void DieWithError(char *errorMessage);

void HandleTCPClient(int clntSocket)
{
  /* エコー文字列のバッファ */
  char echoBuffer[RCVBUFSIZE];
  /* 受信メッセージのサイズ */
  int recvMsgSize;

  /* クライアントからの受信メッセージ */
  if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
    DieWithError("recv() failed");

  /*
   * 受信した文字列を送信し、転送が終了していなければ次を受信する
   */
  /* 0 は転送の終了を意味する */
  while (recvMsgSize > 0) {
    /* メッセージをクライアントにエコーバックする */
    if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
      DieWithError("send() failed");

    /* 受信するデータが残っていないか確認する */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");
  }

  /* クライアントソケットをクローズする */
  close(clntSocket);
}

   
