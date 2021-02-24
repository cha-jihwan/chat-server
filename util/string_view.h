#pragma once


class string_view
{
public:
	string_view(char* str_ptr, size_t size);

	friend bool operator==(const string_view& v1, const string_view& v2);

private:
	char* str_ptr;
	size_t size;
};
