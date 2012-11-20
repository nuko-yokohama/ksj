# contrib/ksj/Makefile

MODULE_big = ksj
OBJS = ksj_io.o ksj2int.o int2ksj.o 

EXTENSION = ksj
DATA = ksj--1.0.sql 

REGRESS = ksj

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/ksj
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
