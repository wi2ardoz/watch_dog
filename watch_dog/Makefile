###############################################################################
# 	FILENAME:	watchdog.h    AUTHOR: Liad Oz	 TEAM: Yellow    LAB: OL100	  #
#																			  #
#	PURPOSE:    Makefile for watchdog project.							 	  #
#				Targets: wd.so, wd_main.out, wd_test.out (DEBUG/RELEASE)	  #
#																			  #
###############################################################################

# subject name
SUBJECT = watchdog
SUBJECT1 = watchdog_main
SUBJECT2 = watchdog_test

# directories: include, so files, o files, bin files
IDIR = include
SDIR = src
LDIR = lib
ODIR = obj
TDIR = test
EDIR = export

# dependancies
DEP = scheduler
IDIR2 = ../../ds/include

# compilation details
CC = gcc
DEBUG = -g
PTHREAD = -pthread
RELEASE = -DNDEBUG -O3
LDFLAGS = -L $(LDIR) -Wl,-rpath=$(LDIR)
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -fpic $(PTHREAD) -I $(IDIR2) -I $(IDIR) 

# list of object file names
OBJNAMES = $(SUBJECT)*.o
SOBJNAMES = lib_$(SUBJECT)*.so lib_$(DEP)*.so
DOBJECTSNAMES = $(SUBJECT)*.d $(DEP)*.d

# list of object file paths
OBJECTS = $(addprefix $(ODIR)/,$(OBJNAMES))
SOBJECTS = $(addprefix $(LDIR)/,$(SOBJNAMES))
DOBJECTSNAMES = $(addprefix $(ODIR)/,$(DOBJNAMES))

# phony targets
.PHONY: clean all release debug test run

# all
all: debug

############################## debug version ################################## 

debug: $(LDIR)/lib_$(SUBJECT)_DEBUG.so $(TDIR)/$(SUBJECT)_DEBUG.out $(TDIR)/$(SUBJECT)_test_DEBUG.out

# wd_DEBUG.so
$(LDIR)/lib_$(SUBJECT)_DEBUG.so: $(ODIR)/$(DEP)_DEBUG.o $(ODIR)/$(SUBJECT)_DEBUG.o
	$(CC) -shared $^ -o $@

$(ODIR)/$(SUBJECT)_DEBUG.o: $(SDIR)/$(SUBJECT).c
	$(CC) -c $(CFLAGS) $(DEBUG) $< -o $@

# wd_DEBUG.out
$(TDIR)/$(SUBJECT)_DEBUG.out: 
	$(CC) $(CFLAGS) $(DEBUG) $(SDIR)/$(SUBJECT1).c $(LDIR)/lib_$(SUBJECT)_DEBUG.so $(LDFLAGS) -o $@

# wd_test_DEBUG.out
$(TDIR)/$(SUBJECT)_test_DEBUG.out: 
	$(CC) $(CFLAGS) $(DEBUG) $(SDIR)/$(SUBJECT2).c $(LDIR)/lib_$(SUBJECT)_DEBUG.so $(LDFLAGS) -o $@

############################# release version #################################

release: $(LDIR)/lib_$(SUBJECT)_RELEASE.so \
	$(TDIR)/$(SUBJECT)_RELEASE.out \
	$(TDIR)/$(SUBJECT)_test_RELEASE.out

# wd_RELEASE.so
$(LDIR)/lib_$(SUBJECT)_RELEASE.so: $(ODIR)/$(DEP)_RELEASE.o \
	$(ODIR)/$(SUBJECT)_RELEASE.o \
	$(CC) -shared $< -o $@

$(ODIR)/$(SUBJECT)_RELEASE.o: $(SDIR)/$(SUBJECT).c
	$(CC) -c $(CFLAGS) $< -o $@

# wd_RELEASE.out
$(TDIR)/$(SUBJECT)_RELEASE.out : 
	$(CC) $(CFLAGS) $(SDIR)/$(SUBJECT1).c
	$(LDIR)/lib_$(SUBJECT)_RELEASE.so $(LDFLAGS) -o $@

# wd_test_RELEASE.out
$(TDIR)/$(SUBJECT)_test_RELEASE.out : 
	$(CC) $(CFLAGS) $(SDIR)/$(SUBJECT2).c
	$(LDIR)/lib_$(SUBJECT)_RELEASE.so $(LDFLAGS) -o $@

################################## utils ######################################
clean:
	rm  ./a.out ./core $(BDIR)/$(SUBJECT) $(SOBJECTS) $(DOBJECTS)
###############################################################################