/* Driver program for date shifting */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

static const char *directories[] = { ".libs", ".", LIBDIR, NULL};
static int verbose = 0;

static void usage(void) {
	fprintf(stderr,
	"usage: dateshift -t \"YYYY-MM-DD [HH:MM:SS]\" [- <command>]\n");
	fprintf(stderr, "Allows the modification of system time for command\n\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "\t-h\tThis help\n");
	fprintf(stderr, "\t-v\tVersion info\n");
	fprintf(stderr, "\t-t\tNew date/time to shift to\n");
	fprintf(stderr, "\t-d\tVerbose output\n");
}

static void show_version(void) {
	fprintf(stderr, "%s version %s\n", PACKAGE, VERSION);
	fprintf(stderr, "Written by Alex Hornby <alex@hornby.org.uk>\n\n");
	fprintf(stderr, "This is free software; see the source for "
			"copying conditions.  There is NO\n"
			"warranty; not even for MERCHANTABILITY or FITNESS FOR A "
			"PARTICULAR PURPOSE\n");
}

static char *
find_intercept_lib(void) {
	const char **dirname;
	char path[1024];
	char new_loc[2049];
	char *lib_location;

	lib_location = NULL;
	for (dirname = directories; *dirname; ++dirname) {
		strcpy(path, *dirname);
		strcat(path, "/libdsintercept.so");
		if (!access(path, R_OK)) {
			lib_location = path;
			break;
		}
	}

	if (!lib_location) {
		fprintf(stderr, "Cannot find libdsintercept.so\n");
		exit(1);
	}

	/* Make lib_location absolute */
	if (lib_location[0] != '/') {
		if (getcwd(new_loc, 1024) == NULL) {
			fprintf(stderr, "error finding library location\n");
			exit(1);
		}
		strcat(new_loc, "/");
		strcat(new_loc, lib_location);
		lib_location = new_loc;
	}

	return strdup(lib_location);
}

void start_command(int offset, char** command) {
	int status;

	if (offset) {
		char* envstr = malloc(2048);
		char *orig_ld_preload = getenv("LD_PRELOAD");
		char * lib_loc = find_intercept_lib();

		if (verbose) {
			fprintf(stderr, "Library location is %s\n", lib_loc);
		}

		sprintf(envstr, "DS_OFFSET=%d", offset);
		putenv(envstr);

		envstr = malloc(2048);
		sprintf(envstr, "DS_VERBOSE=%d", verbose);
		putenv(envstr);

		envstr = malloc(4096);
		sprintf(envstr, "LD_PRELOAD=%s", lib_loc);
		if (orig_ld_preload) {
			strcat(envstr, ":");
			strcat(envstr, orig_ld_preload);
		}

		if (verbose) {
			fprintf(stderr, "Library override is %s\n", envstr);
		}

		putenv(envstr);
		free(lib_loc);
	}

	status = execvp(command[0], command);
	fprintf(stderr, "error %d executing command\n", status);
	if (verbose) {
		for (; *command != NULL; ++command) {
			fprintf(stderr, "%s ", *command);
		}
		fprintf(stderr, "\n");
	}
	exit(1);
}

int main(int argc, char** argv) {
	int c = 0;
	char **command = 0;
	char *datetext = 0;
	struct tm origtime;
	struct tm newtime;
	time_t origtime_t;
	time_t newtime_t;
	char *dp = 0;
	char *defaultcommand[2];

	while (1) {
		c = getopt(argc, argv, "vdht:");
		if (c == -1) {
			break;
		}
		switch (c) {
		case 'h':
			usage();
			exit(0);
			break;
		case 'v':
			show_version();
			exit(0);
			break;
		case 'd':
			verbose = 1;
			break;
		case 't':
			datetext = strdup(optarg);
			break;
		default:
			fprintf(stderr, "option error\n");
			usage();
			exit(1);
			break;
		}
	}

	if (optind < argc) {
		command = &argv[optind];
	}

	if (command == 0) {
		char *c = getenv("SHELL");
		if (c) {
			defaultcommand[0] = strdup(c);
		} else {
			defaultcommand[0] = strdup("/bin/sh");
		}
		defaultcommand[1] = 0;
		command = defaultcommand;
	}

	if (strcmp(*command, "-") == 0) {
		command++;
	}

	origtime_t = time(&origtime_t);
	origtime = *(localtime(&origtime_t));
	newtime = origtime;

	if (datetext == NULL) {
		fprintf(stderr, "You must specify a date/time to shift to\n");
		usage();
		exit(1);
	} else if (strlen(datetext) == 10) {
		dp = strptime(datetext, "%Y-%m-%d", &newtime);
		if (dp == NULL) {
			fprintf(stderr, "Invalid date: %s\n", datetext);
			exit(1);
		}
	} else if (strlen(datetext) == 19) {
		dp = strptime(datetext, "%Y-%m-%d %H:%M:%S", &newtime);
		if (dp == NULL) {
			fprintf(stderr, "Invalid date: %s\n", datetext);
			exit(1);
		}
	} else {
		fprintf(stderr, "Invalid date: %s\n", datetext);
		exit(1);
	}

	newtime_t = mktime(&newtime);
	if (verbose) {
		fprintf(stderr, "Time adjustment is %d seconds\n", (int) (newtime_t
				- origtime_t));
	}
	start_command(newtime_t - origtime_t, command);
	return 0;
}
