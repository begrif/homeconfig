
PREFIX := $(HOME)
INSTALLBIN := $(PREFIX)/bin

PROGRAM = encodetool

encodetool: encodetool.c option-switch.c
	$(CC) encodetool.c -o $@

install: $(PROGRAM)
	mkdir -p $(INSTALLBIN)
	cp $(PROGRAM) $(INSTALLBIN)/
	chmod 755 $(INSTALLBIN)/$(PROGRAM)
	for mode in encode decode; do for style in '%' '&' '='; do \
	    ln -s $(INSTALLBIN)/$(PROGRAM) "$(INSTALLBIN)/$$mode$$style"; \
	  done; done

clean:
	rm -f $(PROGRAM) *.o core
