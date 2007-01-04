#ifndef	maildirquota_h
#define	maildirquota_h

/*
** Copyright 1998 - 1999 Double Precision, Inc.
** See COPYING for distribution information.
*/

#if	HAVE_CONFIG_H
#include	"config.h"
#endif

#include	<sys/types.h>
#include	<stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

static const char maildirquota_h_rcsid[]="$Id: maildirquota.h,v 1.1 2007/01/04 22:06:25 denis Exp $";

int maildir_checkquota(const char *,	/* Pointer to directory */
	int *,	/* Initialized to -1, or opened descriptor for maildirsize */
	const char *,	/* The quota */
	long,		/* Extra bytes planning to add/remove from maildir */
	int);		/* Extra messages planning to add/remove from maildir */

int maildir_addquota(const char *,	/* Pointer to the maildir */
	int,	/* Must be the int pointed to by 2nd arg to checkquota */
	const char *,	/* The quota */
	long,	/* +/- bytes */
	int);	/* +/- files */

int maildir_readquota(const char *,	/* Directory */
	const char *);			/* Quota, from getquota */

int maildir_parsequota(const char *, unsigned long *);
	/* Attempt to parse file size encoded in filename.  Returns 0 if
	** parsed, non-zero if we didn't parse. */

#ifdef  __cplusplus
}
#endif

#endif
