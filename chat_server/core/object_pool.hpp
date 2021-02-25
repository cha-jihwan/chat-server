#pragma once

// 객체 크기가 제한된 오브젝트 풀.
// 싱글 스레드에서 재할당이 없다는 
// 주의 사항
// 만일 스택 할당시 stack 사이즈가 1mb가 넘는 상황이 예상시. 동적 할당이 권장된다. 
// vector를 stack 처럼 사용한다.	// 최근 할당 해제된 객체 재사용 -> 캐시 시간 지역성 고려
// 객체간 붙인다.													-> 캐시 지역성을 고려.

using namespace std;
template <typename T, size_t N>
class bounded_object_pool
{
public:
	// 생성자.
	bounded_object_pool();

	// 객체를 할당후 생성자를 통해 초기화하고 반환한다.
	T* allocate();
	// 객체를 풀에 반환. 
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

	// 제일 최근에 추가된 객체를 가져온다. 
	// 캐시의 시간 지역성 고려.
	T* element = pool_container.back();

	// 벡터 뒤의 원소를 제거.
	pool_container.pop_back();

	// 생성자 호출.
	new(element) T{};

	// 객체를 반환
	return element;
}

// 객체를 반환하고 소멸자를 호출한다.
template <typename T, size_t N>
void bounded_object_pool<T, N>::free(T* obj)
{
	crash_if_false(nullptr != obj);

	// 소멸자 호출.
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
