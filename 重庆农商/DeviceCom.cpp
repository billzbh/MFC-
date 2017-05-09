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


// =============����  API=================
// �򿪴���
int ComOpen(int Comport,long iBaudRate,unsigned int time_set)
{
	//�������8�Ķ˿ں�
	char buff[16]={0};
	sprintf(buff,"COM%d",Comport);
	if(Comport>8)
	{
		sprintf(buff,"\\\\.\\COM%d",Comport);
	}
	
	g_hComHand = CreateFile(buff,	// �˿ں�
				GENERIC_READ|GENERIC_WRITE, // �������д
				0, // ��ռ��ʽ
				NULL,
				OPEN_EXISTING, // �򿪶����Ǵ���
				0, // 0ͬ����ʽ ; FILE_FLAG_OVERLAPPED �첽��ʽ
				NULL);

	if(g_hComHand == INVALID_HANDLE_VALUE)
	{
		return -1;  // ��ʧ�ܣ�����-1
	}
	
	SetupComm(g_hComHand,19620,3000); //���ý��պͷ��仺����
	
	COMMTIMEOUTS TimeOuts;       //��ʱ��������
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout		= 6;
	TimeOuts.ReadTotalTimeoutMultiplier	= 6;
	TimeOuts.ReadTotalTimeoutConstant	= 100; // time_set
	
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 100;
	TimeOuts.WriteTotalTimeoutConstant   = 500;
	SetCommTimeouts(g_hComHand,&TimeOuts); //���ó�ʱ ms
	
	//ͨѶ��������
	DCB dcb;
	GetCommState(g_hComHand,&dcb);
	dcb.BaudRate = iBaudRate;	//������Ϊ9600
	dcb.ByteSize = 8;			//ÿ���ֽ���8λ
	dcb.Parity   = NOPARITY;	//����żУ��λ
	dcb.StopBits = 0;			//����ֹͣλ
	dcb.EvtChar  = (char)0xC0;	//�յ�0xC0�����¼�
	SetCommState(g_hComHand,&dcb);//����ͨѶ����

 	SetCommMask(g_hComHand,EV_RXCHAR); // ���ô��ڵȴ��¼�
	
	//��ֹĿǰ���ڽ���
	PurgeComm(g_hComHand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	
	return 0;
}

// �жϴ����Ƿ��
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

// �رմ���
int ComClose()
{
	if ( IsOpen() )
	{
		PurgeComm(g_hComHand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		BOOL bSuccess = CloseHandle(g_hComHand);
		if(bSuccess)
		{ //�رճɹ�
			g_hComHand = INVALID_HANDLE_VALUE;
			return 0; 
		}
		else
			return -1;
	}
	else{
		g_hComHand = INVALID_HANDLE_VALUE;
		return 0;	// COM�ڱ�����û�д�
	}
}

// ��ȡһ���ֽ�
int ReadByte(unsigned char * data_buff,long timeout)
{
	unsigned long fact_RDdata_count = 0;
	BOOL bReadStat;

	if( IsOpen() )
	{
		COMMTIMEOUTS TimeOuts;       //��ʱ��������
		//�趨����ʱ
		TimeOuts.ReadIntervalTimeout		= 6;
		TimeOuts.ReadTotalTimeoutMultiplier	= 6; //�ܳ�ʱʱ�� = data_count *  ReadTotalTimeoutMultiplier + ReadTotalTimeoutConstant
		TimeOuts.ReadTotalTimeoutConstant	=  timeout; // time_set
		SetCommTimeouts(g_hComHand,&TimeOuts); //���ó�ʱ ms

		
		bReadStat = ReadFile(g_hComHand, data_buff, 1, &fact_RDdata_count, NULL);
		if (!bReadStat || (fact_RDdata_count != 1))
		{
			return -1;	
		}
		return 0;
	}
	else 
	{//����δ��
		return -2;
	}
}

// �����ڻ�����
int ReadCom(unsigned char * data_buff,unsigned int data_count,long timeout)
{
	unsigned long fact_RDdata_count = 0;
	BOOL bReadStat;
	
	if( IsOpen() )
	{

		COMMTIMEOUTS TimeOuts;       //��ʱ��������
		//�趨����ʱ
		TimeOuts.ReadIntervalTimeout		= 6;
		TimeOuts.ReadTotalTimeoutMultiplier	= 6; 

		int Alltimeout = 20;
		if((timeout - (int)(data_count) * 6 +10) > 0)
			Alltimeout = timeout - data_count * 6 +10; //�ܳ�ʱʱ�� = data_count *  ReadTotalTimeoutMultiplier + ReadTotalTimeoutConstant
		TimeOuts.ReadTotalTimeoutConstant	= Alltimeout; // time_set
		SetCommTimeouts(g_hComHand,&TimeOuts); //���ó�ʱ ms


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
	{//����δ��
		return -2;
	}
}


// д���ڻ�����
int WriteCom(unsigned char * data_buff,unsigned int data_count)
{
	BOOL bWriteStat;
	unsigned long fact_WRdata_count = 0;
	
	if( IsOpen() )
	{	
		//����շ���������ֹͣ��̨����д������
		//PurgeComm(hComHand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		PurgeComm(g_hComHand, PURGE_TXABORT | PURGE_TXCLEAR);
		bWriteStat = WriteFile(g_hComHand, data_buff,data_count,&fact_WRdata_count, NULL);
		if (!bWriteStat && ((unsigned int)fact_WRdata_count != data_count))
		{	
			return -1;	
		}
			
		return 0;//�ɹ�
	}
	else
		return -2;//����δ��
}


