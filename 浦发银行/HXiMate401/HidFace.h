#ifndef HIDFACE_H
#define HIDFACE_H

#define VENDOR_ID		0x0483
#define PRODUCT_ID		0x5750


#define IMATE_PORT_ERROR	-1		//�˿ڴ�ʧ��
#define IMATE_PORT_OPENED	-2		//�˿��Ѿ���
#define IMATE_PORT_NOTOPEN	-3		//�˿�δ��
#define IMATE_COMM_ERROR	-4		//ͨѶʧ�ܣ������������������1���ݷ���ʧ�ܣ�2���ݽ���ʧ�ܣ�3���ݽ��ճ�ʱ��
#define IMATE_TIMEOUT		-5		//�����ȴ���ʱ
#define IMATE_OTHER_ERROR	-9		//��������

#define IMATE_FLASH_WRITE_ERROR		-18		//Flash��̴���
#define IMATE_HEX_FORMAT_ERROR		-126	//Hex�ļ���ʽ����


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