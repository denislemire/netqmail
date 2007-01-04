#ifndef	numlib_h
#define	numlib_h

/*
** Copyright 1998 - 1999 Double Precision, Inc.
** See COPYING for distribution information.
*/

#ifdef	__cplusplus
extern "C" {
#endif

static const char numlib_h_rcsid[]="$Id: numlib.h,v 1.1 2007/01/04 22:06:25 denis Exp $";

#if	HAVE_CONFIG_H
#include	"config.h"
#endif

#include	<sys/types.h>
#include	<time.h>

#define	NUMBUFSIZE	60

/* Convert various system types to decimal */

char	*str_time_t(time_t, char *);
char	*str_off_t(off_t, char *);
char	*str_pid_t(pid_t, char *);
char	*str_ino_t(ino_t, char *);
char	*str_uid_t(uid_t, char *);
char	*str_gid_t(gid_t, char *);
char	*str_size_t(size_t, char *);

char	*str_sizekb(unsigned long, char *);	/* X Kb or X Mb */

/* Convert selected system types to hex */

char	*strh_time_t(time_t, char *);
char	*strh_pid_t(pid_t, char *);
char	*strh_ino_t(ino_t, char *);

#ifdef	__cplusplus
}
#endif
#endif
