#ifndef _HXIMATEPINPADWIN32_H
#define _HXIMATEPINPADWIN32_H

#ifndef HXIMATESDK_API
#ifdef HXIMATESDK_EXPORTS
#define HXIMATESDK_API __declspec(dllexport)
#else
#define HXIMATESDK_API __declspec(dllimport)
#endif
#endif

//密码键盘类型
#define PINPAD_MODEL_KMY                0   //凯明扬-KMY3512
#define PINPAD_MODEL_XYD                1   //信雅达-P90
#define PINPAD_MODEL_SZB                2   //苏州银行定制-深圳科羽
#define PINPAD_MODEL_HX                 3   //华信imate401

//加密算法
#define  ALGO_ENCRYPT  1
#define  ALGO_DECRYPT  2

// 设置支持的Pinpad类型，目前支持PINPAD_MODEL_KMY，PINPAD_MODEL_XYD，PINPAD_MODEL_SZB
// in  : model   :密码键盘类型
//				   PINPAD_MODEL_KMY
//			       PINPAD_MODEL_XYD
//				   PINPAD_MODEL_SZB
//				   PINPAD_MODEL_HX
HXIMATESDK_API void HXiMate_PinPad_SetModel(int model);

// Pinpad取消输入
HXIMATESDK_API void HXiMate_PinPad_Cancel();


//Pinpad上电 (通讯波特率为9600 校验方式 0）
//ret :     0   :    成功
//         其它 :    失败 (上电失败时,调用HXiMate_PinPad_GetError获取错误信息)
HXIMATESDK_API int HXiMate_PinPad_PowerOn();

//Pinpad下电
//ret :     0   :    成功
//         其它 :    失败 (操作失败时,调用HXiMate_PinPad_GetError获取错误信息)
HXIMATESDK_API int HXiMate_PinPad_PowerOff();

//获取Pinpad的版本号信息
//out :  version	 :   Pinpad版本信息
//ret :  >0          :  version数据长度
//        其它		 :   失败 (操作失败时,调用HXiMate_PinPad_GetError获取错误信息)
HXIMATESDK_API int HXiMate_PinPad_Version(unsigned char *version);

//获取Pinpad相关操作失败情况下的错误信息
//out  :   error   :错误信息
HXIMATESDK_API void HXiMate_PinPad_GetError(wchar_t *error);

//Pinpad复位自检
//in    :    initFlag    :  true清除Pinpad中的密钥，false不清除密钥
//ret   :    0           :  成功
//			 其它		 :  失败 (操作失败时,调用HXiMate_PinPad_GetError获取错误信息)
HXIMATESDK_API int HXiMate_PinPad_Reset(bool initFlag);

//Pinpad下装主密钥
//in    :  is3des            :  是否采用3DES算法，false表示使用DES算法
//         index		     :	主密钥索引
//         masterKey		 :  主密钥
//         masterLength		 :  主密钥长度
//ret   :    0               :  成功
//         其它              :  失败 (操作失败时,调用HXiMate_PinPad_GetError获取错误信息)  
HXIMATESDK_API int HXiMate_PinPad_DownloadMasterkey(int algorithm, int index, unsigned char *masterKey, int masterLength);

//Pinpad下装工作密钥(主密钥加密）
//in    :   is3des			 : 是否采用3DES算法，false表示使用DES算法
//			masterIndex		 : 主密钥索引
//			workingIndex	 : 工作密钥索引	
//			workingKey		 : 工作密钥
//			workingLength    : 工作密钥长度
//ret   :    0               :  成功
//         其它              :  失败 (操作失败时,调用HXiMate_PinPad_GetError获取错误信息)  
HXIMATESDK_API int HXiMate_PinPad_DownloadWorkingkey(int algorithm, int masterIndex, int workingIndex, unsigned char *workingKey, int workingLength);

//Pinpad输入密码（PinBlock）
//in    :	is3des			: 是否采用3DES算法，false表示使用DES算法
//			isAutoReturn	: 输入到约定长度时是否自动返回（不需要按Enter); true :自动返回 false :需要按Enter
//		:	masterIndex		: 主密钥索引
//			workingIndex	: 工作密钥索引
//			cardNo			: 卡号/帐号（最少12位数字）
//			pinLength		: 需要输入PIN的长度
//			timeout			: 输入密码等待超时时间 <= 255 秒
//out   :   pin				: 加密后的pinblock
//ret   :    >0             : pinblock长度
//         其它             : 失败 (操作失败时,调用HXiMate_PinPad_GetError获取错误信息)  
HXIMATESDK_API int HXiMate_PinPad_InputPinBlock(int algorithm, bool isAutoReturn, int masterIndex, int workingIndex, unsigned char *cardNo, unsigned char *pin, int pinLength, int timeout);

//Pinpad加解密数据
//in	:	is3des			: 是否采用3DES算法，false表示使用DES算法
//			algo			: 算法,取值: ALGO_ENCRYPT, ALGO_DECRYPT, 以ECB方式进行加解密运算
//			masterIndex		: 主密钥索引
//			workingIndex	: 工作密钥索引，如果工作密钥索引取值-1，使用主密钥索引指定的主密钥进行加解密
//			data			: 加解密数据
//			datalength		: 加解密数据的长度,要求8的倍数并小于或等于248字节长度
//out	:	outData			: 加密后的数据
//ret	:    >0             : outData加密数据长度
//         其它             : 失败 (操作失败时,调用HXiMate_PinPad_GetError获取错误信息) 
HXIMATESDK_API int HXiMate_PinPad_Encrypt(int algorithm, int algo, int masterIndex, int workingIndex, unsigned char *data, unsigned char *outData, int datalength);

//Pinpad数据MAC运算（ANSIX9.9）
//in	:	is3des			: 是否采用3DES算法，false表示使用DES算法
//			masterIndex		: 主密钥索引
//			workingIndex	: 工作密钥索引，如果工作密钥索引取值-1，使用主密钥索引指定的主密钥进行加解密
//			data			: 计算Mac原数据
//			datalength		: Mac原数据的长度,要求8的倍数并小于或等于246字节长度
//out	:	outData			: Mac数据
//ret	:    >0             : Mac数据长度
//         其它             : 失败 (操作失败时,调用HXiMate_PinPad_GetError获取错误信息) 
HXIMATESDK_API int HXiMate_PinPad_Mac(int algorithm, int masterIndex, int workingIndex, unsigned char *data, unsigned char *outData, int datalength);

//Pinpad Hash 运算
//in	:	is3des			: 是否采用3DES算法，false表示使用DES算法
//			data			: 计算数据
//			datalength		: 数据的长度
//out	:	outData			: 分散后的数据
//ret	:    >0             : 分散后的数据长度
//         其它             : 失败 (操作失败时,调用HXiMate_PinPad_GetError获取错误信息) 
HXIMATESDK_API int HXiMate_PinPad_Hash(int hashAlgorithm, unsigned char *data, int datalength, unsigned char *outData);

#endif