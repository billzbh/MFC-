#ifndef HXIMATEAPPFACE_H__
#define HXIMATEAPPFACE_H__

#ifdef __cplusplus
extern "C" {
#endif



//int __stdcall AKP_InputPwd(int iComPort);

/*
功能说明:
	设置应用编号 

输入参数:
CommPort	字符串,使用串口的端口号,必须为 4 个字符且前三个字符必为 COM,最后 一位取值为 1~8。例:"COM1"。
ExtPort		字符串,BP 盒扩展口选择指令串。为空时表示不使用扩展盒。例如:选择扩展 口 K 指令串为:"\033%K"。
idnum:		ASCII 码编码,长度 1-20 位; 

返回值:		0 成功 <0 失败
*/
int SDBSetPinPadID(const char * CommPort, const char *ExtPort, const char* idnum);

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
int SDBGetPinPadID(const char * CommPort, const char *ExtPort, char *idnum); 

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
int SDBGetCompanyPinPadID(const char * CommPort, const char *ExtPort, char *SeqNo);

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
int SDBRestDefaultKey(const char * CommPort, const char *ExtPort, const char* TMKIndex);


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
int SDBInputTMK(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char* Tmk1, const char* Tmk2, char* Tmkchk);

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
int SDBLoadTMK(const char * CommPort, const char *ExtPort, const char * NewKEYLen, const char * TMKIndex, const char * NewTMKByOldTMK, const char * KeyValue);

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
int SDBInputKEY(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, const char * KeyValue);

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
int SDBInputKeyNoCheck(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, char * KeyValue);


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
int SDBCheckKEY(const char * CommPort, const char * ExtPort, const char * KEYIndex, const char * TMKIndex, char * KeyLen, char * KeyValue);


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
	pinLength, char * pinCrypt, char * TPKCheck);

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
int SDBGenerateMAC(const char * CommPort, const char * ExtPort, const char * TMKIndex, const int lenOfMacData, const char * macData, char * mac, char * TAKCheck); 



/*
功能说明:
根据帐号得到 ANSI X9.8 格式要求的 12 位帐号
输入参数 :
AccNo			原始帐号 输出参数 :
AnsiX98AccNo	ANSIX9.8格式要求的帐号 
返回值 :
				0, 成功。 <0, 失败。
*/
int SDBFormatAnsiX98Account(char *AccNo, char *AnsiX98AccNo); 

/*
功能说明:
	用户中止当前操作
返回值 :
	-15, 中止当前操作
*/
int SDBPinpadCancel(void); 



#ifdef __cplusplus
}
#endif

#endif
