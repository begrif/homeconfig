/* This same code block gets used in two places of encodetool.c,
 * so it's an include to force it to always be in sync.
 */

    switch(a) {
      case DECODING: case ENCODING:
	  mode = a;
	  break;

      case LOWER: case UPPER:
	  encodecase = a;
	  break;

      case AGGRESSIVE: case CONSERVATIVE: case LAZY:
	  encodework = a;
	  break;

      case URL: case HTML: case MIME:
	  style = a;
	  break;

      /* use separator values don't equal flags. Wah. */
      case NOSEP:
	  usesep = 0;
	  break;

      case NEWLINE:
	  usesep = '\n';
	  break;

      case SPACE:
	  usesep = ' ';
	  break;


      case '?':
          help(pname);
	  return 0;
    }
