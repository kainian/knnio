//
//  ByteBufferTests.m
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

#import <XCTest/XCTest.h>
#import <KNNIO/KNNIO.h>

using namespace knnio;

@interface ByteBufferTests : XCTestCase {
    ByteBufferAllocator _allocator;
}

@end

@implementation ByteBufferTests

- (void)testAllocateAndCount {
    const auto buffer = _allocator.buffer(1024);
    XCTAssertEqual(1024, buffer.capacity());
}

- (void)testSimpleWritesAndReadTest {
    Bytes bytes = _allocator.allocate(1024);
    auto buffer = _allocator.buffer(1024);
    auto written = buffer.writeBytes(bytes, 1024);
    auto writableBytes = buffer.writableBytes();
    XCTAssertEqual(1024, buffer.readableBytes());
    XCTAssertEqual(1024, written);
    
    written = buffer.writeCString("");
    writableBytes = buffer.writableBytes();
    XCTAssertEqual(1024, buffer.readableBytes());
    XCTAssertEqual(0, written);
    
    written = buffer.writeCString("X");
    writableBytes = buffer.writableBytes();
    XCTAssertEqual(1025, buffer.readableBytes());
    XCTAssertEqual(1, written);
    
    written = buffer.writeCString("XXXXX");
    writableBytes = buffer.writableBytes();
    XCTAssertEqual(1030, buffer.readableBytes());
    XCTAssertEqual(5, written);
    
    _allocator.deallocate(bytes);
}

@end
