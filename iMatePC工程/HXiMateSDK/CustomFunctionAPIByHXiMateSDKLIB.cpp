#include "CustomFunctionAPIByHXiMateSDKLIB.h"
#include "stdafx.h"
#include <HXiMateWin32.h>
#include <HXiMatePinPadWin32.h>
#include <stdlib.h>



int __stdcall SDBSetPinPadID(const char * CommPort, const char * ExtPort, const char * idnum)
{
	HXiMate_DeviceClose();
	Sleep(100);
	int ret = HXiMate_DeviceConnect("usb-hid");
	if (ret != 0)
	{
		return -1;
	}
	
	unsigned char indata[20] = {0};
	size_t len = strlen(idnum);
	if (len<0||len>20 ) {
		return -2;
	}
	memcpy(indata, idnum, strlen(idnum));
	ret = HXiMate_DeviceXmemWrite(128, 20, indata);
	if (ret != 0)
	{
		Sleep(100);
		//再试一次
		ret = HXiMate_DeviceXmemWrite(128, 20, indata);
		if (ret != 0) {
			return -3;
		}
	}
	return 0;
}

int __stdcall SDBGetPinPadID(const char * CommPort, const char * ExtPort, char * idnum)
{
	HXiMate_DeviceClose();
	Sleep(100);
	int ret = HXiMate_DeviceConnect("usb-hid");
	if (ret != 0)
	{
		return -1;
	}

	unsigned char outData[20] = {0};
	ret = HXiMate_DeviceXmemRead(128, 20, outData);
	if (ret != 0)
	{
		Sleep(100);
		ret = HXiMate_DeviceXmemRead(128, 20, outData);
		if (ret != 0)
			return -2;
	}

	memcpy(idnum, outData, 20);
	return 0;
}

int __stdcall SDBGetCompanyPinPadID(const char * CommPort, const char * ExtPort, char * SeqNo)
{
	HXiMate_DeviceClose();
	Sleep(100);
	int ret = HXiMate_DeviceConnect("usb-hid");
	if (ret != 0)
	{
		return -1;
	}

	char SN[25] = { 0 };
	ret = HXiMate_DeviceSerialNumber(SN);
	if (ret <= 0)
	{
		Sleep(100);
		ret = HXiMate_DeviceSerialNumber(SN);
		if (ret <= 0)
			return -2;
	}

	memcpy(SeqNo, SN+14,10);//截取最后10位，匹配蓝牙名的序列号
	return 0;
}

int __stdcall SDBRestDefaultKey(const char * CommPort, const char * ExtPort, const char * TMKIndex)
{
	HXiMate_DeviceClose();
	Sleep(100);
	int ret = HXiMate_DeviceConnect("usb-hid");
	if (ret != 0)
	{
		return -1;
	}

	HXiMate_PinPad_SetModel(PINPAD_MODEL_HX);
	ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		return -2;
	}
	int index = atoi(TMKIndex);
	unsigned char masterKey[16];
	memset(masterKey, 0x38, 16);
	ret=HXiMate_PinPad_DownloadMasterkey(1, index,masterKey,16);
	if (ret != 0)
	{
		Sleep(100);
		ret = HXiMate_PinPad_DownloadMasterkey(1, index, masterKey, 16);
		if (ret != 0)
		{
			return -3;
		}
	}
	return 0;
}

int __stdcall SDBInputTMK(const char * CommPort, const char * ExtPort, const char * TMKIndex, const char * Tmk1, const char * Tmk2, char * Tmkchk)
{
	HXiMate_DeviceClose();
	Sleep(100);
	int ret = HXiMate_DeviceConnect("usb-hid");
	if (ret != 0)
	{
		return -1;
	}

	HXiMate_PinPad_SetModel(PINPAD_MODEL_HX);
	ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		return -2;
	}

	int index = atoi(TMKIndex);
	int len1 = strlen(Tmk1);
	int len2 = strlen(Tmk1);
	int algo = 0;

	if (len1 != len2)
	{
		return -3;
	}
	if (len2 != 16 && len2 != 32 && len2 != 48)
	{
		return -4;
	}
	if (len2>16)
	{
		algo = 1;
	}

	//分量1
	unsigned char masterKey1[24];
	HXiMate_TwoOne((unsigned char*)Tmk1, len1, masterKey1);

	//分量2
	unsigned char masterKey2[24];
	HXiMate_TwoOne((unsigned char*)Tmk2, len2, masterKey2);

	//异或得到明文 
	unsigned char masterKey[24];
	for (int i = 0; i < len1/2; i++) {
		masterKey[i] = (masterKey1[i] ^ masterKey2[i]);
	}

	//下装主密钥明文
	ret = HXiMate_PinPad_DownloadMasterkey(algo, index, masterKey, len1/2);
	if (ret!=0)
	{
		Sleep(100);
		ret = HXiMate_PinPad_DownloadMasterkey(algo, index, masterKey, len1 / 2);
		if (ret != 0)
			return -5;
	}

	//计算校验
	unsigned char tmp[8] = { 0 };
	unsigned char outdata[128];
	ret = HXiMate_PinPad_Encrypt(algo, ALGO_ENCRYPT, index, -1, tmp, outdata, 8);
	if (ret <= 0)
	{
		return -6;
	}
	memcpy(Tmkchk, outdata,8);
	return 0;
}

int __stdcall SDBLoadTMK(const char * CommPort, const char * ExtPort, const char * NewKEYLen, const char * TMKIndex, const char * NewTMKByOldTMK, const char * KeyValue)
{
	HXiMate_DeviceClose();
	Sleep(100);
	int ret = HXiMate_DeviceConnect("usb-hid");
	if (ret != 0)
	{
		return -1;
	}

	HXiMate_PinPad_SetModel(PINPAD_MODEL_HX);
	ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		return -2;
	}

	int algo = 0;
	int index = atoi(TMKIndex);
	int keylen = strlen(NewTMKByOldTMK);
	if (keylen != 16 && keylen != 32 && keylen != 48)
	{
		return -3;
	}
	if (keylen > 16) {
		algo = 1;
	}

	//先解密
	unsigned char keydata[24] = {0};
	unsigned char MasterKeydata[24] = {0};
	HXiMate_TwoOne((unsigned char*)NewTMKByOldTMK,keylen,keydata);
	ret = HXiMate_PinPad_Encrypt(algo,ALGO_DECRYPT,index,-1, keydata, MasterKeydata,keylen/2);
	if (ret <= 0)
	{
		ret = HXiMate_PinPad_Encrypt(algo, ALGO_DECRYPT, index, -1, keydata, MasterKeydata, keylen / 2);
		if (ret <= 0)
			return -4;
	}

	//下装主密钥明文
	unsigned char plainkey[24] = { 0 };
	HXiMate_TwoOne(MasterKeydata, keylen, plainkey);
	ret = HXiMate_PinPad_DownloadMasterkey(algo, index, plainkey, keylen / 2);
	if (ret != 0)
	{
		Sleep(100);
		ret = HXiMate_PinPad_DownloadMasterkey(algo, index, plainkey, keylen / 2);
		if (ret != 0)
			return -5;
	}

	//计算校验值
	unsigned char tmp[8] = { 0 };
	unsigned char outdata[128];
	ret = HXiMate_PinPad_Encrypt(algo, ALGO_ENCRYPT, index, -1, tmp, outdata, 8);
	if (ret <= 0)
	{
		ret = HXiMate_PinPad_Encrypt(algo, ALGO_ENCRYPT, index, -1, tmp, outdata, 8);
		if (ret <= 0)
		{
			return -6;
		}
	}

	if (memcmp(outdata, KeyValue, 8) == 0)
	{
		return 0;
	}
	return -7;
}

int __stdcall SDBInputKEY(const char * CommPort, const char * ExtPort, const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, const char * KeyValue)
{
	HXiMate_DeviceClose();
	Sleep(100);
	int ret = HXiMate_DeviceConnect("usb-hid");
	if (ret != 0)
	{
		return -1;
	}

	HXiMate_PinPad_SetModel(PINPAD_MODEL_HX);
	ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		return -2;
	}

	int algo = 0;
	int TMK_index = atoi(TMKIndex);
	int KEY_index = atoi(KEYIndex);
	int keylen = strlen(keyByTMK);
	if (keylen != 16 && keylen != 32 && keylen != 48)
	{
		return -3;
	}
	if (keylen > 16) {
		algo = 1;
	}

	unsigned char keydata[24];
	HXiMate_TwoOne((unsigned char*)keyByTMK, keylen, keydata);
	//下装工作密钥秘文
	ret = HXiMate_PinPad_DownloadWorkingkey(algo, TMK_index, KEY_index, keydata, keylen / 2);
	if (ret != 0)
	{
		Sleep(100);
		ret = HXiMate_PinPad_DownloadWorkingkey(algo, TMK_index, KEY_index, keydata, keylen / 2);
		if (ret != 0)
			return -5;
	}

	//计算校验值
	unsigned char tmp[8] = { 0 };
	unsigned char outdata[128] = {0};
	ret = HXiMate_PinPad_Encrypt(algo,ALGO_ENCRYPT, TMK_index, KEY_index, tmp, outdata, 8);
	if (ret <= 0)
	{
		ret = HXiMate_PinPad_Encrypt(algo,ALGO_ENCRYPT, TMK_index, KEY_index, tmp, outdata, 8);
		if (ret <= 0)
		{
			return -6;
		}
	}

	if (memcmp(outdata, KeyValue, 8) == 0)
	{
		return 0;
	}
	return -7;
}

int __stdcall SDBInputKeyNoCheck(const char * CommPort, const char * ExtPort, const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, char * KeyValue)
{
	HXiMate_DeviceClose();
	Sleep(100);
	int ret = HXiMate_DeviceConnect("usb-hid");
	if (ret != 0)
	{
		return -1;
	}

	HXiMate_PinPad_SetModel(PINPAD_MODEL_HX);
	ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		return -2;
	}

	int algo = 0;
	int TMK_index = atoi(TMKIndex);
	int KEY_index = atoi(KEYIndex);
	int keylen = strlen(keyByTMK);
	if (keylen != 16 && keylen != 32 && keylen != 48)
	{
		return -3;
	}
	if (keylen > 16) {
		algo = 1;
	}

	unsigned char keydata[24];
	HXiMate_TwoOne((unsigned char*)keyByTMK, keylen, keydata);
	//下装工作密钥秘文
	ret = HXiMate_PinPad_DownloadWorkingkey(algo, TMK_index, KEY_index, keydata, keylen / 2);
	if (ret != 0)
	{
		Sleep(100);
		ret = HXiMate_PinPad_DownloadWorkingkey(algo, TMK_index, KEY_index, keydata, keylen / 2);
		if (ret != 0)
			return -5;
	}

	//计算校验值
	unsigned char tmp[8] = { 0 };
	unsigned char outdata[128] = { 0 };
	ret = HXiMate_PinPad_Encrypt(algo, ALGO_ENCRYPT, TMK_index, KEY_index, tmp, outdata, 8);
	if (ret <= 0)
	{
		ret = HXiMate_PinPad_Encrypt(algo, ALGO_ENCRYPT, TMK_index, KEY_index, tmp, outdata, 8);
		if (ret <= 0)
		{
			return -6;
		}
	}

	memcpy(KeyValue,outdata,8);
	return 0;
}

int __stdcall SDBCheckKEY(const char * CommPort, const char * ExtPort, const char * KEYIndex, const char * TMKIndex, char * KeyLen, char * KeyValue)
{
	HXiMate_DeviceClose();
	Sleep(100);
	int ret = HXiMate_DeviceConnect("usb-hid");
	if (ret != 0)
	{
		return -1;
	}

	HXiMate_PinPad_SetModel(PINPAD_MODEL_HX);
	ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		return -2;
	}

	int algo = 1;
	int TMK_index = atoi(TMKIndex);
	int KEY_index = atoi(KEYIndex);

	//计算校验值
	unsigned char tmp[8] = { 0 };
	unsigned char outdata[128] = { 0 };
	ret = HXiMate_PinPad_Encrypt(algo, ALGO_ENCRYPT, TMK_index, KEY_index, tmp, outdata, 8);
	if (ret <= 0)
	{
		ret = HXiMate_PinPad_Encrypt(algo, ALGO_ENCRYPT, TMK_index, KEY_index, tmp, outdata, 8);
		if (ret <= 0)
		{
			return -6;
		}
	}

	memcpy(KeyValue, outdata, 8);
	memcpy(KeyLen, "32", 3);
	return 0;
}

int __stdcall SDBReadCustomerPin(const char * CommPort, const char * ExtPort, const char * TMKIndex, const char * accNo, const char * modex, const char * timeout, char * idnum, char * pinLength, char * pinCrypt, char * TPKCheck)
{
	HXiMate_DeviceClose();
	Sleep(100);
	int ret = HXiMate_DeviceConnect("usb-hid");
	if (ret != 0)
	{
		return -1;
	}

	HXiMate_PinPad_SetModel(PINPAD_MODEL_HX);
	ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		return -2;
	}

	int algo = 1;
	int TMK_index = atoi(TMKIndex);
	int KEY_index = 1;//pin keyIndex
	int Timeout = atoi(timeout);

	unsigned char CardNo[18] = {0};
	memset(CardNo, 0x30, 17);
	memcpy(CardNo+4 ,accNo,12);

	unsigned char pin[16] = {0};
	ret = HXiMate_PinPad_InputPinBlock(algo, false, TMK_index, KEY_index, CardNo, pin, 6, Timeout);
	if (ret <= 0)
	{
		return -3;
	}

	//密码密文和长度
	memcpy(pinCrypt,pin,16);
	memcpy(pinLength, "06", 3);
	
	
	unsigned char outData[20] = { 0 };
	ret = HXiMate_DeviceXmemRead(128, 20, outData);
	if (ret != 0)
	{
		Sleep(100);
		ret = HXiMate_DeviceXmemRead(128, 20, outData);
		if (ret != 0)
			return -4;
	}
	//应用编号
	memcpy(idnum, outData, 20);

	//	TPKCheck
	unsigned char tmp[8] = { 0 };
	unsigned char outdata[128] = { 0 };
	ret = HXiMate_PinPad_Encrypt(algo, ALGO_ENCRYPT, TMK_index, KEY_index, tmp, outdata, 8);
	if (ret <= 0)
	{
		ret = HXiMate_PinPad_Encrypt(algo, ALGO_ENCRYPT, TMK_index, KEY_index, tmp, outdata, 8);
		if (ret <= 0)
		{
			return -5;
		}
	}
	//工作密钥的校验值
	memcpy(TPKCheck, outdata, 8);

	
	return 0;
}

int __stdcall SDBGenerateMAC(const char * CommPort, const char * ExtPort, const char * TMKIndex, const int lenOfMacData, const char * macData, char * mac, char * TAKCheck)
{
	HXiMate_DeviceClose();
	Sleep(100);
	int ret = HXiMate_DeviceConnect("usb-hid");
	if (ret != 0)
	{
		return -1;
	}

	HXiMate_PinPad_SetModel(PINPAD_MODEL_HX);
	ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		return -2;
	}

	int algo = 1;
	int TMK_index = atoi(TMKIndex);
	int KEY_index = 2;//mac keyIndex

	unsigned char data2mac[256] = {0};
	unsigned char result[33] = {0};
	HXiMate_TwoOne((unsigned char *)macData,lenOfMacData,data2mac);//不清楚字符串的处理，暂时先这样写
	ret = HXiMate_PinPad_Mac(algo, TMK_index, KEY_index, data2mac, result, lenOfMacData/2);
	if (ret <= 0)
	{
		ret = HXiMate_PinPad_Mac(algo, TMK_index, KEY_index, data2mac, result, lenOfMacData / 2);
		if (ret <= 0)
			return -3;
	}
	memcpy(mac, result, 32);

	//计算check
	unsigned char tmp[8] = { 0 };
	unsigned char outdata[128] = { 0 };
	ret = HXiMate_PinPad_Encrypt(algo, ALGO_ENCRYPT, TMK_index, KEY_index, tmp, outdata, 8);
	if (ret <= 0)
	{
		ret = HXiMate_PinPad_Encrypt(algo, ALGO_ENCRYPT, TMK_index, KEY_index, tmp, outdata, 8);
		if (ret <= 0)
		{
			return -5;
		}
	}
	//mac密钥的校验值
	memcpy(TAKCheck, outdata, 8);

	return 0;
}

int __stdcall SDBFormatAnsiX98Account(char * AccNo, char * AnsiX98AccNo)
{
	int cardNoLength = strlen(AccNo);
	for (int i = 0; i<12; i++)
		AnsiX98AccNo[i] = AccNo[cardNoLength + i + 3 - 16];
	return 0;
}

int __stdcall SDBPinpadCancel(void)
{
	HXiMate_DeviceCancel();
	HXiMate_PinPad_Cancel();
	return -15;
}
