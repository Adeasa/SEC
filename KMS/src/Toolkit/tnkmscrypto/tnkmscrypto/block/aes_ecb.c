/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     aes_ecb.c, AES �˰���, ECB mode

	 Creadted by DEV3

************************************************/
#ifndef AES_DEBUG
# ifndef NDEBUG
#  define NDEBUG
# endif
#endif
#include <assert.h>

#include "../include/aes.h"
/*	
	Name : S_AES_ECB_Encrypt
	Description: AES �˰��� ECB��� ��ȣȭ �ϴ� �Լ�
	Parameters
	[in] in : ��ȣȭ�� ������
	[out] out : ��ȣȭ�� ������
	[in] key : Ű ����ü
	Return Value : 128bit ECB mode ��ȣȭ
	Note : 
*/	
void S_AES_ECB_Encrypt(U8 *in, U8 *out, AES_KEY_ST *key) 
{

    assert(in && out && key);

	S_AES_Encrypt(in, out, key);
}
/*	
	Name : S_AES_ECB_Decrypt
	Description: AES �˰��� ECB��� ��ȣȭ �ϴ� �Լ�
	Parameters
	[in] in : ��ȣȭ�� ��ȣȭ ������
	[out] out : ��ȣȭ�� ������
	[in] key : Ű ����ü
	Return Value : 128bit ECB mode ��ȣȭ
	Note : 
*/	
void S_AES_ECB_Decrypt(U8 *in, U8 *out, AES_KEY_ST *key) 
{

    assert(in && out && key);

	S_AES_Decrypt(in, out, key);
}