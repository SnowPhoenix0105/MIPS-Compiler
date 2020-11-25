#pragma once

#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <memory>
#include <stdexcept>

using std::swap;

template<typename T>
class GraphVisitHelper
{
private:
	T* data;
public:
	GraphVisitHelper(T* data) : data(data) { }
	T& operator[](size_t y)
	{
		return data[y];
	}
};

template<typename T>
class Graph
{
public:
	friend void swap<T>(Graph<T>& g1, Graph<T>& g2);
	friend class GraphVisitHelper<T>;
private:

	T* data;
	size_t _capacity;
	size_t _size;
	T default_value;
public:
	Graph(T default_value) noexcept : data(nullptr), _capacity(0), _size(0), default_value(default_value) { }
	Graph(size_t size, T default_value) noexcept : data(nullptr), _capacity(0), _size(size), default_value(default_value)
	{
		capacity(size);
	}
	Graph(const Graph<T>& other) 
		: 
		data(new T[other._capacity * other._capacity]),
		_capacity(other._capacity),
		_size(other._size),
		default_value(other.default_value)
	{
		memcpy(data, other.data, _capacity * _capacity * sizeof(T));
	}

	Graph(Graph<T>&& other) noexcept : data(nullptr), _capacity(0), _size(0), default_value(other.default_value)
	{
		swap(*this, other);
	}

	Graph& operator=(const Graph<T>& other)
	{
		delete[] data;
		capacity = other.capacity;
		_size = other._size;
		data = new T[capacity * capacity];
		memcpy(data, other.data, _capacity * _capacity * sizeof(T));
		return *this;
	}

	Graph& operator=(Graph<T>&& other) noexcept
	{
		delete[] data;
		_capacity = 0;
		_size = 0;
		swap(*this, other);
		return *this;
	}

	~Graph()
	{
		if (_capacity != 0)
		{
			delete[] data;
		}
	}

	size_t capacity() const noexcept
	{
		return _capacity;
	}

	size_t size() const noexcept
	{
		return _size;
	}

	size_t capacity(size_t size)
	{
		if (size > (1 << 31))
		{
			throw std::bad_alloc();
		}
		size_t new_capacity = size;
		new_capacity |= (new_capacity >> 1);
		new_capacity |= (new_capacity >> 2);
		new_capacity |= (new_capacity >> 4);
		new_capacity |= (new_capacity >> 8);
		new_capacity |= (new_capacity >> 16);
		++new_capacity;
		T* new_data = new T[new_capacity * new_capacity];
		memset(new_data, default_value, new_capacity * new_capacity * sizeof(T));
		if (_capacity != 0)
		{
			for (size_t i = 0; i != size; ++i)
			{
				memcpy(new_data + new_capacity * i, data + _capacity * i, size * sizeof(T));
			}
			delete[] data;
		}
		data = new_data;
		_capacity = new_capacity;
		return new_capacity;
	}

	/// <summary>
	/// 增加若干节点, 返回增加后最后一个节点的编号
	/// </summary>
	/// <returns></returns>
	size_t alloc_point(size_t num = 1)
	{
		_size += num;
		capacity(_size);
		return _size - 1;
	}

	/// <summary>
	/// 通过 [x][y] 访问某个边的权值.
	/// 不会检查边界.
	/// </summary>
	/// <param name="x"></param>
	/// <returns></returns>
	GraphVisitHelper<T> operator[](size_t x)
	{
		return GraphVisitHelper<T>(data + _capacity * x);
	}

	/// <summary>
	/// 通过 x, y 访问某个边的权值.
	/// 若越界会触发 out_of_range 异常.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns></returns>
	const T& at(size_t x, size_t y) const
	{
		if (x >= _size || y >= _size)
		{
			throw std::out_of_range("@Graph.at(size_t, size_t):\t out of range");
		}
		return data[x * _capacity + y];
	}

	/// <summary>
	/// 连接边 <x, y>;
	/// 若 x, y 越界会触发 out_of_range 异常.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="value">边的权重</param>
	/// <returns></returns>
	Graph<T>& link(size_t x, size_t y, T value)
	{
		if (x >= _size || y >= _size)
		{
			throw std::out_of_range("@Graph.link(size_t, size_t, T):\t out of range");
		}
		data[_capacity * x + y] = value;
		return *this;
	}

	/// <summary>
	/// 连接边 <x, y> <y, x>;
	/// 若 x, y 越界会触发 out_of_range 异常.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="value">边的权重</param>
	/// <returns></returns>
	Graph<T>& d_link(size_t x, size_t y, T value)
	{
		if (x >= _size || y >= _size)
		{
			throw std::out_of_range("@Graph.d_link(size_t, size_t, T):\t out of range");
		}
		data[_capacity * x + y] = value;
		data[_capacity * y + x] = value;
		return *this;
	}
};


template<typename T>
void swap(Graph<T>& g1, Graph<T>& g2)
{
	swap(g1.data, g2.data);
	swap(g1._capacity, g2._capacity);
	swap(g1._size, g2._size);
	swap(g1.default_value, g2.default_value);
}

#endif // !__GRAPH_H__
