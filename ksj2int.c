#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include "ksj_convert.h"

static wchar_t* factors[] = { L"零",L"壱",L"弐",L"参",L"四",L"五",L"六",L"七",L"八",L"九" };
static wchar_t* digits[] = {L"拾",L"百",L"千" };
static wchar_t* positional[] = {L"万", L"億"};


/**
 * @brief 漢数字文字列からint型を生成する
 * param[in] in 変換したい漢数字文字列
 * 最小値：零
 * 最大値：九億九千九百九十九万九千九百九十九
 * return 変換された整数値(0-999999999), -1:変換エラー
 */

#define MAX_BUFFER_SIZE	128
#define KSJ_ERROR -1

int
kansuuji2int(const char* kansuuji) {
    
    wchar_t ws[MAX_BUFFER_SIZE];
    int w_len;
    int i;
    // 状態フラグ
    bool hundred_million_flag = false;
    bool ten_thousand_flag = false;
    bool thousand_flag = false;
    bool hundred_flag = false;
    bool ten_flag = false;

    int c_num;
    int num_without_positional = 0; // 位取りを除いた数
    int factor = 0; // 1つの桁の数
    int num = 0;

    w_len = mbstowcs( ws, kansuuji, MAX_BUFFER_SIZE);


   for (i = 0; i < w_len; i++) {
//       printf("ws[%d] = %lc, mode=%d\n", i, ws[i], ksj2num(ws[i]));
//       printf("num=%d,num_without_positional=%d,factor=%d\n", num, num_without_positional, factor);
       c_num = ksj2num(ws[i]);
       switch (c_num) {
           case KSJ_ERROR:
             return KSJ_ERROR;
           case 100000000:
               if (hundred_million_flag == true) { 
                   return KSJ_ERROR;
               } 

               if (factor == 0) {
                   return KSJ_ERROR;
               }

               num += factor * 100000000;
               num_without_positional = 0;
               hundred_million_flag = true;
               thousand_flag = false;
               hundred_flag = false;
               ten_flag = false;
               factor = 0;
               break;

           case 10000:
               if (ten_thousand_flag == true) { 
                   return KSJ_ERROR;
               } 
               if (num_without_positional == 0 && factor == 0) {
                   return KSJ_ERROR;
               }
               if (factor == 0) {
                   factor = 1;
               }

               if (num_without_positional == 0) {
                   num_without_positional = 1;
               }
               num += (num_without_positional + factor) * 10000;
               num_without_positional = 0;
               ten_thousand_flag = true;;
               thousand_flag = false;
               hundred_flag = false;
               ten_flag = false;
               factor = 0;
               break;

           case 1000:
               if (thousand_flag == true) { 
                   return KSJ_ERROR;
               }
               if (factor == 0) {
                   factor = 1;
               }

               num_without_positional += factor * 1000;
               thousand_flag = true;
               factor = 0;
             break;

           case 100:
               if (hundred_flag == true) { 
                   return KSJ_ERROR;
               } 
               if (factor == 0) {
                   factor = 1;
               }

               num_without_positional  += factor * 100;
               hundred_flag = true;
               factor = 0;
               break;

           case 10:
               if (ten_flag == true) { 
                   return KSJ_ERROR;
               }
               if (factor == 0) {
                   factor = 1;
               }

               num_without_positional += factor * 10;
               ten_flag = true;
               factor = 0;
               break;

           default: // 0-9
               factor = c_num; 
       }
     
//       printf("num=%d,num_without_positional=%d,factor=%d\n", num, num_without_positional, factor);
    }
    num += num_without_positional + factor;

    return num;
}

int
ksj2num(wchar_t wc) {
  int i;
  // 数字かどうか
  for (i=0;i<10;i++) {
    if (wc == *(factors[i])) {
      return i;
    }
  }
  // 桁かどうか
  if (wc == *(digits[0])) {
    return 10;
  }
  if (wc == *(digits[1])) {
    return 100;
  }
  if (wc == *(digits[2])) {
    return 1000;
  }

  // 位取りかどうか
  if (wc == *(positional[0])) {
    return 10000;
  }
  if (wc == *(positional[1])) {
    return 100000000;
  }
  // ここまで一致しなかったらエラー
  return KSJ_ERROR;
}

#ifdef UNIT_TEST
int
main(int argc, char** argv) {
    int num;
    int i;
    setlocale( LC_CTYPE, "ja_JP.UTF-8" );

    for (i=1; i < argc; i++) {
        num = kansuuji2int(argv[i]);
        printf("漢数字=%s, num=%d\n", argv[i], num);
    }
}
#endif // UNIT_TEST
