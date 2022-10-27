/* intercept time functions so that we can apply offset */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/time.h>

static time_t (*ds_old_time)(time_t *t);
static int (*ds_old_gettimeofday)(struct timeval *tv, struct timezone *tz);
static int (*ds_old_clock_gettime)(clockid_t clk_id, struct timespec *tp);
static time_t ds_offset = 0;
static int ds_verbose = 0;

static void ds_initialize() __attribute__ ((constructor));

/* Run at startup */
static void ds_initialize() {
	char *c;
	c = getenv("DS_VERBOSE");
	if (c) {
		ds_verbose = atoi(c);
	}

	if (ds_verbose) {
		fprintf(stderr, "initializing dateshift library\n");
	}

	ds_old_time = dlsym(RTLD_NEXT, "time");
	if (!ds_old_time) {
		fprintf(stderr, "could not get time symbol from libc\n");
		exit(1);
	}

	ds_old_gettimeofday = dlsym(RTLD_NEXT, "gettimeofday");
	if (!ds_old_gettimeofday) {
		fprintf(stderr, "could not get gettimeofday symbol from libc\n");
		exit(1);
	}

#ifdef HAVE_CLOCK_GETTIME
	ds_old_clock_gettime = dlsym(RTLD_NEXT, "clock_gettime");
	if (!ds_old_clock_gettime) {
		fprintf(stderr, "could not get clock_gettime symbol from libc\n");
		exit(1);
	}
#endif

	ds_offset = 0;
	c = getenv("DS_OFFSET");
	if (c) {
		ds_offset = atoi(c);
	}
}

time_t time(time_t *t) {
	time_t res;
	if (!ds_old_time) {
		fprintf(stderr, "dsintercept not initialised yet\n");
		exit(1);
	}

	(*ds_old_time)(&res);
	res += ds_offset;
	if (t) {
		*t = res;
	}
	return res;
}

#ifdef HAVE_CLOCK_GETTIME
int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
	int ret;

	if (!ds_old_clock_gettime) {
		fprintf(stderr, "dsintercept not initialised yet\n");
		ds_initialize();
	}

	ret = (*ds_old_clock_gettime)(clk_id, tp);

	if (tp) {
		tp->tv_sec += ds_offset;
	}
	return ret;
}
#endif

int gettimeofday(struct timeval *tv,
#ifdef GETTIMEOFDAY_HAS_TIMEZONE
		struct timezone *tz
#else
void *tz
#endif
) {
	int ret;

	if (!ds_old_gettimeofday) {
		fprintf(stderr, "dsintercept not initialised yet\n");
		ds_initialize();
	}

	ret = (*ds_old_gettimeofday)(tv, tz);

	if (tv) {
		tv->tv_sec += ds_offset;
	}
	return ret;
}
