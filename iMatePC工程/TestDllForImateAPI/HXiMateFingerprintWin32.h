#ifndef _HXIMATEFINGERPRINTWIN32_H
#define _HXIMATEFINGERPRINTWIN32_H

#ifndef HXIMATESDK_API
#ifdef HXIMATESDK_EXPORTS
#define HXIMATESDK_API __declspec(dllexport)
#else
#define HXIMATESDK_API __declspec(dllimport)
#endif
#endif

#define FINGERPRINT_MODEL_JSABC         0   //江苏银行定制模块
#define FINGERPRINT_MODEL_SHENGTENG     1   //升腾定制（天诚盛业指纹模块）
#define FINGERPRINT_MODEL_ZHONGZHENG    2   //中正


// 设置支持的指纹仪类型
// in   :   model					     :指纹模块类型
//          FINGERPRINT_MODEL_JSABC      ://江苏银行定制模块
//			FINGERPRINT_MODEL_SHENGTENG  ://升腾定制（天诚盛业指纹模块）
//			FINGERPRINT_MODEL_ZHONGZHENG ://中正
HXIMATESDK_API void HXiMate_Fingerprint_SetModel(int model);

//取消 获取指纹特征
HXIMATESDK_API void HXiMate_Fingerprint_Cancel();

//Fingerprint上电 (通讯波特率为9600 校验方式 0)
//ret :     0   :    成功
//         其它 :    失败 (上电失败时,调用HXiMate_Fingerprint_GetError获取错误信息)
HXIMATESDK_API int HXiMate_Fingerprint_PowerOn();

//Fingerprint下电
//ret :     0   :    成功
//         其它 :    失败 (操作失败时,调用HXiMate_Fingerprint_GetError获取错误信息)
HXIMATESDK_API int HXiMate_Fingerprint_PowerOff();

//获取Fingerprint模块的版本号信息
//out :  version  :   Fingerprint版本信息
//ret :  >0       :   version长度
//        其它    :   失败 (操作失败时,调用HXiMate_Fingerprint_GetError获取错误信息)
HXIMATESDK_API int HXiMate_Fingerprint_Version(unsigned char *version);

//采集指纹特征值
//out : 特征值数据
//ret :  >0        :   FeatureData特征值长度
//         其它   :   失败 (操作失败时,调用HXiMate_Fingerprint_GetError获取错误信息)
HXIMATESDK_API int HXiMate_Fingerprint_TakeFeature(unsigned char *FeatureData);

//获取Fingerprint相关操作失败情况下的错误信息
//out  :   error   :错误信息
HXIMATESDK_API void HXiMate_Fingerprint_GetError(wchar_t *error);


#endif