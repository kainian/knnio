//
//  Thread.h
//  knnio
//
//  Created by Jonathan Lee on 8/13/25.
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

#ifndef KNNIO_THREAD_H
#define KNNIO_THREAD_H

#include <KNFoundation/KNFoundation.h>
#include <string>

KN_NAMESPACE_BEGIN(knnio)

class Thread {
    
private:
    
    const std::string name;
    
    const pthread_t handle;
    
public:
    
    Thread(const std::string &name) : name(name), handle(0) {
        
    }
    
    Thread(const std::string &name, pthread_t handle) : name(name), handle(handle) {
        
    }
    
public:
    
    const std::string & getName() const {
        return this->name;
    }
    
    const char * cName() const {
        return this->name.c_str();
    }
    
    pthread_t getHandle() const {
        return this->handle;
    }
    
public:
    
    bool isCurrentThread() const {
        return this->handle == pthread_self();
    }
    
public:
    
    static void spawnAndRun(std::string name, void (^body)(const Thread &thread));
    
};

KN_NAMESPACE_END

#endif /* KNNIO_THREAD_H */
