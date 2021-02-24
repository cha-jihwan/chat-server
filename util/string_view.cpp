#include "../common/pre_compile.h"

string_view::string_view(char* str_ptr, size_t size) 
	: str_ptr{ str_ptr }, size{ size }
{}

bool operator==(const string_view & v1, const string_view & v2)
{
	if (v1.size != v2.size)
	{
		return false;
	}

	return 0 == memcmp(v1.str_ptr, v2.str_ptr, v1.size);
}
