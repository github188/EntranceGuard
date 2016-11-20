#include "CCycleBuffer.h"
#include <assert.h>
#include <memory.h>

CCycleBuffer::CCycleBuffer(int size)

{  
	m_nBufSize = size;  
	m_nReadPos = 0;  
	m_nWritePos = 0;  
	m_pBuf = new char[m_nBufSize];
	m_bEmpty = true;  
	m_bFull = false; 
}  
CCycleBuffer::~CCycleBuffer()  
{  
	delete[] m_pBuf;  
} 

/*
 *	函数介绍  向缓冲区写入数据，返回实际写入的字节数 
 */
int CCycleBuffer::write(char* buf, int count)  
{ 
	mutex.lock();
	if (count <= 0)
	{
		mutex.unlock();
		return 0;
	}
	m_bEmpty = false;  
	// 缓冲区已满，不能继续写入  
	if(m_bFull)   
	{  
		mutex.unlock();
		return 0;  
	}  
	else if(m_nReadPos == m_nWritePos) // 缓冲区为空时  
	{  
		/*                          == 内存模型 == 
				 (empty)             m_nReadPos                (empty)                       
		|----------------------------------|-----------------------------------------| 
									m_nWritePos        m_nBufSize 
		*/  
		int leftcount = m_nBufSize - m_nWritePos;  
		if(leftcount > count)  
		{  
			memcpy(m_pBuf + m_nWritePos, buf, count);  
			m_nWritePos += count;  
			m_bFull = (m_nWritePos == m_nReadPos); 
			bufferNotEmpty.wakeAll();
			mutex.unlock();
			return count;  
		}  
		else  
		{  
			memcpy(m_pBuf + m_nWritePos, buf, leftcount);  
			m_nWritePos = (m_nReadPos > count - leftcount) ? count - leftcount : m_nWritePos;  
			memcpy(m_pBuf, buf + leftcount, m_nWritePos);  
			m_bFull = (m_nWritePos == m_nReadPos);  
			bufferNotEmpty.wakeAll();
			mutex.unlock();
			return leftcount + m_nWritePos;  
		}  
	}   
	else if(m_nReadPos < m_nWritePos) // 有剩余空间可写入  
	{  
		/*                           == 内存模型 == 
			(empty)                 (data)                     (empty) 
		|-------------------|----------------------------|---------------------------| 
					m_nReadPos                m_nWritePos       (leftcount)            
		*/  
		// 剩余缓冲区大小(从写入位置到缓冲区尾)  

		int leftcount = m_nBufSize - m_nWritePos;
		if(leftcount > count)   // 有足够的剩余空间存放  
		{  
			memcpy(m_pBuf + m_nWritePos, buf, count);  
			m_nWritePos += count;  
			m_bFull = (m_nReadPos == m_nWritePos);  
			assert(m_nReadPos <= m_nBufSize);  
			assert(m_nWritePos <= m_nBufSize); 
			bufferNotEmpty.wakeAll();
			mutex.unlock();
			return count;  
		}  
		else       // 剩余空间不足  
		{  
			// 先填充满剩余空间，再回头找空间存放  
			memcpy(m_pBuf + m_nWritePos, buf, leftcount); 

			m_nWritePos = (m_nReadPos >= count - leftcount) ? count - leftcount : m_nReadPos;  
			memcpy(m_pBuf, buf + leftcount, m_nWritePos);  
			m_bFull = (m_nReadPos == m_nWritePos);     
			assert(m_nReadPos <= m_nBufSize);  
			assert(m_nWritePos <= m_nBufSize); 
			bufferNotEmpty.wakeAll();
			mutex.unlock();
			return leftcount + m_nWritePos;  
		}  
	}  
	else  
	{  
		/*                          == 内存模型 == 
			(unread)                 (read)                     (unread) 
		|-------------------|----------------------------|---------------------------| 
					 m_nWritePos    (leftcount)    m_nReadPos                       
		*/  
		int leftcount = m_nReadPos - m_nWritePos;  
		if(leftcount > count)  
		{  
			// 有足够的剩余空间存放  
			memcpy(m_pBuf + m_nWritePos, buf, count);  
			m_nWritePos += count;  
			m_bFull = (m_nReadPos == m_nWritePos);  
			assert(m_nReadPos <= m_nBufSize);  
			assert(m_nWritePos <= m_nBufSize);  
			bufferNotEmpty.wakeAll();
			mutex.unlock();
			return count;  
		}  
		else  
		{  
			// 剩余空间不足时要丢弃后面的数据  
			memcpy(m_pBuf + m_nWritePos, buf, leftcount);  
			m_nWritePos += leftcount;  
			m_bFull = (m_nReadPos == m_nWritePos);  
			assert(m_bFull);  
			assert(m_nReadPos <= m_nBufSize);  
			assert(m_nWritePos <= m_nBufSize); 
			bufferNotEmpty.wakeAll();
			mutex.unlock();
			return leftcount;  
		}  
	}  
}  
/*
 *	函数介绍  从缓冲区读数据，返回实际读取的字节数 
 */
int CCycleBuffer::read(char* buf, int count)  
{  
	mutex.lock();
	if (count <= 0)
	{
		mutex.unlock();
		return 0;
	}
		  
	m_bFull = false;  
	if(m_bEmpty)       // 缓冲区空，不能继续读取数据  
	{  
		mutex.unlock();
		return 0;  
	}  
	else if(m_nReadPos == m_nWritePos)   // 缓冲区满时  
	{  
		/*                          == 内存模型 == 
		 (data)          m_nReadPos                (data)     
		|--------------------------------|--------------------------------------------| 
			 m_nWritePos         m_nBufSize 
		*/  
		int leftcount = m_nBufSize - m_nReadPos;  
		if(leftcount > count)  
		{  
			memcpy(buf, m_pBuf + m_nReadPos, count);  
			m_nReadPos += count;  
			m_bEmpty = (m_nReadPos == m_nWritePos);  
			bufferNotFull.wakeAll();
			mutex.unlock();
			return count;  
		}  
		else  
		{  
			memcpy(buf, m_pBuf + m_nReadPos, leftcount);  
			m_nReadPos = (m_nWritePos > count - leftcount) ? count - leftcount : m_nWritePos;  
			memcpy(buf + leftcount, m_pBuf, m_nReadPos);  
			m_bEmpty = (m_nReadPos == m_nWritePos);  
			bufferNotFull.wakeAll();
			mutex.unlock();
			return leftcount + m_nReadPos;  
		}  
	}  
	else if(m_nReadPos < m_nWritePos)   // 写指针在前(未读数据是连接的)  
	{  
		/*                          == 内存模型 == 
			(read)                 (unread)                      (read)     
		|-------------------|----------------------------|---------------------------| 
					m_nReadPos                m_nWritePos                     m_nBufSize 
		*/  
		int leftcount = m_nWritePos - m_nReadPos;  
		int c = (leftcount > count) ? count : leftcount;  
		memcpy(buf, m_pBuf + m_nReadPos, c);  
		m_nReadPos += c;  
		m_bEmpty = (m_nReadPos == m_nWritePos);  
		assert(m_nReadPos <= m_nBufSize);  
		assert(m_nWritePos <= m_nBufSize);  
		bufferNotFull.wakeAll();
		mutex.unlock();
		return c;  
	}  
	else          // 读指针在前(未读数据可能是不连接的)  
	{  
		/*                          == 内存模型 == 
			  (unread)                (read)                      (unread) 
		|-------------------|----------------------------|---------------------------| 
					 m_nWritePos                  m_nReadPos                  m_nBufSize 

		*/  
		int leftcount = m_nBufSize - m_nReadPos;  
		if(leftcount > count)   // 未读缓冲区够大，直接读取数据  
		{  
			memcpy(buf, m_pBuf + m_nReadPos, count);  
			m_nReadPos += count;  
			m_bEmpty = (m_nReadPos == m_nWritePos);  
			assert(m_nReadPos <= m_nBufSize);  
			assert(m_nWritePos <= m_nBufSize); 
			bufferNotFull.wakeAll();
			mutex.unlock();
			return count;  
		}  
		else       // 未读缓冲区不足，需回到缓冲区头开始读  
		{  
			memcpy(buf, m_pBuf + m_nReadPos, leftcount);  
			m_nReadPos = (m_nWritePos >= count - leftcount) ? count - leftcount : m_nWritePos;  
			memcpy(buf + leftcount, m_pBuf, m_nReadPos);  
			m_bEmpty = (m_nReadPos == m_nWritePos);  
			assert(m_nReadPos <= m_nBufSize);  
			assert(m_nWritePos <= m_nBufSize);
			bufferNotFull.wakeAll();
			mutex.unlock();
			return leftcount + m_nReadPos;  
		}    
	}  
} 

bool CCycleBuffer::isFull()  
{  
	return m_bFull;  
}   
bool CCycleBuffer::isEmpty()  
{  
	return m_bEmpty;  
}  

void CCycleBuffer::setEmpty()  
{  
	mutex.lock();
	m_nReadPos = 0;  
	m_nWritePos = 0;  
	m_bEmpty = true;  
	m_bFull = false;  
	mutex.unlock();
}  

/**
 * 获取缓冲区有效数据长度
 */
int CCycleBuffer::getUsedSize()  
{  
	if(m_bEmpty)  
	{  
		return 0;  
	}  
	else if(m_bFull)  
	{  
		return m_nBufSize;  
	}  
	else if(m_nReadPos < m_nWritePos)  
	{  
		return m_nWritePos - m_nReadPos;  
	}  
	else  
	{  
		return m_nBufSize - m_nReadPos + m_nWritePos;  
	}  
}
/**
 * 获取缓冲区空闲空间数据长度
 */
int CCycleBuffer::getFreeSize() 
{  
	if(m_bEmpty)  
	{  
		return m_nBufSize;  
	}  
	else if(m_bFull)  
	{  
		return 0;  
	}  
	else if(m_nReadPos > m_nWritePos)  
	{  
		return m_nReadPos - m_nWritePos;  
	}  
	else  
	{  
		return m_nBufSize - m_nWritePos + m_nReadPos;  
	}  
}


/*
*	@brief  设置缓冲区大小
*			  若没有缓冲区,则新建缓冲区;若已有缓冲区,则新建缓冲区并按照copy标志，进行旧数据拷贝
*/
bool CCycleBuffer::setBufferSize(int size, bool copy)
{
	//若已有空间，释放掉。开辟新缓冲区，并进行拷贝旧数据
	if (m_pBuf != 0)
	{

		//新开辟缓冲区
		char* buf = new char(size);
		//若需要拷贝
		if (copy)
		{
			if (size <= m_nBufSize)
			{
				//拷贝旧数据
				memcpy(buf, m_pBuf, size);
				qDebug()<<"CCycleBuffer::setBufferSize,重置缓冲区，开辟空间小于原先旧空间，会丢失数据。";
			}
			else
				memcpy(buf, m_pBuf, m_nBufSize);
		}
		//释放旧缓冲区
		delete m_pBuf;
		//指向新缓冲区
		m_pBuf = buf;
		//重置缓冲区大小
		m_nBufSize = size;
	}
	//若没有缓冲区，则开辟缓冲区
	else
	{
		//新开辟缓冲区
		m_pBuf = new char(size);
		//赋值缓冲区大小
		m_nBufSize = size;
	}

	if (m_pBuf != 0)
		return true;
	else
		return false;
}

void CCycleBuffer::waitNotEmpty()
{
	mutex.lock();
	bufferNotEmpty.wait(&mutex);
	mutex.unlock();
}
void CCycleBuffer::waitNotFull()
{
	mutex.lock();
	bufferNotFull.wait(&mutex);
	mutex.unlock();
}