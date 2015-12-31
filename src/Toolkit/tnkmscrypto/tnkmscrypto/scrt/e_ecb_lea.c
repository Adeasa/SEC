
/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_ecb_lea.c  LEA, ECB mode

	 Creadted by DEV3

************************************************/

#ifndef NO_LEA

#include "../include/scrt.h"
#include "../include/lea.h"
#include <string.h>
#include <math.h>

static int S_lea_ecb_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);
static int S_lea_ecb_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);
static int S_lea_ecb_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);

static void S_lea_ecb_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl);

/* LEA 128bit EBC �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_ecb_128_lea=
{
	NID_lea_128_ecb, /* nid */
	16,  /* block length */
	16, /* key length */
	0,  /* iv length */
	S_lea_ecb_128_init_key, /* iv, key setting �Լ� */
	S_lea_ecb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.lea_ks))
};

/* LEA 192bit EBC �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_ecb_192_lea=
{
	NID_lea_192_ecb, /* nid */
	16,  /* block length */
	24, /* key length */
	0,  /* iv length */
	S_lea_ecb_192_init_key, /* iv, key setting �Լ� */
	S_lea_ecb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.lea_ks))
};

/* LEA 256bit EBC �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_ecb_256_lea=
{
	NID_lea_256_ecb, /* nid */
	16,  /* block length */
	32, /* key length */
	0,  /* iv length */
	S_lea_ecb_256_init_key, /* iv, key setting �Լ� */
	S_lea_ecb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.lea_ks))
};

/* structure�� return �ϴ� �Լ� */
/*	
	Name : SCRT_lea_128_ecb
	Description: LEA 128 bit ECB ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ecb_128_lea ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_lea_128_ecb(void)
{
	return(&d_ecb_128_lea);
}

/*	
	Name : SCRT_lea_192_ecb
	Description: LEA 192 bit ECB ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ecb_192_lea ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_lea_192_ecb(void)
{
	return(&d_ecb_192_lea);
}

/*	
	Name : SCRT_lea_256_ecb
	Description: LEA 256 bit ECB ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ecb_256_lea ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_lea_256_ecb(void)
{
	return(&d_ecb_256_lea);
}

/*	
	Name : S_lea_ecb_128_init_key
	Description: ��ȣŰ�� �̿��Ͽ� LEA 128bit ECB ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : LEA ��/��ȣȭ�� ���� LEA_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : 
	[in] isenc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ , �׿� : ��ȣȭ )
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 ���� ��ȯ�Ѵ�
	Note :
*/
static int S_lea_ecb_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;
	
	if (isenc == 1)
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 16);
		//ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.lea_ks), key, 128);
	else
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 16);
		//ret = S_lea_Decrypt_KeySchedule(&(ctx->c.lea_ks), key, 128);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*	
	Name : S_lea_ecb_192_init_key
	Description: ��ȣŰ�� �̿��Ͽ� LEA 192bit ECB ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : LEA ��/��ȣȭ�� ���� LEA_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : 
	[in] isenc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ , �׿� : ��ȣȭ )
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 ���� ��ȯ�Ѵ�
	Note :
*/
static int S_lea_ecb_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;

	if (isenc == 1)
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 24);
		//ret = S_lea_Encrypt_KeySchedule(&(ctx->c.lea_ks), key, 192);
	else
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 24);
		//ret = S_lea_Decrypt_KeySchedule(&(ctx->c.lea_ks), key, 192);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*	
	Name : S_lea_ecb_256_init_key
	Description: ��ȣŰ�� �̿��Ͽ� LEA 256bit ECB ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : LEA ��/��ȣȭ�� ���� LEA_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : 
	[in] isenc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ , �׿� : ��ȣȭ )
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 ���� ��ȯ�Ѵ�
	Note :
*/
static int S_lea_ecb_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;
	
	if (isenc == 1)
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 32);
		//ret = S_lea_Encrypt_KeySchedule(&(ctx->c.lea_ks), key, 256);
	else
		ret = S_LEA_Keyschedule(&(ctx->c.lea_ks), key, 32);
		//ret = S_lea_Decrypt_KeySchedule(&(ctx->c.lea_ks), key, 256);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*	
	Name : S_lea_ecb_cipher
	Description: ��ȣŰ�� �̿��Ͽ� LEA ECB ��� ��ȣ�˰����� �̿��Ͽ� �Է�/��ȣȭ�� �����͸� ��/��ȣȭ �Ѵ�
	Parameters
	[in] ctx : LEA ��/��ȣȭ�� ���� LEA_KEY Ű ����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ �� ������
	[in] in : ��/��ȣȭ �� ������
	[in] inl : ��/��ȣȭ �� ������ ����
	Return Value : 
	Note :
*/
static void S_lea_ecb_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl)
{
	if (ctx->encrypt)
		S_LEA_ECB_Encrypt(&(ctx->c.lea_ks), in, inl, out);
		//S_lea_ECB_Encrypt(&(ctx->c.lea_ks), in, inl, out);
	else
		S_LEA_ECB_Decrypt(&(ctx->c.lea_ks), in, inl, out);
		//S_lea_ECB_Decrypt(&(ctx->c.lea_ks), in, inl, out);
}
#endif