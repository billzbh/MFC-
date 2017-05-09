

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

int FINGERPRINT_COMM_PORT;      //3           //ָ��ģ������iMate�ڲ��˿ں�-ͨѶ���
int FINGERPRINT_POWER_PORT;     //2           //ָ��ģ������iMate�ڲ��˿ں�-��Դ���



//	����:	��⵱ǰ�������ͻ��˹��ߵ��ն��豸
//	����:	��
//	���:	buffer		�豸�б��豸���к�16λ,���㲹0��
//	��"AAAAAAAABBBBBBBB"����ʾ��⵽�豸���к�ΪAAAAAAAABBBBBBBB
//	����:	0�ɹ�	!0ʧ��
_declspec(dllexport)  int __stdcall detectClientDev (char* buffer){
	
	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0)
			return -2;
	}

	//�򿪳ɹ�����ѯ�豸���к�
	unsigned char SNbuffer[25];
	memset(SNbuffer,0,sizeof(SNbuffer));
	if(getSN(SNbuffer)!=0)
		return 3;
	
	unsigned char byte16[16]={0};
	memcpy(byte16,SNbuffer+14,10);
	memcpy(buffer,byte16,16);
	return 0;
}


// ��ȡ�ն��豸����ԿУ��ֵ

//���checkValue

// ���أ�0��Ч���ݰ���1����ɹ�

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



// ���ն��豸USB�ӿڴӽ��յ�����Կ�ַ������ݰ����з�������
// ���룺shopNum		 ��15λ�̻��š�����������ʹ�ã�,
//	     clientDevID     ���ն�POS�š����ַ���ʹ�� 8�ֽڣ�,
//	     bankID          �����кš�����������ʹ�ã�,
//		 keyValue		 ������Կ���ġ���3DES�㷨���ַ������� 32
//
// ���أ�0��Ч���ݰ���1����ɹ�
extern "C" _declspec(dllexport) unsigned char dispatchKeyData(char*shopNum, char*clientDevID,char* bankID,char* keyValue)
{
	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0)
			return -9;
	}
	
	//д�̻���
	
	
	//д�ն˺�
	int length = strlen(clientDevID);
	if(length == 0 || length !=8)
		return 0;

	int ret = WriteTermId((unsigned char*)clientDevID,8);
	if(ret!=0)
		return -1;
	
	//д���к�
	length = strlen(bankID);
	if(length == 0 || length !=2)
		return 0;
	ret = WriteBankID((unsigned char*)bankID,2);
	if(ret!=0)
		return -9;
	
	//2. ��������Կ
	//ret = LoadHXpinpadMasterKey(1,0,(unsigned char*)keyValue,16);
	// ��'0'~'9'��'A'~'F'�ַ���ɵ��ַ����������ΪHexʮ��������������hex������
	unsigned char hex[16]={0};
	DES_StringToHex (keyValue, 32, hex);
	ret = LoadHXpinpadMasterKey(1,0,hex,16);
	if(ret!=0)
		return -3;



/*

	//�ַ����в����ã�ÿ�ζ�ֱ�ӳ�ʼ������ԿΪ16��0( ��ʽ��������Ҫע�͵� )
//	ret = 0;
	if(ret!=2)
	{
		unsigned char masterKey[16];
		memset(masterKey,0,sizeof(masterKey));
		ret = LoadHXpinpadMasterKey(1,1,masterKey,16);
		if(ret!=0)
			return -3;

		//1.�Ƚ�������ԿkeyValue������
		unsigned char PlainKeyData[16];
		ret = encrypt(1,ALGO_DECRYPT,1 ,-1, (unsigned char*)keyValue,16,PlainKeyData);
		if(ret!=0)
			return -5;

		//2. ��������Կ
		ret = LoadHXpinpadMasterKey(1,1,PlainKeyData,16);
		if(ret!=0)
			return -3;

		ret = WriteInitFlag();
		if(ret!=0)
			return -4;

	}else{
		//1.�Ƚ�������ԿkeyValue������
		unsigned char PlainKeyData[16];
		ret = encrypt(1,ALGO_DECRYPT,1 ,-1, (unsigned char*)keyValue,16,PlainKeyData);
		if(ret!=0)
			return -5;

		//2. ��������Կ
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
		return -2;//��������
	 }
	 if(iRet==-1)
	 {
		return -1;//��ʱ
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
		return -2;//��������
	 }
	 if(iRet==-1)
	 {
		return -1;//��ʱ
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
		return -2;//��������
	 }
	 if(iRet==-1)
	 {
		return -1;//��ʱ
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
		return -2;//��������
	 }
	 if(iRet==-1)
	 {
		return -1;//��ʱ
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
		return -2;//��������
	 }
	 if(iRet==-1)
	 {
		return -1;//��ʱ
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
		return -2;//��������
	 }
	 if(iRet==-1)
	 {
		return -1;//��ʱ
	 }
	 return 0;
}

/**
 * Pinpad�ӽ�������
 * algorithm       �㷨��0��DES��1��3DES��2��SM4
 * cryptoMode      �ӽ��ܷ�ʽ��ȡֵ: ALGO_ENCRYPT, ALGO_DECRYPT, ��ECB��ʽ���мӽ�������
 * masterIndex		����Կ����
 * workingIndex	������Կ���������������Կ����ȡֵ-1��ʹ������Կ����ָ��������Կ���мӽ���
 * data			�ӽ�������
 * dataLength		�ӽ������ݵĳ���,Ҫ��8�ı�����С�ڻ����248�ֽڳ���
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
		return -2;//��������
	 }
	 if(iRet==-1)
	 {
		return -1;//��ʱ
	 }
	 if(outlength<9)
	 {
		return -3;//�ӽ������ݳ���
	 }

	 memcpy(outData,receivedBytes+1,outlength-1);	
	 return 0;
}


//��ȡimate�汾��
int getImateVersion(char* version)
{
	 unsigned char receivedBytes[50];
     unsigned char sendBytes[2];
        
     sendBytes[0] = (char)0x60;
     sendBytes[1] = 1; //imate ic���������ó�Ƶģʽ-6MHz

	 int outlength=0;
	 int iRet = iHIDSendReceive(sendBytes,2,receivedBytes,&outlength,2);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//��������
	 }
	 if(iRet==-1)
	 {
		return -1;//��ʱ
	 }
	 memcpy(version,receivedBytes+1,outlength-1);
	 return 0;
}


//����com ��͸
int setComDeviceThroughout(long CustomBaund)
{
	char version[50]={0};
	int ret = getImateVersion(version);
	if(ret!=0)
		return ret;
	if (memcmp(version, "IMATEMINI", 9) == 0 ||
        memcmp(version, "IMATE5.0", 8) == 0||
        memcmp(version, "IMATEIV", 7) == 0) {
        FINGERPRINT_COMM_PORT = 4;           //iMateָ��ģ������iMate�ڲ��˿ں�-ͨѶ��� = UART3_FP
        FINGERPRINT_POWER_PORT = 4;          //iMateָ��ģ������iMate�ڲ��˿ں�-��Դ��� = Vuart_FP
    }
    else if (memcmp(version, "IMATEIII", 8) == 0) {
        FINGERPRINT_COMM_PORT = 5;           //ȱʡֵ��iMate301ָ��ģ������iMate�ڲ��˿ں�-ͨѶ���
        FINGERPRINT_POWER_PORT = 4;          //ȱʡֵ��iMate301ָ��ģ������iMate�ڲ��˿ں�-��Դ���
    }
    else if (memcmp(version, "IMATE", 5) == 0) {
        FINGERPRINT_COMM_PORT = 3;           //ȱʡֵ��iMateָ��ģ������iMate�ڲ��˿ں�-ͨѶ���
        FINGERPRINT_POWER_PORT = 2;          //ȱʡֵ��iMateָ��ģ������iMate�ڲ��˿ں�-��Դ���
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
		return -2;//��������
	}
	if(iRet==-1)
	{
		return -1;//��ʱ
	}
	return 0;
}