
#include <stdio.h>

/* Macros for tweak()ing */
#define datesep		('/')
#define ADD(a,b)	((a)*256+(b))
#define cur		(date[src])
#define cursp		(cur==' ')
#define curnsp		(cur!=' ')
#define next		(date[src+1])
#define nnext		(date[src+2])
#define setnext		(date[dest++])
#define pad		(setnext='\t')
#define skip		(src++)
#define dcp		(setnext=date[skip]) /*(date[dest++]=date[src++])*/
#define finish		if(src==dest) {return;} else {\
			  while(cur){dcp;} \
			  setnext='\0'; \
			}

/* Macros for main */
#define effectlen	(addrlen+((dougno>10)?((dougno>100)?2:1):0))
#define shlen		22
#define shlendg		19
#define shortflag	(dougno==1?(effectlen<shlen):(effectlen<shlendg))
#define vshlen		14
#define vshlendg	11
#define vshortpad	""
/*
#define vshortpad	((dougno==1?(effectlen<vshlen):(effectlen<vshlendg))?\
			 "\t":"")
*/
#define X4k		(0x1000)
#define X1k		(0x400)
#define From		"From "
#define Fsize		5   /* bytes in "From " */

#define BLANK_STATE	1	/* previous line was blank (also initial state) */
#define HEADER_STATE	2	/* previous line non-blank, in headers */
#define NORMAL_STATE	3	/* previous line non-blank, not in headers */

/* Simple rewriting of the date. An RE would be mighty handy here. */
void tweak(date,isshort)char*date;{
  int month,tp=0,src=0,dest=0;
  char time[8];

  if (cursp) {
    if (cur==next) {
      if (isshort) {pad;}
      skip;
    }
    pad;
    skip;
  }
  /* Day of week */
  switch(ADD(cur,next)) {
    case ADD('S','u'):
    case ADD('M','o'):
    case ADD('T','u'):
    case ADD('W','e'):
    case ADD('T','h'):
    case ADD('F','r'):
    case ADD('S','a'):
      dcp; dcp;
      if (curnsp) { skip; } else { finish; }
      break;
    default:
      finish;
  }
  if (cursp) { dcp; } else { finish; }
  /* Month of year */
  switch(ADD(cur,next)) {
    case ADD('J','a'):
      month=1;
      break;
    case ADD('F','e'):
      month=2;
      break;
    case ADD('M','a'): /* March or May */
      if (nnext=='r') {
	month=3;
      } else
      if (nnext=='y') {
	month=5;
      } else
        finish;
      break;
    case ADD('A','p'):
      month=4;
      break;
    case ADD('J','u'): /* June or July */
      if (nnext=='n') {
	month=6;
      } else
      if (nnext=='l') {
	month=7;
      } else
        finish;
      break;
    case ADD('A','u'):
      month=8;
      break;
    case ADD('S','e'):
      month=9;
      break;
    case ADD('O','c'):
      month=10;
      break;
    case ADD('N','o'):
      month=11;
      break;
    case ADD('D','e'):
      month=12;
      break;
    default:
      finish;
  }
  if (month>9) {
    setnext='1';
    setnext=('0'+month-10);
    skip; skip;
  } else {
    setnext='0';
    setnext=('0'+month);
    skip; skip;
  }
  /* Third day in month abbr. */
  if (curnsp) { skip; } else { finish; }
  /* Skip space in date, and set seperator. */
  if (cursp) { setnext=datesep; skip; } else { finish; }
  /* Skip second space in date, if present. */
  if (cursp) { skip; }
  /* Day of month */
  while(curnsp) {dcp;}
  skip; /* space */
  /* Time of day (yank) */
  while((curnsp)&&(tp<8)) {time[tp++]=cur;skip;}
  if (tp!=8) { finish; }
  if (cursp) { setnext=datesep; skip; } else { finish; }
  /* Build in some Y2K problems. Yay! */
  skip; skip; /* century */
  dcp; dcp;   /* year    */
  /* Time of day (put) */
  setnext=' ';
  for(tp=0;tp<8;setnext=time[tp++]);
  /* newline */
  dcp;
  /* null */
  dcp;
}

int main(ac,av)char**av;{
  int lino=0,dougno=0,dougline=0,mesno=0,addrlen=1,pos=0;
  int state;
  FILE *mail;
  char line[X4k /* 4k */],address[X1k /* 1k */],last[X1k /* 1k */],*MAIL;

  MAIL=(char*)getenv("MAIL");
  if (!MAIL) { (void)fprintf(stderr,"Can't find MAIL variable!\n"); return(1);}
  mail=fopen(MAIL,"r");
  if (!mail) { (void)fprintf(stderr,"Can't open mail file!\n"); return(1);}

  address[0]='\f';
  address[1]=0;

  state = BLANK_STATE;

  while (fgets(line,X4k,mail)!=NULL) {
    lino++;
    if (!strncmp(line,From,Fsize)) {

      if(state != BLANK_STATE) {
        (void)printf("Conjoined: line %d, messages: %d and %d\n", lino, mesno, mesno+1);
      }
      mesno++;
      state = HEADER_STATE;


      if (!strncmp(line+Fsize,address,addrlen)) { /* still matching */
	dougno++;
      } else { /* no longer matching */
	if (dougno==1) { /* only found one */
	  tweak(last+addrlen,shortflag);
	  (void)printf("%5d:%3.3d:%s%s",dougline,mesno-1,vshortpad,last);
	  dougline=0;
	} else { /* found more than one (or first time through) */
	  if (dougno) {
	      tweak(last+addrlen,shortflag);
	      (void)printf("%5d:%3.3d:(%d)%s%s",dougline,mesno-dougno,dougno,
		  vshortpad,last);
	  }
	  dougno=1;
	}
	dougline=lino;
	for(pos=0;    last[pos]=line[pos+Fsize]      ;pos++);
	for(pos=0;(address[pos]=line[pos+Fsize])!=' ';pos++);
	address[pos]=0;
	addrlen=pos;
      }
    } else if (*line == '\n') {
      state = BLANK_STATE;
    } else {
      state = NORMAL_STATE;
    }
  }

  if (dougno>1) {
    tweak(last+addrlen,shortflag);
    (void)printf("%5d:%3.3d:(%d)%s%s",dougline,mesno-dougno+1,dougno,
        vshortpad,last);
  } else
  if (dougno) {
    tweak(last+addrlen,shortflag);
    (void)printf("%5d:%3.3d:%s%s",dougline,mesno,vshortpad,last);
  } else {
    (void)printf("%5d:%3.3d: -- NO MAIL\n",0,0);
  }
  return(0);
}

/* TEST FILE next line to end of file
From sendersk@example.com  Tue Aug  2 12:01:01 2011
From: message 1
Subject: mess 1

Body

From senders1@example.com  Tue Aug  2 12:02:02 2011
From: message 2
Subject: mess 2

Body

From sendersa@example.com  Tue Aug  2 12:03:03 2011
From: message 3
Subject: mess 3

Body

From senders2@example.com  Tue Aug  2 12:04:04 2011
From: message 4
Subject: mess 4

Body

From senders2@example.com  Tue Aug  2 12:05:05 2011
From: message 5
Subject: mess 5

Body

From senders2@example.com  Tue Aug  2 12:06:06 2011
From: message 6
Subject: mess 6

Body
From senders2@example.com  Tue Aug  2 12:07:07 2011
From: message 7
Subject: mess 7

Messages 6 and 7 are conjoined

From sendersb@example.com  Tue Aug  2 12:08:08 2011
From: message 8
Subject: mess 8

Body

*/

