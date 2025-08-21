//
//  Thread.cpp
//  npnio
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

#include <NPNIO/Thread.h>
#include <thread>

NP_NAMESPACE_BEGIN(NP)
NP_NAMESPACE_BEGIN(NIO)

struct ThreadBox {
    Thread *thread;
    void (^body)(const Thread &thread);
    ThreadBox(Thread *thread, void (^body)(const Thread &thread)) : thread(thread), body(body) { }
};

static void * run(void *args) {
    ThreadBox *box = (ThreadBox *)args;
    pthread_setname_np(box->thread->cName());
    Thread thread{box->thread->getName(), pthread_self()};
    box->body(thread);
    delete box->thread;
    delete box;
    return nullptr;
}

void Thread::spawnAndRun(std::string name, void (^body)(const Thread &thread)) {
    pthread_t pThread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_set_qos_class_np(&attr, qos_class_main(), 0);
    ThreadBox *box = new ThreadBox(new Thread(name), body);
    pthread_create(&pThread, &attr, run, box);
    pthread_attr_destroy(&attr);
}

NP_NAMESPACE_END
NP_NAMESPACE_END
