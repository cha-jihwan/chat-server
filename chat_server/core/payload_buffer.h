#pragma once


// 데이터 처리 버퍼 클래스.
template <size_t N = 65536>
class payload_buffer
{
public:
	// 생성자.
	payload_buffer() : read_head{}, write_head{}, buffer{}
	{
		buffer = new uint8_t[N];
	}
	payload_buffer(const payload_buffer& other) = delete;
	payload_buffer(payload_buffer&& other) noexcept = delete;

	// 소멸자
	~payload_buffer()
	{
		delete[] buffer;
	}

	// 버퍼에서 사이즈만큼 데이터 읽어오고 버퍼의 공간을 줄인다.
	size_t read(void* dest, size_t size)
	{
		// check
		size_t fit_size;
		size_t explain_size = this->read_head + size;

		if (this->write_head < explain_size)
		{
			fit_size = this->write_head - read_head;
		}
		else
		{
			fit_size = size;
		}

		memmove(dest, &this->buffer[this->read_head], fit_size);

		this->read_head += fit_size;

		return fit_size;
	}

	// 버퍼에 size만큼 데이터 작성.
	size_t write(void* src, size_t size)
	{
		// check
		if (write_head + size > N)
		{
			crash(); // 완성된 패킷이 없다는건 말이 안됨.
		}

		memmove(&buffer[write_head], src, size);

		this->write_head += size;

		return size;
	}

	// 버퍼에서 사이즈만큼 데이터 읽어오고 버퍼의 공간을 놔둔다.
	size_t peek(void* dest, size_t size)
	{
		// check
		size_t fit_size;
		size_t explain_size = this->read_head + size;

		if (this->write_head < explain_size)
		{
			fit_size = this->write_head - read_head;
		}
		else
		{
			fit_size = size;
		}

		memmove(dest, &this->buffer[this->read_head], fit_size);

		return fit_size;
	}

	// 버퍼 정리.
	// 데이터를 앞으로 당긴다.
	void rewind()
	{
		size_t size = this->write_head - this->read_head;
		// 정리.
		memmove(this->buffer, &this->buffer[this->read_head], size);

		this->read_head = 0;
		this->write_head = size;
	}

	// 버퍼의 최대 크기를 return 한다.
	constexpr size_t capacity() const // total size
	{
		return N;
	}

	// 현재 사용중인 데이터 크기를 반환한다.
	const size_t get_use_size() const
	{
		return this->write_head - this->read_head;
	}

	// 현재 버퍼에서 남은 공간의 크기를 반환한다.
	const size_t get_buffer_size() const
	{
		return N - this->write_head;
	}

	// 버퍼의 데이터가 있는 부분을 반환한다.
	char* get_header() const
	{
		return reinterpret_cast<char*>(&this->buffer[read_head]);
	}

	// 버퍼의 포인터를 반환한다.
	const char* get_buffer() const
	{
		return reinterpret_cast<char*>(this->buffer);
	}

	// 
	const char* get_write_buffer() const
	{
		return reinterpret_cast<char*>(&this->buffer[this->write_head]);
	}

	// read_header를 옮긴다.
	void move_read_head(size_t n)
	{
		this->read_head += n;
	}

	// write_header를 옮긴다.
	void move_write_head(size_t n)
	{
		this->write_head += n;
	}

private:
	uint8_t*	buffer;
	uint32_t	read_head;
	uint32_t	write_head;
};

