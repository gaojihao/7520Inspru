/************************************************************************
* Copyright (C)2010,  
* All rights reserved.
* 
* 文件名称:SerialCom.h

* 文件标识: 串口类
* 摘    要: 

* 作    者: 
* 完成日期: 2011/02/22
* 备注:在使用类之前，需使用注册函数注册两个回调函数，原型为：

void (CALLBACK *READ_DATA)(BYTE *buf, DWORD buflen);
用于输出从串口读取的内容。

int(CALLBACK *ERROR_CONTENT)(LPCTSTR);
用于输出错误信息。
************************************************************************/
#ifndef __SERIAL_INTERFACE_CLASS__
#define __SERIAL_INTERFACE_CLASS__


//定义串口输入参数结构体
typedef struct
{
	LPCTSTR tzPort;							 //串口名
	int nBaudRate;							//波特率
	int nDataBits;								//指定每个字节的位数（通常为7或8）
	int nStopBits;								//停止位
	int nParity;									//可以被设置为自解释常量
	BOOL bDTRShake;						//是否禁用DTR线
	BOOL bRTSShake;						//是否禁用RTS线
}Port_Parameters;
/***************************声明回调函数原型*****************************************
函数名称: CALLBACK *ERROR_CONTENT
描    述: 串口错误内容回调函数
输入参数: 错误的具体信息
输出参数: 
返    回: 
********************************************************************************************/
typedef int(CALLBACK *ERROR_CONTENT)(LPCTSTR);
/***************************声明回调函数原型*****************************************
函数名称:CALLBACK *READ_DATA
描    述: 从串口读取数据回调函数
输入参数: UserParam用户参数
buf			指向存储数据的空间
buflen		接受实际读取的字节长度
输出参数: 
返    回: 
********************************************************************************************/
typedef void (CALLBACK *READ_DATA)(BYTE *cBuffer, DWORD dwLen);

class CSerialCom
{
public:
	CSerialCom(ERROR_CONTENT FunctionnameError,READ_DATA FunctionnameRead);
	~CSerialCom();

	// 打开串口
	BOOL OpenPort(Port_Parameters port_param);

	// 串口是否打开
	BOOL IsPortOpen();

	// 关闭串口
	BOOL ClosePort(void);

	// 发送数据
	BOOL SendData(BYTE *sendbuf, DWORD length);

private:
	CSerialCom();
	CSerialCom(CSerialCom &other);
	CSerialCom operator = (CSerialCom &other);

	DCB m_dcb;								/* 串口参数结构体 */
	HANDLE m_hRecvThread;						/* 接收线程句柄 */
	HANDLE m_ExitThreadEvent;				/* 串口接收线程退出事件 */
	HANDLE m_hComm;							/* 串口操作句柄 */
	ERROR_CONTENT m_ErrorFoc;			//保存返回错误的回调函数名
	READ_DATA m_ReadFoc;				//保存返回读出数据的回调函数名

	// 串口接收线程
	static DWORD CommRecvTread(LPVOID lparam);
};


#endif