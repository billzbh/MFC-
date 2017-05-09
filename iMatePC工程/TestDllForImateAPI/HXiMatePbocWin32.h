#ifndef _HXIMATEPBOCWIN32_H
#define _HXIMATEPBOCWIN32_H

#ifndef HXIMATESDK_API
#ifdef HXIMATESDK_EXPORTS
#define HXIMATESDK_API __declspec(dllexport)
#else
#define HXIMATESDK_API __declspec(dllimport)
#endif
#endif

// EMV客户高层接口返回码
#define HXPBOC_HIGH_OK				0  // OK
#define HXPBOC_HIGH_PARA			1  // 参数错误
#define HXPBOC_HIGH_NO_CARD			2  // 无卡
#define HXPBOC_HIGH_NO_APP			3  // 无支持的应用
#define HXPBOC_HIGH_CARD_IO			4  // 卡操作错
#define HXPBOC_HIGH_CARD_SW			5  // 非法卡指令状态字
#define HXPBOC_HIGH_DENIAL			6  // 交易被拒绝
#define HXPBOC_HIGH_TERMINATE		7  // 交易被终止
#define HXPBOC_HIGH_OTHER			8  // 其它错误

// 检测CPU卡片是否存在
// in  :	iSlot		:0 用户卡， 4 SAM卡
// ret :	0			:不存在
// 			1			:存在
HXIMATESDK_API int  HXiMate_PbocTest(int iSlot);

// 对CPU卡进行复位操作
// in	:	iSlot			: 0 用户卡, 4 SAM卡
// out  :	resetDataBytes	: 复位数据
// ret	    :   >0          : 复位数据长度; 
//             <=0          : 复位出错
HXIMATESDK_API int  HXiMate_PbocReset(int iSlot, unsigned char *resetDataBytes);

// 关闭卡片
// in  :	iSlot		:	0 用户卡, 4 SAM卡
// ret	:   不关心
HXIMATESDK_API int HXiMate_PbocClose(int iSlot);

// 执行APDU指令
// in  : iSlot			: 0 用户卡， 4 SAM卡
//		 iInLen   		: Apdu指令长度
// 		 pIn     		: Apdu指令, 格式: Cla Ins P1 P2 Lc DataIn Le
// out : piOutLen 		: Apdu应答长度
//       pOut    		: Apdu应答, 格式: DataOut Sw1 Sw2
// ret : >0				: 卡操作成功,pOut数据长度
//       其它			: 卡操作错
HXIMATESDK_API int HXiMate_PbocExchangeApdu(int iSlot, int iInLen, unsigned char *pIn, unsigned char *pOut);


// 设置IC读卡器类型, 不设置缺省为0
// in :  cardReaderType     :读卡器类型, 0 芯片读卡器, 1射频读卡器, >=4 SAM卡座,
HXIMATESDK_API void HXiMate_SetCardReaderType(int cardReaderType);

//获取卡号，持卡人姓名，持卡人证件号码，应用失效日期、卡序列号
//***该操作需执行之前成功执行了HXiMate_PbocReset操作*****
// out : cardInfo 	: 当读卡信息成功时，输出卡信息，包括获取卡号，持卡人姓名，持卡人证件号码，应用失效日期、卡序列号
//					  各字段之间用","间隔
// ret : 0          : 卡操作成功
//       其他       : 读卡失败
HXIMATESDK_API int HXiMate_PbocReadInfo(char *cardInfo);

// 读取PBOC卡日志的操作请求，该操作需执行之前成功执行了HXiMate_PbocReset操作
// out : cardLog 	: 当读卡日志成功时，输出卡日志信息，retCode > 0，返回的每条日志的长度为strlen(cardLog)/RetCode
// ret : >=0        : 读卡日志成功，返回日志条目个数
//       其他       : 读卡日志失败
HXIMATESDK_API int HXiMate_PbocReadLog(char *cardLog);

//获取PBOC卡相关错误信息
// out : errorInfo  :错误信息
// ret : >0         :errorInfo长度
HXIMATESDK_API int HXiMate_PbocGetError(char *errorInfo);

// 核心初始化
// in  : pszMerchantId   : 商户号[15]
//		 pszTermId       : 终端号[8]
//		 pszMerchantName : 商户名字[40]
//		 uiCountryCode   : 终端国家代码, 1-999
//		 uiCurrencyCode  : 交易货币代码, 1-999
// ret : 0				 : 成功
//       其他		     : 失败，错误码参考 EMV客户高层接口返回码
HXIMATESDK_API int HXiMate_Pboc_iHxPbocHighInitCore(char *pszMerchantId, char *pszTermId, char *pszMerchantName, unsigned int uiCountryCode, unsigned int uiCurrencyCode);

// 交易初始化
// in  : pszDateTime  : 交易日期时间[14], YYYYMMDDhhmmss
//       ulAtc        : 终端交易流水号, 1-999999
//       ucTransType  : 交易类型, 0x00 - 0xFF
//       pszAmount    : 交易金额[12]
// out : pszField55   : 组装好的55域内容, 十六进制可读格式, 预留513字节长度
//       pszPan       : 主账号[19], 可读格式
//       piPanSeqNo   : 主账号序列号, 0-99, -1表示不存在
//       pszTrack2    : 二磁道等效数据[37], 3x格式, 长度为0表示不存在
//       pszExtInfo   : 其它数据, 返回的项目包括: 姓名、证件类型、证件号、应用失效日期、一磁道数据、现金余额、余额上限, 各项目按照上述顺序，数据之间用逗号分割。
// ret : 0			  : 成功
//       其他		  : 失败，错误码参考 EMV客户高层接口返回码
HXIMATESDK_API int HXiMate_Pboc_iHxPbocHighInitTrans(char *pszDateTime, unsigned long ulAtc, unsigned char ucTransType, char *pszAmount,
	char *pszField55, char *pszPan, int piPanSeqNo, char *pszTrack2, char *pszExtInfo);

// 完成交易
// in  : pszIssuerData  : 后台数据, 十六进制可读格式
// out : pszField55     : 组装好的55域内容, 二进制格式, 预留513字节长度
// ret : 0				: 成功
//       其他		    : 失败，错误码参考 EMV客户高层接口返回码
// Note: 除了返回HXPBOC_HIGH_OK外, 返回HXPBOC_HIGH_DENIAL也会返回脚本结果
HXIMATESDK_API int HXiMate_Pboc_iHxPbocHighDoTrans(char* pszIssuerData, char* pszField55);

#endif