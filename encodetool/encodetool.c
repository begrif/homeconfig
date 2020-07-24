/* encode / decode MIME qp ("="), URL encode ("%"), and HTML
 * numeric entities ("&"). Replacement for a bunch of individual
 * utilities called encode% encode& encode= decode% decode& and
 * decode= so this will detect encode/decode and style (plus
 * case option for encode) from the filename it is invoked as.
 *
 * And yes, backslashes are needed to run some of them.
 *
 * The originals only worked on one arg, and were "aggressive".
 * This has more options.
 *
 * Benjamin Elijah Griffin / Eli-the-Bearded
 */
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>	/* for strtol() */
#include <string.h>	/* for strrchr() */
#include <ctype.h>	/* for isxdigit()/isdigit() */


#define NOSEP         '0'	/* zero */
#define AGGRESSIVE    'a'
#define CONSERVATIVE  'c'
#define DECODING      'd'
#define ENCODING      'e'
#define LOWER         'l'
#define NEWLINE       'n'
#define SPACE         's'
#define UPPER         'u'
#define LAZY          'z'
#define URL           '%'
#define MIME          '='
#define HTML          '&'

static struct option opts[] = {
   { "encode",       no_argument,   0,  ENCODING },     /* for mode       */
   { "decode",       no_argument,   0,  DECODING },     /* for mode       */
   { "upper",        no_argument,   0,  UPPER },        /* for encodecase */
   { "lower",        no_argument,   0,  LOWER },        /* for encodecase */
   { "lazy",         no_argument,   0,  LAZY },         /* for encodework */
   { "aggressive",   no_argument,   0,  AGGRESSIVE },   /* for encodework */
   { "conservative", no_argument,   0,  CONSERVATIVE }, /* for encodework */
   { "url",          no_argument,   0,  URL },          /* for style      */
   { "mime",         no_argument,   0,  MIME },         /* for style      */
   { "html",         no_argument,   0,  HTML },         /* for style      */
   { "space",        no_argument,   0,  SPACE },        /* for usesep     */
   { "newline",      no_argument,   0,  NEWLINE },      /* for usesep     */
   { "nosep",        no_argument,   0,  NOSEP },        /* for usesep     */
   { "help",         no_argument,   0,  '?' },          /* invoke help    */
   { 0, 0, 0, 0 }
};

void
help(char *name) {
  printf("encode%% / decode%% (run as \"%s\") help\n", name);
  puts("Usage:\tencode% [ options ] text to encode");
  puts("\tdecode% [ options ] text to decode");
  puts("Encode/decode hexadecimal escapes. The program name used will");
  puts("determine default encode/decode mode and style of encoding, but");
  puts("all defaults can be overridden with options.\n");
  puts("Options");
  puts("   -e   --encode        mode is encoding");
  puts("   -d   --decode        mode is decoding");
  puts("   -u   --upper         when encoding, use upper case");
  puts("   -l   --lower         when encoding, use lower case");
  puts("   -z   --lazy          when encoding, do a lazy job, very little ecnoded");
  puts("   -c   --conservative  when encoding, do a conserative job, encode a lot");
  puts("   -a   --aggressive    when encoding, do an aggressive job, encode it all");
  puts("   -%   --url           url-style : \"%3D\"   or \"%3d\"   for input '='");
  puts("   -=   --mime          mime-style: \"=3D\"   or \"=3d\"   for input '='");
  puts("   -&   --html          html-style: \"&#x3D;\" or \"&#x3d;\" for input '='");
  puts("   -s   --space         in output, separate args with a space");
  puts("   -n   --newline       in output, separate args with a newline");
  puts("   -0   --nosep         in output, don't separate args");
} /* help() */

/* parse the program name and return the same values getopt() would
 * based on name traits. c should be 0 on first run, and is otherwise
 * used for state.
 */
int
nameopt(char *name, int *c)
{
  int rc = ' ';

  if(!name || (name[*c] == 0)) {
    return -1;
  }

  if(*c == 0) {
    /* ENCODE... or Encode... or Decode... of DECODE...
     * There are two things encoded here: case and mode, so we need to get called
     * twice to get them both.
     */
    if(   ((name[0] == 'E') && ((name[1] == 'N') || (name[1] == 'n')))
       || ((name[0] == 'D') && ((name[1] == 'E') || (name[1] == 'e')))
      ) {
      *c = -1;
      return UPPER;
    }
  }

  /* Not upper case OR upper case and already handled that bit */
  if((*c == 0) || (*c == -1)) {

    if(   ((name[0] == 'E') || (name[0] == 'e'))
       && ((name[1] == 'N') || (name[1] == 'n'))) {
      *c = 2;
      return ENCODING;
    }

    if(   ((name[0] == 'D') || (name[0] == 'd'))
       && ((name[1] == 'E') || (name[1] == 'e'))) {
      *c = 2;
      return DECODING;
    }

    *c = 1;
    return rc;
  }

  switch( name[*c] ) {
      case URL: case HTML: case MIME:
	  rc = name[*c];
  }

  *c = *c + 1;
  return rc;
} /* nameopt() */

/* Converts a pair of hexadecimal digits (previously verified)
 * into a 0-255 value; use instead of strtol() because we ONLY
 * want a pair at a time.
 */
int
hexpair(char a, char b)
{
  int n = 0;
  if        (a < 'A') { n  =      a - NOSEP; }
    else if (a < AGGRESSIVE) { n  = 10 + a - 'A'; }
    else              { n  = 10 + a - AGGRESSIVE; }
  n <<= 4;
  if        (b < 'A') { n +=      b - NOSEP; }
    else if (b < AGGRESSIVE) { n += 10 + b - 'A'; }
    else              { n += 10 + b - AGGRESSIVE; }
  return n;
} /* hexpair() */

/* Decode & encoded value; this is tricky since & encoded is both longer
 * than ones and can have multiple bytes in one & block, so the length
 * isn't even constant. And might as well decode decimal version, too.
 *
 * The string should start out at the initial &. Prints (to stdout) the
 * decoded value, or just "&" if not a valid sequence.
 *
 * Returns number of characters consumed from the input string, not
 * counting the "&" itself.
 */
int
decode_amp(char *str)
{
  int len = 0;
  long int out;
  char *endp, unit;

  while(str[len] != ';') {
    if(!str[len]) {
      putchar(str[0]);
      return 0;
    }
    len ++;
  }

  if(str[1] == 'x') {
    /* hex */
    out = strtol(&(str[2]), &endp, 16);
  } else if(isdigit(str[1])) {
    /* decimal */
    out = strtol(&(str[1]), &endp, 10);
  } else {
    /* named, unhandled */
    endp = str;
  }

  if(';' != *endp) {
    /* didn't parse the number */
    putchar(str[0]);
    return 0;
  }
  while(out > 255) {
    unit = out & 255;
    out >>= 8;
    putchar(unit);
  }
  putchar(out);
  return len;
} /* decode_amp() */

int
main(int ac, char**av)
{
  int a,i;	/* used in args parsing */
  int c, sep = 0;
  int mode = ENCODING;
  int style = URL;
  int encodecase = LOWER;
  int encodework = LAZY;
  int usesep = ' ';
  char *pname, *carg, format[BUFSIZ];

  /* program name sans path */
  pname = strrchr(av[0], '/');
  if(pname) { 
    pname++;
  } else {
    pname = av[0];
  }

  /* defaults from program name */
  a = 0;
  i = 0;
  while( a != -1 ) {
    a = nameopt(pname, &i);
#include "option-switch.c"
  }

  /* build string for getopt */
  for(i = 0; opts[i].name; i++) {
    format[i] = opts[i].val;
  }
  format[i] = 0;

  /* options from command line */
  a = 0;
  while( a != -1 ) {
    a = getopt_long(ac, av, format, opts, &i);
#include "option-switch.c"
  }

  
  if(DECODING == mode) {
    while(optind < ac) {
      carg = av[optind];
      if(sep) { putchar(sep); }

      for(c = 0; carg[c] != 0; c++) {
	if(style == carg[c]) {
	  if(HTML == style) {
	    /* & style is a bit tricky, isolate for cleanliness */
            c += decode_amp(&(carg[c]));
	  } else {
	    if(isxdigit(carg[c+1]) && isxdigit(carg[c+2])) {
	      putchar(hexpair(carg[c+1],carg[c+2]));
	      c += 2;
	    } else {
	      /* bad encoding */
	      putchar(carg[c]);
	    }
	  }
	} else {
	  putchar(carg[c]);
	}
      }
      optind++;
      sep = usesep;
    }

    putchar('\n');
    return 0;
  } /* decoding */

  /* must be ENCODING if here */

  if(HTML == style) {
    /* produce a format like "&#x%X;" */
    sprintf(format, "&#x%%%c;",  (LOWER == encodecase)? 'x' : 'X');
  } else if (MIME == style) {
    /* produce a format like "=%x" */
    sprintf(format, "=%%%c",    (LOWER == encodecase)? 'x' : 'X');
  } else {
    /* produce a format like "%%%x" */
    sprintf(format, "%%%%%%%c", (LOWER == encodecase)? 'x' : 'X');
  }

  while(optind < ac) {
    if(sep) { putchar(sep); }
    carg = av[optind];

    for(c = 0; carg[c] != 0; c++) {
      int dothis = 0;

/* be terse for the big multi-case block */
#define C case

      switch(carg[c]) {
	/* each of these encoded if their own style or not lazy encoding */
        C '=': C '%': C '&':
	    if((carg[c] == style) || (LAZY != encodework)) { dothis = 1; }
	    break;

	/* encode spaces in % style or aggressive */
        C ' ':
	    if((URL == style) || (AGGRESSIVE == encodework)) { dothis = 1; }
	    break;

	/* encode angle brackets in & style or non-lazy */
        C '<': C '>':
	    if((URL == style) || (LAZY != encodework)) { dothis = 1; }
	    break;

	/* [0-9A-Za-z._-] only encode in aggressive */
        C '0': C '1': C '2': C '3': C '4': C '5': C '6': C '7': C '8': C '9':
	C 'A': C 'B': C 'C': C 'D': C 'E': C 'F': C 'G': C 'H': C 'I': 
	C 'J': C 'K': C 'L': C 'M': C 'N': C 'O': C 'P': C 'Q': C 'R':
	C 'S': C 'T': C 'U': C 'V': C 'W': C 'X': C 'Y': C 'Z':
	C 'a': C 'b': C 'c': C 'd': C 'e': C 'f': C 'g': C 'h': C 'i':
	C 'j': C 'k': C 'l': C 'm': C 'n': C 'o': C 'p': C 'q': C 'r':
	C 's': C 't': C 'u': C 'v': C 'w': C 'x': C 'y': C 'z': 
	C '.': C '_': C '-':
	    if(AGGRESSIVE == encodework) { dothis = 1; }
	    break;

	/* any thing else, encode if outside trad printable ASCII 
	 * or not lazy
	 */
        default:
	    if((LAZY != encodework) || (carg[c] < 32) || (carg[c] > 127)) {
	      dothis = 1;
	    }
      }

      if(dothis) {
        printf(format, carg[c]);
      } else {
        putchar(carg[c]);
      }
    }

    optind ++;
    sep = usesep;
  }
  putchar('\n');
  return 0;
} /* main() */
