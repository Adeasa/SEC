/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     lea_cbc.c, LEA �˰���, CBC����

	 Creadted by DEV3

************************************************/

#ifndef NO_LEA

#include <stdio.h>
#include "../include/typeconvert.h"
#include "../include/lea.h"

/*	
	Name : S_LEA_CBC_Encrypt
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� LEA CBC ��� ��ȣ�˰������� �Է� �����͸� ��ȣȭ �Ѵ�
	Parameters
	[in] key : LEA Ű ����ü
	[in] ivec : �ʱ⺤��
	[in] in : ��ȣȭ �� ������
	[in] in_len : ��ȣȭ �� ������ ����
	[out] out : ��ȣȭ �� ������
	Return Value :
	Note : 
*/
int S_LEA_CBC_Encrypt(LEA_KEY *key, U8 *ivec, const U8 *in, unsigned int in_len, U8 *out)
{
	unsigned int nBlock1 = in_len >> 4, i=0;
	const unsigned char *iv_ptr = ivec;

	if (out == NULL)
		return -5;
	else if (in == NULL)
		return -4;
	else if ((in_len == 0) || (in_len & 0xf))
		return -4;
	else if (ivec == NULL)
		return -3;
	else if (key == NULL)
		return -1;

	for(i = 0; i < nBlock1; i++, in += 16, out += 16)
	{
		XOR8x16(out, in, iv_ptr);
		S_LEA_Encrypt(key, out, out);

		iv_ptr = out;
	}

	return 0;
}

/*	
	Name : S_LEA_CBC_Decrypt
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� LEA CBC ��� ��ȣ�˰������� ��ȣȭ�� �����͸� ��ȣȭ �Ѵ�
	Parameters
	[in] key : LEA Ű ����ü
	[in] ivec : �ʱ⺤��
	[in] in : ��ȣȭ �� ������
	[in] in_len : ��ȣȭ �� ������ ����
	[out] out : ��ȣȭ �� ������
	Return Value :
	Note : 
*/
int S_LEA_CBC_Decrypt(LEA_KEY *key, U8 *ivec, const U8 *in, unsigned int in_len, U8 *out)
{
	unsigned int remainBlock = in_len >> 4;
	const unsigned char *pIv = ivec;

	if (out == NULL)
		return;
	else if (in == NULL)
		return;
	else if ((in_len == 0) || (in_len & 0xf))
		return;
	else if (ivec == NULL)
		return;
	else if (key == NULL)
		return;

	out += in_len;
	in += in_len;

	while (remainBlock > 1){ // > 1, not >= 1.
		out -= 0x10;
		in -= 0x10;
		pIv = in - 16;

		S_LEA_Decrypt(key, in, out);

		XOR8x16(out, out, pIv);

		remainBlock -= 1;
	}
	
	out -= 0x10;
	in -= 0x10;

	S_LEA_Decrypt(key, in, out);

	XOR8x16(out, out, ivec);

}


#endif