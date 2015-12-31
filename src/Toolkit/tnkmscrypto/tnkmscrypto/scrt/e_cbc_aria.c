
/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_cbc_aria.c  ARIA, CBC mode

	 Creadted by DEV3

************************************************/ 

#ifndef NO_ARIA

#include "../include/scrt.h"
#include "../include/aria.h"
#include <string.h>

static int S_aria_cbc_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);
static int S_aria_cbc_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);
static int S_aria_cbc_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc);

static void S_aria_cbc_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl);

/* �˰����� Ư���� �˸´� ������ structure ������ ���� */
static SCRT_CIPHER d_cbc_128_aria=
{
	NID_aria_128_cbc, /* nid */
	16,  /* block length */
	16,  /* key length */
	16,  /* iv length */
	S_aria_cbc_128_init_key, /* iv, key setting �Լ� */
	S_aria_cbc_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aria_ks))
};

static SCRT_CIPHER d_cbc_192_aria=
{
	NID_aria_192_cbc, /* nid */
	16,  /* block length */
	24, /* key length */
	16,  /* iv length */
	S_aria_cbc_192_init_key, /* iv, key setting �Լ� */
	S_aria_cbc_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aria_ks))
};

static SCRT_CIPHER d_cbc_256_aria=
{
	NID_aria_256_cbc, /* nid */
	16,  /* block length */
	32, /* key length */
	16,  /* iv length */
	S_aria_cbc_256_init_key, /* iv, key setting �Լ� */
	S_aria_cbc_cipher,   /* �Ϻ�ȣȭ �Լ� */
	NULL,				   /* ������ �Լ� */
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+ /* structure size */
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.aria_ks))
};

/*
	Name :  SCRT_aria_128_cbc
 	Description : ARIA CBC 128bit ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�. 
 	Parameters
 	Return Value : d_cbc_128_aria ����ü�� ��ȯ�Ѵ�.
 	Note :
 */
SCRT_CIPHER *SCRT_aria_128_cbc(void)
{
	return(&d_cbc_128_aria);
}

/*
	Name : SCRT_aria_192_cbc
 	Description : ARIA CBC 192bit ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�. 
 	Parameters 
 	Return Value : d_cbc_192_aria ����ü�� ��ȯ�Ѵ�.
 	Note :
 */
SCRT_CIPHER *SCRT_aria_192_cbc(void)
{
	return(&d_cbc_192_aria);
}

/*
	Name : SCRT_aria_256_cbc
 	Description : ARIA CBC 256bit ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�. 
 	Parameters 
 	Return Value : d_cbc_256_aria ����ü�� ��ȯ�Ѵ�.
 	Note :
 */
SCRT_CIPHER *SCRT_aria_256_cbc(void)
{
	return(&d_cbc_256_aria);
}

/*
	Name : S_aria_cbc_128_init_key
 	Description : ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� ARIA 128bit CBC ��� ��ȣ �˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�.
 	Parameters
 	[out] ctx : ARIA ��/��ȣȭ�� ���� Ű ����ü �� ������ ���� ����ü.
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] isenc : ��/��ȣȭ FLAG (1:enc, other : dec)
 	Return Value : �����Ͽ��� ��� 1, ���� �Ͽ��� ��� 0
 	Note :
 */
static int S_aria_cbc_128_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if (isenc == 1)
		ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.aria_ks), key, 128);
	else
		ret = S_ARIA_Decrypt_KeySchedule(&(ctx->c.aria_ks), key, 128);

	if (ret < 0) return 0;

	return 1;
}

/*
	Name : S_aria_cbc_192_init_key
 	Description : ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� ARIA 192bit CBC ��� ��ȣ �˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�.
 	Parameters
 	[out] ctx : ARIA ��/��ȣȭ�� ���� Ű ����ü �� ������ ���� ����ü.
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] isenc : ��/��ȣȭ FLAG (1:enc, other : dec)
 	Return Value : �����Ͽ��� ��� 1, ���� �Ͽ��� ��� 0
 	Note :
 */
static int S_aria_cbc_192_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if (isenc == 1)
		ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.aria_ks), key, 192);
	else
		ret = S_ARIA_Decrypt_KeySchedule(&(ctx->c.aria_ks), key, 192);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*
	Name : S_aria_cbc_256_init_key
	Description : ��ȣŰ �� �ʱ⺤�͸� �̿��Ͽ� ARIA 256bit CBC ��� ��ȣ �˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�.
	Parameters
	[out] ctx : ARIA ��/��ȣȭ�� ���� Ű ����ü �� ������ ���� ����ü.
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	[in] isenc : ��/��ȣȭ ���� FLAG (1:enc, other : dec)
	Return Value : �����Ͽ��� ��� 1, �׿��� ��� 0
 	Note :
 */
static int S_aria_cbc_256_init_key(SCRT_CIPHER_CTX *ctx, U8 *key, U8 *iv, int isenc)
{
	int ret ;
	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);

	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);  /* iv setting */

	if (isenc == 1)
		ret = S_ARIA_Encrypt_KeySchedule(&(ctx->c.aria_ks), key, 256);
	else
		ret = S_ARIA_Decrypt_KeySchedule(&(ctx->c.aria_ks), key, 256);
	
	if (ret < 0) return 0;
	
	return 1;
}

/*
	Name : S_aria_cbc_cipher
 	Description : ��ȣŰ �� �ʱ⺤�� ������ ARIA CBC ��� ��ȣ �˰����� �̿��Ͽ� ��/��ȣȭ �Ѵ�.
 	Parameters
 	[in] ctx : ARIA ��/��ȣȭ�� ���� Ű����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ ��� ������
	[in] in : ��/��ȣȭ �� ������
	[in] inl : ��/��ȣȭ �� ������ ����
 	Return Value :
 	Note :
 */
static void S_aria_cbc_cipher(SCRT_CIPHER_CTX *ctx, U8 *out, U8 *in, unsigned int inl)
{
	if (ctx->encrypt) 
		S_ARIA_CBC_Encrypt(&(ctx->c.aria_ks), (U8 *)&(ctx->iv), in, inl, out);
	else
		S_ARIA_CBC_Decrypt(&(ctx->c.aria_ks), (U8 *)&(ctx->iv), in, inl, out);
}

#endif
