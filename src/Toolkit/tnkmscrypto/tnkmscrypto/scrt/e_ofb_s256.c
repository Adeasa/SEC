/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     e_ofb_s256.c SEED256, OFB64 mode

	 Creadted by DEV3

************************************************/ 

#ifndef NO_SEED
#include "../include/scrt.h"
#include "../include/seed256.h"
#include <stdio.h>
#include <string.h>

static void S_seed256_ofb_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key, unsigned char *iv);
static void S_seed256_ofb_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl);



static SCRT_CIPHER s256_ofb_cipher=
{
	NID_seed256_ofb64,
	1, 32, 16,
	S_seed256_ofb_init_key,
	S_seed256_ofb_cipher,
	NULL,
	sizeof(SCRT_CIPHER_CTX)-sizeof((((SCRT_CIPHER_CTX *)NULL)->c))+
		sizeof((((SCRT_CIPHER_CTX *)NULL)->c.seed256_ks))
};


/*	
	Name : SCRT_seed256_ofb
	Description: SEED OFB ��� ��ȣ�˰����� ����ϱ� ���� ������ ���� ����ü�� ��ȯ�Ѵ�
	Parameters
	Return Value : s_ofb_cipher ����ü�� ��ȯ�Ѵ�
	Note :
*/
SCRT_CIPHER *SCRT_seed256_ofb(void)
{
	return(&s256_ofb_cipher);
}
	
/*	
	Name : S_seed256_ofb_init_key
	Description: ��ȣŰ�� �̿��Ͽ� SEED OFB ��� ��ȣ�˰��� �ʿ��� Ű ����ü �� �˰��� ������ �����Ѵ�
	Parameters
	[out] ctx : SEED ��/��ȣȭ�� ���� SEED_KEY Ű ����ü �� ������ ���� ����ü
	[in] key : ��ȣŰ ������
	[in] iv : �ʱ⺤��
	Return Value : 
	Note :
*/		
static void S_seed256_ofb_init_key(SCRT_CIPHER_CTX *ctx, unsigned char *key,unsigned char *iv)
{
	ctx->num=0;

	if (iv != NULL)
		memcpy(&(ctx->oiv[0]),iv,16);
	memcpy(&(ctx->iv[0]),&(ctx->oiv[0]),16);

	S_SEED256_KeySchedule(&(ctx->c.seed256_ks),key);
}
	
/*	
	Name : S_seed256_ofb_cipher
	Description: ��ȣŰ�� �̿��Ͽ� SEED OFB ��� ��ȣ�˰����� �̿��Ͽ� �Է�/��ȣȭ�� �����͸� ��/��ȣȭ �Ѵ�
	Parameters
	[in] ctx : SEED ��/��ȣȭ�� ���� SEED_KEY Ű ����ü �� ������ ���� ����ü
	[out] out : ��/��ȣȭ �� ������
	[in] in : ��/��ȣȭ �� ������
	[in] inl : ��/��ȣȭ �� ������ ����
	Return Value : 
	Note :
*/
static void S_seed256_ofb_cipher(SCRT_CIPHER_CTX *ctx, unsigned char *out, unsigned char *in, unsigned int inl)
{
	if(ctx->encrypt)
		S_SEED256_OFB_Encrypt(in,out,&(ctx->c.seed256_ks),&(ctx->num),inl,(U8 *)&(ctx->iv));
	else
		S_SEED256_OFB_Decrypt(in,out,&(ctx->c.seed256_ks),&(ctx->num),inl,(U8 *)&(ctx->iv));
}
#endif
