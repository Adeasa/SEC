/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     aria_cfb.c, ARIA, CFB ���

	 Creadted by DEV3

************************************************/

#ifndef NO_ARIA

#include <string.h>
#include "../include/aria.h"

/*	
	Name : S_ARIA_CFB128_Encrypt
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� ARIA CFB ��� ��ȣ�˰������� �Է� �����͸� ��ȣȭ �Ѵ�
	Parameters
	[in] key : ARIA Ű ����ü
	[in] ivec : �ʱ⺤��
	[in] in : ��ȣȭ �� ������
	[in] inbytes : ��ȣȭ �� ������ ����
	[out] out : ��ȣȭ �� ������
	[in] numbits : 128��Ʈ ���� �������� ��������

	Return Value : 
	Note : 
*/
void S_ARIA_CFB128_Encrypt(ARIA_KEY *key, U8 *ivec, U8 *in, int inbytes, U8 *out, int *numbits)
{
	U32 n;
	U32 l = inbytes;
	
	n = *numbits;
	
	while (l--) 
	{
		if (n == 0) 
		{
			S_ARIA_Encrypt(key, ivec, ivec);
		}
		
		ivec[n] = *(out++) = *(in++) ^ ivec[n];
		n = (n + 1) % ARIA_BLOCK_SIZE;
	}
	
	*numbits = n;
}

/*	
	Name : S_ARIA_CFB128_Decrypt
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� ARIA CFB ��� ��ȣ�˰������� ��ȣȭ�� �����͸� ��ȣȭ �Ѵ�
	Parameters
	[in] key : ARIA Ű ����ü
	[in] ivec : �ʱ⺤��
	[in] in : ��ȣȭ �� ������
	[in] inbytes : ��ȣȭ �� ������ ����
	[out] out : ��ȣȭ �� ������
	[in] numbits : 128��Ʈ ���� �������� ��������
	Return Value : 
	Note : 
*/
void S_ARIA_CFB128_Decrypt(ARIA_KEY *key, U8 *ivec, U8 *in, int inbytes, U8 *out, int *numbits)
{
	U32 n;
	U32 l = inbytes;
	U8 c;
	
	n = *numbits;
	
	while (l--) 
	{
		if (n == 0) 
		{
			S_ARIA_Encrypt(key, ivec, ivec);
		}
		c = *(in);
		*(out++) = *(in++) ^ ivec[n];
		ivec[n] = c;
		n = (n + 1) % ARIA_BLOCK_SIZE;
	}
	
	*numbits = n;
}

#endif