#ifndef _HXIMATEMEMORYCARDWIN32_H
#define _HXIMATEMEMORYCARDWIN32_H

#ifndef HXIMATESDK_API
#ifdef HXIMATESDK_EXPORTS
#define HXIMATESDK_API __declspec(dllexport)
#else
#define HXIMATESDK_API __declspec(dllimport)
#endif
#endif

#define	CARD_VOLTAGE_1V8		0
#define	CARD_VOLTAGE_3V0		1
#define	CARD_VOLTAGE_5V0		2

#define SLE4442_TYPE            0x01
#define AT102_TYPE              0x02
#define AT1608_TYPE             0x04
#define AT1604_TYPE             0x05
#define SLE4428_TYPE            0x06
#define AT24Cxx_TYPE            0x10


// ��⿨Ƭ�Ƿ����
// ret : 	0  : ������
// 			1  : ����
//          99 : ��֧�ָù���
HXIMATESDK_API int HXiMate_MemoryTestCard();

// ���洢������(4442\102\1604\1608\4428\24Cxx)
// out   : psResetData ��λ���� < 100bytes, ��һ���ֽ�Ϊ����
// ret   : 0xff		��ʶ��Ŀ�
//		   ����
//					SLE4442_TYPE		0x01
//					AT102_TYPE			0x02
//					AT1608_TYPE			0x04
//					AT1604_TYPE			0x05
//					SLE4428_TYPE		0x06
//					AT24Cxx_TYPE		0x10
HXIMATESDK_API int HXiMate_MemoryTestCardType(unsigned char *psResetData);

// ���ÿ�Ƭ��ѹ
//in     :    voltageTag  
//            CARD_VOLTAGE_1V8		0    
//            CARD_VOLTAGE_3V0		1
//            CARD_VOLTAGE_5V0		2   
HXIMATESDK_API void HXiMate_MemorySetCardVoltage(int voltageTag);

// ����ͨѶ��Կ
// in	:	masterKeyId	: ����ԿID, һ��ʹ��6
//			random		: �������8�ֽ�
// ret  :	0	:	�ɹ�
//			���� ��	ʧ��	
HXIMATESDK_API int HXiMate_MemoryGenCommKey(unsigned int masterKeyId, unsigned char* random);


//SLE4442 �ӿ�

// ��SLE4442��
HXIMATESDK_API void HXiMate_MemorySLE4442_Open(void);

// ��SLE4442��, �Զ�����ѹ�Ϳ�����
// ret    : 	0	 : 	�ɹ�
//			  ����	 :	ʧ��
HXIMATESDK_API int HXiMate_MemorySLE4442_AutoOpen(void);

// �ر�SLE4442��
HXIMATESDK_API void HXiMate_MemorySLE4442_Close(void);

// ��֤SLE4442������
// in      :  securityCode   :   �������ݻ��壬3�ֽڳ���
//      
// return  :  0    :   �ɹ�
//            ���� :   ʧ��   
HXIMATESDK_API int HXiMate_MemorySLE4442_ChkCode(unsigned char *securityCode);

// ��֤SLE4442������(��ȫ��ʽ��
// in	:	securityCode  :   ���ܺ���������ݣ�8�ֽڳ���  
// ret  :	0    :   �ɹ�
//			���� :   ʧ��    
HXIMATESDK_API int HXiMate_MemorySLE4442_ChkCodeEx(unsigned char* securityCode);

// SLE4442������
// in  :
//		offset	:	����ƫ������0~255
//		dataLen	:	���ݳ���
// out:
//		dataBuff:	�����ݻ�������ַ, maxlength=255
HXIMATESDK_API void HXiMate_MemorySLE4442_Read(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// SLE4442д����
// in  :
//		offset	:	����ƫ������0~255
//		dataLen	:	���ݳ���
//		dataBuff:	д���ݻ�������ַ, maxlength=255
HXIMATESDK_API void HXiMate_MemorySLE4442_Write(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

//SLE4428�ӿ�

// ��SLE4428��
HXIMATESDK_API void HXiMate_MemorySLE4428_Open(void);

// ��SLE4428��, �Զ�����ѹ�Ϳ�����
// ret    : 	0	 : 	�ɹ�
//			  ����	 :	ʧ��
HXIMATESDK_API int HXiMate_MemorySLE4428_AutoOpen(void);

// �ر�SLE4428��
HXIMATESDK_API void HXiMate_MemorySLE4428_Close(void);

// ��֤SLE4428������
// in��
//      securityCode  :   �������ݻ��壬2�ֽڳ���
// return��
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemorySLE4428_ChkCode(unsigned char *securityCode);

// ��֤SLE4428�����루��ȫ��ʽ��
// in  :     securityCode  :   ���ܺ���������ݣ�8�ֽڳ���
// ret  :     0   :   �ɹ�
//			���� :   ʧ��
HXIMATESDK_API int HXiMate_MemorySLE4428_ChkCodeEx(unsigned char *securityCode);

// SLE4428������
// in��
//		offset	:	����ƫ������0~1024
//		dataLen	:	���ݳ���
// return��
//		dataBuff:	�����ݻ�������ַ, maxlength = 1024
HXIMATESDK_API void HXiMate_MemorySLE4428_Read(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// SLE4428д����
// in ��
//		offset	:	����ƫ������0~1024
//		dataLen	:	���ݳ���
//		dataBuff:	д���ݻ�������ַ, maxlength = 1024
HXIMATESDK_API void HXiMate_MemorySLE4428_Write(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);


// ��AT102��
HXIMATESDK_API void HXiMate_MemoryAT102_Open(void);

// ��AT102��, �Զ�����ѹ�Ϳ�����
// ret    : 	0	 : 	�ɹ�
//			  ����	 :	ʧ��
HXIMATESDK_API int HXiMate_MemoryAT102_AutoOpen(void);

// �ر�AT102��
HXIMATESDK_API void HXiMate_MemoryAT102_Close(void);

// ��֤AT102������
// ���������
//      securityCode  :   �������ݻ��壬2�ֽڳ���
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT102_ChkCode(unsigned char *securityCode);

// ��֤AT102�����루��ȫ��ʽ��
//  in  :    securityCode  :   ���ܺ���������ݣ�8�ֽڳ���
//  ret :    0    :   �ɹ�
//			  ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT102_ChkCodeEx(unsigned char *securityCode);

// AT102������(��˫�ֽڶ�ȡ��
// ���������
//		wordOffset :	����ƫ������<= 128
//		wordNum	:	���ݳ��ȣ��ֳ��ȣ�, <= 128
// ���������
//		dataBuff:	�����ݻ�������ַ, maxlength = 256
HXIMATESDK_API void HXiMate_MemoryAT102_ReadWords(unsigned int wordOffset, unsigned int wordNum, unsigned char* dataBuff);

// AT102д����(��˫�ֽ�д����
// ���������
//		wordOffset 	:	����ƫ����, <= 128
//		wordNum		:	���ݳ��ȣ��ֳ��ȣ�<= 128
//		dataBuff:	д���ݻ�������ַ, maxlength = 256
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT102_WriteWords(unsigned int wordOffset, unsigned int wordNum, unsigned char* dataBuff);

// AT102������Ӧ����������
// ���������
//		wordOffset 	:	����ƫ����
//		wordNum		:	ɾ���ֳ���
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT102_EraseNonApp(unsigned char wordOffset, unsigned char wordNum);

// AT102����Ӧ����������
// ���������
//		area  	: Ӧ����	1:	Ӧ����1	2:	Ӧ����2
//		limited : ָ����2Ӧ�����Ĳ��������Ƿ������ơ��˲������ҽ���area= 2ʱ�����á�
//		eraseKey: ������Կ, Ӧ����1�Ĳ������볤��Ϊ6bytes��Ӧ����2�Ĳ�������Ϊ4bytes��
//			  Security Level 1 ��ʽ�£�eraseKey=NULL����Ҫ�ṩEraseKey��ʽ���в���
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT102_EraseApp(unsigned int area, unsigned char limited, unsigned char *eraseKey);

// AT102����Ӧ���������ݣ���ȫ��ʽ��
// in  :
//		area  	: Ӧ����	1:	Ӧ����1	2:	Ӧ����2
//		limited : ָ����2Ӧ�����Ĳ��������Ƿ������ơ��˲������ҽ���area= 2ʱ�����á�
//		eraseKey: ������Կ, ���ܺ��Ӧ����1�Ĳ������룬8�ֽڣ�
//			  Security Level 1 ��ʽ�£�eraseKey=NULL����Ҫ�ṩEraseKey��ʽ���в���
// ret  :
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT102_EraseAppEx(unsigned int area, unsigned char limited, unsigned char *eraseKey);

// AT102��Ӧ��������
// ���������
//		area  	: Ӧ����	1:	Ӧ����1	2:	Ӧ����2
// ���������
//		dataBuff:	�����ݻ�������ַ, maxlength = 64
HXIMATESDK_API void HXiMate_MemoryAT102_ReadAZ(unsigned int area, unsigned char* dataBuff);

// AT102дӦ��������
// ���������
//		area  	: Ӧ����	1:	Ӧ����1	2:	Ӧ����2
//		dataBuff:	�����ݻ�������ַ, maxlength = 64
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT102_WriteAZ(unsigned int area, unsigned char* dataBuff);


// AT102������������
// ���������
//		dataBuff:	�����ݻ�������ַ, ��ȡ2�ֽ�����
HXIMATESDK_API void HXiMate_MemoryAT102_ReadMTZ(unsigned char* dataBuff);

// AT102д����������
// ���������
//		dataBuff:	�����ݻ�������ַ, 2�ֽ�����
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT102_UpdateMTZ(unsigned char* dataBuff);


// ��AT1604��
HXIMATESDK_API void HXiMate_MemoryAT1604_Open(void);

// ��AT1604��, �Զ�����ѹ�Ϳ�����
// ret    : 	0	 : 	�ɹ�
//			  ����	 :	ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1604_AutoOpen(void);

// �ر�AT1604��
HXIMATESDK_API void HXiMate_MemoryAT1604_Close(void);

// ��֤AT1604��������
// ���������
//      securityCode  :   �������ݻ��壬2�ֽڳ���
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1604_ChkCode(unsigned char *securityCode);

// ��֤AT1604�������루��ȫ��ʽ��
// in  :
//      securityCode  :   ���ܺ���������ݣ�8�ֽڳ���
// ret  :
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1604_ChkCodeEx(unsigned char *securityCode);

// ��֤AT1604����������
// ���������
//		area		  :   ������1��2��3��4
//      securityCode  :   �������ݻ��壬2�ֽڳ���
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1604_ChkAreaCode(unsigned int area, unsigned char *securityCode);

// ��֤AT1604���������루��ȫ��ʽ��
// in  :
//		area		  :   ������1��2��3��4
//      securityCode  :   ���ܺ���������ݣ�8�ֽڳ���
// ret  :
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1604_ChkAreaCodeEx(unsigned int area, unsigned char *securityCode);

// AT1604������
// ���������
//		offset	:	����ƫ������0~2047
//		dataLen	:	���ݳ���
// ���������
//		dataBuff:	�����ݻ�������ַ, maxlength = 2048
HXIMATESDK_API void HXiMate_MemoryAT1604_Read(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT1604д����
// ���������
//		offset	:	����ƫ������0~2047
//		dataLen	:	���ݳ���
//		dataBuff:	д���ݻ�������ַ, maxlength = 2048
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1604_Write(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT1604����������
// ���������
//		area	:   ������1��2��3��4
//		offset	:	����ƫ������0~511
//		dataLen	:	���ݳ���,1��2��3��512bytes 4��457bytes
// ���������
//		dataBuff:	�����ݻ�������ַ, maxlength = 512
HXIMATESDK_API void HXiMate_MemoryAT1604_ReadAZ(unsigned int area, unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT1604д��������
// ���������
//		area	:   ������1��2��3��4
//		offset	:	����ƫ������0~511
//		dataLen	:	���ݳ���,1��2��3��512bytes 4��457bytes
//		dataBuff:	д���ݻ�������ַ, maxlength = 512
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1604_WriteAZ(unsigned int area, unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT1604��������
// ���������
//		offset	:	����ƫ������0~2047
//		dataLen	:	���ݳ���
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1604_Erase(unsigned int offset, unsigned int dataLen);

// AT1604������������
// ���������
//		area	:   ������1��2��3��4
//		offset	:	����ƫ������0~511
//		dataLen	:	���ݳ���,1��2��3��512bytes 4��457bytes
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1604_EraseAZ(unsigned int area, unsigned int offset, unsigned int dataLen);

// ��֤AT1604��������������
// in  :
//		area		  :   ������1��2��3��4
//      securityCode  :   �������ݻ��壬2�ֽڳ���
// ret  :
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1604_ChkAreaEraseCode(unsigned int area, unsigned char* securityCode);

// ��֤AT1604�������������루��ȫ��ʽ��
// in  :
//		area		  :   ������1��2��3��4
//      securityCode  :   ���ܺ���������ݣ�8�ֽڳ���
// ret  :
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1604_ChkAreaEraseCodeEx(unsigned int area, unsigned char* securityCode);

// AT1604������������
// ���������
//		dataBuff:	�����ݻ�������ַ, length = 2
HXIMATESDK_API void HXiMate_MemoryAT1604_ReadMTZ(unsigned char* dataBuff);

// AT1604д����������
// ���������
//		dataBuff:	д���ݻ�������ַ, length = 2
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1604_WriteMTZ(unsigned char* dataBuff);


// ��AT1608��
// ���������
//		resetData:	��λ����, length = 4bytes
HXIMATESDK_API void HXiMate_MemoryAT1608_Open(unsigned char *resetData);

// ��AT1608��, �Զ�����ѹ�Ϳ�����
// out    :     resetData  :��λ����
// ret    : 	0	       : 	�ɹ�
//			  ����	       :	ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1608_AutoOpen(unsigned char *resetData);

// �ر�AT1608��
HXIMATESDK_API void HXiMate_MemoryAT1608_Close(void);

// AT1608������Ӧ����
// ���������
//      az	: Ӧ������0~7
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1608_SetAZ(unsigned char az);

// ��֤AT1608����д����
// ���������
// 		index		: ���������� д����������(0--7) ������������(0x80--0x87)
//      securityCode: �������ݻ��壬3�ֽڳ���
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1608_ChkCode(unsigned int index, unsigned char *securityCode);

// ��֤AT1608����д���루��ȫ��ʽ��
// in  :
// 		index		: ���������� д����������(0--7) ������������(0x80--0x87)
//      securityCode: ���ܺ���������ݣ�8�ֽڳ���
// ret  :
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1608_ChkCodeEx(unsigned int index, unsigned char *securityCode);

// AT1608����ȫ��֤
// ���������
//      gc	: ��Կ��8�ֽڳ���
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1608_Auth(unsigned char *gc);

// AT1608����ȫ��֤����ȫ��ʽ��
// in  :
//      gc	: ��Կ��16�ֽڳ���
// ret  :
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1608_AuthEx(unsigned char *gc);

// AT1608������
// ���������
//		level	:   0:Ӧ������1��������
//		offset	:	������(0--127)	Ӧ����(0--255)
//		dataLen	:
// ���������
//		dataBuff:	�����ݻ�������ַ, maxlength = 256
HXIMATESDK_API int HXiMate_MemoryAT1608_Read(unsigned int level, unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT1608д����
// ���������
//		level	:   0:Ӧ������1��������
//		offset	:	������(0--127)	Ӧ����(0--255)
//		dataLen	:
//		dataBuff:	�����ݻ�������ַ, maxlength = 256
// �����룺
//      0    :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT1608_Write(unsigned int level, unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// ��AT88SC1608��˿״��
// ������   :	0    :  OK
//              1    :  Error
HXIMATESDK_API int	HXiMate_MemoryAT1608_ReadFuse(unsigned char *Fuse);


// ��AT24Cxx��
HXIMATESDK_API void HXiMate_MemoryAT24Cxx_Open(void);

// ��AT24Cxx��, �Զ���⿨����
HXIMATESDK_API int HXiMate_MemoryAT24Cxx_OpenAuto(void);

// �ر�AT24Cxx��
HXIMATESDK_API void HXiMate_MemoryAT24Cxx_Close(void);

// AT24Cxx������
// ���������
//		offset	:	����ƫ����
//		dataLen	:	���ݳ���
// ���������
//		dataBuff:	�����ݻ�������ַ, ���ݾ������ȷ��
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT24Cxx_Read(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT24Cxxд����
// ���������
//		offset	:	����ƫ����
//		dataLen	:	���ݳ���
//		dataBuff:	д���ݻ�������ַ, ���ݾ������ȷ��
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT24Cxx_Write(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT24C32������
// ���������
//		offset	:	����ƫ����
//		dataLen	:	���ݳ���
// ���������
//		dataBuff:	�����ݻ�������ַ, ���ݾ������ȷ��
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT24C32_Read(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT24C32д����
// ���������
//		offset	:	����ƫ����
//		dataLen	:	���ݳ���
//		dataBuff:	д���ݻ�������ַ, ���ݾ������ȷ��
// �����룺
//      0   :   �ɹ�
//      ���� :   ʧ��
HXIMATESDK_API int HXiMate_MemoryAT24C32_Write(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

#endif
