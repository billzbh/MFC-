// DeviceCom.cpp: implementation of the DeviceCom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "iMateUpdateFirmware.h"
#include "DeviceCom.h"
#include <stdio.h>

static HANDLE g_hComHand = INVALID_HANDLE_VALUE;

HANDLE iGetComHandle(void)
{
	return g_hComHand;
}


// =============串口  API=================
// 打开串口
int ComOpen(int Comport,long iBaudRate,unsigned int time_set)
{
	//处理大于8的端口号
	char buff[16]={0};
	sprintf(buff,"COM%d",Comport);
	if(Comport>8)
	{
		sprintf(buff,"\\\\.\\COM%d",Comport);
	}
	
	g_hComHand = CreateFile(buff,	// 端口号
				GENERIC_READ|GENERIC_WRITE, // 允许读和写
				0, // 独占方式
				NULL,
				OPEN_EXISTING, // 打开而不是创建
				0, // 0同步方式 ; FILE_FLAG_OVERLAPPED 异步方式
				NULL);

	if(g_hComHand == INVALID_HANDLE_VALUE)
	{
		return -1;  // 打开失败，返回-1
	}
	
	SetupComm(g_hComHand,19620,3000); //设置接收和发射缓冲区
	
	COMMTIMEOUTS TimeOuts;       //超时参数设置
	//设定读超时
	TimeOuts.ReadIntervalTimeout		= 6;
	TimeOuts.ReadTotalTimeoutMultiplier	= 6;
	TimeOuts.ReadTotalTimeoutConstant	= 100; // time_set
	
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 100;
	TimeOuts.WriteTotalTimeoutConstant   = 500;
	SetCommTimeouts(g_hComHand,&TimeOuts); //设置超时 ms
	
	//通讯参数设置
	DCB dcb;
	GetCommState(g_hComHand,&dcb);
	dcb.BaudRate = iBaudRate;	//波特率为9600
	dcb.ByteSize = 8;			//每个字节有8位
	dcb.Parity   = NOPARITY;	//无奇偶校验位
	dcb.StopBits = 0;			//两个停止位
	dcb.EvtChar  = (char)0xC0;	//收到0xC0触发事件
	SetCommState(g_hComHand,&dcb);//设置通讯参数

 	SetCommMask(g_hComHand,EV_RXCHAR); // 设置串口等待事件
	
	//终止目前正在进行
	PurgeComm(g_hComHand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	
	return 0;
}

// 判断串口是否打开
BOOL IsOpen()  
{ 
	if (g_hComHand == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else{
		return true;
	}
}

// 关闭串口
int ComClose()
{
	if ( IsOpen() )
	{
		PurgeComm(g_hComHand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		BOOL bSuccess = CloseHandle(g_hComHand);
		if(bSuccess)
		{ //关闭成功
			g_hComHand = INVALID_HANDLE_VALUE;
			return 0; 
		}
		else
			return -1;
	}
	else{
		g_hComHand = INVALID_HANDLE_VALUE;
		return 0;	// COM口本来就没有打开
	}
}

// 读取一个字节
int ReadByte(unsigned char * data_buff,long timeout)
{
	unsigned long fact_RDdata_count = 0;
	BOOL bReadStat;

	if( IsOpen() )
	{
		COMMTIMEOUTS TimeOuts;       //超时参数设置
		//设定读超时
		TimeOuts.ReadIntervalTimeout		= 6;
		TimeOuts.ReadTotalTimeoutMultiplier	= 6; //总超时时间 = data_count *  ReadTotalTimeoutMultiplier + ReadTotalTimeoutConstant
		TimeOuts.ReadTotalTimeoutConstant	=  timeout; // time_set
		SetCommTimeouts(g_hComHand,&TimeOuts); //设置超时 ms

		
		bReadStat = ReadFile(g_hComHand, data_buff, 1, &fact_RDdata_count, NULL);
		if (!bReadStat || (fact_RDdata_count != 1))
		{
			return -1;	
		}
		return 0;
	}
	else 
	{//串口未打开
		return -2;
	}
}

// 读串口缓冲区
int ReadCom(unsigned char * data_buff,unsigned int data_count,long timeout)
{
	unsigned long fact_RDdata_count = 0;
	BOOL bReadStat;
	
	if( IsOpen() )
	{

		COMMTIMEOUTS TimeOuts;       //超时参数设置
		//设定读超时
		TimeOuts.ReadIntervalTimeout		= 6;
		TimeOuts.ReadTotalTimeoutMultiplier	= 6; 

		int Alltimeout = 20;
		if((timeout - (int)(data_count) * 6 +10) > 0)
			Alltimeout = timeout - data_count * 6 +10; //总超时时间 = data_count *  ReadTotalTimeoutMultiplier + ReadTotalTimeoutConstant
		TimeOuts.ReadTotalTimeoutConstant	= Alltimeout; // time_set
		SetCommTimeouts(g_hComHand,&TimeOuts); //设置超时 ms


		bReadStat = ReadFile(g_hComHand, &data_buff[0], data_count, &fact_RDdata_count, NULL);
		if (!bReadStat || (fact_RDdata_count != data_count))
		{
			PurgeComm(g_hComHand, PURGE_TXABORT | PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
			return -1;	
		}

	    PurgeComm(g_hComHand, PURGE_TXABORT | PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
		return 0;
	}
	else 
	{//串口未打开
		return -2;
	}
}


// 写串口缓冲区
int WriteCom(unsigned char * data_buff,unsigned int data_count)
{
	BOOL bWriteStat;
	unsigned long fact_WRdata_count = 0;
	
	if( IsOpen() )
	{	
		//清空收发缓冲区，停止后台读、写操作。
		//PurgeComm(hComHand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		PurgeComm(g_hComHand, PURGE_TXABORT | PURGE_TXCLEAR);
		bWriteStat = WriteFile(g_hComHand, data_buff,data_count,&fact_WRdata_count, NULL);
		if (!bWriteStat && ((unsigned int)fact_WRdata_count != data_count))
		{	
			return -1;	
		}
			
		return 0;//成功
	}
	else
		return -2;//串口未打开
}


