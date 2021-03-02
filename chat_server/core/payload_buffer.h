#pragma once


// ������ ó�� ���� Ŭ����.
template <size_t N = 65536>
class payload_buffer
{
public:
	// ������.
	payload_buffer() : read_head{}, write_head{}, buffer{}
	{
		buffer = new uint8_t[N];
	}
	payload_buffer(const payload_buffer& other) = delete;
	payload_buffer(payload_buffer&& other) noexcept = delete;

	// �Ҹ���
	~payload_buffer()
	{
		delete[] buffer;
	}

	// ���ۿ��� �����ŭ ������ �о���� ������ ������ ���δ�.
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

	// ���ۿ� size��ŭ ������ �ۼ�.
	size_t write(void* src, size_t size)
	{
		// check
		if (write_head + size > N)
		{
			crash(); // �ϼ��� ��Ŷ�� ���ٴ°� ���� �ȵ�.
		}

		memmove(&buffer[write_head], src, size);

		this->write_head += size;

		return size;
	}

	// ���ۿ��� �����ŭ ������ �о���� ������ ������ ���д�.
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

	// ���� ����.
	// �����͸� ������ ����.
	void rewind()
	{
		size_t size = this->write_head - this->read_head;
		// ����.
		memmove(this->buffer, &this->buffer[this->read_head], size);

		this->read_head = 0;
		this->write_head = size;
	}

	// ������ �ִ� ũ�⸦ return �Ѵ�.
	constexpr size_t capacity() const // total size
	{
		return N;
	}

	// ���� ������� ������ ũ�⸦ ��ȯ�Ѵ�.
	const size_t get_use_size() const
	{
		return this->write_head - this->read_head;
	}

	// ���� ���ۿ��� ���� ������ ũ�⸦ ��ȯ�Ѵ�.
	const size_t get_buffer_size() const
	{
		return N - this->write_head;
	}

	// ������ �����Ͱ� �ִ� �κ��� ��ȯ�Ѵ�.
	char* get_header() const
	{
		return reinterpret_cast<char*>(&this->buffer[read_head]);
	}

	// ������ �����͸� ��ȯ�Ѵ�.
	const char* get_buffer() const
	{
		return reinterpret_cast<char*>(this->buffer);
	}

	// 
	const char* get_write_buffer() const
	{
		return reinterpret_cast<char*>(&this->buffer[this->write_head]);
	}

	// read_header�� �ű��.
	void move_read_head(size_t n)
	{
		this->read_head += n;
	}

	// write_header�� �ű��.
	void move_write_head(size_t n)
	{
		this->write_head += n;
	}

private:
	uint8_t*	buffer;
	uint32_t	read_head;
	uint32_t	write_head;
};

