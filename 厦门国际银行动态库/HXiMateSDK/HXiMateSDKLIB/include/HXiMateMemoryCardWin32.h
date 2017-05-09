#ifndef _HXIMATEMEMORYCARDWIN32_H
#define _HXIMATEMEMORYCARDWIN32_H

#ifndef HXIMATESDK_API
#ifdef HXIMATESDK_EXPORTS
#define HXIMATESDK_API __declspec(dllexport)
#else
#define HXIMATESDK_API __declspec(dllimport)
#endif
#endif

#define	CARD_VOLTAGE_1V8		0
#define	CARD_VOLTAGE_3V0		1
#define	CARD_VOLTAGE_5V0		2

#define SLE4442_TYPE            0x01
#define AT102_TYPE              0x02
#define AT1608_TYPE             0x04
#define AT1604_TYPE             0x05
#define SLE4428_TYPE            0x06
#define AT24Cxx_TYPE            0x10


// 检测卡片是否存在
// ret : 	0  : 不存在
// 			1  : 存在
//          99 : 不支持该功能
HXIMATESDK_API int HXiMate_MemoryTestCard();

// 检测存储卡类型(4442\102\1604\1608\4428\24Cxx)
// out   : psResetData 复位数据 < 100bytes, 第一个字节为长度
// ret   : 0xff		不识别的卡
//		   其它
//					SLE4442_TYPE		0x01
//					AT102_TYPE			0x02
//					AT1608_TYPE			0x04
//					AT1604_TYPE			0x05
//					SLE4428_TYPE		0x06
//					AT24Cxx_TYPE		0x10
HXIMATESDK_API int HXiMate_MemoryTestCardType(unsigned char *psResetData);

// 设置卡片电压
//in     :    voltageTag  
//            CARD_VOLTAGE_1V8		0    
//            CARD_VOLTAGE_3V0		1
//            CARD_VOLTAGE_5V0		2   
HXIMATESDK_API void HXiMate_MemorySetCardVoltage(int voltageTag);

// 产生通讯密钥
// in	:	masterKeyId	: 主密钥ID, 一般使用6
//			random		: 随机数，8字节
// ret  :	0	:	成功
//			其它 ：	失败	
HXIMATESDK_API int HXiMate_MemoryGenCommKey(unsigned int masterKeyId, unsigned char* random);


//SLE4442 接口

// 打开SLE4442卡
HXIMATESDK_API void HXiMate_MemorySLE4442_Open(void);

// 打开SLE4442卡, 自动检测电压和卡类型
// ret    : 	0	 : 	成功
//			  其它	 :	失败
HXIMATESDK_API int HXiMate_MemorySLE4442_AutoOpen(void);

// 关闭SLE4442卡
HXIMATESDK_API void HXiMate_MemorySLE4442_Close(void);

// 验证SLE4442卡密码
// in      :  securityCode   :   密码数据缓冲，3字节长度
//      
// return  :  0    :   成功
//            其它 :   失败   
HXIMATESDK_API int HXiMate_MemorySLE4442_ChkCode(unsigned char *securityCode);

// 验证SLE4442卡密码(安全方式）
// in	:	securityCode  :   加密后的密码数据，8字节长度  
// ret  :	0    :   成功
//			其它 :   失败    
HXIMATESDK_API int HXiMate_MemorySLE4442_ChkCodeEx(unsigned char* securityCode);

// SLE4442读数据
// in  :
//		offset	:	数据偏移量，0~255
//		dataLen	:	数据长度
// out:
//		dataBuff:	读数据缓冲区地址, maxlength=255
HXIMATESDK_API void HXiMate_MemorySLE4442_Read(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// SLE4442写数据
// in  :
//		offset	:	数据偏移量，0~255
//		dataLen	:	数据长度
//		dataBuff:	写数据缓冲区地址, maxlength=255
HXIMATESDK_API void HXiMate_MemorySLE4442_Write(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

//SLE4428接口

// 打开SLE4428卡
HXIMATESDK_API void HXiMate_MemorySLE4428_Open(void);

// 打开SLE4428卡, 自动检测电压和卡类型
// ret    : 	0	 : 	成功
//			  其它	 :	失败
HXIMATESDK_API int HXiMate_MemorySLE4428_AutoOpen(void);

// 关闭SLE4428卡
HXIMATESDK_API void HXiMate_MemorySLE4428_Close(void);

// 验证SLE4428卡密码
// in：
//      securityCode  :   密码数据缓冲，2字节长度
// return：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemorySLE4428_ChkCode(unsigned char *securityCode);

// 验证SLE4428卡密码（安全方式）
// in  :     securityCode  :   加密后的密码数据，8字节长度
// ret  :     0   :   成功
//			其它 :   失败
HXIMATESDK_API int HXiMate_MemorySLE4428_ChkCodeEx(unsigned char *securityCode);

// SLE4428读数据
// in：
//		offset	:	数据偏移量，0~1024
//		dataLen	:	数据长度
// return：
//		dataBuff:	读数据缓冲区地址, maxlength = 1024
HXIMATESDK_API void HXiMate_MemorySLE4428_Read(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// SLE4428写数据
// in ：
//		offset	:	数据偏移量，0~1024
//		dataLen	:	数据长度
//		dataBuff:	写数据缓冲区地址, maxlength = 1024
HXIMATESDK_API void HXiMate_MemorySLE4428_Write(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);


// 打开AT102卡
HXIMATESDK_API void HXiMate_MemoryAT102_Open(void);

// 打开AT102卡, 自动检测电压和卡类型
// ret    : 	0	 : 	成功
//			  其它	 :	失败
HXIMATESDK_API int HXiMate_MemoryAT102_AutoOpen(void);

// 关闭AT102卡
HXIMATESDK_API void HXiMate_MemoryAT102_Close(void);

// 验证AT102卡密码
// 输入参数：
//      securityCode  :   密码数据缓冲，2字节长度
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT102_ChkCode(unsigned char *securityCode);

// 验证AT102卡密码（安全方式）
//  in  :    securityCode  :   加密后的密码数据，8字节长度
//  ret :    0    :   成功
//			  其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT102_ChkCodeEx(unsigned char *securityCode);

// AT102读数据(按双字节读取）
// 输入参数：
//		wordOffset :	数据偏移量，<= 128
//		wordNum	:	数据长度（字长度）, <= 128
// 输出参数：
//		dataBuff:	读数据缓冲区地址, maxlength = 256
HXIMATESDK_API void HXiMate_MemoryAT102_ReadWords(unsigned int wordOffset, unsigned int wordNum, unsigned char* dataBuff);

// AT102写数据(按双字节写卡）
// 输入参数：
//		wordOffset 	:	数据偏移量, <= 128
//		wordNum		:	数据长度（字长度）<= 128
//		dataBuff:	写数据缓冲区地址, maxlength = 256
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT102_WriteWords(unsigned int wordOffset, unsigned int wordNum, unsigned char* dataBuff);

// AT102擦除非应用区的数据
// 输入参数：
//		wordOffset 	:	数据偏移量
//		wordNum		:	删除字长度
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT102_EraseNonApp(unsigned char wordOffset, unsigned char wordNum);

// AT102擦除应用区的数据
// 输入参数：
//		area  	: 应用区	1:	应用区1	2:	应用区2
//		limited : 指出第2应用区的擦除次数是否受限制。此参数当且仅当area= 2时起作用。
//		eraseKey: 擦除密钥, 应用区1的擦除密码长度为6bytes，应用区2的擦除密码为4bytes，
//			  Security Level 1 方式下，eraseKey=NULL则不需要提供EraseKey方式进行擦除
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT102_EraseApp(unsigned int area, unsigned char limited, unsigned char *eraseKey);

// AT102擦除应用区的数据（安全方式）
// in  :
//		area  	: 应用区	1:	应用区1	2:	应用区2
//		limited : 指出第2应用区的擦除次数是否受限制。此参数当且仅当area= 2时起作用。
//		eraseKey: 擦除密钥, 加密后的应用区1的擦除密码，8字节，
//			  Security Level 1 方式下，eraseKey=NULL则不需要提供EraseKey方式进行擦除
// ret  :
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT102_EraseAppEx(unsigned int area, unsigned char limited, unsigned char *eraseKey);

// AT102读应用区数据
// 输入参数：
//		area  	: 应用区	1:	应用区1	2:	应用区2
// 输出参数：
//		dataBuff:	读数据缓冲区地址, maxlength = 64
HXIMATESDK_API void HXiMate_MemoryAT102_ReadAZ(unsigned int area, unsigned char* dataBuff);

// AT102写应用区数据
// 输入参数：
//		area  	: 应用区	1:	应用区1	2:	应用区2
//		dataBuff:	读数据缓冲区地址, maxlength = 64
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT102_WriteAZ(unsigned int area, unsigned char* dataBuff);


// AT102读测试区数据
// 输出参数：
//		dataBuff:	读数据缓冲区地址, 获取2字节数据
HXIMATESDK_API void HXiMate_MemoryAT102_ReadMTZ(unsigned char* dataBuff);

// AT102写测试区数据
// 输入参数：
//		dataBuff:	读数据缓冲区地址, 2字节数据
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT102_UpdateMTZ(unsigned char* dataBuff);


// 打开AT1604卡
HXIMATESDK_API void HXiMate_MemoryAT1604_Open(void);

// 打开AT1604卡, 自动检测电压和卡类型
// ret    : 	0	 : 	成功
//			  其它	 :	失败
HXIMATESDK_API int HXiMate_MemoryAT1604_AutoOpen(void);

// 关闭AT1604卡
HXIMATESDK_API void HXiMate_MemoryAT1604_Close(void);

// 验证AT1604卡主密码
// 输入参数：
//      securityCode  :   密码数据缓冲，2字节长度
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1604_ChkCode(unsigned char *securityCode);

// 验证AT1604卡主密码（安全方式）
// in  :
//      securityCode  :   加密后的密码数据，8字节长度
// ret  :
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1604_ChkCodeEx(unsigned char *securityCode);

// 验证AT1604卡分区密码
// 输入参数：
//		area		  :   分区号1、2、3、4
//      securityCode  :   密码数据缓冲，2字节长度
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1604_ChkAreaCode(unsigned int area, unsigned char *securityCode);

// 验证AT1604卡分区密码（安全方式）
// in  :
//		area		  :   分区号1、2、3、4
//      securityCode  :   加密后的密码数据，8字节长度
// ret  :
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1604_ChkAreaCodeEx(unsigned int area, unsigned char *securityCode);

// AT1604读数据
// 输入参数：
//		offset	:	数据偏移量，0~2047
//		dataLen	:	数据长度
// 输出参数：
//		dataBuff:	读数据缓冲区地址, maxlength = 2048
HXIMATESDK_API void HXiMate_MemoryAT1604_Read(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT1604写数据
// 输入参数：
//		offset	:	数据偏移量，0~2047
//		dataLen	:	数据长度
//		dataBuff:	写数据缓冲区地址, maxlength = 2048
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1604_Write(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT1604读分区数据
// 输入参数：
//		area	:   分区号1、2、3、4
//		offset	:	数据偏移量，0~511
//		dataLen	:	数据长度,1、2、3区512bytes 4区457bytes
// 输出参数：
//		dataBuff:	读数据缓冲区地址, maxlength = 512
HXIMATESDK_API void HXiMate_MemoryAT1604_ReadAZ(unsigned int area, unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT1604写分区数据
// 输入参数：
//		area	:   分区号1、2、3、4
//		offset	:	数据偏移量，0~511
//		dataLen	:	数据长度,1、2、3区512bytes 4区457bytes
//		dataBuff:	写数据缓冲区地址, maxlength = 512
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1604_WriteAZ(unsigned int area, unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT1604擦除数据
// 输入参数：
//		offset	:	数据偏移量，0~2047
//		dataLen	:	数据长度
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1604_Erase(unsigned int offset, unsigned int dataLen);

// AT1604擦除分区数据
// 输入参数：
//		area	:   分区号1、2、3、4
//		offset	:	数据偏移量，0~511
//		dataLen	:	数据长度,1、2、3区512bytes 4区457bytes
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1604_EraseAZ(unsigned int area, unsigned int offset, unsigned int dataLen);

// 验证AT1604卡分区擦除密码
// in  :
//		area		  :   分区号1、2、3、4
//      securityCode  :   密码数据缓冲，2字节长度
// ret  :
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1604_ChkAreaEraseCode(unsigned int area, unsigned char* securityCode);

// 验证AT1604卡分区擦除密码（安全方式）
// in  :
//		area		  :   分区号1、2、3、4
//      securityCode  :   加密后的密码数据，8字节长度
// ret  :
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1604_ChkAreaEraseCodeEx(unsigned int area, unsigned char* securityCode);

// AT1604读测试区数据
// 输出参数：
//		dataBuff:	读数据缓冲区地址, length = 2
HXIMATESDK_API void HXiMate_MemoryAT1604_ReadMTZ(unsigned char* dataBuff);

// AT1604写测试区数据
// 输入参数：
//		dataBuff:	写数据缓冲区地址, length = 2
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1604_WriteMTZ(unsigned char* dataBuff);


// 打开AT1608卡
// 输出参数：
//		resetData:	复位数据, length = 4bytes
HXIMATESDK_API void HXiMate_MemoryAT1608_Open(unsigned char *resetData);

// 打开AT1608卡, 自动检测电压和卡类型
// out    :     resetData  :复位数据
// ret    : 	0	       : 	成功
//			  其它	       :	失败
HXIMATESDK_API int HXiMate_MemoryAT1608_AutoOpen(unsigned char *resetData);

// 关闭AT1608卡
HXIMATESDK_API void HXiMate_MemoryAT1608_Close(void);

// AT1608卡设置应用区
// 输入参数：
//      az	: 应用区，0~7
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1608_SetAZ(unsigned char az);

// 验证AT1608卡读写密码
// 输入参数：
// 		index		: 密码索引号 写密码索引号(0--7) 读密码索引号(0x80--0x87)
//      securityCode: 密码数据缓冲，3字节长度
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1608_ChkCode(unsigned int index, unsigned char *securityCode);

// 验证AT1608卡读写密码（安全方式）
// in  :
// 		index		: 密码索引号 写密码索引号(0--7) 读密码索引号(0x80--0x87)
//      securityCode: 加密后的密码数据，8字节长度
// ret  :
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1608_ChkCodeEx(unsigned int index, unsigned char *securityCode);

// AT1608卡安全认证
// 输入参数：
//      gc	: 密钥，8字节长度
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1608_Auth(unsigned char *gc);

// AT1608卡安全认证（安全方式）
// in  :
//      gc	: 密钥，16字节长度
// ret  :
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1608_AuthEx(unsigned char *gc);

// AT1608读数据
// 输入参数：
//		level	:   0:应用区，1：设置区
//		offset	:	设置区(0--127)	应用区(0--255)
//		dataLen	:
// 输出参数：
//		dataBuff:	读数据缓冲区地址, maxlength = 256
HXIMATESDK_API int HXiMate_MemoryAT1608_Read(unsigned int level, unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT1608写数据
// 输入参数：
//		level	:   0:应用区，1：设置区
//		offset	:	设置区(0--127)	应用区(0--255)
//		dataLen	:
//		dataBuff:	读数据缓冲区地址, maxlength = 256
// 返回码：
//      0    :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT1608_Write(unsigned int level, unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// 读AT88SC1608熔丝状况
// 返回码   :	0    :  OK
//              1    :  Error
HXIMATESDK_API int	HXiMate_MemoryAT1608_ReadFuse(unsigned char *Fuse);


// 打开AT24Cxx卡
HXIMATESDK_API void HXiMate_MemoryAT24Cxx_Open(void);

// 打开AT24Cxx卡, 自动检测卡类型
HXIMATESDK_API int HXiMate_MemoryAT24Cxx_OpenAuto(void);

// 关闭AT24Cxx卡
HXIMATESDK_API void HXiMate_MemoryAT24Cxx_Close(void);

// AT24Cxx读数据
// 输入参数：
//		offset	:	数据偏移量
//		dataLen	:	数据长度
// 输出参数：
//		dataBuff:	读数据缓冲区地址, 根据具体情况确定
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT24Cxx_Read(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT24Cxx写数据
// 输入参数：
//		offset	:	数据偏移量
//		dataLen	:	数据长度
//		dataBuff:	写数据缓冲区地址, 根据具体情况确定
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT24Cxx_Write(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT24C32读数据
// 输入参数：
//		offset	:	数据偏移量
//		dataLen	:	数据长度
// 输出参数：
//		dataBuff:	读数据缓冲区地址, 根据具体情况确定
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT24C32_Read(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

// AT24C32写数据
// 输入参数：
//		offset	:	数据偏移量
//		dataLen	:	数据长度
//		dataBuff:	写数据缓冲区地址, 根据具体情况确定
// 返回码：
//      0   :   成功
//      其它 :   失败
HXIMATESDK_API int HXiMate_MemoryAT24C32_Write(unsigned int offset, unsigned int dataLen, unsigned char* dataBuff);

#endif
