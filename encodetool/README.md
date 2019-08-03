encodetool
==========

Encode / decode MIME quoted-printable ("="), URL encode ("%"),
and HTML numeric entities ("&"). Replacement for a bunch of
individual Perl utilities called `encode%`, `decode%`, `encode&`,
etc.  So this, to better emulate that this will detect encode/decode
mode (using first two characters of the program name) and style (a
"%", "=", or "&" anywhere in name) from the filename it is invoked as.
As a bonus, the case option for encode is detected by the case of the
"e" in the "en".

And yes, backslashes were needed to run some of the originals and
some of the name variants this accepts.

The originals only worked on one string, and were "aggressive".
This has more output options.


Usage
-----

Taken from `--help` output:


    Usage:  encode% [ options ] text to encode
            decode% [ options ] text to decode

    Encode/decode hexadecimal escapes. The program name used will
    determine default encode/decode mode and style of encoding, but
    all defaults can be overridden with options.

    Options
       -e   --encode        mode is encoding
       -d   --decode        mode is decoding
       -u   --upper         when encoding, use upper case
       -l   --lower         when encoding, use lower case
       -z   --lazy          when encoding, do a lazy job, very little ecnoded
       -c   --conservative  when encoding, do a conserative job, encode a lot
       -a   --aggressive    when encoding, do an aggressive job, encode it all
       -%   --url           url-style : "%3D"   or "%3d"   for input '='
       -=   --mime          mime-style: "=3D"   or "=3d"   for input '='
       -&   --html          html-style: "&x3D;" or "&x3d;" for input '='
       -s   --space         in output, separate args with a space
       -n   --newline       in output, separate args with a newline
       -0   --nosep         in output, don't separate args


Building
--------

A simple `make` will create the program. `make install` will install to the
user's $HOME/bin

Author
------
Benjamin Elijah Griffin / Eli-the-Bearded
