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

#ifndef KNNIOCORE_BYTEBUFFER_H
#define KNNIOCORE_BYTEBUFFER_H

#include <KNFoundation/KNFoundation.h>
#include <assert.h>

KN_NAMESPACE_BEGIN(knnio)
KN_NAMESPACE_BEGIN(core)

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
    
private:
    
    const Allocate allocate;
    const Reallocate reallocate;
    const Deallocate deallocate;
    
public:
    
    ByteBufferAllocator(Allocate allocate, Reallocate reallocate, Deallocate deallocate) : allocate(allocate), reallocate(reallocate), deallocate(deallocate) {
        
    }
    
    ByteBufferAllocator() : allocate((Allocate)malloc), reallocate((Reallocate)realloc), deallocate((Deallocate)free) {
        
    }
    
    ByteBuffer buffer(uint32_t capacity) const;
};

class ByteBufferStorage: public KN::Nocopy {
    
    friend ByteBuffer;
    
private:
    
    Bytes bytes;
    Size capacity;
    ByteBufferAllocator allocator;
    
public:
    
    ByteBufferStorage(ByteBufferAllocator allocator, Bytes bytes, Size capacity) : allocator(allocator), bytes(bytes), capacity(capacity) {
        
    }
    
    ~ByteBufferStorage() {
        this->allocator.deallocate(bytes);
    }
    
    static ByteBufferStorageBacked reallocated(ByteBufferAllocator allocator, Size minimumCapacity);

};

class ByteBuffer {
    
private:
    
    Size readerIndex;
    Size writerIndex;
    ByteBufferStorageBacked storage;
    
public:
    
    ByteBuffer(ByteBufferAllocator allocator, Size capacity) : storage(ByteBufferStorage::reallocated(allocator, capacity)), readerIndex(0), writerIndex(0) {
        
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
    
    const ByteBufferStorage *get() const {
        return storage.get();
    }
};

KN_NAMESPACE_END
KN_NAMESPACE_END

#endif /* KNNIOCORE_BYTEBUFFER_H */
