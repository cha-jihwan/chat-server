#pragma once

#include "pre_compile.h"

// 만일 스택 할당시 stack 사이즈가 1mb를 넘지 않게 사용해야함.
// -> 걍 동적할당 하셈 ㅋㅋ
// vector를 stack 처럼 사용.
// 최대한 객체간 붙여서 캐시 최적화
template <typename T, size_t N>
class object_pool
{
public:
	object_pool::object_pool()
	{
		pool_container.reserve(N);

		for (T& obj : objects)
		{
			pool_container.push_back(&obj);
		}
	}

	T* allocate()
	{
		if (true == pool_container.empty())
		{
			return nullptr;
		}
		else
		{
			return pool_container.pop_back();
		}
	}

	void release(T* obj)
	{
		crash_if_false( nullptr != obj );

		return pool_container.push_back(obj);
	}

	constexpr size_t capacity()
	{
		return objects.max_size();
	}

	const size_t size() const
	{
		return pool_container.size();
	}

	const bool empty() const
	{
		return pool_container.empty();
	}

private:
	std::array<T, N> objects;
	std::vector<T*>	 pool_container;
};
