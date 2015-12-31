/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     lea_ecb.c, LEA �˰���, ECB ����

	 Creadted by DEV3

************************************************/

#ifndef NO_LEA

#include <stdio.h>
#include "../include/typeconvert.h"
#include "../include/lea.h"

/*	
	Name : S_LEA_ECB_Encrypt
	Description: LEA �˰��� ECB��� ��ȣȭ �ϴ� �Լ�
	Parameters
	[in] in : ��ȣȭ�� ������	
	[in] key : Ű ����ü
	[in] inbytes : ��ȣȭ ��� �� ������ ���̰�
	[out] out : ��ȣȭ�� ������
	Return Value : 
	Note : 
*/	

int S_LEA_ECB_Encrypt(LEA_KEY *key, const U8 *in, unsigned int in_len, U8 *out)
{
	unsigned int remainBlock = in_len >> 4;

	if (!key){
		return -1;
	}

	if (in_len > 0 && (!in || !out)){
		return -4;
	}

	if (in_len & 0xf){
		return -4;
	}

	for (; remainBlock >= 1; remainBlock -= 1, in += 0x10, out += 0x10)
	{
		S_LEA_Encrypt(key, in, out);
	}

	return 0;
}

/*	
	Name : S_LEA_ECB_Decrypt
	Description: LEA �˰��� ECB��� ��ȣȭ �ϴ� �Լ�
	Parameters
	[in] in : ��ȣȭ�� ��ȣȭ ������	
	[in] key : Ű ����ü
	[in] inbytes : ��ȣȭ ��� �� ������ ���̰�
	[out] out : ��ȣȭ�� ������
	Return Value : 
	Note : 
*/

int S_LEA_ECB_Decrypt(LEA_KEY *key, const U8 *in, unsigned int in_len, U8 *out)
{
	unsigned int remainBlock = in_len >> 4;

	if (!key){
		return -1;
	}

	if (in_len > 0 && (!in || !out)){
		return -4;
	}

	if (in_len & 0xf){
		return -4;
	}

	for (; remainBlock >= 1; remainBlock -= 1, in += 0x10, out += 0x10)
	{
		S_LEA_Decrypt(key, in, out);
	}

	return 0;
}

#endif