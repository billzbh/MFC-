#ifndef _HXIMATEFINGERPRINTWIN32_H
#define _HXIMATEFINGERPRINTWIN32_H

#ifndef HXIMATESDK_API
#ifdef HXIMATESDK_EXPORTS
#define HXIMATESDK_API __declspec(dllexport)
#else
#define HXIMATESDK_API __declspec(dllimport)
#endif
#endif

#define FINGERPRINT_MODEL_JSABC         0   //�������ж���ģ��
#define FINGERPRINT_MODEL_SHENGTENG     1   //���ڶ��ƣ����ʢҵָ��ģ�飩
#define FINGERPRINT_MODEL_ZHONGZHENG    2   //����


// ����֧�ֵ�ָ��������
// in   :   model					     :ָ��ģ������
//          FINGERPRINT_MODEL_JSABC      ://�������ж���ģ��
//			FINGERPRINT_MODEL_SHENGTENG  ://���ڶ��ƣ����ʢҵָ��ģ�飩
//			FINGERPRINT_MODEL_ZHONGZHENG ://����
HXIMATESDK_API void HXiMate_Fingerprint_SetModel(int model);

//ȡ�� ��ȡָ������
HXIMATESDK_API void HXiMate_Fingerprint_Cancel();

//Fingerprint�ϵ� (ͨѶ������Ϊ9600 У�鷽ʽ 0)
//ret :     0   :    �ɹ�
//         ���� :    ʧ�� (�ϵ�ʧ��ʱ,����HXiMate_Fingerprint_GetError��ȡ������Ϣ)
HXIMATESDK_API int HXiMate_Fingerprint_PowerOn();

//Fingerprint�µ�
//ret :     0   :    �ɹ�
//         ���� :    ʧ�� (����ʧ��ʱ,����HXiMate_Fingerprint_GetError��ȡ������Ϣ)
HXIMATESDK_API int HXiMate_Fingerprint_PowerOff();

//��ȡFingerprintģ��İ汾����Ϣ
//out :  version  :   Fingerprint�汾��Ϣ
//ret :  >0       :   version����
//        ����    :   ʧ�� (����ʧ��ʱ,����HXiMate_Fingerprint_GetError��ȡ������Ϣ)
HXIMATESDK_API int HXiMate_Fingerprint_Version(unsigned char *version);

//�ɼ�ָ������ֵ
//out : ����ֵ����
//ret :  >0        :   FeatureData����ֵ����
//         ����   :   ʧ�� (����ʧ��ʱ,����HXiMate_Fingerprint_GetError��ȡ������Ϣ)
HXIMATESDK_API int HXiMate_Fingerprint_TakeFeature(unsigned char *FeatureData);

//��ȡFingerprint��ز���ʧ������µĴ�����Ϣ
//out  :   error   :������Ϣ
HXIMATESDK_API void HXiMate_Fingerprint_GetError(wchar_t *error);


#endif