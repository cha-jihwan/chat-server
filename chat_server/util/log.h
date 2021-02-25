#pragma once

#define __FILENAME__ (strrchr(__FILE__,'\\')+1)
#define LOG(fmt, ...) printlog_( __FILENAME__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
void printlog_(const char* filename, const char* funcname, const int lineno, const char* fmt, ...);
