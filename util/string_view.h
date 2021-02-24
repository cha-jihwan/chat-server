#pragma once

/// very very simple string_view
class string_view
{
public:
	string_view();
	string_view(char* str_ptr, size_t size);

	friend bool operator==(const string_view& v1, const string_view& v2);
	friend int operator<(const string_view& v1, const string_view& v2);  // for map

private:
	char* str;
	size_t size;
};
