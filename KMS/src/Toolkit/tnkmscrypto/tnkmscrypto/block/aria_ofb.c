/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     aria_ofb.c, ARIA, OFB ���

	 Creadted by DEV3

************************************************/

#ifndef NO_ARIA

#include <string.h>
#include "../include/aria.h"

/*	
	Name : S_ARIA_OFB128_Encrypt
	Description: OFB ����� 128��Ʈ ��ȣŰ�� ����Ͽ� �� �����͸� ARIA �˰������� ��ȣȭ�Ѵ�.
	Parameters
	[in] key : �� ���忡�� ����� Ű ����ü
	[in] iv : �ʱ� ���Ͱ�
	[in] in : ��ȣȭ ��� �� ������
	[in] inbytes : ��ȣȭ ��� �� ������ ���̰�
	[out] out : ��ȣȭ ����� ������
	[in/out] numbits : 128��Ʈ ���� �������� ��������
	Return Value : 
	Note : 
*/
void S_ARIA_OFB128_Encrypt(ARIA_KEY *key, U8 *iv, U8 *in, unsigned int inbytes, U8 *out, int *numbits)
{
	unsigned int n;
	unsigned int l=inbytes;
	
	n = *numbits;

	while (l--) 
	{
		if (n == 0) 
		{
			S_ARIA_Encrypt(key, (U8 *)iv, (U8 *)iv);
		}
		*(out++) = *(in++) ^ iv[n];
		n = (n+1) % ARIA_BLOCK_SIZE;
	}
	
	*numbits=n;
}

/*	
	Name : S_ARIA_OFB128_Decrypt
	Description: OFB ����� 128��Ʈ ��ȣŰ�� ����Ͽ� �� �����͸� ARIA �˰������� ��ȣȭ�Ѵ�.
	Parameters
	[in] key : �� ���忡�� ����� Ű ����ü
	[in] iv : �ʱ� ���Ͱ�
	[in] in : ��ȣȭ ��� ��ȣȭ ������
	[in] inbytes : ��ȣȭ ��� ��ȣȭ ������ ���̰�
	[out] out : ��ȣȭ�� �� ������
	[in/out] numbits : 128��Ʈ ���� �������� ��������
	Return Value : 
	Note : 
*/
void S_ARIA_OFB128_Decrypt(ARIA_KEY *key, U8 *iv, U8 *in, unsigned int inbytes, U8 *out, int *numbits)
{
	S_ARIA_OFB128_Encrypt(key, iv, in, inbytes, out, numbits);
}

#endif