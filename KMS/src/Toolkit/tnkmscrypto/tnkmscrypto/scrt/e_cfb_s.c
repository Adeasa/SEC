
/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_cfb_s.c SEED, CFB mode

	 Creadted by DEV3

************************************************/

#ifndef NO_SEED

#include "../include/scrt.h"
#include "../include/seed.h"
#include <stdio.h>
#include <string.h>

static void S_seed_cfb_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key, unsigned char *iv);
static void S_seed_cfb_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl);

static SCRT_CIPHER s_cfb_cipher=
{
	NID_seed_cfb64,
	1,16,16,
	S_seed_cfb_init_key,
	S_seed_cfb_cipher,
	NULL,
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.seed_ks)),
};


/*	
	Name : SCRT_seed_cfb
	Description: SEED �˰����� CFB ���忡�� ����� ������ ����ü�� ��ȯ�Ѵ�.
	Parameters
	Return Value : SEED �˰��� CFB ����� ������ ����ü
	Note : 
*/
SCRT_CIPHER *SCRT_seed_cfb(void)
{
	return(&s_cfb_cipher);
}

/*	
	Name : S_seed_cfb_init_key
	Description: SEED �˰����� CFB ���忡�� ����� Ű ����ü���� �����Ѵ�.
	Parameters
	[in/out] ctx : Ű ����ü���� ������ ���ؽ�Ʈ
	[in] key : ��ȣŰ ��
	[in] iv : �ʱ� ���Ͱ�
	Return Value : 
	Note : 
*/	
static void S_seed_cfb_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key, unsigned char *iv)
{
	ctx->num=0;

	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);
	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);

	S_SEED_KeySchedule(&(ctx->c.seed_ks),key);
}
	
/*	
	Name : S_seed_cfb_cipher
	Description: SEED �˰����� CFB ����� ��ȣȭ�� �����Ѵ�.
	Parameters
	[in] ctx : Ű ����ü���� �����ϰ� �ִ� ���ؽ�Ʈ
	[out] out : ��ȣȭ �����
	[in] in : ��ȣȭ ��� ������
	[in] inl : ��ȣȭ ��� ������ ����
	Return Value : 
	Note : 
*/	
static void S_seed_cfb_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl)
{
	if(ctx->encrypt)
		S_SEED_CFB_Encrypt(in,out,&(ctx->c.seed_ks),&(ctx->num),inl,(U8 *)&(ctx->iv));
	else
		S_SEED_CFB_Decrypt(in,out,&(ctx->c.seed_ks),&(ctx->num),inl,(U8 *)&(ctx->iv));
}

static void S_seed_cfb128_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key, unsigned char *iv);
static void S_seed_cfb128_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl);
static SCRT_CIPHER s_cfb128_cipher=
{
	NID_seed_cfb128,
	1,16,16,
	S_seed_cfb128_init_key,
	S_seed_cfb128_cipher,
	NULL,
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.seed_ks)),
};

/*	
	Name : SCRT_seed_cfb128
	Description: SEED �˰����� CFB ���忡�� ����� ������ ����ü�� ��ȯ�Ѵ�.
	Parameters
	Return Value : SEED �˰��� CFB ����� ������ ����ü
	Note : 
*/
SCRT_CIPHER *SCRT_seed_cfb128(void)
{
	return(&s_cfb128_cipher);
}

/*	
	Name : S_seed_cfb128_init_key
	Description: SEED �˰����� CFB ���忡�� ����� Ű ����ü���� �����Ѵ�.
	Parameters
	[in/out] ctx : Ű ����ü���� ������ ���ؽ�Ʈ
	[in] key : ��ȣŰ ��
	[in] iv : �ʱ� ���Ͱ�
	Return Value : 
	Note : 
*/		
static void S_seed_cfb128_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key, unsigned char *iv)
{
	ctx->num=0;

	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);
	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);

	S_SEED_set_key_ex(key, &(ctx->c.seed_ks));
}
	

/*	
	Name : S_seed_cfb128_cipher
	Description: SEED �˰����� CFB ����� ��ȣȭ�� �����Ѵ�.
	Parameters
	[in] ctx : Ű ����ü���� �����ϰ� �ִ� ���ؽ�Ʈ
	[out] out : ��ȣȭ �����
	[in] in : ��ȣȭ ��� ������
	[in] inl : ��ȣȭ ��� ������ ����
	Return Value : 
	Note : 
*/
static void S_seed_cfb128_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl)
{
	if(ctx->encrypt)
		S_SEED_cfb128_encrypt_ex(in, out, inl, &(ctx->c.seed_ks), &(ctx->iv[0]), &(ctx->num));
	else
		S_SEED_cfb128_decrypt_ex(in, out, inl, &(ctx->c.seed_ks), &(ctx->iv[0]), &(ctx->num));
}

#endif
