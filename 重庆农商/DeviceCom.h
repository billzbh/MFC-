// DeviceCom.h: interface for the DeviceCom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVICECOM_H__E9632234_92E8_4653_A90F_363E1A516DA6__INCLUDED_)
#define AFX_DEVICECOM_H__E9632234_92E8_4653_A90F_363E1A516DA6__INCLUDED_

HANDLE iGetComHandle(void);

int WriteCom(unsigned char * data_buff,unsigned int data_count);
int ReadCom(unsigned char * data_buff,unsigned int data_count,long timeout);
int ReadByte(unsigned char * data_buff,long timeout);
int ComClose();
BOOL IsOpen();
int ComOpen(int Comport,long iBaudRate,unsigned int time_set);


#endif // !defined(AFX_DEVICECOM_H__E9632234_92E8_4653_A90F_363E1A516DA6__INCLUDED_)
