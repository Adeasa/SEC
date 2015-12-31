
/***********************************************
	 Copyright (C) 2015, UNETsystem
 
     scrt_enc.c

	 Creadted by DEV3

************************************************/

#include "../include/scrt.h"
#include <stdio.h>
#include <string.h>

/*	
	Name : SCRT_CIPHER_CTX_init
	Description: RSA ��/��ȣȭ�� ���� Ű ����ü ����
	Parameters
	[out] ctx : Ű ����ü ���� ������ ���ؽ�Ʈ ����ü 	
	Return Value : 
	Note :
*/
void SCRT_CIPHER_CTX_init(SCRT_CIPHER_CTX *ctx)
{
	memset(ctx,0,sizeof(SCRT_CIPHER_CTX));
	ctx->cipher=NULL; 
}

/*	
	Name : SCRT_EncryptInit
	Description: ��ȣȭ�� ���� Ű�� ����ü�� �����Ѵ�.
	Parameters
	[out] ctx : Ű ����ü ���� ������ ���ؽ�Ʈ ����ü
	[in] cipher : ��ĪŰ ��ȣȭ Ű������ ������ ����ü
	[in] key : ��ȣŰ ��
	[in] iv : �ʱ� ���Ͱ�
	Return Value : ���н� �����ڵ� ������ 0
	Note :
*/
/* key schedule, iv setting for encryption */
SRESULT SCRT_EncryptInit(SCRT_CIPHER_CTX *ctx, const SCRT_CIPHER *cipher, unsigned char *key, unsigned char *iv)
{	
	SRESULT E_SR;

	E_SR = S_FAILED | SL_BLOCK | SF_SCRT_ENCRYPT_INIT;
	
	if(ctx == NULL) return E_SR | SR_BLOCK_CIPHER_CTX_EMPTY_FAILED ;

	if (cipher != NULL) 
		ctx->cipher=cipher;
	else
		return E_SR | SR_BLOCK_CIPHER_EMPTY_FAILED ;
			
	ctx->cipher->init(ctx,key,iv,1);
	ctx->encrypt=1;
	ctx->buf_len=0;

	return S_SUCCESS;
}

/*	
	Name : SCRT_DecryptInit
	Description: ��ȣȭ�� ���� Ű�� ����ü�� �����Ѵ�.
	Parameters
	[out] ctx : Ű ����ü ���� ������ ���ؽ�Ʈ ����ü
	[in] cipher : ��ĪŰ ��/��ȣȭ Ű������ ������ ���ؽ�Ʈ ����ü
	[in] key : ��ȣŰ ��
	[in] iv : �ʱ� ���Ͱ�
	Return Value : ���н� �����ڵ� ������ 0
	Note
*/
/* key schedule, iv setting for decryption */
SRESULT SCRT_DecryptInit(SCRT_CIPHER_CTX *ctx, const SCRT_CIPHER *cipher, unsigned char *key, unsigned char *iv)
{
	SRESULT E_SR;

	E_SR = S_FAILED | SL_BLOCK | SF_SCRT_DECRYPT_INIT;

	if(ctx == NULL) return E_SR | SR_BLOCK_CIPHER_CTX_EMPTY_FAILED ;

	if (cipher != NULL)
		ctx->cipher=cipher;
	else
		return E_SR | SR_BLOCK_CIPHER_EMPTY_FAILED ;

	ctx->cipher->init(ctx,key,iv,0);
	ctx->encrypt=0;
	ctx->buf_len=0;

	return S_SUCCESS;

}
/*	
	Name : SCRT_EncryptUpdate
	Description: padding ������ �κ� ��ȣȭ
	Parameters
	[in/out] ctx : ��ȣȭ�� ���� Ű�� ������ ���ؽ�Ʈ ����ü	
	[out] out : ��ȣȭ�� ��
	[out] outl : ��ȣȭ�� ������ ����
	[in] in : ��ȣȭ�� �򹮵�����
	[in] inl : ��ȣȭ�� �򹮵����� ����
	Return Value : 
	Note
*/
/*  padding ������ �κ� ��ȣȭ. */
SRESULT SCRT_EncryptUpdate(SCRT_CIPHER_CTX *ctx, unsigned char *out, int *outl, unsigned char *in, int inl)
{
	SRESULT E_SR;
	int i,j,bl;

	E_SR = S_FAILED | SL_BLOCK | SF_SCRT_ENCRYPT_UPDATE ;
	i=ctx->buf_len;
	bl=ctx->cipher->block_size;
	*outl=0;

	if ((inl == 0) && (i != bl))  return E_SR | SR_BLOCK_PADDING_BUFFER_LENGTH_WRONG;
	if (i != 0)
	{
		if (i+inl < bl) 
		{
			memcpy(&(ctx->buf[i]),in,inl);
			ctx->buf_len+=inl;
			return E_SR | SR_BLOCK_PADDING_BLOCK_SIZE_WRONG;
		}
		else /* ������..decrypt fianl���� ȣ��Ǵ� �κ�. */
		{
			j=bl-i;
			if (j != 0) memcpy(&(ctx->buf[i]),in,j);
			ctx->cipher->do_cipher(ctx,out,ctx->buf,bl);
			inl-=j;
			in+=j;
			out+=bl;
			*outl+=bl;
		}
	}
	i=inl%bl;
	inl-=i;
	if (inl > 0)
	{
		ctx->cipher->do_cipher(ctx,out,in,inl);
		*outl+=inl;
	}

	if (i != 0)
		memcpy(ctx->buf,&(in[inl]),i);
	
	ctx->buf_len=i;
	return S_SUCCESS;
}
/*	
	Name : SCRT_EncryptFinal
	Description: padding ���̰�, padding Block ��ȣȭ.
	Parameters
	[in] ctx : 	��ȣȭ�� ���� Ű�� ������ ���ؽ�Ʈ ����ü
	[out] out : ��ȣȭ�� ��
	[out] outl : ��ȣȭ�� ������ ����
	Return Value : 
	Note
*/
/* padding ���̰�, padding Block ��ȣȭ. */
SRESULT SCRT_EncryptFinal(SCRT_CIPHER_CTX *ctx, unsigned char *out, int *outl)
{
	SRESULT E_SR;
	int i,n,b,bl;

	E_SR = S_FAILED | SL_BLOCK | SF_SCRT_ENCRYPT_FINAL;
	b=ctx->cipher->block_size;

	if (b == 1) /* �е��� ���� ���. cfb, ofb ��尡 ���⿡ �ش� */
	{
		*outl=0;
		return S_SUCCESS;
	}

	bl=ctx->buf_len;

	n=b-bl;
	
	/* �е� ���δ�. */
	for (i=bl; i<b; i++)
		ctx->buf[i]=n;

	ctx->cipher->do_cipher(ctx,out,ctx->buf,b);
	*outl=b;
	return S_SUCCESS;
}

/*	
	Name : SCRT_DecryptUpdate
	Description: padding ������ �κ�  ��ȣȭ.
	Parameters
	[in] ctx : ��ȣȭ Ű ���ؽ�Ʈ ����ü
	[in] out : ��ȣȭ�� ��
	[in] outl : ��ȣȭ�� ������ ����
	[in] in : ��ȣȭ�� ������
	[in] inl : ��ȣȭ�� ������ ����
	Return Value : 
	Note
*/
/* padding ������ �κ�  ��ȣȭ.*/
SRESULT SCRT_DecryptUpdate(SCRT_CIPHER_CTX *ctx, unsigned char *out, int *outl, unsigned char *in, int inl)
{
	SRESULT E_SR=0;
	int b=0,bl=0,n=0;
	int keep_last=0;

	E_SR = S_FAILED | SL_BLOCK | SF_SCRT_DECRYPT_UPDATE;
	*outl=0;

	if (inl == 0) return E_SR | SR_INPUT_LENGTH_INVALIED ;
	
	b=ctx->cipher->block_size;

	if (b > 1)
	{
		bl=ctx->buf_len;
		n=inl+bl;
		if (n%b == 0)
		{
			if (inl < b)
			{
				memcpy(&(ctx->buf[bl]),in,inl);
				ctx->buf_len=b;
				*outl=0;
				return E_SR | SR_BLOCK_DECRYPT_BLOCK_SIZE_WRONG ;
			}
			keep_last=1;
			inl-=b; 
		}
	}

	SCRT_EncryptUpdate(ctx,out,outl,in,inl);

	/* �е� block ó��*/
	if (keep_last)
	{
		memcpy(&(ctx->buf[0]),&(in[inl]),b);
		ctx->buf_len=b;
	}
	return S_SUCCESS;
}

/*	
	Name : SCRT_DecryptFinal
	Description: padding Block ��ȣȭ, padding bits ����.
	Parameters
	[in] ctx : 	��ȣȭŰ ���ؽ�Ʈ ����ü
	[in] out : ��ȣȭ�� ������
	[in] outl :  ��ȣȭ�� ������ ����
	Return Value : 
	Note
*/
/* padding Block ��ȣȭ, padding bits ����.*/
SRESULT SCRT_DecryptFinal(SCRT_CIPHER_CTX *ctx, unsigned char *out, int *outl)
{
	SRESULT E_SR=0;
	int i=0,b=0,n=0;

	E_SR = S_FAILED | SL_BLOCK | SF_SCRT_DECRYPT_FINAL ;

	*outl=0;
	b=ctx->cipher->block_size;
	
	if (b > 1)
	{
		if (ctx->buf_len != b)
			return E_SR | SR_BLOCK_PADDING_BUFFER_LENGTH_WRONG;

		SCRT_EncryptUpdate(ctx,ctx->buf,&n,ctx->buf,0);
	 
		if (n != b)
			return E_SR | SR_BLOCK_PADDING_BLOCK_SIZE_WRONG;
	
		n=ctx->buf[b-1];
		if (n > b)
			return E_SR | SR_BLOCK_PADDING_BIT_NUMBERS_WRONG;
			
		for (i=0; i<n; i++)
		{
			if (ctx->buf[--b] != n)
				return E_SR | SR_BLOCK_PADDING_BIT_VALUE_WRONG;
		}
		n=ctx->cipher->block_size-n;
		for (i=0; i<n; i++)
			out[i]=ctx->buf[i];
		*outl=n;
	}
	else
		*outl=0;

	return S_SUCCESS;
}

/*	
	Name : SCRT_Block_Encrypt
	Description: ��ȣȭ�� ���� Ű�� ������ padding ���� Block ��ȣȭ
	Parameters
	[in] ctx : 	��ȣȭŰ ���ؽ�Ʈ ����ü
	[in] in : ��ȣȭ�� �򹮵�����
	[in] inl : ��ȣȭ�� �򹮵����� ����
	[in] keydata :  Ű ����ü
	[in] iv : �ʱ� ���Ͱ�
	[in] out : ��ȣȭ�� ������ 
	[in] outl : ��ȣȭ�� ������ ����
	Return Value : 
	Note
*/
/* encrypt init, update, final�� �ϳ��� ���� �Լ�.*/
SRESULT SCRT_Block_Encrypt(SCRT_CIPHER_CTX *ctx, U8 *in, int inl,U8 *keydata, U8 *iv, U8 *out, int *outl)
{
	SRESULT E_SR;
	long obytes;
	U8 *out1 = NULL, *out2 = NULL; 
	int outl1, outl2;
	int BLOCKSIZE;
	int status;

	E_SR = S_FAILED | SL_BLOCK | SF_SCRT_BLOCK_ENCRYPT ;

	BLOCKSIZE = ctx->cipher->block_size;	

	if (BLOCKSIZE == 1) obytes = inl;
	else
		obytes = ((inl + BLOCKSIZE) / BLOCKSIZE) * BLOCKSIZE ;

    out1 = (U8 *)malloc(obytes);
	out2 = (U8 *)malloc(BLOCKSIZE);

    if(out1 == NULL) 
    {
    	if (out2 != NULL) free(out2); 
    	return E_SR | SR_MEM_ALLOC_FAIL;
    }
	if(out2 == NULL) 
	{
		if (out1 != NULL) free(out1);
		return E_SR | SR_MEM_ALLOC_FAIL;
	}
	
	memset(out1,0,obytes);
	memset(out2,0, BLOCKSIZE);

	outl1= outl2=0;

	status = SCRT_EncryptInit(ctx, ctx->cipher, (U8 *)keydata, (U8 *)iv);		 
	if(ERROR_FAILED(status))
	{	
		FREE(out1);
		FREE(out2);
		return E_SR | SR_BLOCK_ENCRYPT_INIT_FAILED;
	}

	status = SCRT_EncryptUpdate(ctx, out1, &outl1, in, inl);				 
	if(ERROR_FAILED(status))
	{	
		FREE(out1);
		FREE(out2);
		return status;
	}

	status = SCRT_EncryptFinal(ctx, out2, &outl2);					 
	if(ERROR_FAILED(status))
	{	
		FREE(out1);
		FREE(out2);
		return E_SR | SR_BLOCK_ENCRYPT_FINAL_FAILED;
	}

	memcpy(out1+outl1,out2,outl2);
	outl1 = outl1 + outl2;
	memcpy(out,out1,outl1);
	*outl = outl1;

	FREE(out1);
	FREE(out2);
	
	return S_SUCCESS;
}

/*	
	Name : SCRT_Block_Decrypt
	Description: ��ȣȭ�� ���� Ű�� ������ padding ���� Block ��ȣȭ
	Parameters
	[in] ctx : 	��ȣȭŰ ���ؽ�Ʈ ����ü
	[in] in : ��ȣȭ�� ��ȣȭ ������
	[in] inl : ��ȣȭ�� ��ȣȭ ������ ����
	[in] keydata :  Ű ����ü
	[in] iv : �ʱ� ���Ͱ�
	[in] out : ��ȣȭ�� ������ 
	[in] outl : ��ȣȭ�� ������ ����
	Return Value : 
	Note
*/
/* Decryption init, update, final�� �ϳ��� ���� �Լ� */
SRESULT SCRT_Block_Decrypt(SCRT_CIPHER_CTX *ctx, U8 *in, int inl,  U8 *keydata, U8 *iv,U8 *out, int *outl)
{
	/* not using crypto board */
	SRESULT E_SR;
	long obytes;
	U8 *out1 = NULL, *out2 = NULL; 
	int outl1, outl2;
	int BLOCKSIZE;
	int status;
	
	E_SR = S_FAILED | SL_BLOCK | SF_SCRT_BLOCK_DECRYPT;
	
	BLOCKSIZE = ctx->cipher->block_size;	
	
	if (BLOCKSIZE == 1) obytes = inl;
	else
		obytes = ((inl + BLOCKSIZE) / BLOCKSIZE) * BLOCKSIZE ;

    out1 = (U8 *)malloc(obytes);
	out2 = (U8 *)malloc(BLOCKSIZE);

	if(out1 == NULL) 
    {
    	if (out2 != NULL) free(out2); 
    	return E_SR | SR_MEM_ALLOC_FAIL;
    }
	if(out2 == NULL) 
	{
		if (out1 != NULL) free(out1); 
		return E_SR | SR_MEM_ALLOC_FAIL;
	}
	
    memset(out1,0,obytes);
	memset(out2,0, BLOCKSIZE);

	outl1= outl2=0;

	status = SCRT_DecryptInit(ctx, ctx->cipher, (U8 *)keydata, (U8 *)iv);		
	if(ERROR_FAILED(status)) 
	{	
		FREE(out1);
		FREE(out2);
		return E_SR | SR_BLOCK_DECRYPT_INIT_FAILED;
	}

	status = SCRT_DecryptUpdate(ctx, out1, &outl1, in, inl);			
	if(ERROR_FAILED(status)) 
	{	
		FREE(out1);
		FREE(out2);
		return E_SR | SR_BLOCK_DECRYPT_UPDATE_FAILED;
	}

	status = SCRT_DecryptFinal(ctx, out2, &outl2);
	if(ERROR_FAILED(status)) 
	{	
		FREE(out1);
		FREE(out2);
		return E_SR | SR_BLOCK_DECRYPT_FINAL_FAILED;
	}

	memcpy(out1+outl1,out2,outl2);
	outl1 = outl1 + outl2;
	memcpy(out,out1,outl1);
	*outl = outl1;
	
		
	FREE(out1);
	FREE(out2);
	
	return S_SUCCESS;
}
