#include "../common/pre_compile.h"

void printlog_(const char* filename, const char* funcname, const int lineno, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	printf("[FILE: %s, FUNC:%s, LINE:%d] ", filename, funcname, lineno);
	vprintf(fmt, ap);
	va_end(ap);
}