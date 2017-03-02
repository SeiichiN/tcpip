/****************************************************
 * chk_int.c                                        *
 * 色々なデータ型の最大値、最小値                   *
 * http://www1.cts.ne.jp/~clab/hsample/IO/IO19.html *
 ****************************************************/
#include <stdio.h>
#include <limits.h>        /* 文字定数が定義されている */

void main(void)
{
  
    printf("char のサイズ: %d bit\n", CHAR_BIT);
    printf("char の最大値: %u\n", CHAR_MAX);
    printf("char の最小値: %d\n", CHAR_MIN);
    printf("int の最大値: %d\n", INT_MAX);
    printf("int  の最小値: %d\n", INT_MIN);
    printf("long の最大値: %ld\n", LONG_MAX);
    printf("long の最小値: %ld\n", LONG_MIN);
    printf("signed char の最大値: %d\n", SCHAR_MAX);
    printf("shigned char の最小値: %d\n", SCHAR_MIN);
    printf("short の最大値: %d\n", SHRT_MAX);
    printf("short の最小値: %d\n", SHRT_MIN);
    printf("unsigned char の最大値: %u\n", UCHAR_MAX);
    printf("unsigned int の最大値: %u\n", UINT_MAX);
    printf("unsigned long の最大値: %lu\n", ULONG_MAX);
    printf("unsigned short の最大値: %u\n", USHRT_MAX);

            
}
