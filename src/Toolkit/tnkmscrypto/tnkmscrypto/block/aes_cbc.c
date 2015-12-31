/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     aes_cbc.c, AES �˰���, CBC mode

	 Creadted by DEV3

************************************************/
#ifndef AES_DEBUG
# ifndef NDEBUG
#  define NDEBUG
# endif
#endif
#include <assert.h>

#include "../include/aes.h"
#include <string.h>

/*	
	Name : S_AES_CBC_Encrypt
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� AES CBC ��� ��ȣ�˰������� �Է� �����͸� ��ȣȭ �Ѵ�
	Parameters
	[in] in : ��ȣȭ �� ������
	[out] out : ��ȣȭ �� ������
	[in] length : ��ȣȭ �� ������ ����
	[in] key : AES Ű ����ü
	[in] ivec : �ʱ⺤��
	Return Value :
	Note : 
*/
void S_AES_CBC_Encrypt(U8 *in, U8 *out, U32 length, AES_KEY_ST *key, U8 *ivec) 
{

	unsigned long n;
	unsigned long len = length;
	const unsigned char *iv = ivec;

	assert(in && out && key && ivec);

	while (len >= AES_BLOCK_SIZE) 
	{
		for(n=0; n < AES_BLOCK_SIZE; ++n)
			out[n] = in[n] ^ iv[n];
		S_AES_Encrypt(out, out, key);
		iv = out;
		len -= AES_BLOCK_SIZE;
		in += AES_BLOCK_SIZE;
		out += AES_BLOCK_SIZE;
	}

	if (len) 
	{
		for(n=0; n < len; ++n)
			out[n] = in[n] ^ iv[n];
		for(n=len; n < AES_BLOCK_SIZE; ++n)
			out[n] = iv[n];
		S_AES_Encrypt(out, out, key);
		iv = out;
	}
	memcpy(ivec,iv,AES_BLOCK_SIZE);
}

/*	
	Name : S_AES_CBC_Decrypt
	Description: ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� AES CBC ��� ��ȣ�˰������� ��ȣȭ�� �����͸� ��ȣȭ �Ѵ�
	Parameters
	[in] in : ��ȣȭ �� ������
	[out] out : ��ȣȭ �� ������
	[in] length : ��ȣȭ �� ������ ����
	[in] key : AES Ű ����ü
	[in] ivec : �ʱ⺤��
	Return Value :
	Note : 
*/
void S_AES_CBC_Decrypt(U8 *in, U8 *out, U32 length, AES_KEY_ST *key, U8 *ivec) 
{

	unsigned long n;
	unsigned long len = length;
	U8 tmp[AES_BLOCK_SIZE];//unsigned char tmp[AES_BLOCK_SIZE]; dgshin-1204
	const unsigned char *iv = ivec;

	assert(in && out && key && ivec);

	if (in != out) 
	{
		while (len >= AES_BLOCK_SIZE) 
		{
			S_AES_Decrypt(in, out, key);
			for(n=0; n < AES_BLOCK_SIZE; ++n)
				out[n] ^= iv[n];
			iv = in;
			len -= AES_BLOCK_SIZE;
			in  += AES_BLOCK_SIZE;
			out += AES_BLOCK_SIZE;
		}
		if (len) 
		{
			S_AES_Decrypt(in,tmp,key);
			for(n=0; n < len; ++n)
				out[n] = tmp[n] ^ iv[n];
			iv = in;
		}
		memcpy(ivec,iv,AES_BLOCK_SIZE);

	}else 
	{
		while (len >= AES_BLOCK_SIZE) 
		{
			memcpy(tmp, in, AES_BLOCK_SIZE);
			S_AES_Decrypt(in, out, key);
			for(n=0; n < AES_BLOCK_SIZE; ++n)
				out[n] ^= ivec[n];
			memcpy(ivec, tmp, AES_BLOCK_SIZE);
			len -= AES_BLOCK_SIZE;
			in += AES_BLOCK_SIZE;
			out += AES_BLOCK_SIZE;
		}
		if (len) 
		{
			memcpy(tmp, in, AES_BLOCK_SIZE);
			S_AES_Decrypt(tmp, out, key);
			for(n=0; n < len; ++n)
				out[n] ^= ivec[n];
			for(n=len; n < AES_BLOCK_SIZE; ++n)
				out[n] = tmp[n];
			memcpy(ivec, tmp, AES_BLOCK_SIZE);
		}
	}
}

