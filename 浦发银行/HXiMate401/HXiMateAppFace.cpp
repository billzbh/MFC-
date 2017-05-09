

#include "HidFace.h"
#include <Windows.h>
#include "des.h"
#include "str.h"

#define VENDOR_ID		0x0483
#define PRODUCT_ID		0x5750
#define ALGO_ENCRYPT    0
#define ALGO_DECRYPT    1

extern int getSN(unsigned char* termSN);
extern int WriteTermId(unsigned char* termId,int length);
extern int WriteBankID(unsigned char* bankId,int length);
extern int LoadHXpinpadMasterKey(int algorithm,int index,unsigned char *masterKey,int keyLength);
extern int encrypt(int algorithm,int cryptoMode,int masterIndex ,int workingIndex, unsigned char *data,int dataLength,unsigned char*outData);

int FINGERPRINT_COMM_PORT;      //3           //指纹模块连接iMate内部端口号-通讯编号
int FINGERPRINT_POWER_PORT;     //2           //指纹模块连接iMate内部端口号-电源编号



//	功能:	检测当前连接至客户端工具的终端设备
//	输入:	无
//	输出:	buffer		设备列表（设备序列号16位,不足补0）
//	如"AAAAAAAABBBBBBBB"，表示检测到设备序列号为AAAAAAAABBBBBBBB
//	返回:	0成功	!0失败
_declspec(dllexport)  int __stdcall detectClientDev (char* buffer){
	
	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0)
			return -2;
	}

	//打开成功，查询设备序列号
	unsigned char SNbuffer[25];
	memset(SNbuffer,0,sizeof(SNbuffer));
	if(getSN(SNbuffer)!=0)
		return 3;
	
	unsigned char byte16[16]={0};
	memcpy(byte16,SNbuffer+14,10);
	memcpy(buffer,byte16,16);
	return 0;
}


// 获取终端设备主密钥校验值

//输出checkValue

// 返回：0无效数据包；1处理成功

extern "C" _declspec(dllexport) int  getMKcheckValue(char* checkValue)
{
	
	unsigned char ZeroData[8]={0};
	unsigned char encodeData[8]={0};
	int ret = encrypt(1,ALGO_ENCRYPT,0 ,-1, ZeroData,8,encodeData);
	if(ret!=0)
		return 0;

	memcpy(checkValue,encodeData,8);
	return 1;
}



// 对终端设备USB接口从接收到的密钥分发器数据包进行分析处理
// 输入：shopNum		 “15位商户号”（保留，不使用）,
//	     clientDevID     “终端POS号”（浦发行使用 8字节）,
//	     bankID          “分行号”（保留，不使用）,
//		 keyValue		 “主密钥明文”，3DES算法，字符串长度 32
//
// 返回：0无效数据包；1处理成功
extern "C" _declspec(dllexport) unsigned char dispatchKeyData(char*shopNum, char*clientDevID,char* bankID,char* keyValue)
{
	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0)
			return -9;
	}
	
	//写商户号
	
	
	//写终端号
	int length = strlen(clientDevID);
	if(length == 0 || length !=8)
		return 0;

	int ret = WriteTermId((unsigned char*)clientDevID,8);
	if(ret!=0)
		return -1;
	
	//写分行号
	length = strlen(bankID);
	if(length == 0 || length !=2)
		return 0;
	ret = WriteBankID((unsigned char*)bankID,2);
	if(ret!=0)
		return -9;
	
	//2. 下载主密钥
	//ret = LoadHXpinpadMasterKey(1,0,(unsigned char*)keyValue,16);
	// 将'0'~'9'、'A'~'F'字符组成的字符串两两组合为Hex十六进制数，返回hex数长度
	unsigned char hex[16]={0};
	DES_StringToHex (keyValue, 32, hex);
	ret = LoadHXpinpadMasterKey(1,0,hex,16);
	if(ret!=0)
		return -3;



/*

	//浦发银行测试用，每次都直接初始化主密钥为16个0( 正式发布的需要注释掉 )
//	ret = 0;
	if(ret!=2)
	{
		unsigned char masterKey[16];
		memset(masterKey,0,sizeof(masterKey));
		ret = LoadHXpinpadMasterKey(1,1,masterKey,16);
		if(ret!=0)
			return -3;

		//1.先解密主密钥keyValue的明文
		unsigned char PlainKeyData[16];
		ret = encrypt(1,ALGO_DECRYPT,1 ,-1, (unsigned char*)keyValue,16,PlainKeyData);
		if(ret!=0)
			return -5;

		//2. 下载主密钥
		ret = LoadHXpinpadMasterKey(1,1,PlainKeyData,16);
		if(ret!=0)
			return -3;

		ret = WriteInitFlag();
		if(ret!=0)
			return -4;

	}else{
		//1.先解密主密钥keyValue的明文
		unsigned char PlainKeyData[16];
		ret = encrypt(1,ALGO_DECRYPT,1 ,-1, (unsigned char*)keyValue,16,PlainKeyData);
		if(ret!=0)
			return -5;

		//2. 下载主密钥
		ret = LoadHXpinpadMasterKey(1,1,PlainKeyData,16);
		if(ret!=0)
			return -3;
	}
*/
	return 1;
}


static int iHIDSendReceive(unsigned char *sendBuff, int sendLength, 
						unsigned char *receiveBuff, int *receiveLength, 
						int timeout)
{
	if ( iGetHidHandle() < 0)
		return IMATE_PORT_NOTOPEN;

	int ret = IMATE_COMM_ERROR;

	unsigned char packBuffer[512];
	packBuffer[0] = 0x02;
	packBuffer[1] = sendLength;

	unsigned char ucCheck = 0x03;
	for (int i = 0; i < sendLength; i++) {
		packBuffer[i+2] = sendBuff[i];
		ucCheck ^= sendBuff[i];
	}
	packBuffer[2+sendLength] = 0x03;
	packBuffer[3+sendLength] = ucCheck;

	ucHidReset();
	if (ucHidSendBuf(packBuffer, sendLength + 4))
		return IMATE_COMM_ERROR;

	long t = GetTickCount();
	unsigned char buffer[10];
	while (1) {
		if ( GetTickCount()-t > (ULONG)timeout*1000)
			return IMATE_COMM_ERROR;
		Sleep(1);
		if (ucHidTest() == 0)
			continue;
		if (ucHidGetBufBymillsecond(buffer, 1, 20))
			return IMATE_COMM_ERROR;
		if (buffer[0] != 0x02)
			continue;

		if (ucHidGetBufBymillsecond(buffer, 1, 20))
			return IMATE_COMM_ERROR;

		int length = buffer[0];
		if (ucHidGetBuf(receiveBuff, length, 1))
			return IMATE_COMM_ERROR;

		ucHidReset();
		*receiveLength = length;
		break;
	}
	return 0;
}


int getSN(unsigned char* termSN)
{
	unsigned char receivedBytes[50];
    unsigned char sendBytes[1];
	memset(receivedBytes, 0, sizeof(receivedBytes));
	sendBytes[0] = 0x04;
	int outlength=0;
	int iRet = iHIDSendReceive(sendBytes,1,receivedBytes,&outlength,2);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
	 }

	 memcpy(termSN,receivedBytes+1,25);
	 return 0;
}

int WriteTermId(unsigned char* termId,int length)
{

	unsigned char receivedBytes[50];
    unsigned char sendBytes[50];
	memset(sendBytes, 0, sizeof(sendBytes));
	sendBytes[0] = 0x68;
    sendBytes[1] = 0x04;
    sendBytes[2] = 0;
    sendBytes[3] = 20;
    sendBytes[4] = 0;
    sendBytes[5] = length;//len
	memset(sendBytes+6, 0, length);
    memcpy(sendBytes+6, termId, length);

	int outlength=0;
	int iRet = iHIDSendReceive(sendBytes,6+length,receivedBytes,&outlength,2);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
	 }
	 return 0;
}



int WriteBankID(unsigned char* bankId,int length)
{
	unsigned char receivedBytes[50];
    unsigned char sendBytes[50];
	memset(sendBytes, 0, sizeof(sendBytes));
	sendBytes[0] = 0x68;
    sendBytes[1] = 0x04;
    sendBytes[2] = 0;
    sendBytes[3] = 34;//offset
    sendBytes[4] = 0;
    sendBytes[5] = length;//len
	memset(sendBytes+6, 0, length);
    memcpy(sendBytes+6, bankId, length);

	int outlength=0;
	int iRet = iHIDSendReceive(sendBytes,6+length,receivedBytes,&outlength,2);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
	 }
	 return 0;
}

/*
int ReadBankID()
{
	unsigned char receivedBytes[50];
    unsigned char sendBytes[50];
	memset(sendBytes, 0, sizeof(sendBytes));
	sendBytes[0] = 0x68;
    sendBytes[1] = 0x03;
    sendBytes[2] = 0;
    sendBytes[3] = 34;//offset
    sendBytes[4] = 0;
    sendBytes[5] = 2;//len

	int outlength=0;
	int iRet = iHIDSendReceive(sendBytes,6,receivedBytes,&outlength,2);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
	 }
	 
	 if(receivedBytes[1]=='O'&&receivedBytes[2]=='K')
		return 2;
	 else 
		return 1;
}
*/

int LoadHXpinpadMasterKey(int algorithm,int index,unsigned char *masterKey,int keyLength)
{
	 unsigned char receivedBytes[50];
     unsigned char sendBytes[50];
        
     sendBytes[0] = (char)0x90;
     sendBytes[1] = 0; //dowload master key
     sendBytes[2] = (char)index;
     memcpy(sendBytes + 3, masterKey, keyLength);
     if (keyLength == 8)
         memcpy(sendBytes + 3 + 8, masterKey, keyLength);

	 int outlength=0;
	 int iRet = iHIDSendReceive(sendBytes,3+16,receivedBytes,&outlength,2);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
	 }
	 return 0;
}


int LoadHXpinpadWorkingKey(int algorithm,int MasterIndex,int WorkIndex,unsigned char * workKey,int keyLength)
{
	 unsigned char receivedBytes[50];
     unsigned char sendBytes[50];
        
     sendBytes[0] = (char)0x90;
     sendBytes[1] = 1; //dowload working key
     sendBytes[2] = (char)MasterIndex;
	 sendBytes[3] = (char)WorkIndex;
     memcpy(sendBytes + 4, workKey, keyLength);
     if (keyLength == 8)
         memcpy(sendBytes + 4 + 8, workKey, keyLength);

	 int outlength=0;
	 int iRet = iHIDSendReceive(sendBytes,4+16,receivedBytes,&outlength,2);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
	 }
	 return 0;
}

/**
 * Pinpad加解密数据
 * algorithm       算法，0：DES，1：3DES，2：SM4
 * cryptoMode      加解密方式，取值: ALGO_ENCRYPT, ALGO_DECRYPT, 以ECB方式进行加解密运算
 * masterIndex		主密钥索引
 * workingIndex	工作密钥索引，如果工作密钥索引取值-1，使用主密钥索引指定的主密钥进行加解密
 * data			加解密数据
 * dataLength		加解密数据的长度,要求8的倍数并小于或等于248字节长度
 */
int encrypt(int algorithm,int cryptoMode,int masterIndex ,int workingIndex, unsigned char *data,int dataLength,unsigned char*outData)
{
	 unsigned char receivedBytes[256+20];
     unsigned char sendBytes[256+20];
     
	 char theIndex = workingIndex;
     if (workingIndex < 0)
		theIndex = (char)0xff;

     sendBytes[0] = 0x90;
     sendBytes[1] = 5; //encrypt
     sendBytes[2] = algorithm;
     sendBytes[3] = cryptoMode;
     sendBytes[4] = masterIndex;
     sendBytes[5] = theIndex;
     sendBytes[6] = dataLength / 256;
     sendBytes[7] = dataLength % 256;
     memcpy(sendBytes+8, data,  dataLength);

	 int outlength = 0;
	 int iRet = iHIDSendReceive(sendBytes,8+dataLength,receivedBytes,&outlength,5);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
	 }
	 if(outlength<9)
	 {
		return -3;//加解密数据出错
	 }

	 memcpy(outData,receivedBytes+1,outlength-1);	
	 return 0;
}


//获取imate版本号
int getImateVersion(char* version)
{
	 unsigned char receivedBytes[50];
     unsigned char sendBytes[2];
        
     sendBytes[0] = (char)0x60;
     sendBytes[1] = 1; //imate ic卡操作启用超频模式-6MHz

	 int outlength=0;
	 int iRet = iHIDSendReceive(sendBytes,2,receivedBytes,&outlength,2);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
	 }
	 memcpy(version,receivedBytes+1,outlength-1);
	 return 0;
}


//设置com 穿透
int setComDeviceThroughout(long CustomBaund)
{
	char version[50]={0};
	int ret = getImateVersion(version);
	if(ret!=0)
		return ret;
	if (memcmp(version, "IMATEMINI", 9) == 0 ||
        memcmp(version, "IMATE5.0", 8) == 0||
        memcmp(version, "IMATEIV", 7) == 0) {
        FINGERPRINT_COMM_PORT = 4;           //iMate指纹模块连接iMate内部端口号-通讯编号 = UART3_FP
        FINGERPRINT_POWER_PORT = 4;          //iMate指纹模块连接iMate内部端口号-电源编号 = Vuart_FP
    }
    else if (memcmp(version, "IMATEIII", 8) == 0) {
        FINGERPRINT_COMM_PORT = 5;           //缺省值：iMate301指纹模块连接iMate内部端口号-通讯编号
        FINGERPRINT_POWER_PORT = 4;          //缺省值：iMate301指纹模块连接iMate内部端口号-电源编号
    }
    else if (memcmp(version, "IMATE", 5) == 0) {
        FINGERPRINT_COMM_PORT = 3;           //缺省值：iMate指纹模块连接iMate内部端口号-通讯编号
        FINGERPRINT_POWER_PORT = 2;          //缺省值：iMate指纹模块连接iMate内部端口号-电源编号
    }

	unsigned char receivedBytes[50];
    unsigned char sendBytes[9];
        
    sendBytes[0] = (char)0x6A;
	sendBytes[1] = (byte)FINGERPRINT_COMM_PORT;
	sendBytes[2] = 5;		
	sendBytes[3] = (byte)((CustomBaund >> 24) % 256);
	sendBytes[4] = (byte)((CustomBaund >> 16) % 256);
	sendBytes[5] = (byte)((CustomBaund >> 8) % 256);
	sendBytes[6] = (byte)(CustomBaund % 256);
	sendBytes[7] = (byte)(0);//COMM_NONE
	sendBytes[8] = (byte)(FINGERPRINT_POWER_PORT);

	int outlength=0;
	int iRet = iHIDSendReceive(sendBytes,9,receivedBytes,&outlength,2);
	if (iRet > 0 && receivedBytes[0]) {
		return -2;//发生错误
	}
	if(iRet==-1)
	{
		return -1;//超时
	}
	return 0;
}