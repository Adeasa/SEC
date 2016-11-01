/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_cfb_s256.c SEED256, CFB mode

	 Creadted by DEV3

************************************************/

#ifndef NO_SEED
#include "../include/scrt.h"
#include "../include/seed256.h"
#include <stdio.h>
#include <string.h>

static void S_seed256_cfb_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key, unsigned char *iv);
static void S_seed256_cfb_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl);


static SCRT_CIPHER s256_cfb_cipher=
{
	NID_seed256_cfb64,
	1,32,16,
	S_seed256_cfb_init_key,
	S_seed256_cfb_cipher,
	NULL,
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.seed256_ks)),
};


/*	
	Name : SCRT_seed256_cfb
	Description: SEED �˰����� CFB ���忡�� ����� ������ ����ü�� ��ȯ�Ѵ�.
	Parameters
	Return Value : SEED �˰��� CFB ����� ������ ����ü
	Note : 
*/
SCRT_CIPHER *SCRT_seed256_cfb(void)
{
	return(&s256_cfb_cipher);
}

/*	
	Name : S_seed256_cfb_init_key
	Description: SEED �˰����� CFB ���忡�� ����� Ű ����ü���� �����Ѵ�.
	Parameters
	[in/out] ctx : Ű ����ü���� ������ ���ؽ�Ʈ
	[in] key : ��ȣŰ ��
	[in] iv : �ʱ� ���Ͱ�
	Return Value : 
	Note : 
*/	
static void S_seed256_cfb_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key, unsigned char *iv)
{
	ctx->num=0;

	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);
	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);

	S_SEED256_KeySchedule(&(ctx->c.seed256_ks),key);
}
	
/*	
	Name : S_seed256_cfb_cipher
	Description: SEED �˰����� CFB ����� ��ȣȭ�� �����Ѵ�.
	Parameters
	[in] ctx : Ű ����ü���� �����ϰ� �ִ� ���ؽ�Ʈ
	[out] out : ��ȣȭ �����
	[in] in : ��ȣȭ ��� ������
	[in] inl : ��ȣȭ ��� ������ ����
	Return Value : 
	Note : 
*/	
static void S_seed256_cfb_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl)
{
	if(ctx->encrypt)
		S_SEED256_CFB_Encrypt(in,out,&(ctx->c.seed256_ks),&(ctx->num),inl,(U8 *)&(ctx->iv));
	else
		S_SEED256_CFB_Decrypt(in,out,&(ctx->c.seed256_ks),&(ctx->num),inl,(U8 *)&(ctx->iv));
}

#endif
