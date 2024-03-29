#pragma once
#include <stdexcept>

template<typename T>
void Swap(T& a, T& b)
{
	T t = a;
	a = b;
	b = t;
}

template<typename dtype>
class Buffer
{
private:
	int length;
	dtype* buffer;
public:
	void release()
	{
		length = 0;
		if (buffer)
			delete[] buffer;
		buffer = nullptr;
	}

	void reserve(int _size)
	{
		if (_size <= 0)
		{
			release();
			throw std::runtime_error("buffer size can't be under zero");
		}
		length = _size;
		if (buffer)
			delete[] buffer;
		buffer = new dtype[length];
	}

	int size() const
	{
		return length;
	}

	dtype* getBuffer() const
	{
		return buffer;
	}

	Buffer()
	{
		length = 0;
		buffer = nullptr;
	}

	Buffer(int size)
	{
		reserve(size);
	}

	Buffer(const Buffer& _R)
	{
		length = _R.length;
		buffer = _R.buffer;
	}

	~Buffer()
	{
	if (buffer)
		delete[] buffer;
	}

	static void Swap(Buffer* Destination, Buffer* Source)
	{
		Swap(Destination->buffer, Source->buffer);
		Swap(Destination->length, Source->length);
	}
};