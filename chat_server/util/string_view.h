#pragma once




/// very very simple string_view
class string_view
{
public:
	string_view() = default;
	string_view(const char* c_str);
	string_view(char* str_ptr, size_t size);

	bool operator<(const string_view& other);  // for map

	friend bool operator==(const string_view& v1, const string_view& v2);
	friend bool operator<(const string_view& v1, const string_view& v2);  // for map
	
	friend struct string_view_hash;
private:
	char*	str;
	size_t	size;
};


bool operator==(const string_view& v1, const string_view& v2);
bool operator<(const string_view& v1, const string_view& v2);





// very very very very very simple hash
// reference : https://en.cppreference.com/w/cpp/utility/hash
struct string_view_hash
{
	std::size_t operator()(const string_view& s_v) const;
};
