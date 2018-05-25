#ifndef _CAPTURE_H
#define _CAPTURE_H

#define CLEAR(x) memset (&(x), 0, sizeof (x))

struct buffer {
	void *start;
	size_t length;
};


int capture(char *path);

#endif

