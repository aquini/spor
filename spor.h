/*
 * spor.h - commom definition file for spor.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>

#define SPARE_FDS 50
#define MAXLINE (PATH_MAX + (sizeof(unsigned long int) * 3) + 4)


/* Simple definition for database records */
typedef struct db_record {
	mode_t		mode;			/* Permissions bitmap */
	uid_t		uid;			/* Owner Id */
	gid_t		gid;			/* Group Id */
	char		fpath[PATH_MAX];	/* Full pathname */
} DB_RECORD;


/* Description of long options for getopt_long. */
static const struct option l_opts[] = {
   { "store", 1, NULL, 's' },
   { "help", 0, NULL, 'h' },
   { "restore", 1, NULL, 'r' },
   { "dump", 1, NULL, 'd' },
   { "verbose", 0, NULL, 'v' },
};

/* Description of short options for getopt_long. */
static const char* const s_opts = "s:hr:d:v";

/* Usage summary text. */
static const char* const usage_template =
   "--[ Store Permissions & Ownership Recursively ]--\n\n"
   "Usage: %s [ options ] directory \n"
   "   -s, --store <db-file> : Store file permissions & ownership to a"
   " file pointed by <db-file>\n"
   "   -h, --help : Print this information screen.\n"
   "   -r, --restore <db-file> : Restore file permissions & ownership from the"
   " file pointed by <db-file>\n"
   "   -d, --dump <db-file> : Dump the information stored at file <db-file>\n"
   "   -v, --verbose : Verbosely output\n\n";

/* The name of this program. */
extern const char *program_name;

/* If nonzero, print verbose messages. */
extern int verbose;

/* The fd related to <db-file> */
extern FILE *dbfile;

/* functions prototypes */
int ftw_store(const char *wdir, int nfds);
int dump_db(char* file);
int restore_db(char* file);
int check_chown(char *path, uid_t uid, gid_t gid);

