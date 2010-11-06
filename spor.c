/*
 * spor.c - Store Permissions and Ownership Recursively
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

#include "spor.h"

int verbose;
const char *program_name;
FILE *dbfile;

static void print_usage(void)
{
	fprintf (stderr, usage_template, program_name);
	exit (1);
}

int main(int argc, char *argv[])
{
	int opt, mode, nfds;
	int retval = 0;
	char *file;

	mode = 0;
	verbose = 0;
	program_name = argv[0];
	dbfile = NULL;

	while ((opt=getopt_long(argc, argv, s_opts, l_opts, NULL)) != -1) {
		switch (opt) {
		case 's':
		case 'r':
		case 'd':
			/* User specified -s or --store   ||
			   User specified -r or --restore ||
			   User specified -d or --dump */
			mode = opt;
			file = optarg;
			break;
		case 'v':
			/* User specified -v or --verbose */
			verbose = 1;
			break;
		case '?':
		case 'h':
			/* User specified an unrecognized option. */
			print_usage();
		case -1:
			/* Done with options. */
			break;
		default:
			abort();
		}
	}

	if (optind == argc)
		print_usage();

	switch (mode) {
	case 's': {
		dbfile = fopen(file,"w");
		if (dbfile == NULL) {
			fprintf(stderr,
				"ERROR: Impossible to create file %s\n", file);
			return -1;
		}
		nfds = getdtablesize() - SPARE_FDS;
		if (verbose)
			fprintf(stderr, "Working at %s\n", argv[optind]);

		ftw_store(argv[optind], nfds);
		fclose(dbfile);
		}
		break;
	case 'r':
		retval = restore_db(file);
		break;
	case 'd':
		retval = dump_db(file);
		break;
	default:
		break;
	}
	return retval;
}

int dump_db(char* file)
{
	char buff[MAXLINE];
	DB_RECORD db;

	dbfile = fopen(file, "r");
	if (dbfile == NULL) {
		fprintf(stderr,"ERROR: Impossible to read file %s\n",file);
		return -1;
	}

	fprintf(stdout,"--[ Store Permissions & Ownership Recursively ]--\n");
	fprintf(stdout,"Perm.\tOwner  \tFile path\n");
	fprintf(stdout,"=====\t=======\t=========\n");
	while(fgets(buff, MAXLINE, dbfile) != NULL) {
		sscanf(buff,"%d %d %d %s",
					&db.mode, &db.uid, &db.gid, db.fpath);
		fprintf(stdout,"%d\t%d:%d\t%s\n",
					db.mode, db.uid, db.gid, db.fpath);
	}
	fprintf(stdout,"=====\t=======\t=========\n");
	fclose(dbfile);
	return 0;
}

int restore_db(char* file)
{
	int error = 0;
	char buff[MAXLINE];
	DB_RECORD db;

	dbfile = fopen(file,"r");
	if (dbfile == NULL) {
		fprintf(stderr, "ERROR: Impossible to read file %s\n", file);
		return -1;
	}

	while(fgets(buff, MAXLINE, dbfile) != NULL) {
		sscanf(buff,"%d %d %d %s",
					&db.mode, &db.uid, &db.gid, db.fpath);
		if (verbose)
			fprintf(stderr,"RECOVERING [%s] ", db.fpath);

		if ((chmod(db.fpath, (db.mode & 07777)) != 0) ||
				(check_chown(db.fpath, db.uid, db.gid) != 0)) {
			error++;

			if (verbose)
				fprintf(stderr, "FAILED!");
		}

		if (verbose)
			fprintf(stderr, "\n");
	}

	if (verbose && error)
		fprintf(stderr,
			"Recovery process failed to %d files.\n", error);

	fclose(dbfile);
	return error;
}

int check_chown(char *path, uid_t uid, gid_t gid)
{
	struct passwd pwd, *presult;
	struct group grp, *gresult;
	char *buf;
	size_t bufsize;
	int u, g, error = 0;

	bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize == -1)
		bufsize = 16384;

	buf = malloc(bufsize);
	if (buf == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	u = getpwuid_r(uid, &pwd, buf, bufsize, &presult);
	g = getgrgid_r(gid, &grp, buf, bufsize, &gresult);

	if (presult != NULL && gresult != NULL)
		error = chown(path, uid, gid);
	else
		++error;

	free(buf);
	return error;
}

