#ifndef _HXIMATEPINPADWIN32_H
#define _HXIMATEPINPADWIN32_H

#ifndef HXIMATESDK_API
#ifdef HXIMATESDK_EXPORTS
#define HXIMATESDK_API __declspec(dllexport)
#else
#define HXIMATESDK_API __declspec(dllimport)
#endif
#endif

//�����������
#define PINPAD_MODEL_KMY                0   //������-KMY3512
#define PINPAD_MODEL_XYD                1   //���Ŵ�-P90
#define PINPAD_MODEL_SZB                2   //�������ж���-���ڿ���
#define PINPAD_MODEL_HX                 3   //����imate401

//�����㷨
#define  ALGO_ENCRYPT  1
#define  ALGO_DECRYPT  2

// ����֧�ֵ�Pinpad���ͣ�Ŀǰ֧��PINPAD_MODEL_KMY��PINPAD_MODEL_XYD��PINPAD_MODEL_SZB
// in  : model   :�����������
//				   PINPAD_MODEL_KMY
//			       PINPAD_MODEL_XYD
//				   PINPAD_MODEL_SZB
//				   PINPAD_MODEL_HX
HXIMATESDK_API void HXiMate_PinPad_SetModel(int model);

// Pinpadȡ������
HXIMATESDK_API void HXiMate_PinPad_Cancel();


//Pinpad�ϵ� (ͨѶ������Ϊ9600 У�鷽ʽ 0��
//ret :     0   :    �ɹ�
//         ���� :    ʧ�� (�ϵ�ʧ��ʱ,����HXiMate_PinPad_GetError��ȡ������Ϣ)
HXIMATESDK_API int HXiMate_PinPad_PowerOn();

//Pinpad�µ�
//ret :     0   :    �ɹ�
//         ���� :    ʧ�� (����ʧ��ʱ,����HXiMate_PinPad_GetError��ȡ������Ϣ)
HXIMATESDK_API int HXiMate_PinPad_PowerOff();

//��ȡPinpad�İ汾����Ϣ
//out :  version	 :   Pinpad�汾��Ϣ
//ret :  >0          :  version���ݳ���
//        ����		 :   ʧ�� (����ʧ��ʱ,����HXiMate_PinPad_GetError��ȡ������Ϣ)
HXIMATESDK_API int HXiMate_PinPad_Version(unsigned char *version);

//��ȡPinpad��ز���ʧ������µĴ�����Ϣ
//out  :   error   :������Ϣ
HXIMATESDK_API void HXiMate_PinPad_GetError(wchar_t *error);

//Pinpad��λ�Լ�
//in    :    initFlag    :  true���Pinpad�е���Կ��false�������Կ
//ret   :    0           :  �ɹ�
//			 ����		 :  ʧ�� (����ʧ��ʱ,����HXiMate_PinPad_GetError��ȡ������Ϣ)
HXIMATESDK_API int HXiMate_PinPad_Reset(bool initFlag);

//Pinpad��װ����Կ
//in    :  is3des            :  �Ƿ����3DES�㷨��false��ʾʹ��DES�㷨
//         index		     :	����Կ����
//         masterKey		 :  ����Կ
//         masterLength		 :  ����Կ����
//ret   :    0               :  �ɹ�
//         ����              :  ʧ�� (����ʧ��ʱ,����HXiMate_PinPad_GetError��ȡ������Ϣ)  
HXIMATESDK_API int HXiMate_PinPad_DownloadMasterkey(int algorithm, int index, unsigned char *masterKey, int masterLength);

//Pinpad��װ������Կ(����Կ���ܣ�
//in    :   is3des			 : �Ƿ����3DES�㷨��false��ʾʹ��DES�㷨
//			masterIndex		 : ����Կ����
//			workingIndex	 : ������Կ����	
//			workingKey		 : ������Կ
//			workingLength    : ������Կ����
//ret   :    0               :  �ɹ�
//         ����              :  ʧ�� (����ʧ��ʱ,����HXiMate_PinPad_GetError��ȡ������Ϣ)  
HXIMATESDK_API int HXiMate_PinPad_DownloadWorkingkey(int algorithm, int masterIndex, int workingIndex, unsigned char *workingKey, int workingLength);

//Pinpad�������루PinBlock��
//in    :	is3des			: �Ƿ����3DES�㷨��false��ʾʹ��DES�㷨
//			isAutoReturn	: ���뵽Լ������ʱ�Ƿ��Զ����أ�����Ҫ��Enter); true :�Զ����� false :��Ҫ��Enter
//		:	masterIndex		: ����Կ����
//			workingIndex	: ������Կ����
//			cardNo			: ����/�ʺţ�����12λ���֣�
//			pinLength		: ��Ҫ����PIN�ĳ���
//			timeout			: ��������ȴ���ʱʱ�� <= 255 ��
//out   :   pin				: ���ܺ��pinblock
//ret   :    >0             : pinblock����
//         ����             : ʧ�� (����ʧ��ʱ,����HXiMate_PinPad_GetError��ȡ������Ϣ)  
HXIMATESDK_API int HXiMate_PinPad_InputPinBlock(int algorithm, bool isAutoReturn, int masterIndex, int workingIndex, unsigned char *cardNo, unsigned char *pin, int pinLength, int timeout);

//Pinpad�ӽ�������
//in	:	is3des			: �Ƿ����3DES�㷨��false��ʾʹ��DES�㷨
//			algo			: �㷨,ȡֵ: ALGO_ENCRYPT, ALGO_DECRYPT, ��ECB��ʽ���мӽ�������
//			masterIndex		: ����Կ����
//			workingIndex	: ������Կ���������������Կ����ȡֵ-1��ʹ������Կ����ָ��������Կ���мӽ���
//			data			: �ӽ�������
//			datalength		: �ӽ������ݵĳ���,Ҫ��8�ı�����С�ڻ����248�ֽڳ���
//out	:	outData			: ���ܺ������
//ret	:    >0             : outData�������ݳ���
//         ����             : ʧ�� (����ʧ��ʱ,����HXiMate_PinPad_GetError��ȡ������Ϣ) 
HXIMATESDK_API int HXiMate_PinPad_Encrypt(int algorithm, int algo, int masterIndex, int workingIndex, unsigned char *data, unsigned char *outData, int datalength);

//Pinpad����MAC���㣨ANSIX9.9��
//in	:	is3des			: �Ƿ����3DES�㷨��false��ʾʹ��DES�㷨
//			masterIndex		: ����Կ����
//			workingIndex	: ������Կ���������������Կ����ȡֵ-1��ʹ������Կ����ָ��������Կ���мӽ���
//			data			: ����Macԭ����
//			datalength		: Macԭ���ݵĳ���,Ҫ��8�ı�����С�ڻ����246�ֽڳ���
//out	:	outData			: Mac����
//ret	:    >0             : Mac���ݳ���
//         ����             : ʧ�� (����ʧ��ʱ,����HXiMate_PinPad_GetError��ȡ������Ϣ) 
HXIMATESDK_API int HXiMate_PinPad_Mac(int algorithm, int masterIndex, int workingIndex, unsigned char *data, unsigned char *outData, int datalength);

//Pinpad Hash ����
//in	:	is3des			: �Ƿ����3DES�㷨��false��ʾʹ��DES�㷨
//			data			: ��������
//			datalength		: ���ݵĳ���
//out	:	outData			: ��ɢ�������
//ret	:    >0             : ��ɢ������ݳ���
//         ����             : ʧ�� (����ʧ��ʱ,����HXiMate_PinPad_GetError��ȡ������Ϣ) 
HXIMATESDK_API int HXiMate_PinPad_Hash(int hashAlgorithm, unsigned char *data, int datalength, unsigned char *outData);

#endif