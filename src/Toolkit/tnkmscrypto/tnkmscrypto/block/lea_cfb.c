/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     lea_cfb.c, LEA �˰���, CFB128 ����

	 Creadted by DEV3

************************************************/

#ifndef NO_LEA

#include <stdio.h>
#include "../include/typeconvert.h"
#include "../include/lea.h"

/*	
	Name : S_LEA_CFB128_Encrypt
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� LEA CFB128 ��� ��ȣ�˰������� �Է� �����͸� ��ȣȭ �Ѵ�
	Parameters
	[in] key : LEA Ű ����ü
	[in] ivec : �ʱ⺤��
	[in] in : ��ȣȭ �� ������
	[in] in_len : ��ȣȭ �� ������ ����
	[out] out : ��ȣȭ �� ������
	Return Value : 
	Note : 
*/
int S_LEA_CFB128_Encrypt(LEA_KEY *key, U8 *ivec, const U8 *in, unsigned int in_len, U8 *out)
{
	const unsigned char *pIv = ivec;
	unsigned char block[16];
	unsigned int nBlock1 = in_len >> 4, i=0;

	if (!key){
		return -1;
	}

	if (!ivec){
		return -3;
	}

	if (in_len > 0 && (!in||!out)){
		return -4;
	}

	for(i = 0; i < nBlock1; i++, in += 0x10, out += 0x10)
	{
		S_LEA_Encrypt(key, pIv, block);
		XOR8x16(out, block, in);

		pIv = out;
	}

	if (in_len & 0xf){
		S_LEA_Encrypt(key, pIv, block);

		for (i = 0; i < (in_len & 0xf); i++)
		{
			out[i] = block[i] ^ in[i];
		}
	}

	return 0;
}

/*	
	Name : S_LEA_CFB128_Decrypt
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� LEA CFB128 ��� ��ȣ�˰������� ��ȣȭ�� �����͸� ��ȣȭ �Ѵ�
	Parameters
	[in] key : LEA Ű ����ü
	[in] ivec : �ʱ⺤��
	[in] in : ��ȣȭ �� ������
	[in] in_len : ��ȣȭ �� ������ ����
	[out] out : ��ȣȭ �� ������
	Return Value : 
	Note : 
*/
int S_LEA_CFB128_Decrypt(LEA_KEY *key, U8 *ivec, const U8 *in, unsigned int in_len, U8 *out)
{
	U32 n = 0;
	U32 l = in_len;
	U8 c;
	
	if (!key){
		return -1;
	}

	if (!ivec){
		return -3;
	}

	if (in_len > 0 && (!in||!out)){
		return -4;
	}
	
	while (l--) 
	{
		if (n == 0) 
		{
			S_LEA_Encrypt(key, ivec, ivec);
		}
		c = *(in);
		*(out++) = *(in++) ^ ivec[n];
		ivec[n] = c;
		n = (n + 1) % LEA_BLOCK_SIZE;
	}

	return 0;
}


#endif