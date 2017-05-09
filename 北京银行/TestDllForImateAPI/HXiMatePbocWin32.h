#ifndef _HXIMATEPBOCWIN32_H
#define _HXIMATEPBOCWIN32_H

#ifndef HXIMATESDK_API
#ifdef HXIMATESDK_EXPORTS
#define HXIMATESDK_API __declspec(dllexport)
#else
#define HXIMATESDK_API __declspec(dllimport)
#endif
#endif

// EMV�ͻ��߲�ӿڷ�����
#define HXPBOC_HIGH_OK				0  // OK
#define HXPBOC_HIGH_PARA			1  // ��������
#define HXPBOC_HIGH_NO_CARD			2  // �޿�
#define HXPBOC_HIGH_NO_APP			3  // ��֧�ֵ�Ӧ��
#define HXPBOC_HIGH_CARD_IO			4  // ��������
#define HXPBOC_HIGH_CARD_SW			5  // �Ƿ���ָ��״̬��
#define HXPBOC_HIGH_DENIAL			6  // ���ױ��ܾ�
#define HXPBOC_HIGH_TERMINATE		7  // ���ױ���ֹ
#define HXPBOC_HIGH_OTHER			8  // ��������

// ���CPU��Ƭ�Ƿ����
// in  :	iSlot		:0 �û����� 4 SAM��
// ret :	0			:������
// 			1			:����
HXIMATESDK_API int  HXiMate_PbocTest(int iSlot);

// ��CPU�����и�λ����
// in	:	iSlot			: 0 �û���, 4 SAM��
// out  :	resetDataBytes	: ��λ����
// ret	    :   >0          : ��λ���ݳ���; 
//             <=0          : ��λ����
HXIMATESDK_API int  HXiMate_PbocReset(int iSlot, unsigned char *resetDataBytes);

// �رտ�Ƭ
// in  :	iSlot		:	0 �û���, 4 SAM��
// ret	:   ������
HXIMATESDK_API int HXiMate_PbocClose(int iSlot);

// ִ��APDUָ��
// in  : iSlot			: 0 �û����� 4 SAM��
//		 iInLen   		: Apduָ���
// 		 pIn     		: Apduָ��, ��ʽ: Cla Ins P1 P2 Lc DataIn Le
// out : piOutLen 		: ApduӦ�𳤶�
//       pOut    		: ApduӦ��, ��ʽ: DataOut Sw1 Sw2
// ret : >0				: �������ɹ�,pOut���ݳ���
//       ����			: ��������
HXIMATESDK_API int HXiMate_PbocExchangeApdu(int iSlot, int iInLen, unsigned char *pIn, unsigned char *pOut);


// ����IC����������, ������ȱʡΪ0
// in :  cardReaderType     :����������, 0 оƬ������, 1��Ƶ������, >=4 SAM����,
HXIMATESDK_API void HXiMate_SetCardReaderType(int cardReaderType);

//��ȡ���ţ��ֿ����������ֿ���֤�����룬Ӧ��ʧЧ���ڡ������к�
//***�ò�����ִ��֮ǰ�ɹ�ִ����HXiMate_PbocReset����*****
// out : cardInfo 	: ��������Ϣ�ɹ�ʱ���������Ϣ��������ȡ���ţ��ֿ����������ֿ���֤�����룬Ӧ��ʧЧ���ڡ������к�
//					  ���ֶ�֮����","���
// ret : 0          : �������ɹ�
//       ����       : ����ʧ��
HXIMATESDK_API int HXiMate_PbocReadInfo(char *cardInfo);

// ��ȡPBOC����־�Ĳ������󣬸ò�����ִ��֮ǰ�ɹ�ִ����HXiMate_PbocReset����
// out : cardLog 	: ��������־�ɹ�ʱ���������־��Ϣ��retCode > 0�����ص�ÿ����־�ĳ���Ϊstrlen(cardLog)/RetCode
// ret : >=0        : ������־�ɹ���������־��Ŀ����
//       ����       : ������־ʧ��
HXIMATESDK_API int HXiMate_PbocReadLog(char *cardLog);

//��ȡPBOC����ش�����Ϣ
// out : errorInfo  :������Ϣ
// ret : >0         :errorInfo����
HXIMATESDK_API int HXiMate_PbocGetError(char *errorInfo);

// ���ĳ�ʼ��
// in  : pszMerchantId   : �̻���[15]
//		 pszTermId       : �ն˺�[8]
//		 pszMerchantName : �̻�����[40]
//		 uiCountryCode   : �ն˹��Ҵ���, 1-999
//		 uiCurrencyCode  : ���׻��Ҵ���, 1-999
// ret : 0				 : �ɹ�
//       ����		     : ʧ�ܣ�������ο� EMV�ͻ��߲�ӿڷ�����
HXIMATESDK_API int HXiMate_Pboc_iHxPbocHighInitCore(char *pszMerchantId, char *pszTermId, char *pszMerchantName, unsigned int uiCountryCode, unsigned int uiCurrencyCode);

// ���׳�ʼ��
// in  : pszDateTime  : ��������ʱ��[14], YYYYMMDDhhmmss
//       ulAtc        : �ն˽�����ˮ��, 1-999999
//       ucTransType  : ��������, 0x00 - 0xFF
//       pszAmount    : ���׽��[12]
// out : pszField55   : ��װ�õ�55������, ʮ�����ƿɶ���ʽ, Ԥ��513�ֽڳ���
//       pszPan       : ���˺�[19], �ɶ���ʽ
//       piPanSeqNo   : ���˺����к�, 0-99, -1��ʾ������
//       pszTrack2    : ���ŵ���Ч����[37], 3x��ʽ, ����Ϊ0��ʾ������
//       pszExtInfo   : ��������, ���ص���Ŀ����: ������֤�����͡�֤���š�Ӧ��ʧЧ���ڡ�һ�ŵ����ݡ��ֽ����������, ����Ŀ��������˳������֮���ö��ŷָ
// ret : 0			  : �ɹ�
//       ����		  : ʧ�ܣ�������ο� EMV�ͻ��߲�ӿڷ�����
HXIMATESDK_API int HXiMate_Pboc_iHxPbocHighInitTrans(char *pszDateTime, unsigned long ulAtc, unsigned char ucTransType, char *pszAmount,
	char *pszField55, char *pszPan, int piPanSeqNo, char *pszTrack2, char *pszExtInfo);

// ��ɽ���
// in  : pszIssuerData  : ��̨����, ʮ�����ƿɶ���ʽ
// out : pszField55     : ��װ�õ�55������, �����Ƹ�ʽ, Ԥ��513�ֽڳ���
// ret : 0				: �ɹ�
//       ����		    : ʧ�ܣ�������ο� EMV�ͻ��߲�ӿڷ�����
// Note: ���˷���HXPBOC_HIGH_OK��, ����HXPBOC_HIGH_DENIALҲ�᷵�ؽű����
HXIMATESDK_API int HXiMate_Pboc_iHxPbocHighDoTrans(char* pszIssuerData, char* pszField55);

#endif