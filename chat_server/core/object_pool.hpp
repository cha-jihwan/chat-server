#pragma once

// 객체 크기가 제한된 오브젝트 풀.

// 주의 사항
// 만일 스택 할당시 stack 사이즈가 1mb가 넘는 상황이 예상시. 동적 할당이 권장된다. 
// vector를 stack 처럼 사용한다. 객체간 붙여서 캐시 지역성을 고려.

using namespace std;
template <typename T, size_t N>
class bounded_object_pool
{
public:
	bounded_object_pool();
	T* allocate();
	void free(T* obj);
	const size_t capacity();
	const size_t size() const;
	const bool empty() const;

private:
	array<T, N>	objects;
	vector<T*>	pool_container;
};



template <typename T, size_t N>
bounded_object_pool<T, N>::bounded_object_pool()
{
	//static_assert( (sizeof(T) * N) >= (1024 * 1024), "" );

	pool_container.reserve(N);

	for (T& obj : objects)
	{
		pool_container.push_back(&obj);
	}
}

template <typename T, size_t N>
T* bounded_object_pool<T, N>::allocate()
{
	if (pool_container.empty()) 
	{
		return nullptr;
	}

	T* element = pool_container.back();

	pool_container.pop_back();

	// 생성자 호출.
	new(element) T{};

	// 객체를 반환
	return element;
}

template <typename T, size_t N>
void bounded_object_pool<T, N>::free(T* obj)
{
	crash_if_false(nullptr != obj);

	obj->~T();

	return pool_container.push_back(obj);
}

template <typename T, size_t N>
const size_t bounded_object_pool<T, N>::capacity()
{
	return objects.max_size();
}


template <typename T, size_t N>
const size_t bounded_object_pool<T, N>::size() const
{
	return pool_container.size();
}

template <typename T, size_t N>
const bool bounded_object_pool<T, N>::empty() const
{
	return pool_container.empty();
}
