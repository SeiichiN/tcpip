/****************************************
 * what_endian.c
 *
 * http://www-cms.phys.s.u-tokyo.ac.jp/~naoki/CIPINTRO/NETWORK/endian.html
 ****************************************/
#include <stdio.h>

int main(void)
{
  /*
   * 0x7f000001がどのような順で表されるか？
   * 7f,00,00,01 -- ビッグエンディアン
   * 01,00,00,7f -- リトルエンディアン
  */
  unsigned int x = 0x7f000001;
  unsigned char* ptr = (unsigned char *) &x;
  printf("%02x, %02x, %02x, %02x \n", ptr[0], ptr[1], ptr[2], ptr[3]);
  printf("\n");

    /*
     * ビッグエンディアンかリトルエンディアンかを調べる
     */
  /* intは4バイトの整数型である */
    unsigned int n = 1;
    /* 4バイトのchar型にキャストする */
    unsigned char *ptr2 = (unsigned char *) &n;
    printf("%02x, %02x, %02x, %02x \n",
           *ptr2, *(ptr2 +1), *(ptr2 + 2), *(ptr2 + 3));
  printf("\n");
    if (*ptr2)
      printf("Little endian\n");
    else
      printf("Big endian\n");
    
}
