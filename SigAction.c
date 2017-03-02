/*************************************************************
 * SigAction.c -- sigaction()を使ったサンプルプログラム      *
 *                                                           *
 * p.79                                                      *
 *************************************************************/
/* struct sigaction {
 *     void (*sa_handler)(int); // シグナルハンドラ
 *     sigset_t sa_mask;        // ハンドラ実行中にブロックされるシグナル
 *	   int sa_flags;            // デフォルト動作を変更するためのフラグ
 * };
 */
#include <stdio.h>   /* printf() */
#include <signal.h>  /* sigaction() */
#include <unistd.h>  /* pause() */
#include <stdlib.h>  /* exit() */

void DieWithError(char *errorMessage);  /* エラー処理関数 */
void InterruptSignalHandler(int signalType);  /* 割り込みシグナル処理関数 */

int main(int argc, char *argv[])
{
    /* シグラルハンドラを指定する構造体 */
    struct sigaction handler;
    
    /* InterruptSignalHandler()をハンドラ関数として設定 */
    handler.sa_handler = InterruptSignalHandler;
    /* 全シグラルをマスクするマスクを作成 */
    if (sigfillset(&handler.sa_mask) < 0)
        DieWithError("sigfillset() failed");
    /* フラグなし */
    handler.sa_flags = 0;
    
    /* 割り込みシグナルに対する処理を設定 */
    if (sigaction(SIGINT, &handler, 0) < 0)
        DieWithError("sigaction() failed");
    
    for(;;)
        pause();  /* シグナルを受け取るまでプログラムを一時停止 */
    
    exit(0);
}
void InterruptSignalHandler(int signalType)
{
    printf("Interrupt Received. Exiting program.\n");
    exit(1);
}
