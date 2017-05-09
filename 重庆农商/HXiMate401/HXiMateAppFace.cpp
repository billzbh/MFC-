

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


int FINGERPRINT_COMM_PORT;      //3           //指纹模块连接iMate内部端口号-通讯编号
int FINGERPRINT_POWER_PORT;     //2           //指纹模块连接iMate内部端口号-电源编号



//workkey -1 表示主密钥加密，0 表示0索引工作密钥加密
//输出checkValue
// 返回：0无效数据包；1处理成功
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

// 14.4 将二进制源串分解成双倍长度可读的16进制串
// In       : psIn     : 源串
//            iLength  : 源串长度
// Out      : psOut    : 目标串
void vOneTwo(const uchar *psIn, int iLength, uchar *psOut)
{
    static const uchar aucHexToChar[17] = "0123456789ABCDEF";
    int iCounter;

    for(iCounter = 0; iCounter < iLength; iCounter++){
        psOut[2*iCounter] = aucHexToChar[((psIn[iCounter] >> 4)) & 0x0F];
        psOut[2*iCounter+1] = aucHexToChar[(psIn[iCounter] & 0x0F)];
    }
}

// 14.5 将二进制源串分解成双倍长度可读的16进制串, 并在末尾添'\0'
// In       : psIn     : 源串
//            iLength  : 源串长度
// Out      : pszOut   : 目标串
void vOneTwo0(const uchar *psIn, int iLength, uchar *pszOut)
{
    vOneTwo(psIn, iLength, pszOut);
	if(iLength < 0)
		iLength = 0;
    pszOut[2*iLength]=0;
}

// 14.6 将可读的16进制表示串压缩成其一半长度的二进制串
// In       : psIn     : 源串
//            iLength  : 源串长度
// Out      : psOut    : 目标串
// Attention: 源串必须为合法的十六进制表示，大小写均可
//            长度如果为奇数，函数会靠近到比它大一位的偶数
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
功能说明:
	设置应用编号 

输入参数:
CommPort	字符串,使用串口的端口号,必须为 4 个字符且前三个字符必为 COM,最后 一位取值为 1~8。例:"COM1"。
ExtPort		字符串,BP 盒扩展口选择指令串。为空时表示不使用扩展盒。例如:选择扩展 口 K 指令串为:"\033%K"。
idnum:		ASCII 码编码,长度 1-20 位; 

返回值:		0 成功 <0 失败
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
功能说明:
	读取应用编号 输入参数 :
CommPort		字符串, 使用串口的端口号, 必须为4个字符且前三个字符必为COM, 最 后一位取值为 1~8。例 : "COM1"。
ExtPort			字符串, BP 盒扩展口选择指令串。为空时表示不使用扩展盒。例如 : 选择扩展 口 K 指令串为 : "\033%K"。
输出参数 :
idnum:			ASCII 码编码, 长度 1 - 20 位;
返回值:
		0 成功
		<0 失败
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
功能说明
	读取密码键盘的唯一的序列号(不管是哪一批次) 

输入参数:
CommPort		字符串, 使用串口的端口号, 必须为4个字符且前三个字符必为COM, 最 后一位取值为 1~8。例 : "COM1"。
ExtPort			字符串, BP 盒扩展口选择指令串。为空时表示不使用扩展盒。例如 : 选择扩展 口 K 指令串为 : "\033%K"。
输出参数 :
SeqNo:			唯一的序列号(长度不超过 20 位)

返回值 :
			0, 成功。
			不等于 0, 失败。
*/
int SDBGetCompanyPinPadID(const char * CommPort, const char *ExtPort, char *SeqNo)
{
	return 0;
}

/*
功能说明:
	恢复密码键盘出厂密钥, 主密钥为“88888888”, 工作密钥为“00000000” 

输入参数 :
CommPort		字符串, 使用串口的端口号, 必须为 4 个字符且前三个字符必为 COM, 最后一位取值为 1~8。例 : "COM1"。
ExtPort			字符串, BP 盒扩展口选择指令串。为空时表示不使用扩展盒。例如 : 选择扩展 口 K 指令串为 : "\033%K"。
TMKIndex		字符串, 使用 TMK 的索引号, 必须为 3 长度的字符串。取值范围为 "001" - "016"。合法字符为 0~9。例 : "001"。
返回值 :
				0 成功
				<0 失败
*/
int SDBRestDefaultKey(const char * CommPort, const char *ExtPort, const char* TMKIndex)
{
	return 0;
}


/*
功能说明:
录入明文TMK 
输入参数 :
CommPort		字符串, 使用串口的端口号, 必须为 4 个字符且前三个字符必为 COM, 最后 一位取值为 1~8。例 : "COM1"。
ExtPort			字符串, BP 盒扩展口选择指令串。为空时表示不使用扩展盒。例如 : 选择扩展 口 K 指令串为 : "\033%K"。
TMKIndex		字符串, 使用 TMK 的索引号, 必须为 3 长度的字符串。取值范围为 "001" - "016"。合法字符为 0~9。例 : "001"。
Tmk1 :			TMK 明文成份一, ASCII 码编码, 长度 16 / 32 / 48 位;
Tmk2:			TMK 明文成份二, ASCII 码编码, 长度 16 / 32 / 48 位; 输出参数:
Tmkchk:			TMK 校验值, 加密 8 字节 0x00 得到。ASCII 码编码, 取前 4 字节, 长度 8 位, 字母大写;
返回值:
				0 成功
				<0 失败
*/
int SDBInputTMK(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char* Tmk1, const char* Tmk2, char* Tmkchk)
{
	return 0;
}
/*
功能说明:
	密文 TMK 注入密码键盘 
输入参数 :
CommPort		字符串, 使用串口的端口号, 必须为4个字符且前三个字符必为COM, 最 后一位取值为 1~8。例 : "COM1"。
ExtPort			字符串, BP 盒扩展口选择指令串。为空时表示不使用扩展盒。例如 : 选择扩展口 K 指令串为 : "\033%K"。 
NewKEYLen		字符串, 以空字符结束。"1" 代表单倍长(64bits) "2" 代表双倍长(128bits) "3" 代表三倍长(192bits)
TMKIndex		字符串, 使用 TMK 的索引号, 必须为 3 长度的字符串。取值范围为 "001" - "016"。合法字符为 0~9。例 : "001"。
NewTMKByOldTMK	字符串, 用旧终端主密钥加密的密钥, 长度为NewKEYLen决定。 长度可以取 16 / 32 / 48。
KeyValue		字符串, 长度 8 字符, 新 TMK 密钥明文加密 64bit0 取结果左部分。 
输出参数 :
	无 返回值 :
		0 导入成功
		<0 读取失败, 为串口通讯错误码。
*/
int SDBLoadTMK(const char * CommPort, const char *ExtPort, const char * NewKEYLen, const char * TMKIndex, const char * NewTMKByOldTMK, const char * KeyValue)
{
	return 0;
}


/*
功能说明:
	TPK / TAK 注入密码键盘带校验值。 
输入参数 :
CommPort		字符串, 使用串口的端口号, 必须为 4 个字符且前三个字符必为 COM, 最后 一位取值为 1~8。例 : "COM1"。
ExtPort			字符串, BP 盒扩展口选择指令串。为空时表示不使用扩展盒。例如 : 选择扩展 口 K 指令串为 : "\033%K"。
TMKIndex		字符串, 使用 TMK 的索引号, 必须为 3 长度的字符串。取值范围为 "001" - "016"。合法字符为 0~9。例 : "001"。
KEYIndex		字符串, 使用 TPK / TAK 的索引号, 必须为 2 长度的字符串。取值范围为 "01" - "02", 合法字符为 0~2。"01"表示 TPK, "02"表示 TAK。
KeyLen			字符串, 密钥长度标志, 只能取 1 / 2 / 3, 长度 1 字符。例"1"单倍长密钥, "2" 双倍长密钥, "3"三倍长密钥。
keyByTMK		字符串, 用终端主密钥加密的密钥, 长度为 keyLen 决定。长度可以取 16 / 32 / 48。
KeyValue		字符串, 长度 8 字符, 密钥校验值。 
输出参数 :
	无 返回值 :
		0 验证成功。
		<0 读取失败, 为串口通讯错误码。
*/
int SDBInputKEY(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, const char * KeyValue)
{
	return 0;
}


/*
功能说明:
	TPK 注入密码键盘, 输出校验值。
输入参数 :
CommPort			字符串, 使用串口的端口号, 必须为 4 个字符且前三个字符必为 COM,最后一位取值为 1~8。例 : "COM1"。
ExtPort				字符串, BP 盒扩展口选择指令串。为空时表示不使用扩展盒。例如 : 选择扩展口 K 指令串为 : "\033%K"。
TMKIndex			字符串, 使用 TMK 的索引号, 必须为 3 长度的字符串。取值范围为"001" - "016"。合法字符为 0~9。例 : "001"。
KEYIndex			字符串, 使用TPK的索引号, 必须为2长度的字符串。取值范围为"01","01"表示 TPK。
KeyLen				字符串, 密钥长度标志, 只能取1 / 2 / 3, 长度1字符。例"1"单倍长密钥, "2"双倍长密钥, "3"三倍长密钥。
keyByTMK			字符串, 用终端主密钥加密的密钥, 长度为 keyLen 决定。长度可以取16 / 32 / 48。
输出参数 :
KeyValue			字符串, 长度 8 字符, 密钥校验值。
返回值 :
					0 验证成功。
					<0 读取失败, 为串口通讯错误码。
*/
int SDBInputKeyNoCheck(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, char * KeyValue)
{
	return 0;
}

/*
功能说明:
	检查 ZPK / ZAK 密钥校验串 
输入参数 :
CommPort,		字符串, 使用串口的端口号, 必须为 4 个字符且前三个字符必为 COM, 最 后一位取值为 1~8。例 : "COM1"。
ExtPort,		字符串, BP 盒扩展口选择指令串。为空时表示不使用扩展盒。例如 : 选择扩 展口 K 指令串为 : "\033%K"。
KEYIndex		字符串, 使用 TPK / TAK 的索引号, 必须为 2 长度的字符串。取值范围为 "01" - "02", 合法字符为 0~2。"01"表示 TPK, "02"表示 TAK。
TMKIndex		字符串, 使用 TMK 的索引号, 必须为 3 长度的字符串。取值范围为 "001" - "016"。合法字符为 0~9。例 : "001"。
输出参数 :
KeyLen			字符串, 密钥长度标志, 只能取 1 / 2 / 3, 长度 1 字符。例"1"单倍长密钥, "2"双倍长密钥, "3"三倍长密钥。
KeyValue		字符串, 长度 8 字符, 密钥明文加密 64bit0 取结果左部分。
返回值 :
				0 成功。
				<0 失败。
*/
int SDBCheckKEY(const char * CommPort, const char * ExtPort, const char * KEYIndex, const char * TMKIndex, char * KeyLen, char * KeyValue)
{
	return 0;
}

/*
功能说明:
读取客户密码密文
输入参数 :
CommPort		字符串, 使用串口的端口号, 必须为4个字符且前三个字符必为COM, 最后一位取值为 1~8。例 : "COM1"。
ExtPort			字符串, BP 盒扩展口选择指令串。为空时表示不使用扩展盒。例如 : 选择扩展口 K 指令串为 : "\033%K"。
TMKIndex		字符串, 使用 TMK 的索引号, 必须为 3 长度的字符串。取值范围为"001"-"016"。合法字符为 0~9。例:"001"
accNo			字符串,客户的账户标识,长度为 12 个合法账号。合法字符为 0~9。
modex			字符串,语音模式。0:提示"您好!请输入密码";1:提示"请再输入一次密码"。2:提示“密码有误,请重新输入”
timeout			字符串, 输入的等待时间, 单位为秒。密码键盘需要有设置超时功能。
输出参数 :
idnum			应用编号。ASCII 码编码, 长度 1 - 20 位;
pinLength		字符串, 必须为 2 长度的字符串。用于说明当前客户所输入的密码长度。
pinCrypt		字符串, 客户 PIN 密文, 长度为 16 字节。合法字符为 0~9, A~F。仅当函数值返回为 0 有效。
TPKCheck		加密 PIN 的 TPK 密钥校验值。字符串, 长度为 8 位, 字母大写;
返回值:
				0 读取成功, 此时 pinCryptogram 有效。 
				<0 读取失败, 为串口通讯错误码。
*/
int SDBReadCustomerPin(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * accNo, const char * modex, const char * timeout, char * idnum, char *
	pinLength, char * pinCrypt, char * TPKCheck)
{
	return 0;
}

/*
功能说明:
	MAC 计算 
输入参数 :
CommPort		字符串, 使用串口的端口号, 必须为4个字符且前三个字符必为COM, 最 后一位取值为 1~8。例 : "COM1"。
ExtPort			字符串, BP 盒扩展口选择指令串。为空时表示不使用扩展盒。例如 : 选择扩展 口 K 指令串为 : "\033%K"。
TMKIndex		字符串, 使用 TMK 的索引号, 必须为 3 长度的字符串。取值范围为 "001" - "016"。合法字符为 0~9。例 : "001"。
lenOfMacData,	整型, 用于生成 MAC 的数据的长度。
macData,		字符串, 要用于生成 MAC 的数据。 
输出参数 :
mac,			字符串, 以空字符结束, 生成的 MAC 值, 长度为 16 字节。合法字符为 0~9, A~F。
TAKCheck,		以空字符结束, TAK 密钥校验值, 长度为 16 字节, 合法字符为 0~9, A~F 
返回值 :
				0, 生成成功, 此时 mac 有效。
				<0, 读取失败, 为串口通讯错误码。
*/
int SDBGenerateMAC(const char * CommPort, const char * ExtPort, const char * TMKIndex, const int lenOfMacData, const char * macData, char * mac, char * TAKCheck)
{
	return 0;
}


/*
功能说明:
根据帐号得到 ANSI X9.8 格式要求的 12 位帐号
输入参数 :
AccNo			原始帐号 输出参数 :
AnsiX98AccNo	ANSIX9.8格式要求的帐号 
返回值 :
				0, 成功。 <0, 失败。
*/
int SDBFormatAnsiX98Account(char *AccNo, char *AnsiX98AccNo){
	return 0;
}

/*
功能说明:
	用户中止当前操作
返回值 :
	-15, 中止当前操作
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
更改主密钥
输入参数说明：
iComPort   	   //串口号,如1表示COM1,2表示COM2,依次类推； 
iBaudRate  	   //波特率
iComTimeout    //串口操作超时时间 单位为秒(S)
iMainkeyIndex	//主密钥索引号【0-15】，密码键盘根据此索引，//取出存在密码键盘中的主密钥。
iNewMainKeyLen	//新主密钥长度；【8,16,24】
pszNewMainKey  //用iMainkeyIndex指向的主密钥加密的新主密
//钥,将被存入密码键盘(密文)
	pszChkCode		 //主密钥校验码

返回值:
0-成功 非0-失败

备注:
更换密码键盘中的主密钥。【0-15，共16组】
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

	//1.先解密主密钥keyValue的明文
	unsigned char srcKeyData[16];
	unsigned char PlainKeyData[16];
	vTwoOne((const unsigned char *)pszNewMainKey,32,srcKeyData);
	//解密
	int ret = encrypt(1,0,0,-1, (unsigned char*)srcKeyData,16,PlainKeyData);
	if(ret!=0){
		ucHidClose();
		return -100;
	}

	//2. 下载主密钥
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
	ret = encrypt(1,1,0,-1,ZeroData,8,encodeData);//加密
	if(ret!=0){
		ucHidClose();
		return -122;
	}

	//3.比较校验值
	if(memcmp(encodeData,binData,strlen(pszChkCode)/2)==0){	
		ucHidClose();
		return 0;
	}else{
		ucHidClose();
		return -110;
	}
}

/*
更改工作密钥
输入参数说明：
iComPort   	   //串口号,如1表示COM1,2表示COM2,依次类推； 
iBaudRate  	   //波特率
iComTimeout    //串口操作超时时间 单位为秒(S)
iMainkeyIndex	//主密钥索引号【0-15】，密码键盘根据此索引，//取出存在密码键盘中的主密钥。
iWorkKeyIndex   //工作密钥索引号【0-7】
iWorkKeyLen    //工作密钥长度【8,16,24】
pszWorkKey     //用iMainkeyIndex指向的主密钥加密过的工作
//密钥，下发给密码键盘(密文)
pszChkCode		 //工作密钥校验码
返回值:
0-成功 非0-失败
  备注：
1.把用工作密钥加密过的工作密钥发送给密码键盘，以对用户密码进行加密。
2.主密钥默认使用第0组，第1-15组备用。
3.工作密钥默认使用第0组，1-7组备用
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

	//1.下装工作密钥
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
	ret = encrypt(1,1,0,0,ZeroData,8,encodeData);//加密
	if(ret!=0){
		ucHidClose();
		return -122;
	}

	//3.比较校验值
	if(memcmp(encodeData,binData,strlen(pszChkCode)/2)==0){	
		ucHidClose();
		return 0;
	}else{
		ucHidClose();
		return -110;
	}
}

/*
密码输入接口
输入参数说明：
iComPort   	    //串口号,如1表示COM1,2表示COM2,依次类推； 
iBaudRate  	   //波特率
iComTimeout     //串口操作超时时间 单位为秒(S)
iActTimeout      //交互等待超时时间 单位为秒(S)
iVoiceNo        //语音编号(编号在备注中约定)
iPwdMin        //输入密码限制(密码最小长度限制)
iPwdMax        //输入密码限制(密码最大长度限制)
iPwdRetryLimit   //输入密码次数限制,如果该值为0则无限制
bShuffle         //是否乱序输入       true-乱序  false-不乱序
bEncrype        //是否进行密码加密   true-加密  false-不加密
pszAcctNo			//参与加密运算的客户账号

输出参数说明:
pszInputPwd      //客户输入的密码信息
//或“TIMEOUT”-交互超时或RETRYOUT-输入密码次数超过规//定次数
piInputPwdLen    //客户输入的密码长度

返回值:
0-成功 非0-失败
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
获取设备状态
输入参数说明：
iComPort   	   //串口号,如1表示COM1,2表示COM2,依次类推； 
iBaudRate  	   //波特率
iComTimeout    //串口操作超时时间 单位为秒(S)

输出参数说明:
pszDevStatu     //设备状态 “REGULAR”-正常  “BKDOWN”-故障
piDevStatuLen   //设备状态长度

返回值:
0-成功 非0-失败	
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
获取设备信息
输入参数说明：
iComPort   	   //串口号,如1表示COM1,2表示COM2,依次类推； 
iBaudRate  	   //波特率
iComTimeout    //串口操作超时时间 单位为秒(S)

输出参数说明:
pszDevType       //设备状态 这里返回值由你先定
piDevTypeLen     //设备状态长度
pszDevSerialNo    //设备序列号
piDevSerialNoLen  //设备序列号长度

返回值:
0-成功 非0-失败
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
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
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
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
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
        
        
        //下装帐号
        for (int i=0; i<12; i++)
			sendBytes[i+8] = cardNo[strlen(cardNo)+i+3-16];
        
        int outlength=0;
		int iRet = iHIDSendReceive(sendBytes,8+12,receivedBytes,&outlength,timeout+1);
		if (iRet > 0 && receivedBytes[0]) {
			return -2;//发生错误
		}
		if(iRet==-1)
		{
			return -1;//超时
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


