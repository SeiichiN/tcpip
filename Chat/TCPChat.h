#define DEBUG
/***********************************************
 * TCPChat.h -- ヘッダファイル
 * 
 * 関係するファイル
 *		TCPChatClient.c
 *		TCPChatServer.c
 *		ChatTCP.c
 * 
 * 作成日: 2017.01.25
 * 作成: Seiichi Nukayama
 * *********************************************/
#include <stdio.h>  /* printf() fprintf()に必要 */
#include <sys/socket.h> /* socket() connect() send() recv()に必要 */
#include <arpa/inet.h>  /* sockaddr_in inet_addr()に必要 */
#include <stdlib.h>  /* atoi()に必要 */
#include <string.h>  /* memset() に必要 */
#include <unistd.h>  /* close() に必要 */
#include <netinet/in.h> /* htonl() ntohl() */

#define MAXWORD 140


void DieWithError(char *errorMessage);  /* エラー処理関数 */

