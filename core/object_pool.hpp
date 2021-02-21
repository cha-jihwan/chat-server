#pragma once

//// 만일 스택 할당시 stack 사이즈가 1mb를 넘지 않게 사용해야함.
//// -> 걍 동적할당 하셈 ㅋㅋ
//// vector를 stack 처럼 사용.
//// 최대한 객체간 붙여서 캐시 최적화
using namespace std;
template <typename T, size_t N>
class bounded_object_pool
{
public:
	bounded_object_pool();
	T* allocate();
	void free(T* obj);
	constexpr size_t capacity();
	const size_t size() const;
	const bool empty() const;

private:
	array<T, N>	objects;
	vector<T*>	pool_container;
};



template <typename T, size_t N>
bounded_object_pool<T, N>::bounded_object_pool()
{
	pool_container.reserve(N);

	for (T& obj : objects)
	{
		pool_container.push_back(&obj);
	}
}

template <typename T, size_t N>
T* bounded_object_pool<T, N>::allocate()
{
	if (true == pool_container.empty())
	{
		return nullptr;
	}
	else
	{
		T* element = pool_container.back();

		pool_container.pop_back();

		new(element) T{};

		return element;
	}
}

template <typename T, size_t N>
void bounded_object_pool<T, N>::free(T* obj)
{
	crash_if_false(nullptr != obj);

	obj->~T();

	return pool_container.push_back(obj);
}

template <typename T, size_t N>
constexpr size_t bounded_object_pool<T, N>::capacity()
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