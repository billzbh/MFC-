

#include "HidFace.h"
#include <Windows.h>
#include "des.h"
#include "str.h"
#include <stdio.h>
#include "HXiMateAppFace.h"

#define VENDOR_ID		0x0483
#define PRODUCT_ID		0x5750
#define ALGO_ENCRYPT    1
#define ALGO_DECRYPT    2

extern int getSN(unsigned char* termSN);
extern int WriteTermId(unsigned char* termId,int length);
extern int WriteBankID(unsigned char* bankId,int length);
extern int readBankID(unsigned char* bankId,int length);
extern int LoadHXpinpadMasterKey(int algorithm,int index,unsigned char *masterKey,int keyLength);
extern int LoadHXpinpadWorkingKey(int algorithm,int MasterIndex,int WorkIndex,unsigned char * workKey,int keyLength);
extern int encrypt(int algorithm,int cryptoMode,int masterIndex ,int workingIndex, unsigned char *data,int dataLength,unsigned char*outData);
extern int inputBlock(char*cardNo,int algorithm,int masterIndex,int workingIndex,int pinMinLength,int pinMaxLength,int timeout,char *inputblock);


int FINGERPRINT_COMM_PORT;      //3           //ָ��ģ������iMate�ڲ��˿ں�-ͨѶ���
int FINGERPRINT_POWER_PORT;     //2           //ָ��ģ������iMate�ڲ��˿ں�-��Դ���



//workkey -1 ��ʾ����Կ���ܣ�0 ��ʾ0����������Կ����
//���checkValue
// ���أ�0��Ч���ݰ���1����ɹ�
int  getMKcheckValue(char* checkValue,int masterIndex,int workingIndex)
{
	unsigned char ZeroData[8]={0};
	unsigned char encodeData[8]={0};
	int ret = encrypt(1,ALGO_ENCRYPT,masterIndex ,workingIndex, ZeroData,8,encodeData);
	if(ret!=0)
		return 0;
	memcpy(checkValue,encodeData,8);
	return 1;
}

// 14.4 ��������Դ���ֽ��˫�����ȿɶ���16���ƴ�
// In       : psIn     : Դ��
//            iLength  : Դ������
// Out      : psOut    : Ŀ�괮
void vOneTwo(const uchar *psIn, int iLength, uchar *psOut)
{
    static const uchar aucHexToChar[17] = "0123456789ABCDEF";
    int iCounter;

    for(iCounter = 0; iCounter < iLength; iCounter++){
        psOut[2*iCounter] = aucHexToChar[((psIn[iCounter] >> 4)) & 0x0F];
        psOut[2*iCounter+1] = aucHexToChar[(psIn[iCounter] & 0x0F)];
    }
}

// 14.5 ��������Դ���ֽ��˫�����ȿɶ���16���ƴ�, ����ĩβ��'\0'
// In       : psIn     : Դ��
//            iLength  : Դ������
// Out      : pszOut   : Ŀ�괮
void vOneTwo0(const uchar *psIn, int iLength, uchar *pszOut)
{
    vOneTwo(psIn, iLength, pszOut);
	if(iLength < 0)
		iLength = 0;
    pszOut[2*iLength]=0;
}

// 14.6 ���ɶ���16���Ʊ�ʾ��ѹ������һ�볤�ȵĶ����ƴ�
// In       : psIn     : Դ��
//            iLength  : Դ������
// Out      : psOut    : Ŀ�괮
// Attention: Դ������Ϊ�Ϸ���ʮ�����Ʊ�ʾ����Сд����
//            �������Ϊ�����������῿����������һλ��ż��
void vTwoOne(const uchar *psIn, int iLength, uchar *psOut)
{
    uchar ucTmp;
    int   i;

    for(i=0; i<iLength; i+=2) {
        ucTmp = psIn[i];
        if(ucTmp > '9')
            ucTmp = toupper(ucTmp) - 'A' + 0x0a;
        else
            ucTmp &= 0x0f;
        psOut[i/2] = ucTmp << 4;

        ucTmp = psIn[i+1];
        if(ucTmp > '9')
            ucTmp = toupper(ucTmp) - 'A' + 0x0a;
        else
            ucTmp &= 0x0f;
        psOut[i/2] += ucTmp;
    } // for(i=0; i<uiLength; i+=2) {
}


/*
����˵��:
	����Ӧ�ñ�� 

�������:
CommPort	�ַ���,ʹ�ô��ڵĶ˿ں�,����Ϊ 4 ���ַ���ǰ�����ַ���Ϊ COM,��� һλȡֵΪ 1~8����:"COM1"��
ExtPort		�ַ���,BP ����չ��ѡ��ָ���Ϊ��ʱ��ʾ��ʹ����չ�С�����:ѡ����չ �� K ָ�Ϊ:"\033%K"��
idnum:		ASCII �����,���� 1-20 λ; 

����ֵ:		0 �ɹ� <0 ʧ��
*/
int SDBSetPinPadID(const char * CommPort, const char *ExtPort, const char* idnum)
{
	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0){
			return -1;
		}
	}

	int len = strlen(idnum);
	if(len<0||len>20){
		ucHidClose();
		return -2;
	}
	int ret = WriteBankID((unsigned char*)idnum,len);
	if(ret!=0){
		ucHidClose();
		return -3;
	}


	ucHidClose();
	return 0;
}

/*
����˵��:
	��ȡӦ�ñ�� ������� :
CommPort		�ַ���, ʹ�ô��ڵĶ˿ں�, ����Ϊ4���ַ���ǰ�����ַ���ΪCOM, �� ��һλȡֵΪ 1~8���� : "COM1"��
ExtPort			�ַ���, BP ����չ��ѡ��ָ���Ϊ��ʱ��ʾ��ʹ����չ�С����� : ѡ����չ �� K ָ�Ϊ : "\033%K"��
������� :
idnum:			ASCII �����, ���� 1 - 20 λ;
����ֵ:
		0 �ɹ�
		<0 ʧ��
*/
int SDBGetPinPadID(const char * CommPort, const char *ExtPort, char *idnum)
{
	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0){
			return -1;
		}
	}

    unsigned char out[20]={0};
	int ret = readBankID(out,20);
	if(ret!=0){
		ucHidClose();
		return -2;
	}
	memcpy(idnum,out,20);

	ucHidClose();
	return 0;
}

/*
����˵��
	��ȡ������̵�Ψһ�����к�(��������һ����) 

�������:
CommPort		�ַ���, ʹ�ô��ڵĶ˿ں�, ����Ϊ4���ַ���ǰ�����ַ���ΪCOM, �� ��һλȡֵΪ 1~8���� : "COM1"��
ExtPort			�ַ���, BP ����չ��ѡ��ָ���Ϊ��ʱ��ʾ��ʹ����չ�С����� : ѡ����չ �� K ָ�Ϊ : "\033%K"��
������� :
SeqNo:			Ψһ�����к�(���Ȳ����� 20 λ)

����ֵ :
			0, �ɹ���
			������ 0, ʧ�ܡ�
*/
int SDBGetCompanyPinPadID(const char * CommPort, const char *ExtPort, char *SeqNo)
{
	return 0;
}

/*
����˵��:
	�ָ�������̳�����Կ, ����ԿΪ��88888888��, ������ԿΪ��00000000�� 

������� :
CommPort		�ַ���, ʹ�ô��ڵĶ˿ں�, ����Ϊ 4 ���ַ���ǰ�����ַ���Ϊ COM, ���һλȡֵΪ 1~8���� : "COM1"��
ExtPort			�ַ���, BP ����չ��ѡ��ָ���Ϊ��ʱ��ʾ��ʹ����չ�С����� : ѡ����չ �� K ָ�Ϊ : "\033%K"��
TMKIndex		�ַ���, ʹ�� TMK ��������, ����Ϊ 3 ���ȵ��ַ�����ȡֵ��ΧΪ "001" - "016"���Ϸ��ַ�Ϊ 0~9���� : "001"��
����ֵ :
				0 �ɹ�
				<0 ʧ��
*/
int SDBRestDefaultKey(const char * CommPort, const char *ExtPort, const char* TMKIndex)
{
	return 0;
}


/*
����˵��:
¼������TMK 
������� :
CommPort		�ַ���, ʹ�ô��ڵĶ˿ں�, ����Ϊ 4 ���ַ���ǰ�����ַ���Ϊ COM, ��� һλȡֵΪ 1~8���� : "COM1"��
ExtPort			�ַ���, BP ����չ��ѡ��ָ���Ϊ��ʱ��ʾ��ʹ����չ�С����� : ѡ����չ �� K ָ�Ϊ : "\033%K"��
TMKIndex		�ַ���, ʹ�� TMK ��������, ����Ϊ 3 ���ȵ��ַ�����ȡֵ��ΧΪ "001" - "016"���Ϸ��ַ�Ϊ 0~9���� : "001"��
Tmk1 :			TMK ���ĳɷ�һ, ASCII �����, ���� 16 / 32 / 48 λ;
Tmk2:			TMK ���ĳɷݶ�, ASCII �����, ���� 16 / 32 / 48 λ; �������:
Tmkchk:			TMK У��ֵ, ���� 8 �ֽ� 0x00 �õ���ASCII �����, ȡǰ 4 �ֽ�, ���� 8 λ, ��ĸ��д;
����ֵ:
				0 �ɹ�
				<0 ʧ��
*/
int SDBInputTMK(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char* Tmk1, const char* Tmk2, char* Tmkchk)
{
	return 0;
}
/*
����˵��:
	���� TMK ע��������� 
������� :
CommPort		�ַ���, ʹ�ô��ڵĶ˿ں�, ����Ϊ4���ַ���ǰ�����ַ���ΪCOM, �� ��һλȡֵΪ 1~8���� : "COM1"��
ExtPort			�ַ���, BP ����չ��ѡ��ָ���Ϊ��ʱ��ʾ��ʹ����չ�С����� : ѡ����չ�� K ָ�Ϊ : "\033%K"�� 
NewKEYLen		�ַ���, �Կ��ַ�������"1" ��������(64bits) "2" ����˫����(128bits) "3" ����������(192bits)
TMKIndex		�ַ���, ʹ�� TMK ��������, ����Ϊ 3 ���ȵ��ַ�����ȡֵ��ΧΪ "001" - "016"���Ϸ��ַ�Ϊ 0~9���� : "001"��
NewTMKByOldTMK	�ַ���, �þ��ն�����Կ���ܵ���Կ, ����ΪNewKEYLen������ ���ȿ���ȡ 16 / 32 / 48��
KeyValue		�ַ���, ���� 8 �ַ�, �� TMK ��Կ���ļ��� 64bit0 ȡ����󲿷֡� 
������� :
	�� ����ֵ :
		0 ����ɹ�
		<0 ��ȡʧ��, Ϊ����ͨѶ�����롣
*/
int SDBLoadTMK(const char * CommPort, const char *ExtPort, const char * NewKEYLen, const char * TMKIndex, const char * NewTMKByOldTMK, const char * KeyValue)
{
	return 0;
}


/*
����˵��:
	TPK / TAK ע��������̴�У��ֵ�� 
������� :
CommPort		�ַ���, ʹ�ô��ڵĶ˿ں�, ����Ϊ 4 ���ַ���ǰ�����ַ���Ϊ COM, ��� һλȡֵΪ 1~8���� : "COM1"��
ExtPort			�ַ���, BP ����չ��ѡ��ָ���Ϊ��ʱ��ʾ��ʹ����չ�С����� : ѡ����չ �� K ָ�Ϊ : "\033%K"��
TMKIndex		�ַ���, ʹ�� TMK ��������, ����Ϊ 3 ���ȵ��ַ�����ȡֵ��ΧΪ "001" - "016"���Ϸ��ַ�Ϊ 0~9���� : "001"��
KEYIndex		�ַ���, ʹ�� TPK / TAK ��������, ����Ϊ 2 ���ȵ��ַ�����ȡֵ��ΧΪ "01" - "02", �Ϸ��ַ�Ϊ 0~2��"01"��ʾ TPK, "02"��ʾ TAK��
KeyLen			�ַ���, ��Կ���ȱ�־, ֻ��ȡ 1 / 2 / 3, ���� 1 �ַ�����"1"��������Կ, "2" ˫������Կ, "3"��������Կ��
keyByTMK		�ַ���, ���ն�����Կ���ܵ���Կ, ����Ϊ keyLen ���������ȿ���ȡ 16 / 32 / 48��
KeyValue		�ַ���, ���� 8 �ַ�, ��ԿУ��ֵ�� 
������� :
	�� ����ֵ :
		0 ��֤�ɹ���
		<0 ��ȡʧ��, Ϊ����ͨѶ�����롣
*/
int SDBInputKEY(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, const char * KeyValue)
{
	return 0;
}


/*
����˵��:
	TPK ע���������, ���У��ֵ��
������� :
CommPort			�ַ���, ʹ�ô��ڵĶ˿ں�, ����Ϊ 4 ���ַ���ǰ�����ַ���Ϊ COM,���һλȡֵΪ 1~8���� : "COM1"��
ExtPort				�ַ���, BP ����չ��ѡ��ָ���Ϊ��ʱ��ʾ��ʹ����չ�С����� : ѡ����չ�� K ָ�Ϊ : "\033%K"��
TMKIndex			�ַ���, ʹ�� TMK ��������, ����Ϊ 3 ���ȵ��ַ�����ȡֵ��ΧΪ"001" - "016"���Ϸ��ַ�Ϊ 0~9���� : "001"��
KEYIndex			�ַ���, ʹ��TPK��������, ����Ϊ2���ȵ��ַ�����ȡֵ��ΧΪ"01","01"��ʾ TPK��
KeyLen				�ַ���, ��Կ���ȱ�־, ֻ��ȡ1 / 2 / 3, ����1�ַ�����"1"��������Կ, "2"˫������Կ, "3"��������Կ��
keyByTMK			�ַ���, ���ն�����Կ���ܵ���Կ, ����Ϊ keyLen ���������ȿ���ȡ16 / 32 / 48��
������� :
KeyValue			�ַ���, ���� 8 �ַ�, ��ԿУ��ֵ��
����ֵ :
					0 ��֤�ɹ���
					<0 ��ȡʧ��, Ϊ����ͨѶ�����롣
*/
int SDBInputKeyNoCheck(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, char * KeyValue)
{
	return 0;
}

/*
����˵��:
	��� ZPK / ZAK ��ԿУ�鴮 
������� :
CommPort,		�ַ���, ʹ�ô��ڵĶ˿ں�, ����Ϊ 4 ���ַ���ǰ�����ַ���Ϊ COM, �� ��һλȡֵΪ 1~8���� : "COM1"��
ExtPort,		�ַ���, BP ����չ��ѡ��ָ���Ϊ��ʱ��ʾ��ʹ����չ�С����� : ѡ���� չ�� K ָ�Ϊ : "\033%K"��
KEYIndex		�ַ���, ʹ�� TPK / TAK ��������, ����Ϊ 2 ���ȵ��ַ�����ȡֵ��ΧΪ "01" - "02", �Ϸ��ַ�Ϊ 0~2��"01"��ʾ TPK, "02"��ʾ TAK��
TMKIndex		�ַ���, ʹ�� TMK ��������, ����Ϊ 3 ���ȵ��ַ�����ȡֵ��ΧΪ "001" - "016"���Ϸ��ַ�Ϊ 0~9���� : "001"��
������� :
KeyLen			�ַ���, ��Կ���ȱ�־, ֻ��ȡ 1 / 2 / 3, ���� 1 �ַ�����"1"��������Կ, "2"˫������Կ, "3"��������Կ��
KeyValue		�ַ���, ���� 8 �ַ�, ��Կ���ļ��� 64bit0 ȡ����󲿷֡�
����ֵ :
				0 �ɹ���
				<0 ʧ�ܡ�
*/
int SDBCheckKEY(const char * CommPort, const char * ExtPort, const char * KEYIndex, const char * TMKIndex, char * KeyLen, char * KeyValue)
{
	return 0;
}

/*
����˵��:
��ȡ�ͻ���������
������� :
CommPort		�ַ���, ʹ�ô��ڵĶ˿ں�, ����Ϊ4���ַ���ǰ�����ַ���ΪCOM, ���һλȡֵΪ 1~8���� : "COM1"��
ExtPort			�ַ���, BP ����չ��ѡ��ָ���Ϊ��ʱ��ʾ��ʹ����չ�С����� : ѡ����չ�� K ָ�Ϊ : "\033%K"��
TMKIndex		�ַ���, ʹ�� TMK ��������, ����Ϊ 3 ���ȵ��ַ�����ȡֵ��ΧΪ"001"-"016"���Ϸ��ַ�Ϊ 0~9����:"001"
accNo			�ַ���,�ͻ����˻���ʶ,����Ϊ 12 ���Ϸ��˺š��Ϸ��ַ�Ϊ 0~9��
modex			�ַ���,����ģʽ��0:��ʾ"����!����������";1:��ʾ"��������һ������"��2:��ʾ����������,���������롱
timeout			�ַ���, ����ĵȴ�ʱ��, ��λΪ�롣���������Ҫ�����ó�ʱ���ܡ�
������� :
idnum			Ӧ�ñ�š�ASCII �����, ���� 1 - 20 λ;
pinLength		�ַ���, ����Ϊ 2 ���ȵ��ַ���������˵����ǰ�ͻ�����������볤�ȡ�
pinCrypt		�ַ���, �ͻ� PIN ����, ����Ϊ 16 �ֽڡ��Ϸ��ַ�Ϊ 0~9, A~F����������ֵ����Ϊ 0 ��Ч��
TPKCheck		���� PIN �� TPK ��ԿУ��ֵ���ַ���, ����Ϊ 8 λ, ��ĸ��д;
����ֵ:
				0 ��ȡ�ɹ�, ��ʱ pinCryptogram ��Ч�� 
				<0 ��ȡʧ��, Ϊ����ͨѶ�����롣
*/
int SDBReadCustomerPin(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * accNo, const char * modex, const char * timeout, char * idnum, char *
	pinLength, char * pinCrypt, char * TPKCheck)
{
	return 0;
}

/*
����˵��:
	MAC ���� 
������� :
CommPort		�ַ���, ʹ�ô��ڵĶ˿ں�, ����Ϊ4���ַ���ǰ�����ַ���ΪCOM, �� ��һλȡֵΪ 1~8���� : "COM1"��
ExtPort			�ַ���, BP ����չ��ѡ��ָ���Ϊ��ʱ��ʾ��ʹ����չ�С����� : ѡ����չ �� K ָ�Ϊ : "\033%K"��
TMKIndex		�ַ���, ʹ�� TMK ��������, ����Ϊ 3 ���ȵ��ַ�����ȡֵ��ΧΪ "001" - "016"���Ϸ��ַ�Ϊ 0~9���� : "001"��
lenOfMacData,	����, �������� MAC �����ݵĳ��ȡ�
macData,		�ַ���, Ҫ�������� MAC �����ݡ� 
������� :
mac,			�ַ���, �Կ��ַ�����, ���ɵ� MAC ֵ, ����Ϊ 16 �ֽڡ��Ϸ��ַ�Ϊ 0~9, A~F��
TAKCheck,		�Կ��ַ�����, TAK ��ԿУ��ֵ, ����Ϊ 16 �ֽ�, �Ϸ��ַ�Ϊ 0~9, A~F 
����ֵ :
				0, ���ɳɹ�, ��ʱ mac ��Ч��
				<0, ��ȡʧ��, Ϊ����ͨѶ�����롣
*/
int SDBGenerateMAC(const char * CommPort, const char * ExtPort, const char * TMKIndex, const int lenOfMacData, const char * macData, char * mac, char * TAKCheck)
{
	return 0;
}


/*
����˵��:
�����ʺŵõ� ANSI X9.8 ��ʽҪ��� 12 λ�ʺ�
������� :
AccNo			ԭʼ�ʺ� ������� :
AnsiX98AccNo	ANSIX9.8��ʽҪ����ʺ� 
����ֵ :
				0, �ɹ��� <0, ʧ�ܡ�
*/
int SDBFormatAnsiX98Account(char *AccNo, char *AnsiX98AccNo){
	return 0;
}

/*
����˵��:
	�û���ֹ��ǰ����
����ֵ :
	-15, ��ֹ��ǰ����
*/
int SDBPinpadCancel(void){
	
	return 0;
}




int __stdcall AKP_SetInitMainKey(int iComPort,int iBaudRate,int iComTimeout,int iMainkeyIndex, int iMainKeyLen,char *pszMainKey )
{	
	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0){
			return -113;
		}
	}

	if(iMainKeyLen!=16)
		return -104;

    int ret = LoadHXpinpadMasterKey(1,0,(unsigned char *)pszMainKey,16);
	if(ret!=0){
		ucHidClose();
		return -103;
	}
	ucHidClose();
	return 0;
}

/*
��������Կ
�������˵����
iComPort   	   //���ں�,��1��ʾCOM1,2��ʾCOM2,�������ƣ� 
iBaudRate  	   //������
iComTimeout    //���ڲ�����ʱʱ�� ��λΪ��(S)
iMainkeyIndex	//����Կ�����š�0-15����������̸��ݴ�������//ȡ��������������е�����Կ��
iNewMainKeyLen	//������Կ���ȣ���8,16,24��
pszNewMainKey  //��iMainkeyIndexָ�������Կ���ܵ�������
//Կ,���������������(����)
	pszChkCode		 //����ԿУ����

����ֵ:
0-�ɹ� ��0-ʧ��

��ע:
������������е�����Կ����0-15����16�顿
*/
int __stdcall AKP_ChgMainKey(int iComPort , int iBaudRate , int iComTimeout,
				   int iMainkeyIndex , int iNewMainKeyLen, 
				   char *pszNewMainKey,char *pszChkCode)
{
	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0){
			return -113;
		}
	}

	if(iNewMainKeyLen!=32)
		return -104;

	//1.�Ƚ�������ԿkeyValue������
	unsigned char srcKeyData[16];
	unsigned char PlainKeyData[16];
	vTwoOne((const unsigned char *)pszNewMainKey,32,srcKeyData);
	//����
	int ret = encrypt(1,0,0,-1, (unsigned char*)srcKeyData,16,PlainKeyData);
	if(ret!=0){
		ucHidClose();
		return -100;
	}

	//2. ��������Կ
	ret = LoadHXpinpadMasterKey(1,0,PlainKeyData,16);
	if(ret!=0){
		ucHidClose();
		return -103;
	}   

	unsigned char ZeroData[8]={0};
	unsigned char binData[16]={0};
	vTwoOne((const unsigned char*)pszChkCode, strlen(pszChkCode), binData);
	if(pszChkCode==NULL || memcmp(binData,ZeroData,strlen(pszChkCode)/2)==0 )
	{
		ucHidClose();
		return 0;
	}

	unsigned char encodeData[8]={0};
	ret = encrypt(1,1,0,-1,ZeroData,8,encodeData);//����
	if(ret!=0){
		ucHidClose();
		return -122;
	}

	//3.�Ƚ�У��ֵ
	if(memcmp(encodeData,binData,strlen(pszChkCode)/2)==0){	
		ucHidClose();
		return 0;
	}else{
		ucHidClose();
		return -110;
	}
}

/*
���Ĺ�����Կ
�������˵����
iComPort   	   //���ں�,��1��ʾCOM1,2��ʾCOM2,�������ƣ� 
iBaudRate  	   //������
iComTimeout    //���ڲ�����ʱʱ�� ��λΪ��(S)
iMainkeyIndex	//����Կ�����š�0-15����������̸��ݴ�������//ȡ��������������е�����Կ��
iWorkKeyIndex   //������Կ�����š�0-7��
iWorkKeyLen    //������Կ���ȡ�8,16,24��
pszWorkKey     //��iMainkeyIndexָ�������Կ���ܹ��Ĺ���
//��Կ���·����������(����)
pszChkCode		 //������ԿУ����
����ֵ:
0-�ɹ� ��0-ʧ��
  ��ע��
1.���ù�����Կ���ܹ��Ĺ�����Կ���͸�������̣��Զ��û�������м��ܡ�
2.����ԿĬ��ʹ�õ�0�飬��1-15�鱸�á�
3.������ԿĬ��ʹ�õ�0�飬1-7�鱸��
*/
int __stdcall AKP_ChgWorkKey(int iComPort , int iBaudRate , int iComTimeout ,
				   int iMainkeyIndex , int iWorkKeyIndex ,
				   int iWorkKeyLen , char *pszWorkKey,char *pszChkCode ){
	
	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0){
			return -113;
		}
	}

	if(iWorkKeyLen!=32)
		return -104;

	//1.��װ������Կ
	unsigned char srcKeyData[16];
	vTwoOne((const unsigned char *)pszWorkKey,32,srcKeyData);

	int ret = LoadHXpinpadWorkingKey(1,0,0,(unsigned char *)srcKeyData,16);
	if(ret!=0){
		ucHidClose();
		return -103;
	}
	
	unsigned char ZeroData[8]={0};
	unsigned char binData[16]={0};
	vTwoOne((const unsigned char*)pszChkCode, strlen(pszChkCode), binData);
	if(pszChkCode==NULL || memcmp(binData,ZeroData,strlen(pszChkCode)/2)==0 )
	{
		ucHidClose();
		return 0;
	}

	unsigned char encodeData[8]={0};
	ret = encrypt(1,1,0,0,ZeroData,8,encodeData);//����
	if(ret!=0){
		ucHidClose();
		return -122;
	}

	//3.�Ƚ�У��ֵ
	if(memcmp(encodeData,binData,strlen(pszChkCode)/2)==0){	
		ucHidClose();
		return 0;
	}else{
		ucHidClose();
		return -110;
	}
}

/*
��������ӿ�
�������˵����
iComPort   	    //���ں�,��1��ʾCOM1,2��ʾCOM2,�������ƣ� 
iBaudRate  	   //������
iComTimeout     //���ڲ�����ʱʱ�� ��λΪ��(S)
iActTimeout      //�����ȴ���ʱʱ�� ��λΪ��(S)
iVoiceNo        //�������(����ڱ�ע��Լ��)
iPwdMin        //������������(������С��������)
iPwdMax        //������������(������󳤶�����)
iPwdRetryLimit   //���������������,�����ֵΪ0��������
bShuffle         //�Ƿ���������       true-����  false-������
bEncrype        //�Ƿ�����������   true-����  false-������
pszAcctNo			//�����������Ŀͻ��˺�

�������˵��:
pszInputPwd      //�ͻ������������Ϣ
//��TIMEOUT��-������ʱ��RETRYOUT-�����������������//������
piInputPwdLen    //�ͻ���������볤��

����ֵ:
0-�ɹ� ��0-ʧ��
*/
int __stdcall AKP_InputPwd(int iComPort , int iBaudRate , 
				 int iComTimeout , int iActTimeout , 
				 int iVoiceNo ,int iPwdMin,int iPwdMax,
				 int iPwdRetryLimit , bool bShuffle ,bool bEncrypt , 
				 char *pszAcctNo ,char *pszInputPwd  , int *piInputPwdLen)
{
	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0){
			return -113;
		}
	}

	char pinblock[16];
	int ret = inputBlock(pszAcctNo,1,0,0,iPwdMin,iPwdMax,iComTimeout,pinblock);
	if(ret!=0){
		sprintf(pszInputPwd,"TIMEOUT");
		ucHidClose();
		return -103;
	}
	
	vOneTwo0((const unsigned char *)pinblock,8,(unsigned char *)pszInputPwd);
	*piInputPwdLen = strlen(pszInputPwd);
	ucHidClose();
	return 0;
}



/*
��ȡ�豸״̬
�������˵����
iComPort   	   //���ں�,��1��ʾCOM1,2��ʾCOM2,�������ƣ� 
iBaudRate  	   //������
iComTimeout    //���ڲ�����ʱʱ�� ��λΪ��(S)

�������˵��:
pszDevStatu     //�豸״̬ ��REGULAR��-����  ��BKDOWN��-����
piDevStatuLen   //�豸״̬����

����ֵ:
0-�ɹ� ��0-ʧ��	
*/
int __stdcall AKP_GetDevStatu ( int iComPort , int iBaudRate , int iComTimeout ,
					 char *pszDevStatu , int *piDevStatuLen ){

	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0){
			sprintf(pszDevStatu,"BKDOWN");
			*piDevStatuLen = 6;
		}else{
			sprintf(pszDevStatu,"REGULAR");
			*piDevStatuLen = 7;
		}
	}else{
		sprintf(pszDevStatu,"REGULAR");
		*piDevStatuLen = 7;
	}
	ucHidClose();
	return 0;
}


/*
��ȡ�豸��Ϣ
�������˵����
iComPort   	   //���ں�,��1��ʾCOM1,2��ʾCOM2,�������ƣ� 
iBaudRate  	   //������
iComTimeout    //���ڲ�����ʱʱ�� ��λΪ��(S)

�������˵��:
pszDevType       //�豸״̬ ���ﷵ��ֵ�����ȶ�
piDevTypeLen     //�豸״̬����
pszDevSerialNo    //�豸���к�
piDevSerialNoLen  //�豸���кų���

����ֵ:
0-�ɹ� ��0-ʧ��
*/
int __stdcall AKP_GetDevInfo ( int iComPort , int iBaudRate , int iComTimeout ,
					char *pszDevType , int *piDevTypeLen,
					char *pszDevSerialNo , int *piDevSerialNoLen )
{
	char buff[25];
	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0){
			return -113;
		}
	}
	sprintf(pszDevType,"NTBP");
	*piDevTypeLen = 4;
	memset(buff,0,25);
	char ret= getSN((unsigned char*)buff);
	if(ret!=0){
		ucHidClose();
		return -100;
	}
	sprintf(pszDevSerialNo,"%s",buff);
	*piDevSerialNoLen = strlen(buff);
	ucHidClose();
	return 0;
}




int __stdcall AKP_ResFactoryKey ( int iComPort , int iBaudRate , int iComTimeout ){
	
	if(ucHidIfOpened()!=1){
		if(ucHidOpen(VENDOR_ID, PRODUCT_ID)!=0){
			return -113;
		}
	}
	ucHidClose();
	return 0;
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

int readBankID(unsigned char* bankId,int length)
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
	int outlength=0;
	int iRet = iHIDSendReceive(sendBytes,6,receivedBytes,&outlength,3);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//��������
	 }
	 if(iRet==-1)
	 {
		return -1;//��ʱ
	 }
	 memcpy(bankId,receivedBytes+1,length);
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

int inputBlock(char*cardNo,int algorithm,int masterIndex,int workingIndex,int pinMinLength,int pinMaxLength,int timeout,char *inputblock)
{

        if (cardNo == NULL || strlen(cardNo) < 13){
            return -1;
        }
        
        unsigned char receivedBytes[50];
        unsigned char sendBytes[50];
        sendBytes[0] = (unsigned char)0x90;
        sendBytes[1] = 2; //input pinblock
        sendBytes[2] = algorithm;
        sendBytes[3] = masterIndex;
        sendBytes[4] = workingIndex;
        sendBytes[5] = pinMinLength;
        sendBytes[6] = pinMaxLength;
        sendBytes[7] = timeout;
        memcpy(sendBytes+8, "000000000000", 12);
        
        
        //��װ�ʺ�
        for (int i=0; i<12; i++)
			sendBytes[i+8] = cardNo[strlen(cardNo)+i+3-16];
        
        int outlength=0;
		int iRet = iHIDSendReceive(sendBytes,8+12,receivedBytes,&outlength,timeout+1);
		if (iRet > 0 && receivedBytes[0]) {
			return -2;//��������
		}
		if(iRet==-1)
		{
			return -1;//��ʱ
		}
        
        int blockLen = algorithm != 2 ? 8 : 16;
        if (outlength < blockLen && receivedBytes[0]==0) {
            return 0;//cancel
        }
        
        for (i=0; i<blockLen; i++)
            inputblock[i] = receivedBytes[i + 1];

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


