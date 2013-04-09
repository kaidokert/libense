
#define REG(x) *((volatile unsigned int*) (x))

static const unsigned int RCC_BASE = 0x40023800;
static const unsigned int GPIO_A_BASE = 0x40020c00;

struct X {
	static void foo()
	{
		// enable gpioA clock
		REG(RCC_BASE + 0x30) |= 0xf;

		// set gpioA9 to output
		REG(GPIO_A_BASE + 0x00) |= 0x55555555;
		// set speed 50MHz
		REG(GPIO_A_BASE + 0x08) |= 0xaaaaaaaa;
	}

	X()
	{
		foo();
	}
};

X x __attribute__((section(".ccmdata")));

char y[] = "iuztredcvbnmkwerj";

static void print()
{
//	const char foo[] = "brutzelbums";
	for (unsigned char j = 0; ; j++) {
		char c = y[j % sizeof(y)]; {
//		for (char c : foo) {
			for (int i = 0; i < 8; i++) {
				REG(GPIO_A_BASE + 0x14) = ((c & 1) << 12) | (1 << 13);
				for (int i = 0; i < 10000; i++)
					__asm__ __volatile__ ("nop");
				REG(GPIO_A_BASE + 0x14) = 0;
				for (int i = 0; i < 10000; i++)
					__asm__ __volatile__ ("nop");
				c >>= 1;
			}
//			REG(GPIO_A_BASE + 0x14) = 1 << 14;
//			for (int i = 0; i < 100000; i++)
//				__asm__ __volatile__ ("nop");
//			REG(GPIO_A_BASE + 0x14) = 0;
//			for (int i = 0; i < 300000; i++)
//				__asm__ __volatile__ ("nop");
		}
	}
}

void main()
{
	print();
}

extern "C" {
	void* _stack;
	extern void _start();

	__attribute__((section("..isr_vectors"), used))
	void (*const isr_vectors[])() {
		reinterpret_cast<void (*)()>(&_stack),
		_start,
		reinterpret_cast<void (*)()>(4),
		reinterpret_cast<void (*)()>(8),
		reinterpret_cast<void (*)()>(12),
		reinterpret_cast<void (*)()>(16)
	};
}



extern "C" {
	extern void (*_ctors_begin)();
}

// TODO: future-ify all the things

void call_cxx_constructors()
{
	_ctors_begin();
//	cxx_ctor_ptr* ctor = &_ctors_begin;
//	if (*ctor) {
//		(*ctor)();
//	}
//	for (cxx_ctor_ptr* ctor = &_ctors_begin; *ctor; ctor++) {
//		(*ctor)();
//	}
}

extern "C" {
	void _pre_init()
	{
		call_cxx_constructors();
	}
	
	void _post_init()
	{
		main();
	}

	void _post_fini()
	{
	}
}
