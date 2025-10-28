/*
 *      sprintf - write a printf-formatted string to memory at "line"
 */

#include <stdarg.h>
#include <stdio.h>
#include <limits.h>

int __ofmt(FILE *stream, char *line, int buf_size,
                                     const char *format, va_list ap);
int sprintf2(char *s, const char *fmt, ...)
{
	int num;
	va_list ap;

	va_start(ap, fmt);
	num = __ofmt(0, s, INT_MAX, fmt, ap);
	va_end(ap);
	return num;
}

int snprintf2(char *s, int buf_size, const char *fmt, ...)                     // WBR: added snprintf2 function
{
	int num;
	va_list ap;

	va_start(ap, fmt);
	num = __ofmt(0, s, buf_size, fmt, ap);
	va_end(ap);
	return num;
}

int snvprintf2(char *s, int buf_size, const char *fmt, va_list args)
{
	int num;
    va_list ap;

    __va_copy (ap, args);	
	num = __ofmt(0, s, buf_size, fmt, ap);
    return num;
}

