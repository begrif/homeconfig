
INSTALLDIR=$(HOME)/notes

install:
	mkdir -p $(INSTALLDIR)
	for f in `ls -F`; do case $$f in Makefile|Read*|READ*): nope ;; */) tar cf - $$f | (cd $(INSTALLDIR) && tar xf -) ;; *) cp $$f $(INSTALLDIR) || exit ;; esac; done
