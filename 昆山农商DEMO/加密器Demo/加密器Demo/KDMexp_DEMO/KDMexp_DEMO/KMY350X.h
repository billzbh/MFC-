
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the KMY350X_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// KMY350X_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef KMY350X_EXPORTS
#define KMY350X_API __declspec(dllexport)
#else
#define KMY350X_API __declspec(dllimport)
#endif

//#define EndWith03

//--------------------------------------------------
//�漰35+6����������	WINAPI��׼������	JAFFERY
//--------------------------------------------------
KMY350X_API int _stdcall GetDllVer(unsigned char * UReData);                                               //��̬��汾
KMY350X_API int _stdcall OpenPort(int Port, int BaudRate);                                                 //�򿪴���
KMY350X_API int _stdcall ClosePort(void);                                                                  //�رմ���                                                         
KMY350X_API int _stdcall ScoutKeyPress(unsigned char *UcChar);                                             //������
KMY350X_API int _stdcall LoadMainKey(int KeyId, unsigned char * UcKey, unsigned char * UcAuthCode);        //��������Կ
KMY350X_API int _stdcall LoadWorkKey(int MainKeyId, int WorkKeyId, unsigned char * UcKey, unsigned char * UcAuthCode);       //���ع�����Կ
KMY350X_API int _stdcall LoadWorkKeySam(int MainKeyId, int WorkKeyId, int slot, unsigned char * pwd, unsigned char * Ucatalog, unsigned char * UCMD,  unsigned char * UcKey, unsigned char * UcAuthCode);	//���ع�����Կ����SAM����
KMY350X_API int _stdcall DevReset(int IType);                                                              //����λ�Լ�
KMY350X_API int _stdcall SetAccount(unsigned char * UcAccount);                                            //�����˺�
KMY350X_API int _stdcall SetClientSN(unsigned char * UcClientSN);                                          //���ò�Ʒ�ն˺��ַ���
KMY350X_API int _stdcall ReadClientSN(unsigned char * UcClientSN);                                         //��ȡ��Ʒ�ն˺��ַ���
KMY350X_API int _stdcall GetVersion(unsigned char * UcVersion);                                            //ȡ��Ʒ�ײ�汾��
KMY350X_API int _stdcall MACAdd(unsigned char * UcMacStr, unsigned char * UcMacStrResult, int IType);      //����MAC����
KMY350X_API int _stdcall SetMACArithmetic(int IType);                                                      //����MAC�㷨
KMY350X_API int _stdcall SetPINRepairMethod(int RepairType, unsigned char UcRepairVal);                    //����PIN�Ĳ�����ʽ
KMY350X_API int _stdcall SetPINArithmetic(int IType);                                                      //����PIN�㷨
KMY350X_API int _stdcall SetEncryptMethod(int IType);                                                      //���ü��ܷ���
KMY350X_API int _stdcall SetMasterMethod(int IType);													   //��������Կ������ʽ
KMY350X_API int _stdcall SetEcbCbcMode(int IType);														   //���ü����㷨
KMY350X_API int _stdcall SetFunctionKeys(int IType);                                                       //���ù��ܼ�����ʽ
KMY350X_API int _stdcall SetCommunicationMode(int IType);												   //����ͨѶģʽ
KMY350X_API int _stdcall ActivateWorkKey(int MainKeyId, int WorkKeyId);                                    //�������Կ
KMY350X_API int _stdcall ResponseAsc(unsigned char * UcStr, unsigned char * UcRStr);                       //���Լ�����Ӧ�ַ�
KMY350X_API int _stdcall KeyboardControl(int IType);                                                       //���Ϳ��ؼ��̺Ͱ�������
KMY350X_API int _stdcall SingleKeyControl(unsigned char CStr);                                             //�������ؼ��̺Ͱ�����������
KMY350X_API int _stdcall PinStart(int PinLen, int AddMode, int TimeOut);                                   //����������̼���
KMY350X_API int _stdcall ReadPinBlock(unsigned char * UcPinBlock);                                         //ȡ����������
KMY350X_API int _stdcall DataAdd(unsigned char * UcData, unsigned char * UcReData);                        //���ݼ���
KMY350X_API int _stdcall DataUnAdd(unsigned char * UcData, unsigned char * UcReData);                      //���ݽ���
KMY350X_API int _stdcall ICReadPinBlock(unsigned char * UcData, unsigned char * UcReData);                 //ȡ����PIN�������� ������SAM������ģʽ��
KMY350X_API int _stdcall ICSendAPDU(unsigned char * UcData, unsigned char * UcReData);                     //ִ������APDU
KMY350X_API int _stdcall ICPowerOn(unsigned char * UcReData);                                              //�ϵ縴λIC�� (����IC��)
KMY350X_API int _stdcall ICPowerOff(void);                                                                 //��CPU�����ϵ�
KMY350X_API int _stdcall ICReadType(int ISeat, unsigned char * UcType);                                    //��ȡIC������������ (����IC��)
KMY350X_API int _stdcall ICSetType(int ISeat, unsigned char UcType);                                       //����IC������������ (����IC��)
KMY350X_API int _stdcall RSA_MODULUS_DATA(unsigned char * UData);                                          //����RSA��Կ��ģ����
KMY350X_API int _stdcall RSA_SECRET_DATA(unsigned char * UData);                                           //����RSA��Կ��˽Կ����
KMY350X_API int _stdcall RSA_PUBLIC_DATA(unsigned char * UData);                                           //����RSA��Կ�Թ�Կ����
KMY350X_API int _stdcall RSA_ENCRYPT(unsigned char * UData, unsigned char * UReData);                      //RSA���ݼ���
KMY350X_API int _stdcall RSA_DECRYPT(unsigned char * UData, unsigned char * UReData);                      //RSA���ݽ���
KMY350X_API int _stdcall DUKPT_LoadDeriveKey(unsigned char * UcKey);								       //���ػ�����Կ Derivation Key ��DUKPT��
KMY350X_API int _stdcall DUKPT_LoadKSN(unsigned char * UcKSN);											   //���ؼ������б��(Key Serial Number) ��DUKPT��
KMY350X_API int _stdcall DUKPT_CTL(unsigned char UcType);												   //����DUKPT�㷨 ��DUKPT��
KMY350X_API int _stdcall StartMasterKeyInject(int KeyId);												   //����ֱ�ӴӼ�����������Կģʽ
KMY350X_API int _stdcall StartWorkKeyInject(int MainKeyId, int WorkKeyId);								   //����ֱ�ӴӼ������빤����Կģʽ

//
KMY350X_API int _stdcall UNION_API_ReadRtc(char * cpExeStatue,char * cpRtc);
KMY350X_API int _stdcall UNION_API_SetRtc(char * cpExeStatue,char * cpRtc);
KMY350X_API int _stdcall UNION_API_Throb(char * cpExeStatue,char * cpThrobInfo,int * iLen);
KMY350X_API int _stdcall UNION_API_ChgPassword(char * cpExeStatue,char OperId);
KMY350X_API int _stdcall UNION_API_EppInstallUninstall(char * cpExeStatue,char Operator);
KMY350X_API int _stdcall UNION_API_InjectTMK(void);
KMY350X_API int _stdcall UNION_API_PinReadPort(unsigned char * lpBuffer);
KMY350X_API int _stdcall UNION_API_ErrFunc(char * ucMsg_, unsigned char code_);
