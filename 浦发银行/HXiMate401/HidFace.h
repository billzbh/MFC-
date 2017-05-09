#ifndef HIDFACE_H
#define HIDFACE_H

#define VENDOR_ID		0x0483
#define PRODUCT_ID		0x5750


#define IMATE_PORT_ERROR	-1		//端口打开失败
#define IMATE_PORT_OPENED	-2		//端口已经打开
#define IMATE_PORT_NOTOPEN	-3		//端口未打开
#define IMATE_COMM_ERROR	-4		//通讯失败，包括以下三种情况：1数据发送失败；2数据接收失败；3数据接收超时。
#define IMATE_TIMEOUT		-5		//操作等待超时
#define IMATE_OTHER_ERROR	-9		//其它错误

#define IMATE_FLASH_WRITE_ERROR		-18		//Flash编程错误
#define IMATE_HEX_FORMAT_ERROR		-126	//Hex文件格式错误


extern int iGetHidHandle(void);
extern unsigned char ucHidIfOpened(void);
extern unsigned char ucHidOpen(unsigned short vendor_id, unsigned short product_id);
extern unsigned char ucHidClose(void);
extern unsigned char ucHidReset(void);
extern unsigned char ucHidTest(void);
extern unsigned char ucHidGet(void);
extern unsigned char ucHidSendBuf(void *pBuf, unsigned int uiLen);
extern unsigned char ucHidGetBuf(void *pBuf, unsigned int uiLen, unsigned int uiTimeOut);
extern unsigned char ucHidGetBufBymillsecond(void *pBuf, unsigned int uiLen, unsigned int uiTimeOut);
extern unsigned char ucHidTestOnLine(void);

#endif