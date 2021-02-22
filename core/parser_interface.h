template<typename T>
class i_parser
{
public:
	virtual std::vector<T*> parse_payload(char* buffer, size_t size);
};

