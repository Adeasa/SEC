#include "../include/hmac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*	
	Name : S_HMAC_Init
	Description: HMAC ����ü �ʱⰪ ����
	Parameters
	[in/out] ctx : HMAC ����ü
	[in] ctx : HMAC key  
	[in] len : HMAC key ����	
	Return Value : 
	Note :
*/
void S_HMAC_Init(HMAC_CTX *ctx, const void *key, int len,
	       const SCRT_MD *md)
{
	int i,j,reset=0;
	U8 pad[HMAC_MAX_MD_CBLOCK];//unsigned char pad[HMAC_MAX_MD_CBLOCK]; dgshin-1204

	if (md != NULL)
		{
		reset=1;
		ctx->md=md;
		}
	else
		md=ctx->md;

	if (key != NULL)
		{
		reset=1;
		j=SCRT_MD_block_size(md);
		if (j < len)
			{
			SCRT_DigestInit(&ctx->md_ctx,md);
			SCRT_DigestUpdate(&ctx->md_ctx,key,len);
			SCRT_DigestFinal(&(ctx->md_ctx),ctx->key,
				&ctx->key_length);
			}
		else
			{
			memcpy(ctx->key,key,len);
			ctx->key_length=len;
			}
		if(ctx->key_length != HMAC_MAX_MD_CBLOCK)
			memset(&ctx->key[ctx->key_length], 0,
				HMAC_MAX_MD_CBLOCK - ctx->key_length);
		}

	if (reset)	
		{
		for (i=0; i<HMAC_MAX_MD_CBLOCK; i++)
			pad[i]=0x36^ctx->key[i];
		SCRT_DigestInit(&ctx->i_ctx,md);
		SCRT_DigestUpdate(&ctx->i_ctx,pad,SCRT_MD_block_size(md));

		for (i=0; i<HMAC_MAX_MD_CBLOCK; i++)
			pad[i]=0x5c^ctx->key[i];
		SCRT_DigestInit(&ctx->o_ctx,md);
		SCRT_DigestUpdate(&ctx->o_ctx,pad,SCRT_MD_block_size(md));
		}

	memcpy(&ctx->md_ctx,&ctx->i_ctx,sizeof(ctx->i_ctx));
}



/*	
	Name : S_HMAC_Update
	Description: �޽��� ��� �� �����͸� HMAC �˰����� �̿��� �޽��� ��� �Ѵ�
	Parameters
	[in/out] ctx : HMAC ����ü
	[in] data : �޽��� ��� �� ������
	[in] len : �޽��� ��� �� ������ ����
	Return Value : 
	Note :
*/
void S_HMAC_Update(HMAC_CTX *ctx, const unsigned char *data, int len)
{
	SCRT_DigestUpdate(&(ctx->md_ctx),data,len);
}


/*	
	Name : S_HMAC_Final
	Description: �޽��� ��� �� �����͸� HMAC �˰����� �̿��� �޽��� ��ҵ� �����͸� ����Ѵ�
	Parameters
	[in] ctx : HMAC �ؽ� ����ü
	[out] md : �޽��� ��� �� ������
	[out] len : �޽��� ��� �� ������ ����
	Return Value : 
	Note :
*/
void S_HMAC_Final(HMAC_CTX *ctx, unsigned char *md, unsigned int *len)
{
	int j;
	unsigned int i;
	U8 buf[SCRT_MAX_MD_SIZE];//unsigned char buf[SCRT_MAX_MD_SIZE]; dgshin-1204

	j=SCRT_MD_block_size(ctx->md);

	SCRT_DigestFinal(&(ctx->md_ctx),buf,&i);
	memcpy(&(ctx->md_ctx),&(ctx->o_ctx),sizeof(ctx->o_ctx));
	SCRT_DigestUpdate(&(ctx->md_ctx),buf,i);
	SCRT_DigestFinal(&(ctx->md_ctx),md,len);
}

/*	
	Name : S_HMAC_cleanup
	Description: HMAC_CTX ����ü�� �޸𸮸� ��� �ʱ�ȭ ��(0x00���� �ʱ�ȭ)
	Parameters
	[in] ctx : HMAC �ؽ� ����ü
	Return Value : 
	Note :
*/
void S_HMAC_cleanup(HMAC_CTX *ctx)
{
	memset(ctx,0,sizeof(HMAC_CTX));
}

