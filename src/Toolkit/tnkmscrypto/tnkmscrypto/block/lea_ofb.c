/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     lea_ofb.c, LEA �˰���, OFB128 ����

	 Creadted by DEV3

************************************************/

#ifndef NO_LEA

#include <stdio.h>
#include "../include/typeconvert.h"
#include "../include/lea.h"

/*	
	Name : S_LEA_OFB128_Encrypt
	Description: OFB ����� 128��Ʈ ��ȣŰ�� ����Ͽ� �� �����͸� LEA �˰������� ��ȣȭ�Ѵ�.
	Parameters
	[in] key : �� ���忡�� ����� Ű ����ü
	[in] iv : �ʱ� ���Ͱ�
	[in] in : ��ȣȭ ��� �� ������
	[in] in_len : ��ȣȭ ��� �� ������ ���̰�
	[out] out : ��ȣȭ ����� ������
	Return Value : 
	Note : 
*/
int S_LEA_OFB128_Encrypt(LEA_KEY *key, U8 *ivec, const U8 *in, unsigned int in_len, U8 *out)
{
	unsigned int numBlock1 = in_len >> 4, i=0;

	if (!key){
		return -1;
	}

	if (!ivec){
		return -3;
	}

	if (in_len > 0 && (!out || !in)){
		return -4;
	}

	for(i = 0; i < numBlock1; i++, in += 0x10, out += 0x10)
	{
		S_LEA_Encrypt(key, ivec, ivec);

		XOR8x16(out, in, ivec);
	}

	if((numBlock1 << 4) != in_len)
	{
		S_LEA_Encrypt(key, ivec, ivec);

		for(i = 0; i < in_len - (numBlock1 << 4); i++)
			out[i] = ivec[i] ^ in[i];
	}

	return 0;
}

/*	
	Name : S_LEA_OFB128_Decrypt
	Description: OFB ����� 128��Ʈ ��ȣŰ�� ����Ͽ� �� �����͸� LEA �˰������� ��ȣȭ�Ѵ�.
	Parameters
	[in] key : �� ���忡�� ����� Ű ����ü
	[in] iv : �ʱ� ���Ͱ�
	[in] in : ��ȣȭ ��� ��ȣȭ ������
	[in] in_len : ��ȣȭ ��� ��ȣȭ ������ ���̰�
	[out] out : ��ȣȭ�� �� ������
	Return Value : 
	Note : 
*/
int S_LEA_OFB128_Decrypt(LEA_KEY *key, U8 *ivec, const U8 *in, unsigned int in_len, U8 *out)
{
	int ret;
	ret = S_LEA_OFB128_Encrypt(key, ivec, in, in_len, out);

	return ret;
}
#endif