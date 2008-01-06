#include <sys/types.h>
#include <sys/socket.h>
#include "tcpto.h"
#include "open.h"
#include "lock.h"
#include "seek.h"
#include "now.h"
#include "ip.h"
#include "ipalloc.h"
#include "byte.h"
#include "datetime.h"
#include "readwrite.h"

char tcpto_buf[2048];

static int flagwasthere;
static int fdlock;

static int getbuf()
{
 int r;
 int fd;

 fdlock = open_write("queue/lock/tcpto");
 if (fdlock == -1) return 0;
 fd = open_read("queue/lock/tcpto");
 if (fd == -1) { close(fdlock); return 0; }
 if (lock_ex(fdlock) == -1) { close(fdlock); close(fd); return 0; }
 r = read(fd,tcpto_buf,sizeof(tcpto_buf));
 close(fd);
 if (r < 0) { close(fdlock); return 0; }
 r >>= 5;
 if (!r) close(fdlock);
 return r;
}

/*
  struct tcpto {  32bytes
    char af;             +0
    char nul[3];
    char resason;        +4
    char nul[3];
    ulong when;          +8
    char nul[4];
    union { ip4, ip6 };  +16
*/

int tcpto(ix)
struct ip_mx *ix;
{
 int af = ix->af;
 struct ip_address *ip = &ix->addr.ip;
 int n;
 int i;
 char *record;
 datetime_sec when;

 flagwasthere = 0;

 n = getbuf();
 if (!n) return 0;
 close(fdlock);

 record = tcpto_buf;
 for (i = 0;i < n;++i)
  {
   if (af == record[0] && byte_equal(ip->d,af==AF_INET ? 4 : 16,record+16))
    {
     flagwasthere = 1;
     if (record[4] >= 2)
      {
       when = (unsigned long) (unsigned char) record[11];
       when = (when << 8) + (unsigned long) (unsigned char) record[10];
       when = (when << 8) + (unsigned long) (unsigned char) record[9];
       when = (when << 8) + (unsigned long) (unsigned char) record[8];

       if (now() - when < ((60 + (getpid() & 31)) << 6))
	 return 1;
      }
     return 0;
    }
   record += 32;
  }
 return 0;
}

void tcpto_err(af,ip,flagerr)
int af;
struct ip_address *ip; int flagerr;
{
 int n;
 int i;
 char *record;
 datetime_sec when;
 datetime_sec firstwhen;
 int firstpos;
 datetime_sec lastwhen;

 if (!flagerr)
   if (!flagwasthere)
     return; /* could have been added, but not worth the effort to check */

 n = getbuf();
 if (!n) return;

 record = tcpto_buf;
 for (i = 0;i < n;++i)
  {
   if (af == record[0] && byte_equal(ip->d,af == AF_INET ? 4 : 16,record+16))
    {
     if (!flagerr)
       record[4] = 0;
     else
      {
       lastwhen = (unsigned long) (unsigned char) record[11];
       lastwhen = (lastwhen << 8) + (unsigned long) (unsigned char) record[10];
       lastwhen = (lastwhen << 8) + (unsigned long) (unsigned char) record[9];
       lastwhen = (lastwhen << 8) + (unsigned long) (unsigned char) record[8];
       when = now();

       if (record[4] && (when < 120 + lastwhen)) { close(fdlock); return; }

       if (++record[4] > 10) record[4] = 10;
       record[8] = when; when >>= 8;
       record[9] = when; when >>= 8;
       record[10] = when; when >>= 8;
       record[11] = when;
      }
     if (seek_set(fdlock,i << 5) == 0)
       if (write(fdlock,record,32) < 32)
         ; /*XXX*/
     close(fdlock);
     return;
    }
   record += 32;
  }

 if (!flagerr) { close(fdlock); return; }

 record = tcpto_buf;
 for (i = 0;i < n;++i)
  {
   if (!record[4]) break;
   record += 32;
  }

 if (i >= n)
  {
   firstpos = -1;
   record = tcpto_buf;
   for (i = 0;i < n;++i)
    {
     when = (unsigned long) (unsigned char) record[11];
     when = (when << 8) + (unsigned long) (unsigned char) record[10];
     when = (when << 8) + (unsigned long) (unsigned char) record[9];
     when = (when << 8) + (unsigned long) (unsigned char) record[8];
     when += (record[4] << 10);
     if ((firstpos < 0) || (when < firstwhen))
      {
       firstpos = i;
       firstwhen = when;
      }
     record += 32;
    }
   i = firstpos;
  }

 if (i >= 0)
  {
   record = tcpto_buf + (i << 5);
   record[0] = af;
   byte_copy(record+16,16,&ip);
   when = now();
   record[8] = when; when >>= 8;
   record[9] = when; when >>= 8;
   record[10] = when; when >>= 8;
   record[11] = when;
   record[4] = 1;
   if (seek_set(fdlock,i << 5) == 0)
     if (write(fdlock,record,32) < 32)
       ; /*XXX*/
  }

 close(fdlock);
}
