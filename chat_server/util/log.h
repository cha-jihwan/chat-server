#pragma once

#define LOG(fmt, ...) printlog_(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
void printlog_(const char* filename, const char* funcname, const int lineno, const char* fmt, ...);
