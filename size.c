/****************************************
 * size.c -- 変数のサイズを調べる
 ****************************************/
#include <stdio.h>

int main(void)
{

	    /* 受信するデータ */
    typedef struct {
      int centsDeposited;
      int centsWithdrawn;
      unsigned short numDeps;
      unsigned short numWds;
    } msgBuf;

    msgBuf msgrvdata;  /* 受信データ用変数 */
    
    printf("構造体 msgrvdata\n");
    printf("centsDeposited= %p   %ld\n", &msgrvdata.centsDeposited, sizeof(msgrvdata.centsDeposited));
    printf("centsWithdrawn= %p   %ld\n", &msgrvdata.centsWithdrawn, sizeof(msgrvdata.centsWithdrawn));
    printf("numDeps=        %p   %ld\n", &msgrvdata.numDeps, sizeof(msgrvdata.numDeps));
    printf("numWds=         %p   %ld\n", &msgrvdata.numWds, sizeof(msgrvdata.numWds));
    printf("msgrvdata=      %p   %ld\n", &msgrvdata, sizeof(msgrvdata));
    
    return 0;
}
