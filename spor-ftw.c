/*
 * spor-ftw.c - File Tree Walking Interface for spor.c
 * (c) Copyright, 2009 By Rafael Aquini, <aquini@linux.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define _XOPEN_SOURCE 500

#include <ftw.h>
#include <stdio.h>

/* The name of this program. */
extern const char *program_name;

/* If nonzero, print verbose messages. */
extern int verbose;

/* The fd related to <db-file> */
extern FILE *dbfile;

int process(const char *file, const struct stat *sb,
						int flag, struct FTW *s);

void ftw_store(const char *wdir, int nfds)
{
	int flags = FTW_PHYS;
	if (nftw(wdir, process, nfds, flags) != 0) {
		if (verbose)
			fprintf(stderr, "%s - %s: stopped early!\n",
							program_name, wdir);
	}
}

int process(const char *file, const struct stat *sb, int flag, struct FTW *s)
{
	int retval = 0;
	const char *name = file;

	switch (flag) {
	case FTW_F:
	case FTW_D:
		if (verbose)
			fprintf(stderr,"[%d] [%d:%d] [%s]\n",
					sb->st_mode,sb->st_uid,sb->st_gid,name);

		fprintf(dbfile,"%d %d %d %s\n",
					sb->st_mode,sb->st_uid,sb->st_gid,name);
		break;

	default: 
		break;
	}
	return retval;
}

