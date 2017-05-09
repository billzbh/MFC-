#ifndef _HXIMATEWIN32_H
#define _HXIMATEWIN32_H

#ifndef HXIMATESDK_API
#ifdef HXIMATESDK_EXPORTS
#define HXIMATESDK_API __declspec(dllexport)
#else
#define HXIMATESDK_API __declspec(dllimport)
#endif
#endif

//����iMate������
//in     : mode			: 	"bluetooth" ��������   
//							"usb-hid"   USB-HID��������  
//ret    : 0  			:	�ɹ�
//         ����			:	ʧ��
HXIMATESDK_API int  HXiMate_DeviceConnect(char *mode);

//�ر���iMate������
HXIMATESDK_API void HXiMate_DeviceClose();

//ȡ������
HXIMATESDK_API void HXiMate_DeviceCancel();

// ��ѯiMate�̼��汾��
// out   :  version		:  �̼��汾��
//          ��ʽ		:  "A.A,B.B.B" : Ӳ���͹̼��汾������AΪӲ���汾��BΪ�̼��汾
// ret   :  >0			:  �ɹ�,version���ݳ���
//          ����		:  ʧ��
HXIMATESDK_API int HXiMate_DeviceVersions(char *version);

// iMate��Ʒ���к�
// out   :   serialNo    :  ��Ʒ���к�
// ret   :  >0			 :	�ɹ�,serialNo���ݳ���
//          ����		 :	ʧ��
HXIMATESDK_API int HXiMate_DeviceSerialNumber(char *serialNo);

//д�ն˺�(�������12���ַ�)
//in    :  termId �ն˺��ֽ�����
//         length �ն˺ų��ȣ����12���ֽ�
//ret   :  >0     �ɹ�
//         -1 ����ʱ   -2 ��ͨѶ����  -3�����ȴ���   
HXIMATESDK_API int HXiMate_WriteTermId(unsigned char* termId, int length);

//��ȡ�ն˺�(�������12���ַ�)
//out    :  termId �ն˺��ֽ�����
//ret   :  >0     �ɹ�
//         -1 ����ʱ   -2 ��ͨѶ����  -3�����ȴ���   
HXIMATESDK_API int HXiMate_ReadTermId(unsigned char* termId);


// ȡiMate��ص���
// ret	:	>0		:	��ص���ֵ
//			<=0		:	��ȡʧ��
HXIMATESDK_API int HXiMate_DeviceBatteryLevel();

// �ȴ��¼��������ſ�ˢ����Pboc IC���롢������Ƶ��
// in	:	eventMask	:eventMask��bit����ʶ�ȴ��¼�
//						  0x01    �ȴ�ˢ���¼�
//						  0x02    �ȴ��忨�¼�
//					      0x04    �ȴ���Ƶ�¼�
//					      0xFF    �ȴ������¼�
//		:	timeOut		:	��ȴ�ʱ�䣨�룩
// out	:	outData		:	�¼��������
// ret	:	>0			:	outData���ݳ���
//			<=0			:	����ʧ��
HXIMATESDK_API int HXiMate_WaitEvent(unsigned char eventMask, unsigned char* outData, int timeOut);

// ������������Ƿ�����������TRUE��ʾ��������
// return   :   true    :��������
//              false   :δ����
HXIMATESDK_API bool HXiMate_DeviceIsConnecting();

// ���iMate�Ƿ��ڹ���״̬������TRUE��ʾ���ڹ����С�
// return   :   true    :���ڹ�����
//              false   :�豸����
HXIMATESDK_API bool HXiMate_DeviceIsWorking();

// ��ȡ��չ�ڴ�Ĳ�������
// in     :   offset     :   ����չ�ڴ�ƫ����
//            length     :   ����չ�ڴ��ֽ���
// out    :   outData    :   ��չ�ڴ�����
// return :   0          :   ��ȡ�ɹ�
//            ����       :   ��ȡʧ��
HXIMATESDK_API int  HXiMate_DeviceXmemRead(int offset, int length, unsigned char *outData);

// д��չ�ڴ�Ĳ�������
// in     :   offset     :   д������ƫ����
//			  inLength   :   д�����ݳ���
//            inData     :   д������
// return :   0          :   д��ɹ�
//            ����       :   д��ʧ��
HXIMATESDK_API int  HXiMate_DeviceXmemWrite(int offset, int inLength, unsigned char *indata);

// iMate������һ��
HXIMATESDK_API void HXiMate_DeviceBuzzer();

//����ʧ��ʱ,������ȡiMate������Ϣ
// out   : error		:������Ϣ
HXIMATESDK_API void HXiMate_GetError(wchar_t *error);

// ˢ������
// in	:	timeOut		:��ȴ�ʱ�䣨�룩
// out	:	track2		:���ŵ����� >= 37 +1
//			track3		:���ŵ����� >=104 +1
// ret  :	0			:��ȡ�ɹ�
//          ����		:��ȡʧ��
HXIMATESDK_API int HXiMate_SwipeCard(wchar_t* track2, wchar_t* track3, int timeOut);

//�����֤
// in	:	timeOut		:��ȴ�ʱ�䣨�룩
// out	:	info		:���֤��Ϣ >=128 + 1
//			photoData	:��Ƭ���� >= 1024
// ret    : 0           :��ȡ�ɹ�
//          ����        :��ȡʧ��
HXIMATESDK_API int HXiMate_ReadIdCard(wchar_t* info, unsigned char* photoData, int timeOut);

//�����֤
// in	:	timeOut		:��ȴ�ʱ�䣨�룩
// out	:	info		:���֤��Ϣ:����,�Ա�,����,����(��,��,��),סַ,���֤����,��������,��Ч��,������Ϣ;������Ϣ��","�ָ�
//			photoData	:��Ƭ���� >= 1024
// ret    : 0           :��ȡ�ɹ�
//          ����        :��ȡʧ��
HXIMATESDK_API int HXiMate_ReadIdCard2(wchar_t* info, unsigned char* photoData, int timeOut);


//oneTwo ������ hexString��ʽ���ַ�����
// in	:	Data		:Դ����
//			DataLength  :���ݳ���
// out	:   outData	    :Ŀ������
// ret    : = 0         :ת���ɹ�,��ʾĿ�����ݵĳ���
//          ����        :ת��ʧ��
HXIMATESDK_API int HXiMate_OneTwo0(unsigned char* Data,int DataLength, unsigned char* outData);


// hexString��ʽ���ַ���תΪ ѹ����BCD��
// in	:	Data		:Դ����
//			DataLength  :���ݳ���
// out	:   outData	    :Ŀ������
// ret    : = 0         :ת���ɹ�,��ʾĿ�����ݵĳ���
//          ����        :ת��ʧ��
HXIMATESDK_API int HXiMate_TwoOne(unsigned char* Data, int DataLength, unsigned char* outData);


//����ͨѶ�ӿ�
HXIMATESDK_API int HXiMate_SendReceive(unsigned char *inData, int inLength, unsigned char *outData, int *outLength, int timeOut); //timeOut����


HXIMATESDK_API char* HXiMate_WcharToChar(const wchar_t* wp);


HXIMATESDK_API wchar_t* HXiMate_CharToWchar(const char* c);

#endif


