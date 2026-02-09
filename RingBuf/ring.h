/******************************************************************************
* 文件名称: ring.h
* 内容摘要: 循环队列头文件
* 创建者の: 孔佳伟
* 个人主页: https://gitee.com/thin-wind/jump
* 修改记录: 
******************************************************************************/

#ifndef __RING_H_
#define __RING_H_

#ifdef __cplusplus
extern "C" {
#endif

// 循环队列初始化
#define RING_INIT(queue) \
(void)({(queue)->head = 0; (queue)->tail = 0; (queue)->len = 0;})

// 缓冲区指针 a 前移，若已超出缓冲区右侧，则指针循环
#define RING_INC(a) ((a) = (a) < (RING_BUF_SIZE-1) ? (a)+1 : 0)

// 循环队列中已存放数据个数
#define RING_NUMS(queue) ((queue)->len)

// 往循环队列 queue 中放入一个数据
#define RING_PUT(c, queue) \
(void)({(queue)->buf[(queue)->head] = (c); RING_INC((queue)->head); (queue)->len < RING_BUF_SIZE ? (queue)->len++ : RING_INC((queue)->tail);})

// 从循环队列 queue 中取出一个数据
#define RING_GET(queue, c) \
(void)({c = (queue)->buf[(queue)->tail]; RING_INC((queue)->tail); (queue)->len ? (queue)->len-- : 0;})

/******************************************** 使用说明 ******************************************************
* 使用前先定义以下内容 
* #define RING_BUF_SIZE 8
* struct RingQueue 
* {
* 	 U32 head;					// 循环队列缓冲器中数据头指针
* 	 U32 tail;					// 循环队列缓冲器中数据尾指针
*    U32 len;                   // 循环队列缓冲器数据长度
*    U32 buf[RING_BUF_SIZE];    // 循环队列缓冲器的缓冲区
* };
* struct RingQueue myQueue;
* 使用：
    * 放入一个数据：RING_PUT(1, &myQueue);
    * 取出一个数据：U32 data; RING_GET(&myQueue, data);
*/

#ifdef __cplusplus
}
#endif

#endif

