#include <string.h>

extern void main();



extern "C" {

typedef void (*cxx_ctor_ptr)();

extern const cxx_ctor_ptr _ctors_array[];

extern int _ccmdata_begin;
extern int _ccmdata_end;
extern int _ccmdata_content_begin;
extern int _ccmdata_content_end;

extern int _data_begin;
extern int _data_end;
extern int _data_content_begin;
extern int _data_content_end;



void _pre_init()
{
	memcpy(&_data_begin,
			&_data_content_begin,
			reinterpret_cast<char*>(&_data_content_end) - reinterpret_cast<char*>(&_data_content_begin));

	memcpy(&_ccmdata_begin,
			&_ccmdata_content_begin,
			reinterpret_cast<char*>(&_ccmdata_content_end) - reinterpret_cast<char*>(&_ccmdata_content_begin));

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
