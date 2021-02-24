#include "../common/pre_compile.h"

//string_view::string_view() : str{}, size{}
//{}

string_view::string_view(const char * c_str)
	: str{ const_cast<char*>(c_str) }
{
	int size{};
	while (NULL != c_str[size])
	{
		size += 1;
	}

	this->size = size;
}

string_view::string_view(char* str_ptr, size_t size)
	: str{ str_ptr }, size{ size }
{}

bool string_view::operator<(const string_view & other)
{
	return false;
}

bool operator==(const string_view & v1, const string_view & v2)
{
	if (v1.size != v2.size)
	{
		return false;
	}

	return 0 == memcmp(v1.str, v2.str, v1.size);
}



bool operator<(const string_view& left, const string_view& right)
{
	size_t min_size = min(left.size, right.size);

	for (int n{}; n < min_size; ++n)
	{
		if (left.str[n] != right.str[n])
		{
			return left.str[n] < right.str[n];
		}
	}

	return left.size < right.size;
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

std::size_t string_view_hash::operator()(const string_view & s_v) const
{
	size_t total{};

	for (int n{}; n < s_v.size; ++n )
	{
		total += (int)s_v.str[n];
	}

	std::size_t h1 = std::hash<size_t>{}(total);
	std::size_t h2 = std::hash<size_t>{}(s_v.size);

	return h1 ^ (h2 << 1);
}
