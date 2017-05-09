#ifndef _HXIMATEMIFCARDWIN32_H
#define _HXIMATEMIFCARDWIN32_H

#ifndef HXIMATESDK_API
#ifdef HXIMATESDK_EXPORTS
#define HXIMATESDK_API __declspec(dllexport)
#else
#define HXIMATESDK_API __declspec(dllimport)
#endif
#endif

// �����Ƶ��
// out      :serialNo			: ��Ƭϵ�к�
// return   : >0				: �ɹ�, ��Ƭϵ�к��ֽ���
//             0				: ʧ��
HXIMATESDK_API unsigned int HXiMate_MifCard(unsigned char *serialNo);

// ��CPU��Ƶ�����и�λ����
// out      :   resetDataBytes	: ��λ����
// return   :   >0				: ��λ���ݳ���; 
//              0				: ��λ����
HXIMATESDK_API unsigned int  HXiMate_MifReset(unsigned char *resetDataBytes);

// MIF CPU��APDU
// in     :  psApduIn			: apdu���
//			 uiInLen			: apdu�������
//			 psApduOut			: apdu���ش�
//			 puiOutLen			: apdu���ش�����
// return :   0					: �ɹ�
//           ����				: ʧ��
HXIMATESDK_API unsigned int HXiMate_MifApdu(unsigned char *apduIn, unsigned int uiInLen, unsigned char *apduOut, unsigned int OutLen);

// M1��������֤
// in      :  ucSecNo			: ������
//			  ucKeyAB			: Կ���ͣ�0x00��A���룬0x04: B����
//			  psKey				: 6�ֽڵ���Կ
// return  :  0					: �ɹ�
//            ����				: ʧ��
HXIMATESDK_API unsigned int HXiMate_MifAuth(unsigned char ucSecNo, unsigned char ucKeyAB, unsigned char *key);

// M1�������ݿ�
// in         :  ucSecNo	    : ������
//			     ucBlock        : ���
// out        :  data		    : 16�ֽڵ�����
// return     :  0              : �ɹ�
//               ����           : ʧ��
HXIMATESDK_API unsigned int HXiMate_MifReadBlock(unsigned char ucSecNo, unsigned char ucBlock, unsigned char *data);

// M1��д���ݿ�
// in        :   ucSecNo		: ������
//			     ucBlock		: ���
//			     data			: 16�ֽڵ�����
// return    :    0				: �ɹ�
//               ����			: ʧ��
HXIMATESDK_API unsigned int HXiMate_MifWriteBlock(unsigned char ucSecNo, unsigned char ucBlock, unsigned char *data);

// M1Ǯ����ֵ
// in        :  ucSecNo			: ������
//			    ucBlock			: ���
//			    ulValue			: ֵ
// return    :  0				: �ɹ�
//              ����			: ʧ��
HXIMATESDK_API unsigned int HXiMate_MifIncrement(unsigned char ucSecNo, unsigned char ucBlock, unsigned long ulValue);

// M1Ǯ����ֵ
// in    :   ucSecNo			: ������
//			 ucBlock			: ���
//			 ulValue			: ֵ
// return:   0					: �ɹ�
//           ����				: ʧ��
HXIMATESDK_API unsigned int HXiMate_MifDecrement(unsigned char ucSecNo, unsigned char ucBlock, unsigned long ulValue);

// M1���鿽��
// in    ��  ucSrcSecNo			: Դ������
//			 ucSrcBlock			: Դ���
//			 ucDesSecNo			: Ŀ��������
//			 ucDesBlock			: Ŀ�Ŀ��
// return��  0					: �ɹ�
//           ����				: ʧ��
HXIMATESDK_API unsigned int HXiMate_MifCopy(unsigned char ucSrcSecNo, unsigned char ucSrcBlock, unsigned char ucDesSecNo, unsigned char ucDesBlock);


#endif
