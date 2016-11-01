
/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     digest.c  functions for Hash.

	 Creadted by DEV3

************************************************/ 
#include "../include/scrt.h"
#include <stdio.h>
#include <string.h>

/* �ؽ� ������ �� ���� */
/*	
	Name : SCRT_DigestInit
	Description : �ؽ� ������ �� ����
	Parameters
	[in/out] ctx : �ؽ� �˰��� ������ ���� ����ü
	[in] type : ctx->digest�� ������ ���� ����ü
	Note :
*/
void SCRT_DigestInit(SCRT_MD_CTX *ctx, const SCRT_MD *type)
{
	SRESULT E_SR ;
	E_SR = S_FAILED | SL_HASH | SF_SCRT_DIGEST_INIT ;

	ctx->digest=type;
	type->init(&(ctx->md));
}

/* �ؽ��Լ� ���� */
/*	
	Name : SCRT_DigestUpdate
	Description : �ؽ��Լ� ����
	Parameters
	[in/out] ctx : �ؽ� �˰��� ������ ���� ����ü
	[in] data : �ؽ��ϰ��� �ϴ� ������
	[in] count : �ؽ��ϰ��� �ϴ� ������ ����
	Note :
*/
void SCRT_DigestUpdate(SCRT_MD_CTX *ctx, const void *data, unsigned int count)
{
	SRESULT E_SR ;
	E_SR = S_FAILED | SL_HASH | SF_SCRT_DIGEST_UPDATE ;

	ctx->digest->update(&(ctx->md.base[0]),data,(unsigned long)count);
}

/* �ؽ� �Լ� ���� �� ������ */
/*	
	Name : SCRT_DigestFinal
	Description : �ؽ� �Լ� ���� �� ������
	Parameters
	[in] ctx : �ؽ� �˰��� ������ ���� ����ü
	[in/out] md : �ؽõ� �����
	[in/out] size :  �ؽõ� ����� ����
	Note :
*/
void SCRT_DigestFinal(SCRT_MD_CTX *ctx, unsigned char *md, unsigned int *size)
{
	SRESULT E_SR ;
	E_SR = S_FAILED | SL_HASH | SF_SCRT_DIGEST_FINAL ;

	ctx->digest->final(&(ctx->md.base[0]),md);
	if (size != NULL)
		*size=ctx->digest->md_size;
	memset(&(ctx->md),0,sizeof(ctx->md));
}


/* init, update, final �Ѳ����� ���� */
/*	
	Name : SCRT_Digest
	Description : init, update, final �Ѳ����� ����
	Parameters
	[in] ctx : �ؽ� �˰��� ������ ���� ����ü
	[in] in : �ؽ��ϰ��� �ϴ� ����
	[in] inl : �ؽ��ϰ��� �ϴ� ���� ����
	[in] out : 	�ؽõ� �����			
	[in] outl : �ؽõ� ������� ����
	Note :
*/
SRESULT	SCRT_Digest(SCRT_MD_CTX *ctx, const void *in, unsigned int inl, unsigned char *out, unsigned int *outl)
{
	
	SRESULT E_SR ;
	E_SR = S_FAILED | SL_HASH | SF_SCRT_DIGEST ;

	SCRT_DigestInit(ctx, ctx->digest);
	SCRT_DigestUpdate(ctx, in,inl);
	SCRT_DigestFinal(ctx, out, outl);

	return S_SUCCESS;
}

/* MD_CTX�� copy */
/*	
	Name : SCRT_MD_CTX_copy
	Description : �ؽþ˰��� ������ ���� ����ü MD_CTX�� copy
	Parameters
	[in] out : ��� ����ü
	[in] in : �����ϰ��� �ϴ� �ؽ� �˰��� ������ ���� ����ü
	Note : �����ϸ� 0, �����ϸ� 0x00135035 ����
*/
SRESULT SCRT_MD_CTX_copy(SCRT_MD_CTX *out, SCRT_MD_CTX *in)
{
	SRESULT E_SR;

	E_SR = S_FAILED | SL_HASH | SF_SCRT_MD_CTX_COPY ;

    if ((in == NULL) || (in->digest == NULL))
		return E_SR | SR_DIGEST_MD_CTX_EMPTY_FAILED ;
	
	memcpy((char *)out,(char *)in,in->digest->ctx_size);

	return S_SUCCESS;
}    
