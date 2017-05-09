#ifndef HXIMATEAPPFACE_H__
#define HXIMATEAPPFACE_H__




/*
��ȡ�豸״̬
�������˵����
iComPort   	   //���ں�,��1��ʾCOM1,2��ʾCOM2,�������ƣ� 
iBaudRate  	   //������
iComTimeout    //���ڲ�����ʱʱ�� ��λΪ��(S)

�������˵��:
pszDevStatu     //�豸״̬ ��REGULAR��-����  ��BKDOWN��-����
piDevStatuLen   //�豸״̬����

����ֵ:
0-�ɹ� ��0-ʧ��	
*/
int _declspec(dllexport) __stdcall AKP_GetDevStatu ( int iComPort , int iBaudRate , int iComTimeout ,
char *pszDevStatu , int *piDevStatuLen );


/*
��ȡ�豸��Ϣ
�������˵����
iComPort   	   //���ں�,��1��ʾCOM1,2��ʾCOM2,�������ƣ� 
iBaudRate  	   //������
iComTimeout    //���ڲ�����ʱʱ�� ��λΪ��(S)

�������˵��:
pszDevType       //�豸״̬ ���ﷵ��ֵ�����ȶ�
piDevTypeLen     //�豸״̬����
pszDevSerialNo    //�豸���к�
piDevSerialNoLen  //�豸���кų���

����ֵ:
0-�ɹ� ��0-ʧ��
*/
int _declspec(dllexport) __stdcall AKP_GetDevInfo ( int iComPort , int iBaudRate , int iComTimeout ,char *pszDevType , int *piDevTypeLen
char *pszDevSerialNo , int *piDevSerialNoLen );


/*
���ó�ʼ����Կ
�������˵����
iComPort   	   //���ں�,��1��ʾCOM1,2��ʾCOM2,�������ƣ� 
iBaudRate  	   //������
iComTimeout    //���ڲ�����ʱʱ�� ��λΪ��(S)
iMainkeyIndex	//����Կ�����š�0-15����������̸��ݴ�������//ȡ��������������е�����Կ��
iMainKeyLen     //����Կ���ȣ���8,16,24�� 
pszMainKey      //��iMainkeyIndexָ�������Կ���ܵ�������
//Կ,���������������(����)
*/
int _declspec(dllexport) __stdcall AKP_SetInitMainKey(int iComPort , int iBaudRate , int iComTimeout ,
 int iMainkeyIndex, int iMainKeyLen, char *pszMainKey );

/*
��������Կ
�������˵����
iComPort   	   //���ں�,��1��ʾCOM1,2��ʾCOM2,�������ƣ� 
iBaudRate  	   //������
iComTimeout    //���ڲ�����ʱʱ�� ��λΪ��(S)
iMainkeyIndex	//����Կ�����š�0-15����������̸��ݴ�������//ȡ��������������е�����Կ��
iNewMainKeyLen	//������Կ���ȣ���8,16,24��
pszNewMainKey  //��iMainkeyIndexָ�������Կ���ܵ�������
//Կ,���������������(����)
	pszChkCode		 //����ԿУ����

����ֵ:
0-�ɹ� ��0-ʧ��

��ע:
������������е�����Կ����0-15����16�顿
*/
int _declspec(dllexport) __stdcall AKP_ChgMainKey(int iComPort , int iBaudRate , int iComTimeout ,
int iMainkeyIndex , 
int iNewMainKeyLen , char *pszNewMainKey,
char *pszChkCode);


/*
���Ĺ�����Կ
�������˵����
iComPort   	   //���ں�,��1��ʾCOM1,2��ʾCOM2,�������ƣ� 
iBaudRate  	   //������
iComTimeout    //���ڲ�����ʱʱ�� ��λΪ��(S)
iMainkeyIndex	//����Կ�����š�0-15����������̸��ݴ�������//ȡ��������������е�����Կ��
iWorkKeyIndex   //������Կ�����š�0-7��
iWorkKeyLen    //������Կ���ȡ�8,16,24��
pszWorkKey     //��iMainkeyIndexָ�������Կ���ܹ��Ĺ���
//��Կ���·����������(����)
pszChkCode		 //������ԿУ����
����ֵ:
0-�ɹ� ��0-ʧ��
  ��ע��
1.���ù�����Կ���ܹ��Ĺ�����Կ���͸�������̣��Զ��û�������м��ܡ�
2.����ԿĬ��ʹ�õ�0�飬��1-15�鱸�á�
3.������ԿĬ��ʹ�õ�0�飬1-7�鱸��
*/
int _declspec(dllexport) __stdcall AKP_ChgWorkKey(int iComPort , int iBaudRate , int iComTimeout ,
int iMainkeyIndex , int iWorkKeyIndex ,
int iWorkKeyLen , char *pszWorkKey,
char *pszChkCode );

/*
�ָ�������Կ
����ֵ:
0-�ɹ� ��0-ʧ��

��ע:
�����������е�����Կ�͹�����Կ
*/
int _declspec(dllexport) __stdcall AKP_ResFactoryKey ( int iComPort , int iBaudRate , int iComTimeout );





#endif
