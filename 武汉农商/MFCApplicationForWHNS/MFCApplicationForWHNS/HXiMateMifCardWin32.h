#ifndef _HXIMATEMIFCARDWIN32_H
#define _HXIMATEMIFCARDWIN32_H

#ifndef HXIMATESDK_API
#ifdef HXIMATESDK_EXPORTS
#define HXIMATESDK_API __declspec(dllexport)
#else
#define HXIMATESDK_API __declspec(dllimport)
#endif
#endif

// 检测射频卡
// out      :serialNo			: 卡片系列号
// return   : >0				: 成功, 卡片系列号字节数
//             0				: 失败
HXIMATESDK_API unsigned int HXiMate_MifCard(unsigned char *serialNo);

// 对CPU射频卡进行复位操作
// out      :   resetDataBytes	: 复位数据
// return   :   >0				: 复位数据长度; 
//              0				: 复位出错
HXIMATESDK_API unsigned int  HXiMate_MifReset(unsigned char *resetDataBytes);

// MIF CPU卡APDU
// in     :  psApduIn			: apdu命令串
//			 uiInLen			: apdu命令串长度
//			 psApduOut			: apdu返回串
//			 puiOutLen			: apdu返回串长度
// return :   0					: 成功
//           其它				: 失败
HXIMATESDK_API unsigned int HXiMate_MifApdu(unsigned char *apduIn, unsigned int uiInLen, unsigned char *apduOut, unsigned int OutLen);

// M1卡扇区认证
// in      :  ucSecNo			: 扇区号
//			  ucKeyAB			: 钥类型，0x00：A密码，0x04: B密码
//			  psKey				: 6字节的密钥
// return  :  0					: 成功
//            其它				: 失败
HXIMATESDK_API unsigned int HXiMate_MifAuth(unsigned char ucSecNo, unsigned char ucKeyAB, unsigned char *key);

// M1卡读数据块
// in         :  ucSecNo	    : 扇区号
//			     ucBlock        : 块号
// out        :  data		    : 16字节的数据
// return     :  0              : 成功
//               其它           : 失败
HXIMATESDK_API unsigned int HXiMate_MifReadBlock(unsigned char ucSecNo, unsigned char ucBlock, unsigned char *data);

// M1卡写数据块
// in        :   ucSecNo		: 扇区号
//			     ucBlock		: 块号
//			     data			: 16字节的数据
// return    :    0				: 成功
//               其它			: 失败
HXIMATESDK_API unsigned int HXiMate_MifWriteBlock(unsigned char ucSecNo, unsigned char ucBlock, unsigned char *data);

// M1钱包加值
// in        :  ucSecNo			: 扇区号
//			    ucBlock			: 块号
//			    ulValue			: 值
// return    :  0				: 成功
//              其它			: 失败
HXIMATESDK_API unsigned int HXiMate_MifIncrement(unsigned char ucSecNo, unsigned char ucBlock, unsigned long ulValue);

// M1钱包减值
// in    :   ucSecNo			: 扇区号
//			 ucBlock			: 块号
//			 ulValue			: 值
// return:   0					: 成功
//           其它				: 失败
HXIMATESDK_API unsigned int HXiMate_MifDecrement(unsigned char ucSecNo, unsigned char ucBlock, unsigned long ulValue);

// M1卡块拷贝
// in    ：  ucSrcSecNo			: 源扇区号
//			 ucSrcBlock			: 源块号
//			 ucDesSecNo			: 目的扇区号
//			 ucDesBlock			: 目的块号
// return：  0					: 成功
//           其它				: 失败
HXIMATESDK_API unsigned int HXiMate_MifCopy(unsigned char ucSrcSecNo, unsigned char ucSrcBlock, unsigned char ucDesSecNo, unsigned char ucDesBlock);


#endif
