//
//  ByteBuffer.h
//  knnio
//
//  Created by Jonathan Lee on 8/8/25.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//

#ifndef KNNIO_BYTEBUFFER_H
#define KNNIO_BYTEBUFFER_H

#include <KNFoundation/KNFoundation.h>
#include <assert.h>

KN_NAMESPACE_BEGIN(knnio)

class ByteBufferAllocator;
class ByteBufferStorage;
class ByteBuffer;

typedef size_t Size;
typedef uint8_t * Bytes;

using ByteBufferStorageBacked = KN::CopyOnWriteBacked<ByteBufferStorage>;

class ByteBufferAllocator {
    
    friend ByteBufferStorage;
    
public:
    
    typedef Bytes (*Allocate)(Size);
    typedef Bytes (*Reallocate)(Bytes, Size);
    typedef void (*Deallocate)(Bytes);
    typedef void (*Memmove)(Bytes, const Bytes, Size);
    
public:
    
    const Allocate allocate;
    const Reallocate reallocate;
    const Deallocate deallocate;
    const Memmove memmove;
    
public:
    
    ByteBufferAllocator(Allocate allocate, Reallocate reallocate, Deallocate deallocate, Memmove memmove) : allocate(allocate), reallocate(reallocate), deallocate(deallocate), memmove(memmove) {
        
    }
    
    ByteBufferAllocator() : allocate((Allocate)::malloc), reallocate((Reallocate)::realloc), deallocate((Deallocate)::free), memmove((Memmove)::memmove) {
        
    }
    
    ByteBuffer buffer(uint32_t capacity) const;
};

class ByteBufferStorage {
    
    friend ByteBuffer;
    
private:
    
    Bytes bytes;
    Size capacity;
    ByteBufferAllocator allocator;
    
public:
    
    ByteBufferStorage(ByteBufferAllocator allocator, Bytes bytes, Size capacity) : allocator(allocator), bytes(bytes), capacity(capacity) {
        
    }
    
    ByteBufferStorage(const ByteBufferStorage &storage);
    
    ~ByteBufferStorage() {
        this->allocator.deallocate(bytes);
    }
    
    Size setBytes(const Bytes bytes, Size size, Size atIndex);
    
    static ByteBufferStorageBacked allocated(ByteBufferAllocator allocator, Size minimumCapacity);
    
    void reallocated(Size minimumNeededCapacity);

};

class ByteBuffer {
    
private:
    
    Size readerIndex;
    Size writerIndex;
    ByteBufferStorageBacked storage;
    
public:
    
    ByteBuffer(ByteBufferAllocator allocator, Size capacity) : storage(ByteBufferStorage::allocated(allocator, capacity)), readerIndex(0), writerIndex(0) {
        
    }
    
public:
    
    Size writableBytes() const {
        if (this->capacity() > this->writerIndex) {
            return this->capacity() - this->writerIndex;
        } else {
            return 0;
        }
    }
    
    Size readableBytes() const {
        if (this->writerIndex > this->readerIndex) {
            return this->writerIndex - this->readerIndex;
        } else {
            return 0;
        }
    }
    
    Size capacity() const {
        return this->get()->capacity;
    }
    
public:
    
    Size writeBytes(const Bytes bytes, Size size) {
        Size written = this->setBytes(bytes, size, this->writerIndex);
        this->moveWriterIndexForwardBy(written);
        return written;
    }
    
    Size writeString(const std::string &string) {
        return this->writeBytes((Bytes)string.data(), string.size());
    }
    
    Size writeCString(const char * cString) {
        return this->writeBytes((Bytes)cString, strlen(cString));
    }
    
private:
    
    void moveReaderIndexTo(Size index) {
        assert(index >= 0 && index <= this->writerIndex);
        this->readerIndex = index;
    }
    
    void moveReaderIndexForwardBy(Size offset) {
        this->moveReaderIndexTo(this->readerIndex + offset);
    }
    
    void moveWriterIndexTo(Size index) {
        assert(index >= 0 && index <= this->capacity());
        this->writerIndex = index;
    }
    
    void moveWriterIndexForwardBy(Size offset) {
        this->moveWriterIndexTo(this->writerIndex + offset);
    }
    
    Size setBytes(Bytes bytes, Size size, Size atIndex) {
        return this->storage->setBytes(bytes, size, atIndex);
    }
    
private:
    
    const ByteBufferStorage *get() const {
        return storage.get();
    }
};

KN_NAMESPACE_END

#endif /* KNNIO_BYTEBUFFER_H */
