#include "scan.h"
#include "env.h"

int ucspitls(void)
{
  unsigned long fd;
  char *fdstr;
   
  if (!(fdstr=env_get("SSLCTLFD")))
    return 0;
  if (!scan_ulong(fdstr,&fd))
    return 0;
  if (write((int)fd, "y", 1) < 1)
    return 0;
 
  if (!(fdstr=env_get("SSLREADFD")))
    return 0;
  if (!scan_ulong(fdstr,&fd))
    return 0;
  if (dup2((int)fd,0) == -1)
    return 0;
 
  if (!(fdstr=env_get("SSLWRITEFD")))
    return 0; 
  if (!scan_ulong(fdstr,&fd))
    return 0;
  if (dup2((int)fd,1) == -1)
    return 0;
 
  return 1;
}
