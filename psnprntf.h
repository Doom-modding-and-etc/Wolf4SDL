#ifndef PSNPRINTF_H
#define PSNPRINTF_H

#ifdef PSNPRNTF

#ifdef HAVE_SIZE_T
#ifdef _MSC_VER
typedef unsigned long __int64 size_t;
#else
typedef unsigned long long size_t;
#endif
#else
typedef unsigned int size_t;
#endif

int psnprintf(char* str, size_t n, const char* format, ...);
int pvsnprintf(char* str, size_t n, const char* format, va_list ap);

/* haleyjd 08/01/09: rewritten to use a structure */
typedef struct psvnfmt_vars_s {
	char* pinsertion;
	unsigned int      nmax;
	const char* fmt;
	int         flags;
	int         width;
	int         precision;
	char        prefix;
} pvsnfmt_vars;

/* Use these directly if you want to avoid overhead of psnprintf
 * Return value is number of characters printed (or number printed
 * if there had been enough room).
 */
int pvsnfmt_char(pvsnfmt_vars* info, char c);

typedef union pvsnfmt_intparm_u {
	int   i;
	void* p;
} pvsnfmt_intparm_t;

int pvsnfmt_int(pvsnfmt_vars* info, pvsnfmt_intparm_t* ip);

int pvsnfmt_str(pvsnfmt_vars* info, const char* s);

int pvsnfmt_double(pvsnfmt_vars* info, double d);

/* These are the flags you need (use logical OR) for the flags parameter of
 * fmt functions above.
 */
enum
{
	FLAG_DEFAULT = 0x00,
	FLAG_LEFT_ALIGN = 0x01, // -
	FLAG_SIGNED = 0x02, // +
	FLAG_ZERO_PAD = 0x04, // 0
	FLAG_SIGN_PAD = 0x08, // ' '
	FLAG_HASH = 0x10 // #
};
 /* Portable strnlen function (doesn't exist on all systems!) */
unsigned int pstrnlen(const char* s, size_t count);
#endif
#endif /* ifdef PSNPRINTF_H */
