
INSTALLDIR=$(HOME)/bin

install:
	mkdir -p $(INSTALLDIR)
	for f in `ls`; do case $$f in Makefile|Read*|READ*): nope ;; *) cp $$f $(INSTALLDIR) || exit ; chmod 755 $(INSTALLDIR)/$$f ;; esac; done
