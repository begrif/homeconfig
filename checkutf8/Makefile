
PREFIX = $(HOME)
CC=gcc
POD2MAN=/usr/local/bin/pod2man-5.24.3
CPPFLAGS=-D_BSD_SOURCE 

CHECKPROGS = checkutf8 check8859 checkascii
CHECKPROGS_SRCS = checkutf8.c utf-8.c
CHECKPROGS_OBJS=$(CHECKPROGS_SRCS:%.c=%.o)
TESTPROG = utf8test
ALL_PROG = $(CHECKPROGS) $(TESTPROG)

GENERATED_DOCS = checkutf8.1
MAN = $(GENERATED_DOCS)

default: all

all: $(PROG) $(CHECKPROGS)

# limited test suite
test: $(TESTPROG)
	@echo Ran tests for: $>
 
install: $(PROG) $(CHECKPROGS) $(MAN)
	chmod 755 $(PROG) $(CHECKPROGS)
	mkdir -p $(PREFIX)/bin
	cp $(PROG) $(CHECKPROGS) $(PREFIX)/bin/
	if [ -d $(PREFIX)/man/man1 ] ; then cp $(MAN) $(PREFIX)/man/man1 ; fi

$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
 
checkutf8: $(CHECKPROGS_OBJS)
	rm -f $(CHECKPROGS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(CHECKPROGS_OBJS) $(LIBS)

check8859: checkutf8
	ln -s $> $@

checkascii: checkutf8
	ln -s $> $@


.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<
 
.SUFFIXES: .1 .pod
.pod.1:
	$(POD2MAN) $< > $@

clean:
	rm -f $(ALL_PROG) core $(GENERATED_DOCS) *.o *~

utf8test: utf-8.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -D_RUN_TESTS -o $@ utf-8.c
	./$@ && echo $@ PASSED ALL TESTS

