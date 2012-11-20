#include <stdio.h>
#include <string.h>
#include <math.h>

#include "ksj_convert.h"

static char* factors[] = { "零","壱","弐","参","四","五","六","七","八","九" };
static char* digits[] = { "", "拾","百","千" };
static char* positional[] = { "", "万", "億","兆","系","垓"};

static int pow10int(int digit) {
    int i;
    int r = 1;
    if (digit < 0 || digit > 8) {
        // underflow or overflow
        return -1;
    }

    for (i=0; i < digit; i++) {
      r = r * 10;
    }
    return r;
}

/**
 * @brief int型から漢数字文字列を生成する
 * param[in] in 変換したい整数値(0-999999999)
 * param[out] 変換された漢数字領域（0x00クリアした十分な領域を外部で確保する）
 * return 0:正常、-1:範囲外エラー
 */
int
int2kansuuji(int  in, char* kansuuji) {
    int quotient;
    int digit;
    int mod = in;
    int i;

    if (in < 0 || in > 999999999) {
      return -1;
    }

    if (in == 0) {
      // 0の特殊処理
      strcat(kansuuji, factors[0]);
      return 0;
    }

    for (i = 8; i >= 0; i--) {
        digit = pow10int(i);
        quotient = mod / digit;
        if (quotient != 0) {
            strcat(kansuuji, factors[quotient]);
            strcat(kansuuji, digits[i % 4]);
        }
        // 位取り出力
        if ((i % 4) == 0) {
            if (quotient != 0) {
                strcat(kansuuji, positional[i /4]);
            }
        }
        mod = mod % digit;
    }

    return 0;
}

// テスト用メイン
#ifdef UNIT_TEST
int
main(int argc, char** argv) {
    int i;
    int test_data;
    char kansuuji[64];

    {
        memset(kansuuji, 0x00, 64);
        test_data = 123456;
        int2kansuuji(test_data, kansuuji);
        printf("test_data=%d, kansuuji=%s\n", test_data, kansuuji);

        memset(kansuuji, 0x00, 64);
        test_data = 10001; 
        int2kansuuji(test_data, kansuuji);
        printf("test_data=%d, kansuuji=%s\n", test_data, kansuuji);

        memset(kansuuji, 0x00, 64);
        test_data = 0; 
        int2kansuuji(test_data, kansuuji);
        printf("test_data=%d, kansuuji=%s\n", test_data, kansuuji);

        memset(kansuuji, 0x00, 64);
        test_data = 100010001; 
        int2kansuuji(test_data, kansuuji);
        printf("test_data=%d, kansuuji=%s\n", test_data, kansuuji);

        memset(kansuuji, 0x00, 64);
        test_data = 99999999; 
        int2kansuuji(test_data, kansuuji);
        printf("test_data=%d, kansuuji=%s\n", test_data, kansuuji);

        memset(kansuuji, 0x00, 64);
        test_data = 987654321; 
        int2kansuuji(test_data, kansuuji);
        printf("test_data=%d, kansuuji=%s\n", test_data, kansuuji);
    }
}
#endif // UNIT_TEST
