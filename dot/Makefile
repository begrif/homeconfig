
INSTALLDIR=$(HOME)/
# INSTALLDIR=../Home/

install:
	mkdir -p $(INSTALLDIR)
	for f in `ls -F`; do case $$f in Makefile|Read*|READ*): nope ;; */) mkdir -p $(INSTALLDIR)/.$$f ; (cd $$f && tar cf - .) | (cd $(INSTALLDIR)/.$$f && tar xf -) ;; *) cp $$f $(INSTALLDIR)/.$$f || exit ;; esac; done
