//#include "pre_compile.h"
//
//
//template <typename T, size_t N>
//bounded_object_pool<T, N>::bounded_object_pool()
//{
//	pool_container.reserve(N);
//
//	for (T& obj : objects)
//	{
//		pool_container.push_back(&obj);
//	}
//}
//
//template <typename T, size_t N>
//T* bounded_object_pool<T, N>::allocate()
//{
//	if (true == pool_container.empty())
//	{
//		return nullptr;
//	}
//	else
//	{
//		T* element =  pool_container.back();
//
//		pool_container.pop_back();
//
//		new(element) T{};
//
//		return element;
//	}
//}
//
//template <typename T, size_t N>
//void bounded_object_pool<T, N>::free(T* obj)
//{
//	crash_if_false( nullptr != obj );
//
//	obj->~T();
//
//	return pool_container.push_back(obj);
//}
//
//template <typename T, size_t N>
//constexpr size_t bounded_object_pool<T, N>::capacity()
//{
//	return objects.max_size();
//}
//
//
//template <typename T, size_t N>
//const size_t bounded_object_pool<T, N>::size() const
//{
//	return pool_container.size();
//}
//
//template <typename T, size_t N>
//const bool bounded_object_pool<T, N>::empty() const
//{
//	return pool_container.empty();
//}
//
