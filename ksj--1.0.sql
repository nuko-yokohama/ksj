/* contrib/ksj/ksj--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION ksj" to load this file. \quit

CREATE TYPE ksj;

CREATE FUNCTION ksj_in(cstring)
RETURNS ksj
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE FUNCTION ksj_out(ksj)
RETURNS cstring
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT IMMUTABLE;

CREATE TYPE ksj (
        INTERNALLENGTH = 4,
        INPUT = ksj_in,
        OUTPUT = ksj_out,
        STORAGE = plain
);

--
-- 四則演算
-- 
CREATE FUNCTION ksj_add(ksj, ksj)
RETURNS ksj
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR + (
    leftarg = ksj,
    rightarg = ksj,
    procedure = ksj_add,
    commutator = +
);

CREATE FUNCTION ksj_sub(ksj, ksj)
RETURNS ksj
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR - (
    leftarg = ksj,
    rightarg = ksj,
    procedure = ksj_sub
);

CREATE FUNCTION ksj_mul(ksj, ksj)
RETURNS ksj
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR * (
    leftarg = ksj,
    rightarg = ksj,
    procedure = ksj_mul
);

CREATE FUNCTION ksj_div(ksj, ksj)
RETURNS ksj
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR / (
    leftarg = ksj,
    rightarg = ksj,
    procedure = ksj_div
);

CREATE FUNCTION ksj_mod(ksj, ksj)
RETURNS ksj
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR % (
    leftarg = ksj,
    rightarg = ksj,
    procedure = ksj_mod
);

--
-- 集約演算
--

-- SUM
CREATE AGGREGATE sum (ksj)
(
    sfunc = ksj_add,
    stype = ksj,
    initcond = '零'
);


--  MAX
CREATE FUNCTION ksj_max(ksj, ksj)
RETURNS ksj
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE AGGREGATE max (ksj)
(
    sfunc = ksj_max,
    stype = ksj,
    initcond = '零'
);

-- MIN
CREATE FUNCTION ksj_min(ksj, ksj)
RETURNS ksj
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;
CREATE AGGREGATE min (ksj)
(
    sfunc = ksj_min,
    stype = ksj,
    initcond = '九億九千九百九拾九万九千九百九拾九'
);


--
-- 比較演算
--

-- 等しい
CREATE FUNCTION ksj_eq(ksj, ksj)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
    leftarg = ksj,
    rightarg = ksj,
    procedure = ksj_eq,
    commutator = =,
    RESTRICT = eqsel
);

-- 等しくない
CREATE FUNCTION ksj_ne(ksj, ksj)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <> (
    leftarg = ksj,
    rightarg = ksj,
    procedure = ksj_ne,
    RESTRICT = neqsel
);

-- より小さい
CREATE FUNCTION ksj_lt(ksj, ksj)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
    leftarg = ksj,
    rightarg = ksj,
    procedure = ksj_lt,
    RESTRICT = scalarltsel
);


-- より大きい
CREATE FUNCTION ksj_gt(ksj, ksj)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR > (
    leftarg = ksj,
    rightarg = ksj,
    procedure = ksj_gt,
    RESTRICT = scalargtsel
);

-- 以下
CREATE FUNCTION ksj_le(ksj, ksj)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <= (
    leftarg = ksj,
    rightarg = ksj,
    procedure = ksj_le,
    RESTRICT = scalarltsel
);


-- 以上
CREATE FUNCTION ksj_ge(ksj, ksj)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR >= (
    leftarg = ksj,
    rightarg = ksj,
    procedure = ksj_ge,
    RESTRICT = scalargtsel
);


-- B-treeサポートルーチン
CREATE FUNCTION ksj_cmp(ksj, ksj)
RETURNS integer
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

-- ksj演算子クラス
CREATE OPERATOR CLASS ksj_ops
    DEFAULT FOR TYPE ksj USING btree AS
        OPERATOR        1       < ,
        OPERATOR        2       <= ,
        OPERATOR        3       = ,
        OPERATOR        4       >= ,
        OPERATOR        5       > ,
        FUNCTION        1       ksj_cmp(ksj, ksj);


-- CAST関数とCAST定義
CREATE FUNCTION ksj_to_int32(ksj)
RETURNS integer
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (ksj AS integer) WITH FUNCTION ksj_to_int32(ksj);

CREATE FUNCTION int32_to_ksj(integer)
RETURNS ksj
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (integer AS ksj) WITH FUNCTION int32_to_ksj(integer);
