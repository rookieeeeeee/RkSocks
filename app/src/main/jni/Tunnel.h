//
// Created by 张开海 on 2023/11/3.
//

#ifndef R_TUNNEL_H
#define R_TUNNEL_H

#include "EventLoop.h"
#include "Buffer.h"
#include <netinet/in.h>

/**
 * 数据转发通道
 */
namespace R {
    class Tunnel;

    enum TunnelStage {
        STAGE_HANDSHAKE,  //握手
        STAGE_TRANSFER    //传输
    };

    class TunnelContext {
    public:
        /**
         * socket
         */
        int fd;
        /**
         * 此端绑定的主机地址信息
         */
        sockaddr_in sockAddr;
        /**
         * 读缓冲区
         */
        Buffer *inBuffer;
        /**
         * 写缓冲区
         */
        Buffer *outBuffer;
        /**
         * 状态
         */
        TunnelStage tunnelStage;
        /**
         * 持有tunnel引用
         */
        Tunnel *tunnel;

        TunnelContext(int fd,
                      sockaddr_in sockaddrIn,
                      int capacity,
                      TunnelStage tunnelStage,
                      Tunnel *tunnel) :
                fd(fd),
                sockAddr(sockaddrIn),
                inBuffer(new Buffer(capacity)),
                outBuffer(new Buffer(capacity)),
                tunnelStage(tunnelStage),
                tunnel(tunnel) {

        }

        virtual ~TunnelContext() {
            if (inBuffer) {
                delete inBuffer;
                inBuffer = 0;
            }
            if (outBuffer) {
                delete outBuffer;
                outBuffer = 0;
            }
        }
    };

    class Tunnel {
    private:
        friend class Socks5Server;

    protected:
        char *mBuffer;
        int mBufferLen;
        std::shared_ptr<EventLoop> mLooper;
    public:
        TunnelContext *inbound;
        TunnelContext *outbound;

        TunnelContext *getPeer(TunnelContext *context) {
            if (context == inbound) {
                return outbound;
            } else {
                return inbound;
            }
        }

        Tunnel() = delete;

        /**
         *
         * @param bufferLen 缓冲区长度，一般设置与MTU大小一致即可
         * @param loop 事件循环
         */
        Tunnel(int bufferLen, std::shared_ptr<EventLoop> &loop);

        virtual ~Tunnel();

        /**
        * 创建出站连接，并为[outbound]赋值
        *
        * @param tunnel
        * @return
        */
        virtual bool attachOutboundContext() = 0;

        /**
         * 入站处理握手
         *
         * 握手完成设置[TunnelContext.tunnelStage]为TunnelStage::TRANSFER
         *
         * @param tunnel
         */
        virtual void handleInboundHandshake() {};

        /**
         * 出处理握手
         *
         * 握手完成设置[TunnelContext.tunnelStage]为TunnelStage::TRANSFER
         *
         * @param tunnel
         */
        virtual void handleOutboundHandshake() {};

        /**
         * inbound->outbound 加密数据
         *
         * @param tunnel
         */
        virtual void packData() = 0;

        /**
         * outbound->inbound 解密数据
         *
         * @param tunnel
         */
        virtual void unpackData() = 0;

        virtual void handleRead(TunnelContext *context) = 0;

        virtual void handleWrite(TunnelContext *context) = 0;

        virtual void handleClosed(TunnelContext *context) = 0;

        /**
         * 监听此通道的读写事件
         */
        virtual void registerTunnelEvent();
    };

} // R

#endif //R_TUNNEL_H
