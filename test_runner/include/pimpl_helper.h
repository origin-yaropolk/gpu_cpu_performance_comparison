#pragma once

#include <utility>
#include <memory>

template <typename T>
struct PimplHelper
{
	PimplHelper()
		: element(new T)
	{
	}

	template <typename... Args>
	PimplHelper(Args&&... pack)
		: element(new T(std::forward<Args>(pack)...))
	{
	}

	~PimplHelper() = default;

	T* operator->()
	{
		return element.get();
	}

	T& operator*()
	{
		return *element.get();
	}

	std::unique_ptr<T> element;
};