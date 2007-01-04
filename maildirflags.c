/*
** Copyright 2000 Double Precision, Inc.
** See COPYING for distribution information.
*/

#include	<sys/types.h>
#include	<string.h>

static const char rcsid[]="$Id: maildirflags.c,v 1.1 2007/01/04 22:06:25 denis Exp $";

int maildir_hasflag(const char *filename, char flag)
{
	const char *p=strrchr(filename, '/');

	if (p)
		filename=p+1;

	p=strrchr(p, ':');
	if (p && strncmp(p, ":2,", 3) == 0 &&
	    strchr(p+3, flag))
		return (1);
	return (0);
}
