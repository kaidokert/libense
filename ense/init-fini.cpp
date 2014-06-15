#include <string.h>

extern void main();



extern "C" {

typedef void (*cxx_ctor_ptr)();

extern const cxx_ctor_ptr _ctors_array[];

extern char _ccmdata_begin;
extern char _ccmdata_content_begin;
extern char _ccmdata_content_end;

extern char _data_begin;
extern char _data_content_begin;
extern char _data_content_end;

extern char _bss_begin;
extern char _bss_end;



void _pre_init()
{
	memcpy(&_data_begin, &_data_content_begin, &_data_content_end - &_data_content_begin);

	memcpy(&_ccmdata_begin, &_ccmdata_content_begin, &_ccmdata_content_end - &_ccmdata_content_begin);

	memset(&_bss_begin, 0, &_bss_end - &_bss_begin);

	for (size_t i = 0; _ctors_array[i]; i++) {
		_ctors_array[i]();
	}
}

void _post_init()
{
	main();
}

void _post_fini()
{
}

}
