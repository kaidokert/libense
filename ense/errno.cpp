#include <errno.h>

extern "C" {

int* __errno_location()
{
	static int val;
	return &val;
}

}
