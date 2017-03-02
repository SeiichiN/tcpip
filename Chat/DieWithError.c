/****************************************
 * DieWithError.c                       *
 *     エラーメッセージを表示する       *
 *                                      *
 * 同掲書 p.38                          *
 ****************************************/
#include <stdio.h>  /* perror() に必要 */
#include <stdlib.h>  /* exit() に必要 */

void DieWithError(char *errorMessage)
{
  perror(errorMessage);
  exit(1);
}
