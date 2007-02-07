#include <sys/types.h>
#include "readwrite.h"
#include "sig.h"
#include "exit.h"
#include "open.h"
#include "seek.h"
#include "fmt.h"
#include "alloc.h"
#include "str.h"
#include "stralloc.h"
#include "substdio.h"
#include "datetime.h"
#include "now.h"
#include "fork.h"
#include "wait.h"
#include "extra.h"
#include "auto_qmail.h"
#include "auto_uids.h"
#include "date822fmt.h"
#include "fmtqfn.h"
#include "env.h"
#include "control.h"
#include "domainkeys.h"

#define DEATH 86400 /* 24 hours; _must_ be below q-s's OSSIFIED (36 hours) */
#define ADDR 1003

char inbuf[2048];
struct substdio ssin;
char outbuf[256];
struct substdio ssout;

datetime_sec starttime;
struct datetime dt;
unsigned long mypid;
unsigned long uid;
char *pidfn;
int messfd;
int readfd;

void die(e) int e; { _exit(e); }
void die_write() { die(53); }
void die_read() { die(54); }
void sigalrm() { /* thou shalt not clean up here */ die(52); }
void sigbug() { die(81); }
void maybe_die_dk(e) DK_STAT e; {
  switch(e) {
  case DK_STAT_BADKEY: _exit(55);
  case DK_STAT_CANTVRFY: _exit(74);
  case DK_STAT_NORESOURCE: _exit(51);
  case DK_STAT_ARGS: _exit(12);
  case DK_STAT_SYNTAX: _exit(31);
  case DK_STAT_INTERNAL: _exit(81);
  }
}

unsigned int pidfmt(s,seq)
char *s;
unsigned long seq;
{
 unsigned int i;
 unsigned int len;

 len = 0;
 i = fmt_str(s,"/tmp/qmail-dk."); len += i; if (s) s += i;
 i = fmt_ulong(s,mypid); len += i; if (s) s += i;
 i = fmt_str(s,"."); len += i; if (s) s += i;
 i = fmt_ulong(s,starttime); len += i; if (s) s += i;
 i = fmt_str(s,"."); len += i; if (s) s += i;
 i = fmt_ulong(s,seq); len += i; if (s) s += i;
 ++len; if (s) *s++ = 0;

 return len;
}

void pidopen()
{
 unsigned int len;
 unsigned long seq;

 seq = 1;
 len = pidfmt((char *) 0,seq);
 pidfn = alloc(len);
 if (!pidfn) die(51);

 for (seq = 1;seq < 10;++seq)
  {
   if (pidfmt((char *) 0,seq) > len) die(81); /* paranoia */
   pidfmt(pidfn,seq);
   messfd = open_excl(pidfn);
   if (messfd != -1) return;
  }

 die(63);
}

char tmp[FMT_ULONG];

char *dksign = 0;
stralloc dksignature = {0};
stralloc dkoutput = {0};
char *dkverify = 0;
char *dkqueue = 0;
DK_LIB *dklib;
DK *dk;
DK_STAT st;

static void write_signature(DK *dk, char *keyfn)
{
 char advice[2048];
 char *selector;
 char *from;
 static stralloc keyfnfrom = {0};
 int i;

 from = dk_from(dk);
 i = str_chr(keyfn, '%');
 if (keyfn[i]) {
   if (!stralloc_copyb(&keyfnfrom,keyfn,i)) die(51);
   if (!stralloc_cats(&keyfnfrom,from)) die(51);
   if (!stralloc_cats(&keyfnfrom,keyfn + i + 1)) die(51);
 } else {
   if (!stralloc_copys(&keyfnfrom,keyfn)) die(51);
 }
 if (!stralloc_0(&keyfnfrom)) die(51);

 switch(control_readfile(&dksignature,keyfnfrom.s,0)) {
 case 0:
   if (keyfn[i]) return;
   die(32);
 case 1: break;
 default: die(31);
 }
 for(i=0; i < dksignature.len; i++)
   if (dksignature.s[i] == '\0') dksignature.s[i] = '\n';
 if (!stralloc_0(&dksignature)) die(51);

 st = dk_getsig(dk, dksignature.s, advice, sizeof(advice));
 maybe_die_dk(st);

 if (!stralloc_cats(&dkoutput,
   "Comment: DomainKeys? See http://antispam.yahoo.com/domainkeys\n"
	   "DomainKey-Signature: a=rsa-sha1; q=dns; c=nofws;\n"
	   "  s=")) die(51);
 selector = keyfn;
 while (*keyfn) {
   if (*keyfn == '/') selector = keyfn+1;
    keyfn++;
 }
 if (!stralloc_cats(&dkoutput,selector)) die(51);
 if (!stralloc_cats(&dkoutput,"; d=")) die(51);
 if (from) {
   if (!stralloc_cats(&dkoutput,from)) die(51);
 } else if (!stralloc_cats(&dkoutput,"unknown")) die(51);
 if (!stralloc_cats(&dkoutput,";\n  b=")) die(51);
 if (!stralloc_cats(&dkoutput,advice)) die(51);
 if (!stralloc_cats(&dkoutput,"  ;\n")) die(51);
}

static char *binqqargs[2] = { "bin/qmail-queue", 0 } ;

void main(int argc, char *argv[])
{
 unsigned int len;
 char ch;
 int pim[2];
 int wstat;
 unsigned long pid;

 sig_blocknone();
 umask(033);

 dksign = env_get("DKSIGN");
 dkverify = env_get("DKVERIFY");
 if (!dksign && !dkverify)
   dksign = "/etc/domainkeys/%/default";

 dkqueue = env_get("DKQUEUE");
 if (dkqueue && *dkqueue) binqqargs[0] = dkqueue;
 else if (str_equal(argv[0]+str_rchr(argv[0], '/'), "/qmail-queue"))
   binqqargs[0] = "bin/qmail-queue.orig";

 if (dksign || dkverify) {
   dklib = dk_init(0);
   if (!dklib) die(51);
 }
 if (dksign) {
    dk = dk_sign(dklib, &st, DK_CANON_NOFWS);
    if (!dk) die(31);
 } else if (dkverify) {
    dk = dk_verify(dklib, &st);
    if (!dk) die(31);
 }

 mypid = getpid();
 uid = getuid();
 starttime = now();
 datetime_tai(&dt,starttime);

 sig_pipeignore();
 sig_miscignore();
 sig_alarmcatch(sigalrm);
 sig_bugcatch(sigbug);

 alarm(DEATH);

 pidopen();
 readfd = open_read(pidfn);
 if (unlink(pidfn) == -1) die(63);

 substdio_fdbuf(&ssout,write,messfd,outbuf,sizeof(outbuf));
 substdio_fdbuf(&ssin,read,0,inbuf,sizeof(inbuf));

 for (;;) {
   register int n;
   register char *x;
   int i;

   n = substdio_feed(&ssin);
   if (n < 0) die_read();
   if (!n) break;
   x = substdio_PEEK(&ssin);
   if (dksign || dkverify)
     for(i=0; i < n; i++) {
       if (x[i] == '\n') st = dk_message(dk, "\r\n", 2);
       else st = dk_message(dk, x+i, 1);
       maybe_die_dk(st);
      }
   if (substdio_put(&ssout,x,n) == -1) die_write();
   substdio_SEEK(&ssin,n);
 }

 if (substdio_flush(&ssout) == -1) die_write();

 if (dksign || dkverify) {
   st = dk_eom(dk, (void *)0);
   maybe_die_dk(st);
   if (dksign) {
     write_signature(dk, dksign);
   } else if (dkverify) {
     char *status;
     if (!stralloc_copys(&dkoutput,"DomainKey-Status: ")) die(51);
     switch(st) {
     case DK_STAT_OK:         status = "good        "; break;
     case DK_STAT_BADSIG:     status = "bad         "; break;
     case DK_STAT_NOSIG:      status = "no signature"; break;
     case DK_STAT_NOKEY:
     case DK_STAT_CANTVRFY:   status = "no key      "; break;
     case DK_STAT_BADKEY:     status = "bad key     "; break;
     case DK_STAT_INTERNAL:
     case DK_STAT_ARGS:
     case DK_STAT_SYNTAX:     status = "bad format  "; break;
     case DK_STAT_NORESOURCE: status = "no resources"; break;
     case DK_STAT_REVOKED:    status = "revoked     "; break;
     }
     if (!stralloc_cats(&dkoutput,status)) die(51);
     if (!stralloc_cats(&dkoutput,"\n")) die(51);
     if (dkverify[str_chr(dkverify, 'A'+st)]) die(13);
     if (dkverify[str_chr(dkverify, 'a'+st)]) die(82);
   }
 }

 if (pipe(pim) == -1) die(59);
 
 switch(pid = vfork()) {
 case -1:
   close(pim[0]); close(pim[1]);
   die(58);
 case 0:
   close(pim[1]);
   if (fd_move(0,pim[0]) == -1) die(120);
   if (chdir(auto_qmail) == -1) die(61);
   execv(*binqqargs,binqqargs);
   die(120);
 }

 close(pim[0]);

 substdio_fdbuf(&ssin,read,readfd,inbuf,sizeof(inbuf));
 substdio_fdbuf(&ssout,write,pim[1],outbuf,sizeof(outbuf));

 if (substdio_bput(&ssout,dkoutput.s,dkoutput.len) == -1) die_write();
 switch(substdio_copy(&ssout,&ssin))
  {
   case -2: die_read();
   case -3: die_write();
  }

 if (substdio_flush(&ssout) == -1) die_write();
 close(pim[1]);

 if (wait_pid(&wstat,pid) != pid)
   die(57);
 if (wait_crashed(wstat))
   die(57);
 die(wait_exitcode(wstat));

}
