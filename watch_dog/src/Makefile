#######################################################################################
# SUBJECT: Watch Dog MAKEFILE
#
# DEVELOPER: Rotem Cohen				TEAM: Green
#
#######################################################################################
#Variable Definitions

IDIR1 = ../include
IDIR2 = ../../../ds/include
ODIR = ../obj
LDIR = ../lib
DSDIR = ../../../ds/src
ORIGIN = $(LDIR)
TDIR = ../test

TARGET1 = watchdog_main
TARGET2 = watchdog_test

DS_SRCS = scheduler.c task.c uid.c pq_heap.c dynamic_vector.c heap.c
DS_OBJS_DEBUG = $(addprefix $(ODIR)/,$(patsubst %.c,%_DEBUG.o,$(notdir $(DS_SRCS))))
DS_OBJS_RELEASE = $(addprefix $(ODIR)/,$(patsubst %.c,%_RELEASE.o,$(notdir $(DS_SRCS))))

CC = gcc -pthread -ansi -pedantic-errors -Wall -Wextra -I $(IDIR1) -I $(IDIR2) -fPIC
DEBUG = -g
REALESE = -DNDEBUG -O3

deps := $(patsubst %.o,%.d,$(OBJS))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

##########################################################################################################

all: debug release

##########################################################################################################
#builds release version

release: $(LDIR)/lib_schesduler_RELEASE.so $(TARGET1)_RELEASE.out $(TARGET2)_RELEASE.out

$(TARGET2)_RELEASE.out: $(TDIR)/$(TARGET2).c
	$(CC) $^ -L$(ORIGIN) -Wl,-rpath=$(ORIGIN) -o $@ -l_schesduler_RELEASE

$(TARGET1)_RELEASE.out: $(TARGET1).c
	$(CC) $^ -L$(ORIGIN) -Wl,-rpath=$(ORIGIN) -o $@ -l_schesduler_RELEASE

$(LDIR)/lib_schesduler_RELEASE.so: $(DS_OBJS_RELEASE) $(ODIR)/watchdog_RELEASE.o
	$(CC) -shared $(DS_OBJS_RELEASE) $(ODIR)/watchdog_RELEASE.o -o $@

$(ODIR)/watchdog_RELEASE.o: watchdog.c
	$(CC) -c watchdog.c -o $@ $(DEPFLAGS)

$(ODIR)/%_RELEASE.o: $(DSDIR)/scheduler/%.c
	$(CC) -c $(RELEASE) $< -o $@ $(DEPFLAGS)

$(ODIR)/%_RELEASE.o: $(DSDIR)/uid/%.c
	$(CC) -c $(RELEASE) $< -o $@ $(DEPFLAGS)

$(ODIR)/%_RELEASE.o: $(DSDIR)/heap/%.c
	$(CC) -c $(RELEASE) $< -o $@ $(DEPFLAGS)

$(ODIR)/%_RELEASE.o: $(DSDIR)/pq_heap/%.c
	$(CC) -c $(RELEASE) $< -o $@ $(DEPFLAGS)

$(ODIR)/%_RELEASE.o: $(DSDIR)/dynamic_vector/%.c
	$(CC) -c $(RELEASE) $< -o $@ $(DEPFLAGS)

############################################################################################################
#builds debug version

debug: $(LDIR)/lib_schesduler_DEBUG.so $(TARGET1)_DEBUG.out $(TARGET2)_DEBUG.out

$(TARGET1)_DEBUG.out: $(TARGET1).c
	$(CC) $(DEBUG) $^ -L$(ORIGIN) -Wl,-rpath=$(ORIGIN) -o $@ -l_schesduler_DEBUG

$(LDIR)/lib_schesduler_DEBUG.so: $(DS_OBJS_DEBUG) $(ODIR)/watchdog_DEBUG.o
	$(CC) $(DEBUG) -shared $(DS_OBJS_DEBUG) $(ODIR)/watchdog_DEBUG.o -o $@

$(TARGET2)_DEBUG.out: $(TDIR)/$(TARGET2).c
	$(CC) $(DEBUG) $^ -L$(ORIGIN) -Wl,-rpath=$(ORIGIN) -o $@ -l_schesduler_DEBUG

$(ODIR)/watchdog_DEBUG.o: watchdog.c
	$(CC) -c $(DEBUG) watchdog.c -o $@ $(DEPFLAGS)

$(ODIR)/%_DEBUG.o: $(DSDIR)/scheduler/%.c
	$(CC) -c $(DEBUG) $< -o $@ $(DEPFLAGS)

$(ODIR)/%_DEBUG.o: $(DSDIR)/uid/%.c
	$(CC) -c $(DEBUG) $< -o $@ $(DEPFLAGS)

$(ODIR)/%_DEBUG.o: $(DSDIR)/heap/%.c
	$(CC) -c $(DEBUG) $< -o $@ $(DEPFLAGS)

$(ODIR)/%_DEBUG.o: $(DSDIR)/pq_heap/%.c
	$(CC) -c $(DEBUG) $< -o $@ $(DEPFLAGS)

$(ODIR)/%_DEBUG.o: $(DSDIR)/dynamic_vector/%.c
	$(CC) -c $(DEBUG) $< -o $@ $(DEPFLAGS)
############################################################################################################
#Utilities
.PHONY: clean all except source file

clean:
	rm -f $(ODIR)/*.o $(LDIR)/*.so *.out $(ODIR)/*.d
