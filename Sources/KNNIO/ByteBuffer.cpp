//
//  ByteBuffer.cpp
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

#include <KNNIO/ByteBuffer.h>

KN_NAMESPACE_BEGIN(knnio)

static Size nextPowerOf2ClampedToMax(Size minimumCapacity) {
    if (minimumCapacity <= 0) {
        return 1;
    }
    Size capacity = minimumCapacity;
    
    capacity -= 1;
    capacity |= capacity >> 1;
    capacity |= capacity >> 2;
    capacity |= capacity >> 4;
    capacity |= capacity >> 8;
    capacity |= capacity >> 16;
    if (capacity != UINT32_MAX) {
        return capacity += 1;
    }
    return capacity;
}

ByteBuffer ByteBufferAllocator::buffer(uint32_t capacity) const {
    return ByteBuffer(*this, capacity);
}

ByteBufferStorage::ByteBufferStorage(const ByteBufferStorage &storage) : allocator(storage.allocator) {
    this->capacity = nextPowerOf2ClampedToMax(capacity);
    this->bytes = allocator.allocate(this->capacity);
}

Size ByteBufferStorage::setBytes(const Bytes bytes, Size size, Size atIndex) {
    if (size <= 0) {
        return 0;
    }
    Size bytesCount = size;
    Size newEndIndex = atIndex + bytesCount;
    if (newEndIndex > this->capacity) {
        this->reallocated(newEndIndex);
    }
    this->allocator.memmove(this->bytes + atIndex, bytes, size);
    return size;
}

ByteBufferStorageBacked ByteBufferStorage::allocated(ByteBufferAllocator allocator, Size minimumCapacity) {
    Size capacity = nextPowerOf2ClampedToMax(minimumCapacity);
    return KN::CopyOnWriteMake<ByteBufferStorage>(allocator, allocator.allocate(capacity), capacity);
}

void ByteBufferStorage::reallocated(Size minimumNeededCapacity) {
    this->capacity = nextPowerOf2ClampedToMax(minimumNeededCapacity);
    Bytes newBytes = this->allocator.reallocate(this->bytes, this->capacity);
    if (newBytes) {
        this->bytes = newBytes;
    }
}

KN_NAMESPACE_END
