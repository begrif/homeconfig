
INSTALLDIR=$(HOME)/notes

install:
	mkdir -p $(INSTALLDIR)
	for f in `ls -F`; do \
	    case $$f in \
	        Makefile|Read*|READ*) : nope ;; \
		*/) tar cf - $$f | (cd $(INSTALLDIR) && tar xf -) ;; \
		*) cp $$f $(INSTALLDIR) || exit ;; \
	    esac; \
	done

# Notes that this "find-updates" will look at the install dir for changes,
# while the one in ../dot/Makefile only looks at files already in the repo.
# -type f -name .\* -prune -> exclude *files* (not dirs) starting with .
# -o -type f -print        -> print remaining files
find-updates:
	for f in `cd $(INSTALLDIR) && find . -type f -name .\* -prune -o -type f -print`; do \
	    case $$f in \
	        ./Makefile|./Read*|./READ*) : nope ;; \
	        *) cmp $(INSTALLDIR)/$$f $$f || cp $(INSTALLDIR)/$$f $$f ;; \
	    esac; \
	done
