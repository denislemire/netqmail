/*
** Copyright 1998 - 1999 Double Precision, Inc.
** See COPYING for distribution information.
*/

#if HAVE_CONFIG_H
#include "config.h"
#endif
#include        "maildirquota.h"
#include        <stdlib.h>
#include        <string.h>
#include        <errno.h>
#include        <sys/stat.h>

static const char rcsid[]="$Id: overmaildirquota.c,v 1.1 2007/01/04 22:06:25 denis Exp $";



int user_over_maildirquota( const char *dir, const char *q)
{
struct  stat    stat_buf;
int     quotafd;
int     ret_value;

        if (fstat(0, &stat_buf) == 0 && S_ISREG(stat_buf.st_mode) &&
                stat_buf.st_size > 0 && *q)
        {
                if (maildir_checkquota(dir, &quotafd, q, stat_buf.st_size, 1)
                        && errno != EAGAIN)
                {
                        if (quotafd >= 0)       close(quotafd);
                        ret_value = 1;
                } else {
                        maildir_addquota(dir, quotafd, q, stat_buf.st_size, 1);
                        if (quotafd >= 0)       close(quotafd);
                        ret_value = 0;
                }
        } else {
                ret_value = 0;
        }

        return(ret_value);
}
