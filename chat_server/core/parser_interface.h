template<typename T>
class i_parser
{
public:
	static size_t parse_payload(char* in_buffer, size_t in_size, std::vector<T>& out_buffer);
};

template<typename T>
inline size_t i_parser<T>::parse_payload(char* in_buffer, size_t in_size, std::vector<T>& out_buffer)
{
	return size_t();
}
