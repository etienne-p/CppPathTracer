//
// Created by etienne cella on 2018-05-28.
//
#pragma once

#include <iostream>
#include <cstdint>
#include <cassert>
#include <memory>
#include "Util.h"

// TODO note the non copyable but movable type
class Buffer
{
    using Byte = uint8_t;
public:
    Buffer(size_t byteSize_) : size(0), capacity(byteSize_) { data = (Byte *) malloc(byteSize_); }
    // movable
    Buffer(Buffer &&other) : data(other.data), size(other.size), capacity(other.capacity) {
        // other has lost the resource
        other.data = nullptr;
        other.size = other.capacity = 0;
    }
    Buffer &operator=(Buffer &&other) {
        if (this != &other) {
            data = other.data;
            size = other.size;
            capacity = other.capacity;
            // other has lost the resource
            other.data = nullptr;
            other.size = other.capacity = 0;
        }
        return *this;
    }

    // check for nullptr as we support move and may have lost ownership of the resource
    ~Buffer() { if (data != nullptr) free(data); }

    template<typename T>
    static std::unique_ptr<Buffer> create(size_t size_) { return make_unique<Buffer>(size_ * sizeof(T)); }

    void clear() { size = 0; }
    size_t getCapacity() const { return capacity; }
    Byte* data;
    size_t size;
private:
    // non copyable
    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;
    size_t capacity;
};


template <typename T>
class StackView
{
public:
    StackView(Buffer * buffer_) : buffer(buffer_){}

    void push(const T& item)
    {
        assert(buffer->size + stride() <= buffer->getCapacity());
        memcpy(buffer->data + buffer->size, &item, stride());
        buffer->size += stride();
    }

    void pop(T& item)
    {
        assert(buffer->size >= stride());
        buffer->size -= stride();
        memcpy(&item, buffer->data + buffer->size, stride());
    }

    size_t size() const { return buffer->size / stride(); }

    bool empty() const { return size() == 0; }

    int stride() const { return sizeof(T); };

private:
    Buffer* buffer;
};

template <typename T>
class VectorView
{
public:
    VectorView(Buffer * buffer_) : buffer(buffer_){}

    size_t size() const { return buffer->size / stride(); }

    T& operator[](size_t i)
    {
        assert(stride() * (i + 1) <= buffer->getCapacity());
        return *(T*)(buffer->data + stride() * i);
    }

    const T& operator[](size_t i) const
    {
        assert(stride() * (i + 1) <= buffer->getCapacity());
        return *(T*)(buffer->data + stride() * i);
    }

    int stride() const { return sizeof(T); };

private:
    Buffer* buffer;
};

template <typename T>
class ConstVectorView
{
public:
    ConstVectorView(const Buffer * buffer_) : buffer(buffer_){}

    size_t size() const { return buffer->size / stride(); }

    const T& operator[](size_t i) const
    {
        assert(stride() * (i + 1) <= buffer->getCapacity());
        return *(T*)(buffer->data + stride() * i);
    }

    int stride() const { return sizeof(T); };

private:
    const Buffer* buffer;
};