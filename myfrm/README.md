myfrm
=====

An mbox file "From" line extractor. This is very old code of mine, probably
from early 1990s, and was old in style when it was new. It did get a bug
fix change in 2011, but I didn't modernize it. Not super pretty.

Unlike other mbox from line programs, this one includes line and message
offset in the output. The line number was super useful to me circa 1993. 
Message numbers are still useful with mailx. Also unlike other mbox from line
programs, this will group together multiple messages from the same sender
making `myfrm|tail` be more useful when a badly configured alert has sent 500
messages in a few minutes.

Build
-----
make
make install

Author
------
Benjamin Elijah Griffin / Eli the Bearded
