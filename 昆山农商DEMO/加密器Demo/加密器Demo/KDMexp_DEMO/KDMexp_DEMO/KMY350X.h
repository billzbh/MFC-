
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
//涉及35+6个导出函数	WINAPI标准导出库	JAFFERY
//--------------------------------------------------
KMY350X_API int _stdcall GetDllVer(unsigned char * UReData);                                               //动态库版本
KMY350X_API int _stdcall OpenPort(int Port, int BaudRate);                                                 //打开串口
KMY350X_API int _stdcall ClosePort(void);                                                                  //关闭串口                                                         
KMY350X_API int _stdcall ScoutKeyPress(unsigned char *UcChar);                                             //监测键盘
KMY350X_API int _stdcall LoadMainKey(int KeyId, unsigned char * UcKey, unsigned char * UcAuthCode);        //下载主密钥
KMY350X_API int _stdcall LoadWorkKey(int MainKeyId, int WorkKeyId, unsigned char * UcKey, unsigned char * UcAuthCode);       //下载工作密钥
KMY350X_API int _stdcall LoadWorkKeySam(int MainKeyId, int WorkKeyId, int slot, unsigned char * pwd, unsigned char * Ucatalog, unsigned char * UCMD,  unsigned char * UcKey, unsigned char * UcAuthCode);	//下载工作密钥（限SAM卡）
KMY350X_API int _stdcall DevReset(int IType);                                                              //程序复位自检
KMY350X_API int _stdcall SetAccount(unsigned char * UcAccount);                                            //设置账号
KMY350X_API int _stdcall SetClientSN(unsigned char * UcClientSN);                                          //设置产品终端号字符串
KMY350X_API int _stdcall ReadClientSN(unsigned char * UcClientSN);                                         //读取产品终端号字符串
KMY350X_API int _stdcall GetVersion(unsigned char * UcVersion);                                            //取产品底层版本号
KMY350X_API int _stdcall MACAdd(unsigned char * UcMacStr, unsigned char * UcMacStrResult, int IType);      //数据MAC运算
KMY350X_API int _stdcall SetMACArithmetic(int IType);                                                      //设置MAC算法
KMY350X_API int _stdcall SetPINRepairMethod(int RepairType, unsigned char UcRepairVal);                    //设置PIN的补偿方式
KMY350X_API int _stdcall SetPINArithmetic(int IType);                                                      //设置PIN算法
KMY350X_API int _stdcall SetEncryptMethod(int IType);                                                      //设置加密方法
KMY350X_API int _stdcall SetMasterMethod(int IType);													   //设置主密钥工作方式
KMY350X_API int _stdcall SetEcbCbcMode(int IType);														   //设置加密算法
KMY350X_API int _stdcall SetFunctionKeys(int IType);                                                       //设置功能键处理方式
KMY350X_API int _stdcall SetCommunicationMode(int IType);												   //设置通讯模式
KMY350X_API int _stdcall ActivateWorkKey(int MainKeyId, int WorkKeyId);                                    //激活工作密钥
KMY350X_API int _stdcall ResponseAsc(unsigned char * UcStr, unsigned char * UcRStr);                       //测试键盘响应字符
KMY350X_API int _stdcall KeyboardControl(int IType);                                                       //发送开关键盘和按键声音
KMY350X_API int _stdcall SingleKeyControl(unsigned char CStr);                                             //单个开关键盘和按键声音控制
KMY350X_API int _stdcall PinStart(int PinLen, int AddMode, int TimeOut);                                   //启动密码键盘加密
KMY350X_API int _stdcall ReadPinBlock(unsigned char * UcPinBlock);                                         //取键盘中密码
KMY350X_API int _stdcall DataAdd(unsigned char * UcData, unsigned char * UcReData);                        //数据加密
KMY350X_API int _stdcall DataUnAdd(unsigned char * UcData, unsigned char * UcReData);                      //数据解密
KMY350X_API int _stdcall ICReadPinBlock(unsigned char * UcData, unsigned char * UcReData);                 //取键盘PIN加密数据 （限于SAM卡加密模式）
KMY350X_API int _stdcall ICSendAPDU(unsigned char * UcData, unsigned char * UcReData);                     //执行命令APDU
KMY350X_API int _stdcall ICPowerOn(unsigned char * UcReData);                                              //上电复位IC卡 (所有IC卡)
KMY350X_API int _stdcall ICPowerOff(void);                                                                 //给CPU卡座断电
KMY350X_API int _stdcall ICReadType(int ISeat, unsigned char * UcType);                                    //读取IC卡座及卡类型 (所有IC卡)
KMY350X_API int _stdcall ICSetType(int ISeat, unsigned char UcType);                                       //设置IC卡座及卡类型 (所有IC卡)
KMY350X_API int _stdcall RSA_MODULUS_DATA(unsigned char * UData);                                          //下载RSA密钥对模数据
KMY350X_API int _stdcall RSA_SECRET_DATA(unsigned char * UData);                                           //下载RSA密钥对私钥数据
KMY350X_API int _stdcall RSA_PUBLIC_DATA(unsigned char * UData);                                           //下载RSA密钥对公钥数据
KMY350X_API int _stdcall RSA_ENCRYPT(unsigned char * UData, unsigned char * UReData);                      //RSA数据加密
KMY350X_API int _stdcall RSA_DECRYPT(unsigned char * UData, unsigned char * UReData);                      //RSA数据解密
KMY350X_API int _stdcall DUKPT_LoadDeriveKey(unsigned char * UcKey);								       //下载基础密钥 Derivation Key （DUKPT）
KMY350X_API int _stdcall DUKPT_LoadKSN(unsigned char * UcKSN);											   //下载键盘序列编号(Key Serial Number) （DUKPT）
KMY350X_API int _stdcall DUKPT_CTL(unsigned char UcType);												   //控制DUKPT算法 （DUKPT）
KMY350X_API int _stdcall StartMasterKeyInject(int KeyId);												   //启动直接从键盘输入主密钥模式
KMY350X_API int _stdcall StartWorkKeyInject(int MainKeyId, int WorkKeyId);								   //启动直接从键盘输入工作密钥模式

//
KMY350X_API int _stdcall UNION_API_ReadRtc(char * cpExeStatue,char * cpRtc);
KMY350X_API int _stdcall UNION_API_SetRtc(char * cpExeStatue,char * cpRtc);
KMY350X_API int _stdcall UNION_API_Throb(char * cpExeStatue,char * cpThrobInfo,int * iLen);
KMY350X_API int _stdcall UNION_API_ChgPassword(char * cpExeStatue,char OperId);
KMY350X_API int _stdcall UNION_API_EppInstallUninstall(char * cpExeStatue,char Operator);
KMY350X_API int _stdcall UNION_API_InjectTMK(void);
KMY350X_API int _stdcall UNION_API_PinReadPort(unsigned char * lpBuffer);
KMY350X_API int _stdcall UNION_API_ErrFunc(char * ucMsg_, unsigned char code_);
