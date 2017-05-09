#ifndef HXIMATEAPPFACE_H__
#define HXIMATEAPPFACE_H__

#ifdef __cplusplus
extern "C" {
#endif



//int __stdcall AKP_InputPwd(int iComPort);

/*
����˵��:
	����Ӧ�ñ�� 

�������:
CommPort	�ַ���,ʹ�ô��ڵĶ˿ں�,����Ϊ 4 ���ַ���ǰ�����ַ���Ϊ COM,��� һλȡֵΪ 1~8����:"COM1"��
ExtPort		�ַ���,BP ����չ��ѡ��ָ���Ϊ��ʱ��ʾ��ʹ����չ�С�����:ѡ����չ �� K ָ�Ϊ:"\033%K"��
idnum:		ASCII �����,���� 1-20 λ; 

����ֵ:		0 �ɹ� <0 ʧ��
*/
int SDBSetPinPadID(const char * CommPort, const char *ExtPort, const char* idnum);

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
int SDBGetPinPadID(const char * CommPort, const char *ExtPort, char *idnum); 

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
int SDBGetCompanyPinPadID(const char * CommPort, const char *ExtPort, char *SeqNo);

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
int SDBRestDefaultKey(const char * CommPort, const char *ExtPort, const char* TMKIndex);


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
int SDBInputTMK(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char* Tmk1, const char* Tmk2, char* Tmkchk);

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
int SDBLoadTMK(const char * CommPort, const char *ExtPort, const char * NewKEYLen, const char * TMKIndex, const char * NewTMKByOldTMK, const char * KeyValue);

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
int SDBInputKEY(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, const char * KeyValue);

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
int SDBInputKeyNoCheck(const char * CommPort, const char *ExtPort, const char * TMKIndex, const char * KEYIndex, const char * KeyLen, const char * keyByTMK, char * KeyValue);


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
int SDBCheckKEY(const char * CommPort, const char * ExtPort, const char * KEYIndex, const char * TMKIndex, char * KeyLen, char * KeyValue);


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
	pinLength, char * pinCrypt, char * TPKCheck);

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
int SDBGenerateMAC(const char * CommPort, const char * ExtPort, const char * TMKIndex, const int lenOfMacData, const char * macData, char * mac, char * TAKCheck); 



/*
����˵��:
�����ʺŵõ� ANSI X9.8 ��ʽҪ��� 12 λ�ʺ�
������� :
AccNo			ԭʼ�ʺ� ������� :
AnsiX98AccNo	ANSIX9.8��ʽҪ����ʺ� 
����ֵ :
				0, �ɹ��� <0, ʧ�ܡ�
*/
int SDBFormatAnsiX98Account(char *AccNo, char *AnsiX98AccNo); 

/*
����˵��:
	�û���ֹ��ǰ����
����ֵ :
	-15, ��ֹ��ǰ����
*/
int SDBPinpadCancel(void); 



#ifdef __cplusplus
}
#endif

#endif
