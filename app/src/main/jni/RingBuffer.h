//
// Created by 张开海 on 2023/11/2.
//

#ifndef R_RINGBUFFER_H
#define R_RINGBUFFER_H

#include <unistd.h>
#include <string>

namespace R {

    class RingBuffer {

        char *mBuffer;
        int mCapacity;
        int mAvailable;
        int mHead;
        int mTail;

    public:
        RingBuffer(int capacity);

        virtual ~RingBuffer();

        /**
         * 将本buffer写入dst
         * @return 实际写入的字节数
         */
        int writeTo(RingBuffer *dstBuffer);

        /**
         * 从srcBuffer中拷贝内容
         * @return 实际拷贝的字节数
         */
        int readFrom(RingBuffer *buffer);

        int readFrom(RingBuffer *buffer, int size);

        int write(const char *buffer, int length);

        int read(char *buffer, int length);

        /**
         * 读1字节
         */
        uint8_t read1();

        /**
         * 读2字节
         */
        uint16_t read2();

        /**
         * 读4字节
         */
        uint32_t read4();

        /**
        * 读8字节
        */
        uint64_t read64();

        /**
         * 空闲字节数
         */
        int available() const;

        /**
         * 已使用数据长度
         */
        int length() const;

        /**
         * 总容量
         */
        int capacity() const;

        /**
         * 是否空
         */
        bool empty() const;

        /**
         * 非空
         */
        bool notEmpty() const;

        /**
         * 回滚
         */
        void unread(int n);

        /**
         * 读1字节
         */
        uint8_t get(int i);

        void set(int i,uint8_t value);

        /**
         * 读1字节
         */
        int write1(uint8_t data);

        /**
         * 读2字节
         */
        int write2(uint16_t data);

        /**
         * 读4字节
         */
        int write4(uint32_t data);

        /**
        * 读8字节
        */
        int write8(uint64_t data);

        std::string toString();

    };

} // R

#endif //R_RINGBUFFER_H
