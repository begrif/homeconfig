
INSTALLDIR=$(HOME)/
# INSTALLDIR=../Home/

install:
	mkdir -p $(INSTALLDIR) $(INSTALLDIR)/.empty
	for f in `ls -F`; do \
	    case $$f in \
	        Makefile|Read*|READ*) : nope ;; \
		*/) mkdir -p $(INSTALLDIR)/.$$f ; \
		    (cd $$f && tar cf - .) | \
		        (cd $(INSTALLDIR)/.$$f && tar xf -) ;; \
		*) cp $$f $(INSTALLDIR)/.$$f || exit ;; \
	    esac; \
	done

# Notes that this "find-updates" only looks at files here for changes,
# while the one in ../cheat/Makefile will look at the install dir.
# -type f -name .\* -prune -> exclude *files* (not dirs) starting with .
# -o -type f -print        -> print remaining files
find-updates:
	for f in `find . -type f -name .\* -prune -o -type f -print`; do \
	    case $$f in \
	        ./Makefile|./Read*|./READ*) : nope ;; \
	        *) d=$${f#./} ; \
		   cmp $(INSTALLDIR)/.$$d $$f || cp $(INSTALLDIR)/.$$d $$f ;; \
	    esac; \
	done

