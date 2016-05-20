/* Copyright (c) 2016 Cameron Harper
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * 
 * */

#include "buffer_pool.h"

#include <iostream>
#include <cassert>


BufferPool<2, 50> inputPool;
BufferPool<4, 100> outputPool;

int
main(void)
{
    cout << "In this app we have two buffer pools: inputPool and outputPool" << endl;
    cout << "inputPool: " << &inputPool << endl;
    cout << "outputPool: " << &outputPool << endl;
    cout << endl;
    
    cout << "Get two buffers from inputPool" << endl;

    Buffer *b1 = inputPool.allocBuffer();
    Buffer *b2 = inputPool.allocBuffer();
    assert(b1 != nullptr && b2 != nullptr);

    cout << "b1: " << b1 << endl;
    cout << "b2: " << b2 << endl;
    cout << endl;
    
    cout << "Get third buffer (this should return nullptr)" << endl;

    Buffer *b3 = inputPool.allocBuffer();
    assert(b3 == nullptr);

    cout << "b3: " << b3 << endl;
    cout << endl;
    
    cout << "Now, free b2 (this should set b2 to nullptr)" << endl;
    
    inputPool.freeBuffer(&b2);
    assert(b2 == nullptr);

    cout << "b2: " << b2 << endl;
    cout << endl;

    cout << "Try to allocate b3 again" << endl;
    b3 = inputPool.allocBuffer();
    assert(b3 != nullptr);

    cout << "b3: " << b3 << endl;
    cout << endl;

    cout << "Now imagine our application needs to allocate a buffers with a different priority to b1, b2 and b3." << endl;
    cout << "In this example the app can make use of outputPool" << endl;
    cout << endl;

    cout << "Get four buffers from outputPool" << endl;

    Buffer *b4 = outputPool.allocBuffer();
    Buffer *b5 = outputPool.allocBuffer();
    Buffer *b6 = outputPool.allocBuffer();
    Buffer *b7 = outputPool.allocBuffer();

    cout << "b4: " << b4 << endl;
    cout << "b5: " << b5 << endl;
    cout << "b6: " << b6 << endl;
    cout << "b7: " << b7 << endl;
    cout << endl;

    cout << "now write \"hello world\" to b4" << endl;

    size_t wrote = b4->append((const uint8_t *)"hello world", strlen("hello world"));

    assert(wrote == strlen("hello world"));
}
