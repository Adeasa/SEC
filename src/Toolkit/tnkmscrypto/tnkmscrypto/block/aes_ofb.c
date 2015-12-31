/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     aes_ofb.c, AES �˰���, OFB128 mode

	 Creadted by DEV3

************************************************/
#ifndef AES_DEBUG
# ifndef NDEBUG
#  define NDEBUG
# endif
#endif
#include <assert.h>
#include "../include/aes.h"

/* The input and output encrypted as though 128bit ofb mode is being
 * used.  The extra state information to record how much of the
 * 128bit block we have used is contained in *num;
 */
/*	
	Name : S_AES_OFB128_Encrypt
	Description: OFB ����� 128��Ʈ ��ȣŰ�� ����Ͽ� �� �����͸� AES �˰������� ��ȣȭ�Ѵ�.
	Parameters
	[in] in : ��ȣȭ ��� �� ������
	[out] out : ��ȣȭ ����� ������
	[in] length : ��ȣȭ ��� �� ������ ���̰�
	[in] key : �� ���忡�� ����� Ű ����ü
	[in] ivec : �ʱ� ���Ͱ�
	[in/out] num : 128��Ʈ ���� �������� ��������
	Return Value : 
	Note : 
*/
void S_AES_OFB128_Encrypt(U8 *in, U8 *out, U32 length, AES_KEY_ST *key, U8 *ivec, unsigned int *num) 
{
	unsigned int n;
	unsigned long l=length;

	assert(in && out && key && ivec && num);

	n = *num;

	while (l--) {
		if (n == 0) {
			S_AES_Encrypt(ivec, ivec, key);
		}
		*(out++) = *(in++) ^ ivec[n];
		n = (n+1) % AES_BLOCK_SIZE;
	}

	*num=n;
}

/*	
	Name : S_AES_OFB128_Decrypt
	Description: OFB ����� 128��Ʈ ��ȣŰ�� ����Ͽ� ��ȣȭ �����͸� AES �˰������� ��ȣȭ�Ѵ�.
	Parameters
	[in] in : ��ȣȭ ��� ��ȣȭ ������
	[out] out : ��ȣȭ�� �� ������
	[in] length : ��ȣȭ ��� ��ȣȭ ������ ���̰�
	[in] key : �� ���忡�� ����� Ű ����ü
	[in] ivec : �ʱ� ���Ͱ�
	[in/out] num : 128��Ʈ ���� �������� ��������
	Return Value : 
	Note : 
*/
void S_AES_OFB128_Decrypt(U8 *in, U8 *out, U32 length, AES_KEY_ST *key, U8 *ivec, unsigned int *num) 
{
	unsigned int n;
	unsigned long l=length;

	assert(in && out && key && ivec && num);

	n = *num;

	while (l--) {
		if (n == 0) {
			S_AES_Encrypt(ivec, ivec, key);
		}
		*(out++) = *(in++) ^ ivec[n];
		n = (n+1) % AES_BLOCK_SIZE;
	}

	*num=n;
}
