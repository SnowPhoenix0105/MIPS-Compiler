#pragma once

#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <memory>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <new>

using std::swap;
using std::out_of_range;
using std::vector;
using std::make_pair;
using std::unordered_map;



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
	friend class GraphVisitHelper<T>;

private:
	T* data;
	size_t _capacity;
	size_t _size;
	T default_value;

	size_t index_of(size_t x, size_t y)
	{
		return x * _capacity + y;
	}

public:
	explicit Graph(T default_value) noexcept 
		: data(nullptr), _capacity(0), _size(0), default_value(default_value) { }
	
	Graph(size_t point_num, T default_value) noexcept 
		: data(nullptr), _capacity(0), _size(point_num), default_value(default_value)
	{
		capacity(point_num);
	}

	Graph(const Graph<T>& other) 
		: 
		data((T*)::operator new[](other._capacity * other._capacity * sizeof(T))),
		_capacity(other._capacity),
		_size(other._size),
		default_value(other.default_value)
	{
		for (size_t i = 0; i != _capacity * _capacity; ++i)
		{
			new(data + i) T(std::move_if_noexcept(other.data[i]));
		}
	}

	Graph(Graph<T>&& other) noexcept 
		: data(nullptr), _capacity(0), _size(0), default_value(other.default_value)
	{
		swap(other);
	}

	Graph& operator=(const Graph<T>& other)
	{
		delete[] data;
		_capacity = other._capacity;
		_size = other._size;
		data = (T*)::operator new[](_capacity * _capacity * sizeof(T));
		for (size_t i = 0; i != _capacity * _capacity; ++i)
		{
			new(data + i) T(std::move_if_noexcept(other.data[i]));
		}
		return *this;
	}

	Graph& operator=(Graph<T>&& other) noexcept
	{
		delete[] data;
		_capacity = 0;
		_size = 0;
		swap(other);
		return *this;
	}

	~Graph()
	{
		if (_capacity != 0)
		{
			delete[] data;
		}
	}

	/// <summary>
	/// �ڵ������
	/// </summary>
	/// <returns></returns>
	size_t point_num() const noexcept
	{
		return _size;
	}

	/// <summary>
	/// �ڵ������
	/// </summary>
	/// <returns></returns>
	size_t size() const noexcept
	{
		return _size;
	}

	size_t capacity() const noexcept
	{
		return _capacity;
	}

	/// <summary>
	/// ���ô˺�����, ��֤�����ڲ����·���ռ�������, ���Խ��ڵ��������� size.
	/// </summary>
	/// <param name="size"></param>
	/// <returns></returns>
	size_t capacity(size_t size)
	{
		if (size < _capacity)
		{
			return _capacity;
		}
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

		// ����ֵ���п���, ���������ӵĿռ�ͨ�� default_value ��ʼ��
		T* new_data = (T*)::operator new[](new_capacity * new_capacity * sizeof(T));
		size_t i = 0;
		for (; i != _capacity; ++i)
		{
			size_t j = 0;
			for (; j != _capacity; ++j)
			{
				new(new_data + index_of(i, j)) T(std::move_if_noexcept(data[index_of(i, j)]));
			}
			for (; j != new_capacity; ++j)
			{
				new(new_data + index_of(i, j)) T(default_value);
			}
		}
		for (; i != new_capacity; ++i)
		{
			for (size_t j = 0; j != new_capacity; ++j)
			{
				new(new_data + index_of(i, j)) T(default_value);
			}
		}
		if (_capacity != 0)
		{
			delete[] data;
		}
		data = new_data;
		_capacity = new_capacity;
		return new_capacity;
	}

	/// <summary>
	/// �������ɽڵ�, �������Ӻ����һ���ڵ�ı��
	/// </summary>
	/// <returns></returns>
	size_t alloc_point(size_t num = 1)
	{
		_size += num;
		capacity(_size);
		return _size - 1;
	}

	/// <summary>
	/// ͨ�� [x][y] ����ĳ���ߵ�Ȩֵ.
	/// ������߽�.
	/// </summary>
	/// <param name="x"></param>
	/// <returns></returns>
	GraphVisitHelper<T> operator[](size_t x)
	{
		return GraphVisitHelper<T>(data + _capacity * x);
	}

	/// <summary>
	/// ͨ�� x, y ����ĳ���ߵ�Ȩֵ.
	/// ��Խ��ᴥ�� out_of_range �쳣.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns></returns>
	const T& at(size_t x, size_t y) const
	{
		if (x >= _size || y >= _size)
		{
			throw out_of_range("@Graph.at(size_t, size_t):\t out of range");
		}
		return data[index_of(x, y)];
	}

	/// <summary>
	/// ���ӱ� <x, y>;
	/// �� x, y Խ��ᴥ�� out_of_range �쳣.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="value">�ߵ�Ȩ��</param>
	/// <returns></returns>
	Graph<T>& link(size_t x, size_t y, T value)
	{
		if (x >= _size || y >= _size)
		{
			throw out_of_range("@Graph.link(size_t, size_t, T):\t out of range");
		}
		data[index_of(x, y)] = value;
		return *this;
	}

	/// <summary>
	/// ���ӱ� <x, y> <y, x>;
	/// �� x, y Խ��ᴥ�� out_of_range �쳣.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="value">�ߵ�Ȩ��</param>
	/// <returns></returns>
	Graph<T>& d_link(size_t x, size_t y, T value)
	{
		if (x >= _size || y >= _size)
		{
			throw out_of_range("@Graph.d_link(size_t, size_t, T):\t out of range");
		}
		data[index_of(x, y)] = value;
		data[index_of(y, x)] = value;
		return *this;
	}

	void swap(Graph<T>& g2)
	{
		std::swap(data, g2.data);
		std::swap(_capacity, g2._capacity);
		std::swap(_size, g2._size);
		std::swap(default_value, g2.default_value);
	}

	static void swap(Graph<T>& g1, Graph<T>& g2)
	{
		g1.swap(g2);
	}
};


#endif // !__GRAPH_H__
