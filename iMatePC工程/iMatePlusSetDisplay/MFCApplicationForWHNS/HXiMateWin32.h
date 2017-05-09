#ifndef _HXIMATEWIN32_H
#define _HXIMATEWIN32_H

#ifndef HXIMATESDK_API
#ifdef HXIMATESDK_EXPORTS
#define HXIMATESDK_API __declspec(dllexport)
#else
#define HXIMATESDK_API __declspec(dllimport)
#endif
#endif

//打开与iMate的连接
//in     : mode			: 	"bluetooth" 蓝牙连接   
//							"usb-hid"   USB-HID有线连接  
//ret    : 0  			:	成功
//         其它			:	失败
HXIMATESDK_API int  HXiMate_DeviceConnect(char *mode);

//关闭与iMate的连接
HXIMATESDK_API void HXiMate_DeviceClose();

//取消操作
HXIMATESDK_API void HXiMate_DeviceCancel();

// 查询iMate固件版本号
// out   :  version		:  固件版本号
//          格式		:  "A.A,B.B.B" : 硬件和固件版本，其中A为硬件版本，B为固件版本
// ret   :  >0			:  成功,version数据长度
//          其它		:  失败
HXIMATESDK_API int HXiMate_DeviceVersions(char *version);

// iMate产品序列号
// out   :   serialNo    :  产品序列号
// ret   :  >0			 :	成功,serialNo数据长度
//          其它		 :	失败
HXIMATESDK_API int HXiMate_DeviceSerialNumber(char *serialNo);

//写终端号(长度最大12个字符)
//in    :  termId 终端号字节数组
//         length 终端号长度，最大12个字节
//ret   :  >0     成功
//         -1 ：超时   -2 ：通讯错误  -3：长度错误   
HXIMATESDK_API int HXiMate_WriteTermId(unsigned char* termId, int length);

//读取终端号(长度最大12个字符)
//out    :  termId 终端号字节数组
//ret   :  >0     成功
//         -1 ：超时   -2 ：通讯错误  -3：长度错误   
HXIMATESDK_API int HXiMate_ReadTermId(unsigned char* termId);


// 取iMate电池电量
// ret	:	>0		:	电池电量值
//			<=0		:	读取失败
HXIMATESDK_API int HXiMate_DeviceBatteryLevel();

// 等待事件，包括磁卡刷卡、Pboc IC插入、放置射频卡
// in	:	eventMask	:eventMask的bit来标识等待事件
//						  0x01    等待刷卡事件
//						  0x02    等待插卡事件
//					      0x04    等待射频事件
//					      0xFF    等待所有事件
//		:	timeOut		:	最长等待时间（秒）
// out	:	outData		:	事件输出数据
// ret	:	>0			:	outData数据长度
//			<=0			:	操作失败
HXIMATESDK_API int HXiMate_WaitEvent(unsigned char eventMask, unsigned char* outData, int timeOut);

// 检测蓝牙连接是否正常，返回TRUE表示连接正常
// return   :   true    :连接正常
//              false   :未连接
HXIMATESDK_API bool HXiMate_DeviceIsConnecting();

// 检测iMate是否在工作状态，返回TRUE表示正在工作中。
// return   :   true    :正在工作中
//              false   :设备空闲
HXIMATESDK_API bool HXiMate_DeviceIsWorking();

// 读取扩展内存的操作请求
// in     :   offset     :   读扩展内存偏移量
//            length     :   读扩展内存字节数
// out    :   outData    :   扩展内存数据
// return :   0          :   读取成功
//            其它       :   读取失败
HXIMATESDK_API int  HXiMate_DeviceXmemRead(int offset, int length, unsigned char *outData);

// 写扩展内存的操作请求
// in     :   offset     :   写入数据偏移量
//			  inLength   :   写入数据长度
//            inData     :   写入数据
// return :   0          :   写入成功
//            其它       :   写入失败
HXIMATESDK_API int  HXiMate_DeviceXmemWrite(int offset, int inLength, unsigned char *indata);

// iMate蜂鸣响一声
HXIMATESDK_API void HXiMate_DeviceBuzzer();

//操作失败时,用来获取iMate错误信息
// out   : error		:错误信息
HXIMATESDK_API void HXiMate_GetError(wchar_t *error);

// 刷磁条卡
// in	:	timeOut		:最长等待时间（秒）
// out	:	track2		:二磁道数据 >= 37 +1
//			track3		:三磁道数据 >=104 +1
// ret  :	0			:读取成功
//          其它		:读取失败
HXIMATESDK_API int HXiMate_SwipeCard(wchar_t* track2, wchar_t* track3, int timeOut);

//读身份证
// in	:	timeOut		:最长等待时间（秒）
// out	:	info		:身份证信息 >=128 + 1
//			photoData	:照片数据 >= 1024
// ret    : 0           :读取成功
//          其它        :读取失败
HXIMATESDK_API int HXiMate_ReadIdCard(wchar_t* info, unsigned char* photoData, int timeOut);

//读身份证
// in	:	timeOut		:最长等待时间（秒）
// out	:	info		:身份证信息:姓名,性别,民族,生日(年,月,日),住址,身份证号码,发卡日期,有效期,其它信息;各项信息以","分隔
//			photoData	:照片数据 >= 1024
// ret    : 0           :读取成功
//          其它        :读取失败
HXIMATESDK_API int HXiMate_ReadIdCard2(wchar_t* info, unsigned char* photoData, int timeOut);


//oneTwo ，生成 hexString格式的字符串。
// in	:	Data		:源数据
//			DataLength  :数据长度
// out	:   outData	    :目标数据
// ret    : = 0         :转换成功,表示目标数据的长度
//          其它        :转换失败
HXIMATESDK_API int HXiMate_OneTwo0(unsigned char* Data,int DataLength, unsigned char* outData);


// hexString格式的字符串转为 压缩的BCD。
// in	:	Data		:源数据
//			DataLength  :数据长度
// out	:   outData	    :目标数据
// ret    : = 0         :转换成功,表示目标数据的长度
//          其它        :转换失败
HXIMATESDK_API int HXiMate_TwoOne(unsigned char* Data, int DataLength, unsigned char* outData);


//基础通讯接口
HXIMATESDK_API int HXiMate_SendReceive(unsigned char *inData, int inLength, unsigned char *outData, int *outLength, int timeOut); //timeOut：秒


HXIMATESDK_API char* HXiMate_WcharToChar(const wchar_t* wp);


HXIMATESDK_API wchar_t* HXiMate_CharToWchar(const char* c);

#endif


