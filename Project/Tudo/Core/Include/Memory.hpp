/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef MEMORY_HPP_
#define MEMORY_HPP_
#include "SystemTypes.hpp"
#include "BigError.hpp"

namespace Tudo
{
	namespace Memory
	{
		void Copy(void* dst, const void* src, uint64 size);

		template<typename T, uint64 N>
		uint64 ArrayCount(T(&arr)[N])
		{
			return N;
		}
	}

	template<typename T>
	class SafePtr
	{
	public:
		SafePtr(T* ptr = nullptr) : mPtr(ptr) {}

		SafePtr(const SafePtr&) = delete;
		SafePtr& operator=(const SafePtr&) = delete;

		SafePtr(SafePtr&& other) noexcept : mPtr(other.mPtr)
		{
			other.mPtr = nullptr;
		}

		~SafePtr()
		{
			Free();
		}

		void Free()
		{
			delete mPtr;
			mPtr = nullptr;
		}

		T* Release()
		{
			T* rt = mPtr;
			mPtr = nullptr;
			return rt;
		}

		void Reset(T* ptr = nullptr)
		{
			Free();
			mPtr = ptr;
		}

		bool Null() const
		{
			return mPtr == nullptr;
		}

		T* Get() const
		{
			if (Null())
				throw BigError("Memory is invalid!");
			return mPtr;
		}

		explicit operator bool() const
		{
			return mPtr != nullptr;
		}

		T* operator->() const
		{
			return Get();
		}

		T& operator*() const
		{
			return *Get();
		}

		SafePtr& operator=(SafePtr&& other) noexcept
		{
			if (this != &other)
			{
				Free();
				mPtr = other.mPtr;
				other.mPtr = nullptr;
			}
			return *this;
		}

	private:
		T* mPtr;
	};
}
#endif