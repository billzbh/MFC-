#ifndef HXIMATEAPPFACE_H__
#define HXIMATEAPPFACE_H__




/*
获取设备状态
输入参数说明：
iComPort   	   //串口号,如1表示COM1,2表示COM2,依次类推； 
iBaudRate  	   //波特率
iComTimeout    //串口操作超时时间 单位为秒(S)

输出参数说明:
pszDevStatu     //设备状态 “REGULAR”-正常  “BKDOWN”-故障
piDevStatuLen   //设备状态长度

返回值:
0-成功 非0-失败	
*/
int _declspec(dllexport) __stdcall AKP_GetDevStatu ( int iComPort , int iBaudRate , int iComTimeout ,
char *pszDevStatu , int *piDevStatuLen );


/*
获取设备信息
输入参数说明：
iComPort   	   //串口号,如1表示COM1,2表示COM2,依次类推； 
iBaudRate  	   //波特率
iComTimeout    //串口操作超时时间 单位为秒(S)

输出参数说明:
pszDevType       //设备状态 这里返回值由你先定
piDevTypeLen     //设备状态长度
pszDevSerialNo    //设备序列号
piDevSerialNoLen  //设备序列号长度

返回值:
0-成功 非0-失败
*/
int _declspec(dllexport) __stdcall AKP_GetDevInfo ( int iComPort , int iBaudRate , int iComTimeout ,char *pszDevType , int *piDevTypeLen
char *pszDevSerialNo , int *piDevSerialNoLen );


/*
设置初始主密钥
输入参数说明：
iComPort   	   //串口号,如1表示COM1,2表示COM2,依次类推； 
iBaudRate  	   //波特率
iComTimeout    //串口操作超时时间 单位为秒(S)
iMainkeyIndex	//主密钥索引号【0-15】，密码键盘根据此索引，//取出存在密码键盘中的主密钥。
iMainKeyLen     //主密钥长度；【8,16,24】 
pszMainKey      //用iMainkeyIndex指向的主密钥加密的新主密
//钥,将被存入密码键盘(明文)
*/
int _declspec(dllexport) __stdcall AKP_SetInitMainKey(int iComPort , int iBaudRate , int iComTimeout ,
 int iMainkeyIndex, int iMainKeyLen, char *pszMainKey );

/*
更改主密钥
输入参数说明：
iComPort   	   //串口号,如1表示COM1,2表示COM2,依次类推； 
iBaudRate  	   //波特率
iComTimeout    //串口操作超时时间 单位为秒(S)
iMainkeyIndex	//主密钥索引号【0-15】，密码键盘根据此索引，//取出存在密码键盘中的主密钥。
iNewMainKeyLen	//新主密钥长度；【8,16,24】
pszNewMainKey  //用iMainkeyIndex指向的主密钥加密的新主密
//钥,将被存入密码键盘(密文)
	pszChkCode		 //主密钥校验码

返回值:
0-成功 非0-失败

备注:
更换密码键盘中的主密钥。【0-15，共16组】
*/
int _declspec(dllexport) __stdcall AKP_ChgMainKey(int iComPort , int iBaudRate , int iComTimeout ,
int iMainkeyIndex , 
int iNewMainKeyLen , char *pszNewMainKey,
char *pszChkCode);


/*
更改工作密钥
输入参数说明：
iComPort   	   //串口号,如1表示COM1,2表示COM2,依次类推； 
iBaudRate  	   //波特率
iComTimeout    //串口操作超时时间 单位为秒(S)
iMainkeyIndex	//主密钥索引号【0-15】，密码键盘根据此索引，//取出存在密码键盘中的主密钥。
iWorkKeyIndex   //工作密钥索引号【0-7】
iWorkKeyLen    //工作密钥长度【8,16,24】
pszWorkKey     //用iMainkeyIndex指向的主密钥加密过的工作
//密钥，下发给密码键盘(密文)
pszChkCode		 //工作密钥校验码
返回值:
0-成功 非0-失败
  备注：
1.把用工作密钥加密过的工作密钥发送给密码键盘，以对用户密码进行加密。
2.主密钥默认使用第0组，第1-15组备用。
3.工作密钥默认使用第0组，1-7组备用
*/
int _declspec(dllexport) __stdcall AKP_ChgWorkKey(int iComPort , int iBaudRate , int iComTimeout ,
int iMainkeyIndex , int iWorkKeyIndex ,
int iWorkKeyLen , char *pszWorkKey,
char *pszChkCode );

/*
恢复出厂密钥
返回值:
0-成功 非0-失败

备注:
清除密码键盘中的主密钥和工作密钥
*/
int _declspec(dllexport) __stdcall AKP_ResFactoryKey ( int iComPort , int iBaudRate , int iComTimeout );





#endif
