#pragma once


template <size_t N = 65536>
class payload_buffer
{
public:
	payload_buffer() : read_head{}, write_head{}, buffer{}
	{
		buffer = new uint8_t[N];
	}
	payload_buffer(const payload_buffer& other) = delete;
	payload_buffer(payload_buffer&& other) noexcept = delete;
	~payload_buffer()
	{
		delete[] buffer;
	}

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

		memcpy(dest, &this->buffer[this->read_head], fit_size);

		this->read_head += fit_size;

		return fit_size;
	}

	size_t write(void* src, size_t size)
	{
		// check
		if (this->write_head + size > N)
		{
			crash(); // �ϼ��� ��Ŷ�� ���ٴ°� ���� �ȵ�.
		}

		memcpy(&this->buffer[this->write_head], src, size);

		this->write_head += size;

		return size;
	}

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

		memcpy(dest, &this->buffer[this->read_head], fit_size);

		return fit_size;
	}

	void rewind()
	{
		size_t size = this->write_head - this->read_head;
		// ����.
		memcpy(this->buffer, &this->buffer[this->read_head], size);
		this->read_head = 0;
		this->write_head = size;
	}

	constexpr size_t capacity() const // total size
	{
		return N;
	}

	const size_t get_use_size() const
	{
		return this->write_head - this->read_head;
	}

	const size_t get_buffer_size() const
	{
		return N - this->write_head;
	}

	char* get_header() const
	{
		return reinterpret_cast<char*>(&this->buffer[read_head]);
	}

	const char* get_buffer() const
	{
		return reinterpret_cast<char*>(this->buffer);
	}

	const char* get_write_buffer() const
	{
		return reinterpret_cast<char*>(&this->buffer[this->write_head]);
	}

	void move_read_head(size_t n)
	{
		this->read_head += n;
	}

	void move_write_head(size_t n)
	{
		this->write_head += n;
	}

private:
	uint8_t*	buffer;
	uint32_t	read_head;
	uint32_t	write_head;
};

