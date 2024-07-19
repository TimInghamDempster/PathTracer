#pragma once
#include <cstddef>
#include <vector>

template <typename T>
class memory_pool {
private:
	std::vector<char*> memory_blocks = std::vector<char*>();
	std::vector<T*> free_list = std::vector<T*>();
	std::size_t m_block_size;

public:
	memory_pool(std::size_t block_size) : m_block_size(block_size) {}
	memory_pool(memory_pool const&) = delete;
	memory_pool& operator=(memory_pool const&) = delete;
	memory_pool(memory_pool&&) = delete;
	memory_pool& operator=(memory_pool&&) = delete;
	~memory_pool() 
	{
		for (char* block : memory_blocks)
		{
			delete[] block;
		}
	}

	T* allocate() 
	{
		if (free_list.empty())
		{
			char* block = new char[m_block_size * sizeof(T)];
			memory_blocks.push_back(block);
			for (int i = 0; i < m_block_size; i++)
			{
				free_list.push_back(reinterpret_cast<T*>(block + i * sizeof(T)));
			}
		}
		T* ptr = free_list.back();
		T* initPtr = new(ptr) T();
		free_list.pop_back();
		return initPtr;
	}

	void deallocate(T* ptr)
	{ 
		ptr->T::~T();
		free_list.push_back(ptr);
	}
};
