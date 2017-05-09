#ifndef HXIMATEAPPFACE_H__
#define HXIMATEAPPFACE_H__


#ifdef __cplusplus
extern "C" {
#endif


//	功能:	检测当前连接至客户端工具的终端设备
//	输入:	无
//	输出:	buffer		设备列表（设备序列号16位,不足补0）
//	如"AAAAAAAABBBBBBBB"，表示检测到设备序列号为AAAAAAAABBBBBBBB
//	返回:	0成功	!0失败
_declspec(dllexport) int __stdcall detectClientDev (char* buffer);

// 对终端设备USB接口从接收到的密钥分发器数据包进行分析处理
// 输入：shopNum		 “15位商户号”（保留，不使用）,
//	     clientDevID     “终端POS号”（保留，不使用）,
//	     bankID          “分行号”（保留，不使用）,
//		 keyValue		 “密钥密文”，3DES算法，密钥长度128位，背夹初始密钥为00000000000000000000000000000000，工作密钥11111111111111111111111111111111
//
// 返回：0无效数据包；1处理成功
_declspec(dllexport) unsigned char dispatchKeyData(char*shopNum, char*clientDevID,char* bankID,char* keyValue);


// 获取终端设备主密钥校验值
//输出checkValue
// 返回：0无效数据包；1处理成功
_declspec(dllexport)  int getMKcheckValue(char* checkValue);




#ifdef __cplusplus
}
#endif

#endif
