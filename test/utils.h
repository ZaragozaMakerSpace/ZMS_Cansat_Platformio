// ------------------ SERIAL CONFIG --------------------//
#define DEBUG false
#define PRINTDEBUG true
#define SERIALDEBUG Serial
#define SERIALBAUDS 115200

#define DUMPS(s)                                                               \
	{ SERIALDEBUG.print(F(s)); }
#define DUMPSLN(s)                                                             \
	{ SERIALDEBUG.println(F(s)); }
#define DUMPPRINTLN()                                                          \
	{ SERIALDEBUG.println(); }
#define DUMPV(v)                                                               \
	{ SERIALDEBUG.print(v); }
#define DUMPLN(s, v)                                                           \
	{                                                                          \
		DUMPS(s);                                                              \
		DUMPV(v);                                                              \
		DUMPPRINTLN();                                                         \
	}
#define DUMP(s, v)                                                             \
	{                                                                          \
		DUMPS(s);                                                              \
		DUMPV(v);                                                              \
	}