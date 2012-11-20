#include <stdio.h>
#include <string.h>

#include "ksj.h"
#include "ksj_convert.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(ksj_in);
PG_FUNCTION_INFO_V1(ksj_out);

Datum ksj_in(PG_FUNCTION_ARGS);
Datum ksj_out(PG_FUNCTION_ARGS);

Datum ksj_in(PG_FUNCTION_ARGS) {
    int32 ksj_internal;
    char* result;
    char* ksj_str = PG_GETARG_CSTRING(0);
    setlocale( LC_CTYPE, "ja_JP.UTF-8" );
//    elog(NOTICE, "ksj_in, ksj_ste=%s", ksj_str);
    ksj_internal = kansuuji2int(ksj_str);

//    elog(NOTICE, "ksj_in, ksj_internal=%d", ksj_internal);
    if (ksj_internal == -1) {
	    elog(ERROR, "ksj parse error, ksj=%s", ksj_str);
    }

    result = (char*) palloc(sizeof(int32));
    *((int32*) result) = ksj_internal;

//    elog(NOTICE, "ksj_in, result=%d", *((int32*) result));
    PG_RETURN_POINTER(result);
}

Datum ksj_out(PG_FUNCTION_ARGS) {
    int32* ksj_internal = (int32*) PG_GETARG_POINTER(0);
    int ret;
    char ksj_str[128]; // 暫定
    char* result;
    
//   elog(NOTICE, "ksj_out, *ksj_internal=%d", *ksj_internal);
    // 漢数字化
    memset(ksj_str, 0x00, 128);
    ret = int2kansuuji( *ksj_internal, ksj_str);
    if (ret == -1) {
        elog(ERROR, "ksj range error, ksj_internal=%d", *ksj_internal);
    }
    
    result = palloc(128);
    strcpy(result, ksj_str);

//    elog(NOTICE, "ksj_out, result=%s", result);
    PG_RETURN_POINTER(result);
}

// 四則演算
PG_FUNCTION_INFO_V1(ksj_add);
Datum ksj_add(PG_FUNCTION_ARGS);

Datum ksj_add(PG_FUNCTION_ARGS) {
    int32* ksj_l = (int32*) PG_GETARG_POINTER(0);
    int32* ksj_r = (int32*) PG_GETARG_POINTER(1);

    char* result;
    

    result = (char*) palloc(sizeof(int32));
    *((int32*) result) = ( *ksj_l + *ksj_r);

    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(ksj_sub);
Datum ksj_sub(PG_FUNCTION_ARGS);

Datum ksj_sub(PG_FUNCTION_ARGS) {
    int32* ksj_l = (int32*) PG_GETARG_POINTER(0);
    int32* ksj_r = (int32*) PG_GETARG_POINTER(1);

    char* result;
    
    result = (char*) palloc(sizeof(int32));
    *((int32*) result) = ( *ksj_l - *ksj_r);

    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(ksj_mul);
Datum ksj_mul(PG_FUNCTION_ARGS);

Datum ksj_mul(PG_FUNCTION_ARGS) {
    int32* ksj_l = (int32*) PG_GETARG_POINTER(0);
    int32* ksj_r = (int32*) PG_GETARG_POINTER(1);

    char* result;
    

    result = (char*) palloc(sizeof(int32));
    *((int32*) result) = ( *ksj_l * *ksj_r);

    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(ksj_div);
Datum ksj_div(PG_FUNCTION_ARGS);

Datum ksj_div(PG_FUNCTION_ARGS) {
    int32* ksj_l = (int32*) PG_GETARG_POINTER(0);
    int32* ksj_r = (int32*) PG_GETARG_POINTER(1);

    char* result;
    

    result = (char*) palloc(sizeof(int32));
    *((int32*) result) = ( *ksj_l / *ksj_r);

    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(ksj_mod);
Datum ksj_mod(PG_FUNCTION_ARGS);

Datum ksj_mod(PG_FUNCTION_ARGS) {
    int32* ksj_l = (int32*) PG_GETARG_POINTER(0);
    int32* ksj_r = (int32*) PG_GETARG_POINTER(1);

    char* result;
    

    result = (char*) palloc(sizeof(int32));
    *((int32*) result) = ( *ksj_l % *ksj_r);

    PG_RETURN_POINTER(result);
}


// 集約演算用関数

// SUM関数用はksj_addで代替可能

// MAX関数用
PG_FUNCTION_INFO_V1(ksj_max);
Datum ksj_max(PG_FUNCTION_ARGS);

Datum ksj_max(PG_FUNCTION_ARGS) {
    int32* ksj_l = (int32*) PG_GETARG_POINTER(0);
    int32* ksj_r = (int32*) PG_GETARG_POINTER(1);

    char* result;

    result = (char*) palloc(sizeof(int32));
    if (*ksj_l > *ksj_r ) {
        *((int32*) result) = *ksj_l;
    } else {
        *((int32*) result) = *ksj_r;
    }

    PG_RETURN_POINTER(result);
}

// MIN関数用
PG_FUNCTION_INFO_V1(ksj_min);
Datum ksj_min(PG_FUNCTION_ARGS);

Datum ksj_min(PG_FUNCTION_ARGS) {
    int32* ksj_l = (int32*) PG_GETARG_POINTER(0);
    int32* ksj_r = (int32*) PG_GETARG_POINTER(1);

    char* result;

    result = (char*) palloc(sizeof(int32));
    if (*ksj_l < *ksj_r ) {
        *((int32*) result) = *ksj_l;
    } else {
        *((int32*) result) = *ksj_r;
    }

    PG_RETURN_POINTER(result);
}

// TODO:AVG関数用
// 面倒そうなのでPending
// PostgreSQL文書によると「平均値計算は典型的に配列を状態遷移値として使って実装され」るとのこと。

// TODO:AVG用状態遷移関数
// TODO:AVG用最終集約計算関数


// 比較演算子
// 比較用の内部関数を作成しておくと便利
static int
ksj_cmp_internal(int32 a, int32 b)
{
    if (a < b)
        return -1;
    if (a > b)
        return 1;
    return 0;
}

// eq 演算子関数
PG_FUNCTION_INFO_V1(ksj_eq);
Datum ksj_eq(PG_FUNCTION_ARGS);

Datum
ksj_eq(PG_FUNCTION_ARGS)
{
    int32* a = (int32*) PG_GETARG_POINTER(0);
    int32* b = (int32*) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(ksj_cmp_internal( *a, *b) == 0);
}

// ne 演算子関数
PG_FUNCTION_INFO_V1(ksj_ne);
Datum ksj_ne(PG_FUNCTION_ARGS);

Datum
ksj_ne(PG_FUNCTION_ARGS)
{
    int32* a = (int32*) PG_GETARG_POINTER(0);
    int32* b = (int32*) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(ksj_cmp_internal( *a, *b) != 0);
}

// lt 演算子関数
PG_FUNCTION_INFO_V1(ksj_lt);
Datum ksj_lt(PG_FUNCTION_ARGS);

Datum
ksj_lt(PG_FUNCTION_ARGS)
{
    int32* a = (int32*) PG_GETARG_POINTER(0);
    int32* b = (int32*) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(ksj_cmp_internal( *a, *b) < 0);
}

// gt 演算子関数
PG_FUNCTION_INFO_V1(ksj_gt);
Datum ksj_gt(PG_FUNCTION_ARGS);

Datum
ksj_gt(PG_FUNCTION_ARGS)
{
    int32* a = (int32*) PG_GETARG_POINTER(0);
    int32* b = (int32*) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(ksj_cmp_internal( *a, *b) > 0);
}

// le 演算子関数
PG_FUNCTION_INFO_V1(ksj_le);
Datum ksj_le(PG_FUNCTION_ARGS);

Datum
ksj_le(PG_FUNCTION_ARGS)
{
    int32* a = (int32*) PG_GETARG_POINTER(0);
    int32* b = (int32*) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(ksj_cmp_internal( *a, *b) <= 0);
}

// ge 演算子関数
PG_FUNCTION_INFO_V1(ksj_ge);
Datum ksj_ge(PG_FUNCTION_ARGS);

Datum
ksj_ge(PG_FUNCTION_ARGS)
{
    int32* a = (int32*) PG_GETARG_POINTER(0);
    int32* b = (int32*) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(ksj_cmp_internal( *a, *b) >= 0);
}

// 
// btreeインデクスサポート
//
// cmp演算子関数
PG_FUNCTION_INFO_V1(ksj_cmp);
Datum ksj_cmp(PG_FUNCTION_ARGS);

Datum
ksj_cmp(PG_FUNCTION_ARGS)
{
    int32* a = (int32*) PG_GETARG_POINTER(0);
    int32* b = (int32*) PG_GETARG_POINTER(1);

    PG_RETURN_INT32(ksj_cmp_internal( *a, *b));
}

// CAST関数

// ksj -> int32
PG_FUNCTION_INFO_V1(ksj_to_int32);
Datum ksj_to_int32(PG_FUNCTION_ARGS);

Datum
ksj_to_int32(PG_FUNCTION_ARGS)
{
    int32* a = (int32*) PG_GETARG_POINTER(0);

    PG_RETURN_INT32( *a );
}

// int32 -> ksj
PG_FUNCTION_INFO_V1(int32_to_ksj);
Datum int32_to_ksj(PG_FUNCTION_ARGS);

Datum
int32_to_ksj(PG_FUNCTION_ARGS)
{
    int32 a =  PG_GETARG_INT32(0);
    char* result = (char*) palloc(sizeof(int32));

    memcpy(result, &a, sizeof(int32));

    PG_RETURN_POINTER(result);
}

