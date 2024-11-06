#ifndef _CANSAT_CONFIG_H
#define _CANSAT_CONFIG_H

// ------------------ SERIAL CONFIG --------------------//
#define PRINTDEBUG true
#define SERIALDEBUG Serial

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
#endif