
INSTALLDIR=$(HOME)/bin

# leave blank to use system default or set to an explicit path
USE_PERL=

install:
	mkdir -p $(INSTALLDIR)
	for f in `ls`; do \
	    case $$f in \
	        Makefile|Installer|Read*|READ*) : nope ;; \
		*) ./Installer $$f $(INSTALLDIR) $(USE_PERL) || exit ;;\
	    esac; \
	done
	if [ -x /usr/bin/locate ] ; then \
	    echo "have native locate, renaming file locate" ; \
	    mv $(INSTALLDIR)/locate $(INSTALLDIR)/flocate ; \
	fi
