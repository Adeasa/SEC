
/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_cbc_aria.c  ARIA, CBC mode

	 Creadted by DEV3

************************************************/

#ifndef NO_ARIA

#include "../include/scrt.h"
#include "../include/aria.h"
#include <string.h>
#include <math.h>

static int S_aria_ecb_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);
static int S_aria_ecb_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);
static int S_aria_ecb_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);

static void S_aria_ecb_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl);

/* ARIA 128bit EBC �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_ecb_128_aria=
{
	NID_aria_128_ecb, /* nid */
	16,  /* block length */
	16, /* key length */
	0,  /* iv length */
	S_aria_ecb_128_init_key, /* iv, key setting �Լ� */
	S_aria_ecb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aria_ks))
};

/* ARIA 192bit EBC �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_ecb_192_aria=
{
	NID_aria_192_ecb, /* nid */
	16,  /* block length */
	24, /* key length */
	0,  /* iv length */
	S_aria_ecb_192_init_key, /* iv, key setting �Լ� */
	S_aria_ecb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aria_ks))
};

/* ARIA 256bit EBC �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_ecb_256_aria=
{
	NID_aria_256_ecb, /* nid */
	16,  /* block length */
	32, /* key length */
	0,  /* iv length */
	S_aria_ecb_256_init_key, /* iv, key setting �Լ� */
	S_aria_ecb_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aria_ks))
};

/* structure�� return �ϴ� �Լ� */
/*	
	Name : SCRT_aria_128_ecb
	Description: ARIA 128 bit ECB ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ecb_128_aria ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_aria_128_ecb(void)
{
	return(&d_ecb_128_aria);
}

/*	
	Name : SCRT_aria_192_ecb
	Description: ARIA 192 bit ECB ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ecb_192_aria ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_aria_192_ecb(void)
{
	return(&d_ecb_192_aria);
}

/*	
	Name : SCRT_aria_256_ecb
	Description: ARIA 256 bit ECB ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : d_ecb_256_aria ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_aria_256_ecb(void)
{
	return(&d_ecb_256_aria);
}

/*	
	Name : S_aria_ecb_128_init_key
	Description: ��ȣŰ�� �̿��Ͽ� ARIA 128bit ECB ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : ARIA ��/��ȣȭ�� ���� ARIA_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : 
	[in] isenc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ , �׿� : ��ȣȭ )
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 ���� ��ȯ�Ѵ�
	Note :
*/
static int S_aria_ecb_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;
	
	if (isenc == 1)
		ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.aria_ks), key, 128);
	else
		ret = S_ARIA_Decrypt_KeySchedule(&(ctx->c.aria_ks), key, 128);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*	
	Name : S_aria_ecb_192_init_key
	Description: ��ȣŰ�� �̿��Ͽ� ARIA 192bit ECB ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : ARIA ��/��ȣȭ�� ���� ARIA_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : 
	[in] isenc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ , �׿� : ��ȣȭ )
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 ���� ��ȯ�Ѵ�
	Note :
*/
static int S_aria_ecb_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;

	if (isenc == 1)
		ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.aria_ks), key, 192);
	else
		ret = S_ARIA_Decrypt_KeySchedule(&(ctx->c.aria_ks), key, 192);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*	
	Name : S_aria_ecb_256_init_key
	Description: ��ȣŰ�� �̿��Ͽ� ARIA 256bit ECB ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : ARIA ��/��ȣȭ�� ���� ARIA_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : 
	[in] isenc : ��/��ȣȭ ���� FLAG ( 1 : ��ȣȭ , �׿� : ��ȣȭ )
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0 ���� ��ȯ�Ѵ�
	Note :
*/
static int S_aria_ecb_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;
	
	if (isenc == 1)
		ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.aria_ks), key, 256);
	else
		ret = S_ARIA_Decrypt_KeySchedule(&(ctx->c.aria_ks), key, 256);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*	
	Name : S_aria_ecb_cipher
	Description: ��ȣŰ�� �̿��Ͽ� ARIA ECB ��� ��ȣ�˰����� �̿��Ͽ� �Է�/��ȣȭ�� �����͸� ��/��ȣȭ �Ѵ�
	Parameters
	[in] ctx : ARIA ��/��ȣȭ�� ���� ARIA_KEY Ű ����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ �� ������
	[in] in : ��/��ȣȭ �� ������
	[in] inl : ��/��ȣȭ �� ������ ����
	Return Value : 
	Note :
*/
static void S_aria_ecb_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl)
{
	if (ctx->encrypt)
		S_ARIA_ECB_Encrypt(&(ctx->c.aria_ks), in, inl, out);
	else
		S_ARIA_ECB_Decrypt(&(ctx->c.aria_ks), in, inl, out);
}
#endif