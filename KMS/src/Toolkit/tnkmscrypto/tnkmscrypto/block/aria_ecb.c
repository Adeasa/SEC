/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     aria_ecb.c, ARIA, ECB ���

	 Creadted by DEV3

************************************************/

#ifndef NO_ARIA

#include <string.h>
#include "../include/aria.h"

/*	
	Name : S_ARIA_ECB_Encrypt
	Description: ARIA �˰��� ECB��� ��ȣȭ �ϴ� �Լ�
	Parameters
	[in] in : ��ȣȭ�� ������	
	[in] key : Ű ����ü
	[in] inbytes : ��ȣȭ ��� �� ������ ���̰�
	[out] out : ��ȣȭ�� ������
	Return Value : 
	Note : 
*/	
void S_ARIA_ECB_Encrypt(ARIA_KEY *key, U8 *in, int inbytes, U8 *out)
{
	U32 len = inbytes;
	
	while (len >= ARIA_BLOCK_SIZE) 
	{
		S_ARIA_Encrypt(key, in, out);
		len -= ARIA_BLOCK_SIZE;
		in += ARIA_BLOCK_SIZE;
		out += ARIA_BLOCK_SIZE;
	}
	
	if (len) 
	{
		S_ARIA_Encrypt(key, in, out);
	}
}
/*	
	Name : S_ARIA_ECB_Decrypt
	Description: ARIA �˰��� ECB��� ��ȣȭ �ϴ� �Լ�
	Parameters
	[in] in : ��ȣȭ�� ��ȣȭ ������	
	[in] key : Ű ����ü
	[in] inbytes : ��ȣȭ ��� �� ������ ���̰�
	[out] out : ��ȣȭ�� ������
	Return Value : 
	Note : 
*/
void S_ARIA_ECB_Decrypt(ARIA_KEY *key, U8 *in, int inbytes, U8 *out)
{
	U32 len = inbytes;
	
	while (len >= ARIA_BLOCK_SIZE) 
	{
		S_ARIA_Decrypt(key, in, out);
		len -= ARIA_BLOCK_SIZE;
		in += ARIA_BLOCK_SIZE;
		out += ARIA_BLOCK_SIZE;
	}
	
	if (len) 
	{
		S_ARIA_Decrypt(key, in, out);
	}
}

#endif
