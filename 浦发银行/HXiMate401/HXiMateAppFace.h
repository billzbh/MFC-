#ifndef HXIMATEAPPFACE_H__
#define HXIMATEAPPFACE_H__


#ifdef __cplusplus
extern "C" {
#endif


//	����:	��⵱ǰ�������ͻ��˹��ߵ��ն��豸
//	����:	��
//	���:	buffer		�豸�б��豸���к�16λ,���㲹0��
//	��"AAAAAAAABBBBBBBB"����ʾ��⵽�豸���к�ΪAAAAAAAABBBBBBBB
//	����:	0�ɹ�	!0ʧ��
_declspec(dllexport) int __stdcall detectClientDev (char* buffer);

// ���ն��豸USB�ӿڴӽ��յ�����Կ�ַ������ݰ����з�������
// ���룺shopNum		 ��15λ�̻��š�����������ʹ�ã�,
//	     clientDevID     ���ն�POS�š�����������ʹ�ã�,
//	     bankID          �����кš�����������ʹ�ã�,
//		 keyValue		 ����Կ���ġ���3DES�㷨����Կ����128λ�����г�ʼ��ԿΪ00000000000000000000000000000000��������Կ11111111111111111111111111111111
//
// ���أ�0��Ч���ݰ���1����ɹ�
_declspec(dllexport) unsigned char dispatchKeyData(char*shopNum, char*clientDevID,char* bankID,char* keyValue);


// ��ȡ�ն��豸����ԿУ��ֵ
//���checkValue
// ���أ�0��Ч���ݰ���1����ɹ�
_declspec(dllexport)  int getMKcheckValue(char* checkValue);




#ifdef __cplusplus
}
#endif

#endif
