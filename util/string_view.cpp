#include "../common/pre_compile.h"

string_view::string_view() : str{}, size{}
{}

string_view::string_view(char* str_ptr, size_t size)
	: str{ str_ptr }, size{ size }
{}

bool operator==(const string_view & v1, const string_view & v2)
{
	if (v1.size != v2.size)
	{
		return false;
	}

	return 0 == memcmp(v1.str, v2.str, v1.size);
}

//bool operator<(const string_view& v1, const string_view& v2)
//{
//	if (v1.size != v2.size)
//	{
//		return 
//	}
//
//	for (int n{}; n < v1.size; ++n)
//	{
//		if (false == (v1.str[n] == v2.str[n]))
//		{
//			return 
//		}
//	}
//	return false;
//}
