#pragma once

template <size_t N = 65536>
class circular_buffer
{
public:
	circular_buffer();
	~circular_buffer();

	// 현재 사용 중인 버퍼의 크기.
	size_t get_use_size(void);

	// 남은 버퍼의 크기/
	size_t get_free_size(void);		// 현재 버퍼에 남은 용량 얻기. 
	size_t	direct_enqueue_size(void);
	size_t	direct_dequeue_size(void);

	// 버퍼에서 데이터를 복사 하지만 버퍼의 데이터는 건들지 않는다.
	size_t peek(char* dest, size_t size);
	// 버퍼에서 데이터를 복사 하지만 버퍼의 데이터 제거.
	size_t	dequeue(char* dest, size_t size);
	// 데이터에서 버퍼로 복사
	size_t	enqueue(char* data, size_t size);

	void clear(void);
	void move_rear(size_t size);
	void move_front(size_t size);

	// getter
	char* get_buffer(void) const;
	char* get_rear_buffer(void) const;
	char* get_front_buffer(void) const;

	bool empty() const noexcept;
	const size_t capacity() const;

private:
	size_t front; // front가 가리키는 공간은 당므에 데이터 넣을 곳.
	size_t rear;

	char* buffer;
};

template<size_t N>
inline circular_buffer<N>::circular_buffer() : buffer{}, front{ 0 }, rear{ 0 }
{
	static_assert(N >= 2, "Capacity must be greater than 2.");
	
	buffer = new char[N];
}

template<size_t N>
inline circular_buffer<N>::~circular_buffer()
{
	if (buffer != nullptr)
	{
		delete[] buffer;
	}
}

template<size_t N>
inline size_t circular_buffer<N>::get_use_size(void)
{
	if (front >= rear)
	{
		return front - rear;
	}
	else
	{
		return N - rear + front;
	}
}

//	남은 버퍼의 크기/
template<size_t N>
inline size_t circular_buffer<N>::get_free_size(void)
{
	size_t used_size;

	if (front >= rear)
	{
		used_size = front - rear;
	}
	else
	{
		used_size = N - rear + front;
	}

	return N - used_size - 1;
}

template<size_t N>
inline size_t circular_buffer<N>::direct_enqueue_size(void)
{
	if (rear > front)
	{
		return rear - front - 1;
	}
	else
	{
		return N - front;
	}

}

template<size_t N>
inline size_t circular_buffer<N>::direct_dequeue_size(void)
{
	if (rear > front)
	{
		return N - rear;
	}
	else
	{
		return front - rear;//-1;
	}
}

template<size_t N>
inline size_t circular_buffer<N>::peek(char * dest, size_t size)
{
	size_t front_capture = this->front;
	size_t rear_capture = this->rear;

	// 사용 크기.
	size_t use_size = front_capture >= rear_capture ? front_capture - rear_capture : N - rear_capture + front_capture;
	if (use_size == 0)
	{
		return 0;
	}

	size_t temp_dequeue_size = rear_capture > front_capture ? N - rear_capture : front_capture - rear_capture;
	size_t to_peek_size = size > use_size ? use_size : size;
	size_t current_space_size = temp_dequeue_size > to_peek_size ? to_peek_size : temp_dequeue_size;
	size_t next_space_size = to_peek_size - current_space_size;

	memmove(dest, &buffer[rear_capture], current_space_size);

	if (next_space_size)
	{
		memmove(dest + current_space_size, &buffer[0], next_space_size);
	}

	return to_peek_size;
}

template<size_t N>
inline size_t circular_buffer<N>::dequeue(char * dest, size_t size)
{
	size_t rear_capture = this->rear;
	size_t front_capture = this->front;

	size_t use_size = front_capture >= rear_capture ? front_capture - rear_capture : N - rear_capture + front_capture;//
	if (use_size == 0)
		return 0;

	size_t temp_dequeue_size = rear_capture > front_capture ? N - rear_capture : front_capture - rear_capture;
	size_t to_load_size = size > use_size ? use_size : size;
	size_t current_space_size = temp_dequeue_size > to_load_size ? to_load_size : temp_dequeue_size;
	size_t next_space_size = to_load_size - current_space_size;


	memmove(dest, &buffer[rear_capture], current_space_size);

	if (0 < next_space_size)
	{
		memmove(dest + current_space_size, &buffer[0], next_space_size);
	}

	rear_capture += to_load_size;
	rear_capture %= N;

	this->rear = rear_capture;

	return to_load_size;
}

template<size_t N>
inline size_t circular_buffer<N>::enqueue(char * data, size_t size)
{
	size_t rear_capture = this->rear;
	size_t front_capture = this->front;

	size_t use_size = front_capture >= rear_capture ? front_capture - rear_capture : N - rear_capture + front_capture;
	size_t free_size = N - use_size - 1;
	if (free_size == 0)
	{
		return 0;
	}

	size_t temp_direct_enqueue_size = rear_capture > front_capture ? rear_capture - front_capture - 1 : N - front_capture;
	size_t to_store_size = size > free_size ? free_size : size;
	size_t current_space_size = temp_direct_enqueue_size < to_store_size ? temp_direct_enqueue_size : to_store_size;
	size_t next_space_size = to_store_size - current_space_size;

	// 넣을수잇는 만큼만 넣고
	memmove(&buffer[front_capture], data, current_space_size);

	// 다음공간 크기 를 넣음.
	if (0 < next_space_size)
	{
		memmove(&buffer[0], data + current_space_size, next_space_size);
	}

	front_capture += to_store_size;
	front_capture %= N;

	this->front = front_capture;

	return to_store_size;
}

template<size_t N>
inline void circular_buffer<N>::clear(void)
{
	front = rear = 0;
}

template<size_t N>
inline void circular_buffer<N>::move_rear(size_t size)
{
	this->rear += size;
	this->rear %= N;
}

template<size_t N>
inline void circular_buffer<N>::move_front(size_t size)
{
	this->front += size;
	this->front %= N;
}

template<size_t N>
inline char * circular_buffer<N>::get_buffer(void) const
{
	return buffer;
}

template<size_t N>
inline char * circular_buffer<N>::get_rear_buffer(void) const
{
	return buffer + rear;
}

template<size_t N>
inline char* circular_buffer<N>::get_front_buffer(void) const
{
	return buffer + front;
}

template<size_t N>
inline bool circular_buffer<N>::empty() const noexcept
{
	return front == rear;
}

template<size_t N>
inline const size_t circular_buffer<N>::capacity() const
{
	return N;
}
