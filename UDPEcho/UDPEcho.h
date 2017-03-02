/****************************************
 * UDPEcho.h                            *
 *                                      *
 *     UDPEchoClient.c UDPEchoServer.c  *
 *     のためのヘッダファイル           *
 ****************************************/
#include <stdio.h>  /*  printf(), fprintf() */
#include <sys/socket.h>  /*  sochet(), bind() */
#include <arpa/inet.h>  /*  sockaddr_in, inet_ntoa() */
#include <stdlib.h>  /*  atoi() */
#include <string.h>  /*  memset() */
#include <unistd.h>  /*  close() */

#define ECHOMAX 8  /* エコー文字列の最大長 */

void DieWithError(char *errorMessage);  /* 外部エラー処理関数 */
