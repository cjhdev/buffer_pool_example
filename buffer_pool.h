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
#ifndef BUFFER_POOL_H
#define BUFFER_POOL_H

/**
 * @defgroup buffer_pool Fixed size buffer pool without dynamic memory allocation
 *
 * A buffer pool suitable for systems where use of dynamic memory
 * is not desireable.
 * 
 * Requires C++11.
 * 
 * */

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>

using namespace std;

/** Buffer object passed to application code */
class Buffer {

    /** let BufferPool class access internals */
    template<size_t, size_t>
    friend class BufferPool;

    uint8_t *dataPtr;   /**< pointer to buffer memory */
    size_t dataMax;     /**< maximum byte length of `data` */
    size_t dataLen;     /**< length of data in buffer set by application */

    public:

        /** @return constant pointer to buffer */
        const uint8_t *data()
        {
            return dataPtr;
        }

        /** @return byte size of allocated storage */
        size_t capacity()
        {
            return dataMax;
        }

        /** clear contents of buffer */
        void clear()
        {
            dataLen = 0;
        }

        /** @return byte length of data in `data` buffer */
        size_t length()
        {
            return dataLen;
        }

        /** Append data to the buffer
         *
         * @param[in] data data to write to buffer
         * @param[in] len byte length of `data`
         *
         * @return number of bytes successfully written to buffer
         *
         * */
        size_t append(const uint8_t *data, size_t len)
        {
            size_t write = len;

            if((dataMax - dataLen) < write){

                write = (dataMax - dataLen);
            }

            memcpy(&dataPtr[dataLen], data, write);
            dataLen += write;

            return write;
        }
};


/**
 * Contains buffer memory, freeList and alloc/free functions.
 *
 * Uses template parameters to set the size of the pool and the size
 * of each buffer in the pool.
 *
 * */
template<size_t POOL_SIZE, size_t BUFFER_SIZE>
class BufferPool {

    /** storage element */
    struct element {
    
        Buffer buffer;              /**< buffer pointers passed to application */
        struct element *next;       /**< next storage element */        
        uint8_t data[BUFFER_SIZE];  /**< buffer memory */
    };

    /** array of storage elements */
    struct element pool[POOL_SIZE];

    /** single linked list of free storage elements */
    struct element *freeList;
    
    public:

        /** 
         * Clears all data and links freeList
         * 
         * */
        BufferPool(void)
        {
            static_assert(POOL_SIZE > 0, "POOL_SIZE must be at least 1");

            size_t i;
            memset(pool, 0, sizeof(pool));
            freeList = pool;

            for(i=0; i < (POOL_SIZE-1); i++){

                pool[i].next = &pool[i+1];
                pool[i].buffer.dataPtr = pool[i].data;
                pool[i].buffer.dataMax = BUFFER_SIZE;                
            }

            pool[i].buffer.dataPtr =  pool[i].data;
            pool[i].buffer.dataMax = BUFFER_SIZE;                
        };

        /**
         * Allocates a Buffer (or nullptr if BufferPool is exhausted)
         * 
         * @return Buffer
         * @retval nullptr (BufferPool is exhausted)
         *
         *  */
        Buffer *allocBuffer(void)
        {
            Buffer *retval = nullptr;

            if(freeList != nullptr){

                retval = &freeList->buffer;
                freeList = freeList->next;
                retval->dataLen = 0U;
            }

            return retval;
        };

        /**
         * Returns previously allocated buffer to freeList.
         *
         * Also clears buffer pointer to prevent reuse.
         * 
         * @param[in] b previously allocated buffer object
         * 
         */
        void freeBuffer(Buffer **b)
        {
            assert(b != nullptr);
            assert(*b != nullptr);
            assert((*b >= &pool->buffer) && (*b <= &pool[POOL_SIZE-1].buffer));
            
            ((struct element *)(*b))->next = freeList;  /* OK since this is POD */
            freeList = (struct element *)(*b);
            *b = nullptr;
        }    
};

#endif


