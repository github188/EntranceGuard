/**
* 项    目   PET/CT软件 
* 模    块   FPGA
* 文件名称   CCycleBuffer.h
* 文件路径   FPGA/CCycleBuffer.h
* 摘    要   FPGA的循环缓冲区类
* 作    者   刘兆邦
* 完成日期   2012年12月07日
* Copyright (c) 2011~2012, 中科院苏州医工所医学影像室电子组.
* All rights reserved.
* Version v1.0
*/
#ifndef CCycleBuffer_H
#define CCycleBuffer_H
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>
class CCycleBuffer
{

public:
	/** 
	 *  function 构造函数
	 *  @param   size    缓冲区大小默认1M，
	 *					 也可以通过setBufferSize(int size)进行重置缓冲区大小，
	 *					 但是一定要在缓冲区为空的前提下，否则会丢失已有数据。
	 *  @param   port  端口
	 *  @return  bool  返回状态
	 *                 true,成功; false,失败
	 *  @author  Liuzhaobang
	 *  @date    2012-12-7
	 */
	CCycleBuffer(int size = 1024*1024);
	virtual ~CCycleBuffer();
	int write(char* buf, int count);
	int read(char* buf, int count);
	
	/** 
	 *  function 缓冲区是否已满?
	 *  @return  bool  返回状态
	 *                 true,真; false,假
	 *  @author  Liuzhaobang
	 *  @date    2012-12-7
	 */
	bool isFull();
	/** 
	 *  function 缓冲区是否空?
	 *  @return  bool  返回状态
	 *                 true,真; false,假
	 *  @author  Liuzhaobang
	 *  @date    2012-12-7
	 */
	bool isEmpty();
	/** 
	 *  function 设置缓冲区为空
	 *  @return  bool  返回状态
	 *                 true,真; false,假
	 *  @author  Liuzhaobang
	 *  @date    2012-12-7
	 */
	void setEmpty();
	/** 
	 *  function 返回当前缓冲区已用空间
	 *  @return  int   返回已用空间大小(byte)
	 *  @author  Liuzhaobang
	 *  @date    2012-12-7
	 */
	int getUsedSize();
	/** 
	 *  function 返回当前缓冲区可用空间
	 *  @return  int   返回空闲空间大小(byte)
	 *  @author  Liuzhaobang
	 *  @date    2012-12-7
	 */
	int getFreeSize();  
	/** 
	 *  function 返回缓冲空间大小
	 *  @return  int   返回缓冲空间大小
	 *  @author  Liuzhaobang
	 *  @date    2012-12-7
	 */
	int getBufSize()
	{
		return m_nBufSize;
	}
	/** 
	 *  function 返回缓冲空间开始位置
	 *  @return  int   返回缓冲空间开始位置
	 *  @author  Liuzhaobang
	 *  @date    2012-12-7
	 */
	int getStart()
	{
		return m_nReadPos;
	}
	/** 
	 *  function 返回缓冲空间结束位置
	 *  @return  int   返回缓冲空间结束位置
	 *  @author  Liuzhaobang
	 *  @date    2012-12-7
	 */
	int getEnd()
	{
		return m_nWritePos;
	}
	/** 
	 *  function 设置缓冲区大小
	 *  @param   size   新开辟缓冲区大小
	 *  @param   copy   是否拷贝已有数据至新缓冲区,默认false,不拷贝.
	 *  @return  bool   返回状态
	 *                  true,真; false,假
	 *  @author  Liuzhaobang
	 *  @date    2012-12-7
	 */
	bool setBufferSize(int size, bool copy = false);
	/**
	*  function 等待不空的情况下唤醒
	*  @author  Liuzhaobang
	*  @date    2012-12-7
	*/
	void waitNotEmpty();
	/**
	*  function 等待不满的情况下唤醒
	*  @author  Liuzhaobang
	*  @date    2012-12-7
	*/
	void waitNotFull();
private:
	//空标示符
	bool m_bEmpty;
	//满标示符
	bool m_bFull;
	//缓冲区指针
	char * m_pBuf;
	//缓冲区大小
	int m_nBufSize;
	//始位置
	int m_nReadPos;
	//尾位置
	int m_nWritePos;
	//数据互斥
	QMutex mutex;
	//等待条件-不空
	QWaitCondition bufferNotEmpty;
	//等待条件-不满
	QWaitCondition bufferNotFull;
};
#endif // CCycleBuffer_H
