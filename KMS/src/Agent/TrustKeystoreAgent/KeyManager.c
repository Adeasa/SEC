#include "KeyManager.h"


int	g_nAgentErrorCode = 0;
char	g_szAgentErrorMsg[255 + 1] = {0};

//////////////////////////////////////////////////////////////////////////
// int KM_GetKey(Agent_Ctx *pContext, char* szKeyID, Key *pKey)
// ���� : memory, DBFile, KMS���� Ű�� ������ ��ȣȭ �ؼ� ����.
// Input : Agent Context, KeyID
// Output : KeyStruct
// Return : ���� 0, ���� errorcode
//////////////////////////////////////////////////////////////////////////
int KM_GetKey(Agent_Ctx *pContext, char* szKeyID, Key *pKey)
{
	int nRet = 0;
	Key storeKey;

	memset(&storeKey, 0x00, sizeof(Key));

	PRINT_DEBUG("KM_GetKey Start.");

	// Memory üũ
	if((nRet = KM_GetKeyFromMemory(pContext, szKeyID, pKey)) != TK_SUCCESS)
	{
		// KeyDB File�� �����ϴ� ��� Key DB���� Key�� ������
		if((nRet = KM_CheckKeyDBFile(pContext)) == TK_SUCCESS)
		{
			if((nRet = KM_GetKeyFromKeyDBFile(pContext, szKeyID, pKey)) != TK_SUCCESS)
				goto error;		
		}
		else
		{
			// KMS üũ
			if((nRet = KM_GetKeyFromKMS(pContext, szKeyID, pKey)) != TK_SUCCESS)
				goto error;
		}
		// Key HMAC üũ
		if((nRet = KM_VerifyKeyHMAC(pContext, pKey)) != TK_SUCCESS)
			goto error;
		// DB �Ǵ� KMS���� Ű�� ���� �� ���, Ű���� ��ȣȭ �Ͽ� �޸𸮿� �����Ѵ�.
		memcpy(&storeKey, pKey, sizeof(Key));		
		if((nRet = KM_EncryptKey(pContext, &storeKey)) != TK_SUCCESS)
			goto error;
		if((nRet = KM_SetKeyIntoMemory(pContext, &storeKey)) != TK_SUCCESS)
			goto error;
	}

	PRINT_DEBUG("KM_GetKey Success.");

error:
	PRINT_DEBUG("KM_GetKey End.\n");
	return nRet;
}
//////////////////////////////////////////////////////////////////////////
// int KM_CheckKeyDBFile(Agent_Ctx *pContext)
// ���� : KeyDB ������ �����ϴ��� ���� üũ
// Input : Agent Context
// Output :
// Return : 0(���� ����), -1(���� ����)
//////////////////////////////////////////////////////////////////////////
int KM_CheckKeyDBFile(Agent_Ctx *pContext)
{
	int nRet = 0;
	char szKeyDBPATH[MAX_PATH] = "";
	FILE *pfile = NULL;

#ifndef NO_KEY_DB
	sprintf(szKeyDBPATH, "%s/%s", pContext->pConfig->agentHomeDir, KEY_DB_FILE_NAME);

	// file check
	if((pfile = fopen(szKeyDBPATH, "r")) == NULL)
	{
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_CHECKKEYDBFILE_FILE_NOT_FOUND;
		goto error;
	}

	PRINT_DEBUG("KM_CheckKeyDBFile Success.");

error:
	
	if(pfile)
		fclose(pfile);
#else
	// Key DB ������ ������� �ʴ� ���.
	nRet = -1;
#endif
	return nRet;
}
//////////////////////////////////////////////////////////////////////////
// int KM_GetKeyFromKeyDBFile(Agent_Ctx *pContext, char* szKeyID, Key *pKey)
// ���� : KeyDB ���Ͽ��� �Է¹��� Ű ���̵� �ش��ϴ� Ű ����ü�� ������
// ��ȣȭ �� Ű ���� ���̸� ����.
// Input : Agent Context, KeyID
// Output : KeyStruct
// Return : ���� 0, ���� errorcode
//////////////////////////////////////////////////////////////////////////
int KM_GetKeyFromKeyDBFile(Agent_Ctx *pContext, char* szKeyID, Key *pKey)
{
	int nRet = 0;
	char szKeyDBFile[MAX_PATH] = "";
	UString hmacKey;
	UString kek;
	UString decodedKeyValue;
	UString decKeyValue;

	memset(&hmacKey, 0x00, sizeof(UString));
	memset(&kek, 0x00, sizeof(UString));
	memset(&decodedKeyValue, 0x00, sizeof(UString));
	memset(&decKeyValue, 0x00, sizeof(UString));

	PRINT_DEBUG("KM_GetKeyFromKeyDBFile Start.");

	sprintf(szKeyDBFile, "%s/%s", pContext->pConfig->agentHomeDir, KEY_DB_FILE_NAME);

	if((nRet = TK_GetKeyFromDB(szKeyDBFile, szKeyID, pKey)) != TK_SUCCESS)
		goto error;
	
	// HMAC Key ����. HMAC Key = ,sha256(IP):4,1,3,2
	{
		UString hmacKeyMaterial;

		hmacKeyMaterial.value = pContext->pConfig->agentIP;
		hmacKeyMaterial.length = strlen(pContext->pConfig->agentIP);

		if((nRet = KM_Make_KeyFromMaterial(&hmacKeyMaterial, &hmacKey)) != TK_SUCCESS)
			goto error;
	}

	// KEK ���� kek = HMAC(kek material). kek material = ip+�е�.
	{
		UString kekMaterial;
		char szKEKMaterial[KEK_MATERIAL_LEN] = "";

		memcpy(szKEKMaterial, IP_PADDING, strlen(IP_PADDING));
		memcpy(szKEKMaterial, pContext->pConfig->agentIP, strlen(pContext->pConfig->agentIP));

		kekMaterial.value = szKEKMaterial;
		kekMaterial.length = sizeof(szKEKMaterial);

		if((nRet = TK_Make_HMAC(SHA256, &hmacKey, &kekMaterial, &kek)) != TK_SUCCESS)
			goto error;
	}

	// Ű�� b64decode
	if((nRet = TK_Base64_Decode(pKey->enc_key_value, &decodedKeyValue)) != TK_SUCCESS)
		goto error;
	// Ű�� ��ȣȭ	
	if((nRet = TK_Aes128_Decrypt(&decKeyValue, &decodedKeyValue, &kek)) != TK_SUCCESS)
		goto error;
	// Ű ����ü�� ��ȣȭ Ű���� ���� ����
	memset(pKey->enc_key_value, 0x00, sizeof(pKey->enc_key_value));
	memcpy(pKey->enc_key_value, decKeyValue.value, decKeyValue.length);
	pKey->key_size = decKeyValue.length;

	PRINT_DEBUG("KM_GetKeyFromKeyDBFile Success.");

error:

	TK_Free_UString(&hmacKey);
	TK_Free_UString(&kek);
	TK_Free_UString(&decodedKeyValue);
	TK_Free_UString(&decKeyValue);

	PRINT_DEBUG("KM_GetKeyFromKeyDBFile End.");

	return nRet;
}
//////////////////////////////////////////////////////////////////////////
// int KM_GetKeyFromMemory(Agent_Ctx *pContext, char* szKeyID, Key *pKey = NULL)
// ���� : �Է¹��� Ű ���̵� �ش��ϴ� Ű ����ü�� �޸𸮿��� ����
// ��ȣȭ �� Ű ���� ���̸� ����.
// pKey == NULL�̸� �޸𸮿� ID�� �ִ��� ���θ� üũ
// Input : Agent Context, KeyID
// Output : KeyStruct
// Return : ���� 0, ���� errorcode
//////////////////////////////////////////////////////////////////////////
int KM_GetKeyFromMemory(Agent_Ctx *pContext, char* szKeyID, Key *pKey)
{
	int nRet = 0;
	int i = 0;

	PRINT_DEBUG("KM_GetKeyFromMemory Start.");

	if(szKeyID == NULL || strlen(szKeyID) == 0)
	{
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_GETKEYFROMMEMORY_ARG_ERROR;
		goto error;
	}

	if(pContext->pKeyList->nKeyCount == 0)
	{
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_GETKEYFROMMEMORY_NO_KEY;
		goto error;
	}

	// �޸𸮿��� �����ϴ� Ű�� ������ ���� ���� ������ ����Ǿ� ����Ž������ Ű�� �˻�.
	for(i = 0; i < pContext->pKeyList->nKeyCount; i++)
	{
		size_t nCurKeyIDLen = strlen(pContext->pKeyList->pKey[i].key_id);
		int nCompareKeyIDLen = nCurKeyIDLen > strlen(szKeyID) ? nCurKeyIDLen : strlen(szKeyID);

		if(strncmp(pContext->pKeyList->pKey[i].key_id, szKeyID, nCompareKeyIDLen) == 0)
		{
			// pKey�� NULL�� ��� Ű���� ��ȯ ������ �ʰ�, return 0�� Ű�� ������ �˷��ֱ⸸ �Ѵ�.
			if(pKey != NULL)
			{
				memcpy(pKey, &(pContext->pKeyList->pKey[i]), sizeof(Key));
				// Ű�� ��ȣȭ
				if((nRet = KM_DecryptKey(pContext, pKey)) != TK_SUCCESS)
					goto error;
			}			
			break;
		}
		else
		{
			// key id�� ã�� ����
			if(i == pContext->pKeyList->nKeyCount -1 )
			{				
				nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_GETKEYFROMMEMORY_NOT_FOUND_KEYID;
				goto error;
			}
		}
	}

	PRINT_DEBUG("KM_GetKeyFromMemory Success.");

error:

	PRINT_DEBUG("KM_GetKeyFromMemory End.\n");
	return nRet;
}
//////////////////////////////////////////////////////////////////////////
// int KM_SetKeyIntoMemory(Agent_Ctx *pContext, Key *pKey)
// ���� : �޸𸮿� Ű ����ü ����
// Input : Agent Context, KeyStruct
// Output :
// Return : ���� 0, ���� errorcode
//////////////////////////////////////////////////////////////////////////
int KM_SetKeyIntoMemory(Agent_Ctx *pContext, Key *pKey)
{
	int nRet = 0;
	int i = 0;

	PRINT_DEBUG("KM_SetKeyIntoMemory Start.");

	// Dynamic Ű�� ��� �޸𸮿� �������� �ʴ´�.
	if(strncmp(pKey->key_Type, "D", sizeof(pKey->key_Type)) == 0)
		goto error;

	if(pContext->pKeyList->pKey == NULL && pContext->pKeyList->nKeyCount == 0)
	{
		if((pContext->pKeyList->pKey = (Key*)TK_MemAlloc(sizeof(Key))) == NULL)		
		{
			nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_MEMALLOC;
			goto error;
		}
		pContext->pKeyList->nKeyCount = 1;
		memcpy(pContext->pKeyList->pKey, pKey, sizeof(Key));
	}
	else
	{
		int nFindKey = 0;

		for(i = 0; i < pContext->pKeyList->nKeyCount; i++)
		{
			// ���� �޸𸮿� �ִ� Ű �� ���� ID�� ���� Ű�� �ִ��� Ȯ��
			if(strncmp(pContext->pKeyList->pKey[i].key_id, pKey->key_id, sizeof(pContext->pKeyList->pKey[i].key_id)) == 0)
			{
				// ID�� ������ �����.
				memcpy(&pContext->pKeyList->pKey[i], pKey, sizeof(Key));
				nFindKey = 1;
				break;
			}
		}
		if(nFindKey == 0)
		{
			// ���� ID�� ������ �߰��Ѵ�.
			if((pContext->pKeyList->pKey = (Key*)TK_ReAlloc(pContext->pKeyList->pKey, sizeof(Key) * (pContext->pKeyList->nKeyCount + 1))) == NULL)				
			{
				nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_REALLOC;
				goto error;
			}
			memcpy(&pContext->pKeyList->pKey[pContext->pKeyList->nKeyCount++], pKey, sizeof(Key));
		}

	}
	PRINT_DEBUG("KM_SetKeyIntoMemory Success.");

error:

	PRINT_DEBUG("KM_SetKeyIntoMemory End.\n");

	return nRet;
}
//////////////////////////////////////////////////////////////////////////
// int KM_SetKeyListIntoMemory(Agent_Ctx *pContext, KeyList *pKeyList)
// ���� : �޸𸮿� Ű ����Ʈ ����
// Input : Agent Context, KeyList
// Output :
// Return : ���� 0, ���� errorcode
//////////////////////////////////////////////////////////////////////////
int KM_SetKeyListIntoMemory(Agent_Ctx *pContext, KeyList *pKeyList)
{
	int nRet = 0;
	int i = 0;

	if(pKeyList == NULL || pKeyList->pKey == NULL || pKeyList->nKeyCount == 0)
	{
		nRet = -1;
		goto error;
	}

	for(i = 0; i < pKeyList->nKeyCount; i++)
	{
		// �� Ű�� ��ȣȭ �Ͽ� ����.
		if((nRet = KM_EncryptKey(pContext, &pKeyList->pKey[i])) != TK_SUCCESS)
			goto error;
		if((nRet = KM_SetKeyIntoMemory(pContext, &pKeyList->pKey[i])) != TK_SUCCESS)
			goto error;
	}
error:
	return nRet;
}
//////////////////////////////////////////////////////////////////////////
// int KM_GetKeyFromKMS(Agent_Ctx *pContext, char* szKeyID, Key *pKey)
// ���� : Kms�� Ű ���̵� ��û�ϰ� Ű�� �������
// ��ȣȭ �� Ű ���� ���̸� ����.
// KeyID�� null�� ��� Agent�� ��� ������ ��� Ű ����
// Input : Agent Context, Key
// Output : Key
// Return : ���� 0, ���� errorcode
//////////////////////////////////////////////////////////////////////////
int KM_GetKeyFromKMS(Agent_Ctx *pContext, char* szKeyID, Key *pKey)
{
	int nRet = 0;	
	char *pReqMsg = NULL;
	char *pResMsg = NULL;
	OP_RESPONSE response;
	UString kek;
	UString decodedKeyValue;
	UString decKeyValue;

	memset(&response, 0x00, sizeof(OP_RESPONSE));
	memset(&kek, 0x00, sizeof(UString));
	memset(&decodedKeyValue, 0x00, sizeof(UString));
	memset(&decKeyValue, 0x00, sizeof(UString));
	
	PRINT_DEBUG("KM_GetKeyFromKMS Start.");

	if((nRet = KM_MakeRequest(pContext, OP_REQUEST_KEY, szKeyID, &pReqMsg)) != TK_SUCCESS)
		goto error;
	if((nRet = KM_RequestToKMS(pContext, pReqMsg, &pResMsg)) != TK_SUCCESS)
		goto error;
	if((nRet = TK_ParseResponse(pResMsg, &response)) != TK_SUCCESS)
		goto error;
	memcpy(pKey, &response.keyList.pKey[0], sizeof(Key));
	// KEK ����
	{
		UString kekMaterial;
		kekMaterial.value = pKey->key_id;
		kekMaterial.length = strlen(pKey->key_id);

		if((nRet = KM_Make_KeyFromMaterial(&kekMaterial, &kek)) != TK_SUCCESS)
			goto error;
	}
	// Ű�� b64decode
	if((nRet = TK_Base64_Decode(pKey->enc_key_value, &decodedKeyValue)) != TK_SUCCESS)
		goto error;
	// Ű�� ��ȣȭ	
	if((nRet = TK_Aes128_Decrypt(&decKeyValue, &decodedKeyValue, &kek)) != TK_SUCCESS)
		goto error;
	// Ű ����ü�� ��ȣȭ Ű���� ���� ����
 	memset(pKey->enc_key_value, 0x00, sizeof(pKey->enc_key_value));
 	memcpy(pKey->enc_key_value, decKeyValue.value, decKeyValue.length);
	pKey->key_size = decKeyValue.length;

	PRINT_DEBUG("KM_GetKeyFromKMS Success");
error:

	TK_MemFree((void**)&pReqMsg);
	TK_MemFree((void**)&pResMsg);
	TK_MemFree((void**)&response.keyList.pKey);
	TK_Free_UString(&kek);
	TK_Free_UString(&decodedKeyValue);
	TK_Free_UString(&decKeyValue);

	PRINT_DEBUG("KM_GetKeyFromKMS End.\n");

	return nRet;
}
//////////////////////////////////////////////////////////////////////////
// int KM_GetKeyListFromKMS(Agent_Ctx *pContext, char* szKeyIDList, KeyList *pKeyList)
// ���� : Kms�� Ű ���̵� ����Ʈ�� ��û�ϰ� Ű ����Ʈ�� �������
// KeyIDList�� null�� ��� Agent�� ��� ������ ��� Ű ����
// ��� �� pKeyList->pKey �޸� ���� �ؾ� ��.
// Input : Agent Context, KeyIDList
// Output : KeyList
// Return : ���� 0, ���� errorcode
//////////////////////////////////////////////////////////////////////////
int KM_GetKeyListFromKMS(Agent_Ctx *pContext, char* szKeyIDList, KeyList *pKeyList)
{
	int nRet = 0;
	int i = 0;
	char *pReqMsg = NULL;
	char *pResMsg = NULL;
	OP_RESPONSE response;
	UString kek;
	UString decodedKeyValue;
	UString decKeyValue;
	
	memset(&response, 0x00, sizeof(OP_RESPONSE));
	memset(&kek, 0x00, sizeof(UString));
	memset(&decodedKeyValue, 0x00, sizeof(UString));
	memset(&decKeyValue, 0x00, sizeof(UString));

	if((nRet = KM_MakeRequest(pContext, OP_REQUEST_KEY, szKeyIDList, &pReqMsg)) != TK_SUCCESS)
		goto error;	
	if((nRet = KM_RequestToKMS(pContext, pReqMsg, &pResMsg)) != TK_SUCCESS)
		goto error;
	if((nRet = TK_ParseResponse(pResMsg, &response)) != TK_SUCCESS)
		goto error;
	memcpy(pKeyList, &response.keyList, sizeof(KeyList));
	
	for(i = 0; i < pKeyList->nKeyCount; i++)
	{
		// KEK ����
		{
			UString kekMaterial;
			kekMaterial.value = pKeyList->pKey[i].key_id;
			kekMaterial.length = strlen(pKeyList->pKey[i].key_id);

			if((nRet = KM_Make_KeyFromMaterial(&kekMaterial, &kek)) != TK_SUCCESS)
				goto error;
		}

		// Ű�� b64decode
		if((nRet = TK_Base64_Decode(pKeyList->pKey[i].enc_key_value, &decodedKeyValue)) != TK_SUCCESS)
			goto error;
		// Ű�� ��ȣȭ	
		if((nRet = TK_Aes128_Decrypt(&decKeyValue, &decodedKeyValue, &kek)) != TK_SUCCESS)
			goto error;
		// Ű ����ü�� ��ȣȭ Ű���� ���� ����
		memset(pKeyList->pKey[i].enc_key_value, 0x00, sizeof(pKeyList->pKey[i].enc_key_value));
		memcpy(pKeyList->pKey[i].enc_key_value, decKeyValue.value, decKeyValue.length);
		pKeyList->pKey[i].key_size = decKeyValue.length;

		TK_Free_UString(&kek);
		TK_Free_UString(&decodedKeyValue);
		TK_Free_UString(&decKeyValue);
	}

error:
	TK_MemFree((void**)&pReqMsg);
	TK_MemFree((void**)&pResMsg);

	TK_Free_UString(&kek);
	TK_Free_UString(&decodedKeyValue);
	TK_Free_UString(&decKeyValue);

	return nRet;
}
//////////////////////////////////////////////////////////////////////////
// int KM_GetUpdateAvailableKeyIDList(Agent_Ctx *pContext, char* szKeyIDListIn, char* szKeyIDListOut)
// ���� : KeyID����Ʈ�� �Է¹޾� �� �� ������Ʈ�� ������ KeyID����Ʈ�� ��ȯ.
// szKeyIDListIn == null�� ��� �޸��� ��� KeyID ����Ʈ ��ȯ
// update ������ Ű�� ���� ��� szKeyIDListOut = NULL ��ȯ
// Input : Agent Context, szKeyIDListIn(key_id+'|'+key_id+...)
// Output : szKeyIDListOut(key_id+'|'+key_id+...)
// Return : ���� 0, ���� errorcode
//////////////////////////////////////////////////////////////////////////
int KM_GetUpdateAvailableKeyIDList(Agent_Ctx *pContext, char* szKeyIDListIn, char** szKeyIDListOut)
{
	int nRet = 0;
	int i = 0;
	char *pToken = NULL;
	char *szKeyIDList = NULL;
	char seperate[] = "|";

	if(szKeyIDListIn == NULL || strlen(szKeyIDListIn) == 0)	// �޸� ���� ��ü Ű ����Ʈ ����.
	{
		for(i = 0; i < pContext->pKeyList->nKeyCount; i++)
		{
			char *pKey_id = pContext->pKeyList->pKey[i].key_id;

			if(*szKeyIDListOut == NULL)
			{
				if((*szKeyIDListOut = (char*)TK_MemAlloc(strlen(pKey_id) +1)) == NULL)				
				{
					nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_MEMALLOC;
					goto error;
				}
				strncpy(*szKeyIDListOut, pKey_id, strlen(pKey_id));
			}
			else
			{
				if((*szKeyIDListOut = (char*)TK_ReAlloc(*szKeyIDListOut, TK_MemSize(*szKeyIDListOut) + strlen(pKey_id) +2)) == NULL) // +2 for '|' and \0
				{
					nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_REALLOC;
					goto error;
				}

				strcat(*szKeyIDListOut, "|");
				strcat(*szKeyIDListOut, pKey_id);				
			}
		}

	}
	else // ���޹��� Ű �߿� �޸𸮻� �ִ� Ű ����.
	{
		if((szKeyIDList = (char*) TK_MemAlloc(strlen(szKeyIDListIn)+1)) == NULL)
		{
			nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_MEMALLOC;
			goto error;
		}
		strncpy(szKeyIDList, szKeyIDListIn, strlen(szKeyIDListIn));

		pToken = strtok(szKeyIDList, seperate);

		while(pToken != NULL)
		{
			if(KM_GetKeyFromMemory(pContext, pToken, NULL) == TK_SUCCESS)
			{
				if(*szKeyIDListOut == NULL)
				{
					if((*szKeyIDListOut = (char*)TK_MemAlloc(strlen(pToken) +1)) == NULL)
					{
						nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_MEMALLOC;
						goto error;
					}
					strncpy(*szKeyIDListOut, pToken, strlen(pToken));
				}
				else
				{
					if((*szKeyIDListOut = (char*)TK_ReAlloc(*szKeyIDListOut, TK_MemSize(*szKeyIDListOut) + strlen(pToken) +2)) == NULL) // +2 for '|' and \0
					{
						nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_REALLOC;
						goto error;
					}
					sprintf(*szKeyIDListOut, "|%s", pToken);
				}
			}
			pToken = strtok(NULL, seperate);
		}
	}

	if(*szKeyIDListOut == NULL)
	{
		// ������Ʈ ������ Ű�� ����.
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_GET_UPDATEAVAILABLEKEYIDLIST_NOKEY;
	}
error:
	TK_MemFree((void**)&szKeyIDList);

	return nRet;
}

//////////////////////////////////////////////////////////////////////////
// int KeyStructToKeyInfo(KeyStruct *pKey, char* szKeyInfo)
// ���� : Ű ����ü�� �Է¹޾� ���ڿ� ����
// Input : Key ����ü
// Output : Key Info (KeyID+'|'+b64E(KeyValue)+'|'+KeyAlgo+'|'+OPMode)
// Return : 
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// int KM_MakeRequest(Agent_Ctx *pContext, int nOPCode, char* szArg, char*szRequest)
// ���� : ������ ��û�ϴ� ��û �޽���(http)�� ����� �Լ�
// Input : Agent Context, OPCode, Arguments
// Output : Request
// Return : ���� 0, ���� errorcode
//////////////////////////////////////////////////////////////////////////
int KM_MakeRequest(Agent_Ctx *pContext, int nOPCode, char* szArg, char** szRequest)
{
	int nRet = 0;
	char http_header[256] = "";
	char http_msg[256] = "";
	char method[] = "POST";
	int nContentLength = 0;
		
	// common request header		
	sprintf(http_msg, "ProtocolVersion=%d&AgentID=%s&AgentCheckValue=%s&", TK_PROTOCOL_VERSION, pContext->pConfig->agentID, pContext->pConfig->agentHMAC);

	// request
	switch(nOPCode)
	{
	case OP_REQUEST_KEY:
	case OP_REQUEST_ENCRYPT:
	case OP_REQUEST_DECRYPT:		
		
		sprintf(http_msg + strlen(http_msg), "OPCode=%d&OPArg=", nOPCode);
		nContentLength = strlen(http_msg) + strlen(szArg) + strlen("\r\n");
		// make http header
		sprintf(http_header, "%s %s HTTP/1.1\r\nHost: %s:%s\r\nContent-Length: %d\r\n\r\n", method, KMS_REQPAGE, pContext->pConfig->kmsIP, pContext->pConfig->kmsPort, nContentLength);

		if((*szRequest = (char*) TK_MemAlloc(strlen(http_header) + nContentLength + 1)) == NULL)		
		{
			nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_MEMALLOC;
			goto error;
		}	

		sprintf(*szRequest, "%s%s%s\r\n", http_header, http_msg, szArg);		

		break;
	default:
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_MAKEREQUEST_INVALID_OPCODE;
		goto error;

		break;
	}
 	PRINT_DEBUG(*szRequest);

error:
	return nRet;
}

//////////////////////////////////////////////////////////////////////////
// int KM_RequestToKMS(Agent_Ctx *pContext, char* szSendMsg, char* szRecvMsg)
// ���� : KMS ������ ��û �޽���(SSL)�� ������ ���� �޽����� �޴� �Լ�
// Input : Agent Context, SendMsg
// Output : RecvMsg
// Return : ���� 0, ���� errorcode
//////////////////////////////////////////////////////////////////////////
int KM_RequestToKMS(Agent_Ctx *pContext, char* szSendMsg, char** szRecvMsg)
{
	int nRet = 0;
	int got = 0;
	char reply[1024+1] = {0};
	char *pRecv = NULL;
	int nRecvLen = 0;

	if((nRet = TK_Connect(pContext->sslCtx, pContext->pConfig->kmsIP, strtol(pContext->pConfig->kmsPort, NULL, 10), 0)) != TK_SUCCESS)
		goto error;

	if((nRet = TK_SEND(szSendMsg, strlen(szSendMsg))) != TK_SUCCESS)
		goto error;

	if((nRet = TK_HTTP_RECV(szRecvMsg, &nRecvLen)) != TK_SUCCESS)
		goto error;
	PRINT_DEBUG("Receive Message : ");
	PRINT_DEBUG(*szRecvMsg);

error:

	TK_Disconnect();
	return nRet;
}

int KM_VerifyKeyHMAC(Agent_Ctx *pContext, Key *pKey)
{
	int nRet = 0;
	char HMACInput[256] = {0};
	int nHMACInputLen = 0;
	UString usHMACKey;
	UString usHMACResult;
	char * pszHMACResultB64Enc = NULL;

	memset(&usHMACKey, 0x00, sizeof(UString));
	memset(&usHMACResult, 0x00, sizeof(UString));

	PRINT_DEBUG("KM_VerifyKeyHMAC Start.");

	if(pKey == NULL)
	{
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VERIFYKEYHMAC_ARG_ERROR;
		goto error;
	}

	// make plain text	
	memcpy(HMACInput, pKey->key_id, strlen(pKey->key_id));
	nHMACInputLen = strlen(pKey->key_id);

	memcpy(HMACInput + nHMACInputLen, pKey->enc_key_value, pKey->key_size);	// enc_key_value = ��ȣȭ�� Ű ��
	nHMACInputLen += pKey->key_size;

	memcpy(HMACInput + nHMACInputLen, pKey->key_algo, strlen(pKey->key_algo));
	nHMACInputLen += strlen(pKey->key_algo);

	memcpy(HMACInput + nHMACInputLen, pKey->op_mode, strlen(pKey->op_mode));
	nHMACInputLen += strlen(pKey->op_mode);

	memcpy(HMACInput + nHMACInputLen, pKey->expose_level, strlen(pKey->expose_level));
	nHMACInputLen += strlen(pKey->expose_level);

	memcpy(HMACInput + nHMACInputLen, pKey->valid_start, strlen(pKey->valid_start));
	nHMACInputLen += strlen(pKey->valid_start);

	memcpy(HMACInput + nHMACInputLen, pKey->valid_end, strlen(pKey->valid_end));
	nHMACInputLen += strlen(pKey->valid_end);

	// make HMAC Key
	{
		UString usHMACKeyMaterial;
		usHMACKeyMaterial.value = pKey->key_id;
		usHMACKeyMaterial.length = strlen(pKey->key_id);

		if((nRet = KM_Make_KeyFromMaterial(&usHMACKeyMaterial, &usHMACKey)) != TK_SUCCESS)
			goto error;
	}
	// make HMAC
	{
		UString usHMACText;
		usHMACText.value = HMACInput;
		usHMACText.length = nHMACInputLen;
		
		if((nRet = TK_Make_HMAC(SHA256, &usHMACKey, &usHMACText, &usHMACResult)) != TK_SUCCESS)
			goto error;
	}
	// b64Encode HMAC
	if((nRet = TK_Base64_Encode(&usHMACResult, &pszHMACResultB64Enc)) != TK_SUCCESS)
		goto error;
	// compare HMAC
	if(strncmp(pszHMACResultB64Enc, pKey->key_hmac, strlen(pKey->key_hmac)) != 0)
	{
		// ���� ����!
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VERIFYKEYHMAC_VERIFY_ERROR;
		goto error;
	}

	PRINT_DEBUG("KM_VerifyKeyHMAC Success");

error:
	
	TK_Free_UString(&usHMACKey);
	TK_Free_UString(&usHMACResult);
	TK_MemFree((void**)&pszHMACResultB64Enc);

	PRINT_DEBUG("KM_VerifyKeyHMAC End.\n");

	return nRet;
}

int KM_VerifyKeyValidDate(Agent_Ctx *pContext, Key *pKey)
{
	int nRet = 0;

	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	int nCurYear = 0, nCurMonth = 0, nCurDay = 0;
	int nDiffYear = 0, nDiffMonth = 0, nDiffDay = 0;	

	PRINT_DEBUG("KM_VerifyKeyValidDate Start.");

	if(strlen(pKey->valid_start) == 0 && strlen(pKey->valid_end))
		goto error;

	// ���� ���� �ð� ���ϱ�.
	nCurYear = now->tm_year + 1900;
	nCurMonth = now->tm_mon + 1;
	nCurDay = now->tm_mday;

	// valid_start�� ��.
	if(strlen(pKey->valid_start) > 0)
	{
		nDiffYear = strtol(pKey->valid_start, NULL, 10);
		nDiffMonth = strtol(&pKey->valid_start[5], NULL, 10);
		nDiffDay = strtol(&pKey->valid_start[8], NULL, 10);

		if(nCurYear < nDiffYear)
		{
			nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VALIDDATE_BEFORE;
			goto error;
		}
		else if(nCurYear == nDiffYear)
		{
			if(nCurMonth < nDiffMonth)
			{
				nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VALIDDATE_BEFORE;
				goto error;
			}
			else if(nCurMonth == nDiffMonth)
			{
				if(nCurDay < nDiffDay)
				{
					nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VALIDDATE_BEFORE;
					goto error;
				}
			}
		}
	}
	// valid_end�� ��.
	if(strlen(pKey->valid_end) > 0)
	{
		nDiffYear = strtol(pKey->valid_end, NULL, 10);
		nDiffMonth = strtol(&pKey->valid_end[5], NULL, 10);
		nDiffDay = strtol(&pKey->valid_end[8], NULL, 10);

		if(nCurYear > nDiffYear)
		{
			nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VALIDDATE_EXPIRED;
			goto error;
		}
		else if(nCurYear == nDiffYear)
		{
			if(nCurMonth > nDiffMonth)
			{
				nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VALIDDATE_EXPIRED;
				goto error;
			}
			else if(nCurMonth == nDiffMonth)
			{
				if(nCurDay > nDiffDay)
				{
					nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VALIDDATE_EXPIRED;
					goto error;
				}
			}
		}
	}

error:
	PRINT_DEBUG("KM_VerifyKeyValidDate End.");
	return nRet;

}

int KM_VerifyKeyExposeLevel(Agent_Ctx *pContext, Key *pKey, int nUsage)
{
	int nRet = 0;
	int nExposeLevel;

	// expose level ������ ���� ��� �׳� ���
	if(strlen(pKey->expose_level) == 0)
		goto error;

	nExposeLevel = strtol(pKey->expose_level, NULL, 10);

	PRINT_DEBUG("KM_VerifyKeyExposeLevel Start.");

	switch(nUsage)
	{
	case USE_KEY_SERVER_ONLY:
	case USE_KEY_IN_AGENT:
	case USE_KEY_OUT_OF_AGENT:
		break;
	default:
		// ���ǵ��� ���� Ű �뵵
		nRet = TK_AGENT_ERROR_FAILED_EXPOSELEVEL_UNDEFINED_KEYUSAGE;
		goto error;
		break;;
	}

	if(nExposeLevel == USE_KEY_SERVER_ONLY)
	{
		// Critical Server error.
		nRet = TK_AGENT_ERROR_FAILED_EXPOSELEVEL_SERVER_ONLY_KEY;
	}
	else if(nExposeLevel < nUsage)
		nRet = TK_AGENT_ERROR_FAILED_EXPOSELEVEL;

error:

	PRINT_DEBUG("KM_VerifyKeyExposeLevel End.");

	return nRet;
}

int KM_InitComm(Agent_Ctx *pContext)
{
	int nRet = 0;
	PRINT_DEBUG("[ KM_InitComm ] Start.");

	if((nRet = TK_COMM_INIT(&pContext->sslCtx, SSL_SERVER_CA_CERT_NAME, pContext->pConfig->agentHomeDir)) != TK_SUCCESS)
		goto error;

	PRINT_DEBUG("KM_InitComm Success.");
error:

	PRINT_DEBUG("[ KM_InitComm ] End.\n");

	return nRet;
}

int KM_CheckCtx(Agent_Ctx *pContext)
{
	int nRet = 0;

	if(pContext == NULL || pContext->pConfig == NULL || pContext->pKeyList == NULL)
	{
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_CHECKCTX_INITIALIZE;
	}

	return nRet;
}
void KM_CtxFree(Agent_Ctx **ppContext)
{
	TK_COMM_FINAL(&(*ppContext)->sslCtx);

	if(*ppContext)
	{
		if((*ppContext)->pKeyList)
		{
			if((*ppContext)->pKeyList->pKey)
			{
				TK_MemFree((void**)&(*ppContext)->pKeyList->pKey);
			}
		}
		TK_MemFree((void**)&(*ppContext)->pKeyList);

		if((*ppContext)->pConfig)
		{
			TK_MemFree((void**)&(*ppContext)->pConfig);
		}
	}
	TK_MemFree((void**)ppContext);

	return;
}

int KM_SetConfig(Agent_Ctx *pContext, char *szConfPath)
{
	int		nRet = 0;
	FILE	*pfile = NULL;
	char	szConfFileName[MAX_PATH] = {0};
	char	szBuff[MAX_PATH] = {0};		// file read ����
	char	seperate[] = "= \t\n\r";
	char	*pToken = NULL;
	char	*pNextToken = NULL;

	PRINT_DEBUG("KM_SetConfig Start.");

	if(pContext == NULL || pContext->pConfig == NULL || szConfPath == NULL || strlen(szConfPath) < 1)
	{
		PRINT_DEBUG("*** KM_SetConfig argument error.");
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_SETCONFIG_ARG_ERROR;
		goto error;
	}	

	strncpy(pContext->pConfig->agentHomeDir, szConfPath, sizeof(pContext->pConfig->agentHomeDir) -1);
 	sprintf(szConfFileName, "%s/%s", pContext->pConfig->agentHomeDir, AGENT_CONF_FILE_NAME);

	PRINT_DEBUG(szConfFileName);

	if((pfile = fopen(szConfFileName, "r")) == NULL)
	{
		PRINT_DEBUG("*** KM_SetConfig file open error.");
		
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_SETCONFIG_FILEOPEN;
		goto error;
	}
	while(!feof(pfile))
	{		
		if(fgets(szBuff, sizeof(szBuff), pfile) == NULL)
			break;

		if((pToken = strtok(szBuff, seperate)) == NULL)
			continue;

		if(strcmp(pToken, "kmsIP") == 0)
		{
			if((pToken = strtok(NULL, seperate)) == NULL)
				continue;
			strncpy(pContext->pConfig->kmsIP, pToken, sizeof(pContext->pConfig->kmsIP) -1);
			PRINT_DEBUG("kmsIP");
			PRINT_DEBUG(pContext->pConfig->kmsIP);
		}
		else if(strcmp(pToken, "kmsPort") == 0)
		{			
			if((pToken = strtok(NULL, seperate)) == NULL)
				continue;
			strncpy(pContext->pConfig->kmsPort, pToken, sizeof(pContext->pConfig->kmsPort) -1);
			PRINT_DEBUG("kmsPort");
			PRINT_DEBUG(pContext->pConfig->kmsPort);
		}
		else if(strcmp(pToken, "agentID") == 0)
		{
			if((pToken = strtok(NULL, seperate)) == NULL)
				continue;
			strncpy(pContext->pConfig->agentID, pToken, sizeof(pContext->pConfig->agentID) -1);
			PRINT_DEBUG("agentID");
			PRINT_DEBUG(pContext->pConfig->agentID);
		}
		else if(strcmp(pToken, "agentType") == 0)
		{
			if((pToken = strtok(NULL, seperate)) == NULL)
				continue;
			strncpy(pContext->pConfig->agentType, pToken, sizeof(pContext->pConfig->agentType) -1);
			PRINT_DEBUG("agentType");
			PRINT_DEBUG(pContext->pConfig->agentType);
		}
		else if(strcmp(pToken, "agentHint") == 0)
		{
			// strtok�� �Ľ��� ��� base64encode ���ڿ��� �е�����'='�� ���ǵ� �� �����Ƿ� ������� �ʴ´�.
			unsigned int i = 0;
			int nAgentHintPtr = 0;
			int bStartStr = 0;		// base64encode ���ڿ��� '='�� �츮�� ���� �÷���

			pToken = pToken + strlen(pToken);
			while(1)
			{
				if(*pToken != '\0' && *pToken != ' ' && *pToken != '=' && *pToken != '\t' && *pToken != '\r' && *pToken != '\n')
					break;
				else
					pToken++;
			}

			for(i = 0; i < strlen(pToken); i++)
			{
				if(bStartStr == 0 && pToken[i] != ' ' && pToken[i] != '=' && pToken[i] != '\t' && pToken[i] != '\r' && pToken[i] != '\n')
				{
					pContext->pConfig->agentHint[nAgentHintPtr++] = pToken[i];
					bStartStr = 1;
				}
				else if(bStartStr == 1 && pToken[i] != ' ' && pToken[i] != '\t' && pToken[i] != '\r' && pToken[i] != '\n')
				{
					pContext->pConfig->agentHint[nAgentHintPtr++] = pToken[i];					
				}
			}
			PRINT_DEBUG("agentHint");
			PRINT_DEBUG(pContext->pConfig->agentHint);
		}
		else if(strcmp(pToken, "Integrity") == 0)
		{
			// strtok�� �Ľ��� ��� base64encode ���ڿ��� �е�����'='�� ���ǵ� �� �����Ƿ� ������� �ʴ´�.
			unsigned int i = 0;
			int nIntegrityPtr = 0;
			int bStartStr = 0;		// base64encode ���ڿ��� '='�� �츮�� ���� �÷���

			pToken = pToken + strlen(pToken);
			while(1)
			{
				if(*pToken != '\0' && *pToken != ' ' && *pToken != '=' && *pToken != '\t' && *pToken != '\r' && *pToken != '\n')
					break;
				else
					pToken++;
			}

			for(i = 0; i < strlen(pToken); i++)
			{
				if(bStartStr == 0 && pToken[i] != ' ' && pToken[i] != '=' && pToken[i] != '\t' && pToken[i] != '\r' && pToken[i] != '\n')
				{
					pContext->pConfig->Integrity[nIntegrityPtr++] = pToken[i];
					bStartStr = 1;
				}
				else if(bStartStr == 1 && pToken[i] != ' ' && pToken[i] != '\t' && pToken[i] != '\r' && pToken[i] != '\n')
				{
					pContext->pConfig->Integrity[nIntegrityPtr++] = pToken[i];					
				}
			}
			PRINT_DEBUG("Integrity");
			PRINT_DEBUG(pContext->pConfig->Integrity);
		}
	}

error:

	if(pfile)
		fclose(pfile);

	PRINT_DEBUG("KM_SetConfig End.\n");

	return nRet;
}

int KM_VerifyAgentHint(Agent_Ctx *pContext)
{
	int nRet = 0;
	char *pszHash_B64 = NULL;
	UString usHash;	

#ifdef WIN32
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	IP_ADAPTER_INFO AdapterInfo[16];			// Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);		// Save the memory size of buffer
#else
	struct ifconf ifc;
	struct ifreq ifr[10];
	int sd, ifc_num, i;
	struct in_addr addr; 
	char host[NI_MAXHOST] = {0};
#endif

	memset(&usHash, 0x00, sizeof(UString));

	// agentType�� 1�̸�(=�����̸�) AgentHint üũ�� ���� �ʰ� IP�� 0.0.0.0�� �ִ´�.
	if(strncmp(pContext->pConfig->agentType, "1", 1) == 0)
	{
		PRINT_DEBUG("agentType == 1");
		strncpy(pContext->pConfig->agentIP, "0.0.0.0", sizeof(pContext->pConfig->agentIP) -1);
		goto error;
	}

#ifdef WIN32
	PRINT_DEBUG("KM_VerifyAgentHint Start.");

	if((nRet = GetAdaptersInfo(AdapterInfo, &dwBufLen)) != TK_SUCCESS)
	{
		PRINT_DEBUG("*** GetAdaptersInfo error.");
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VERIFYAGENTHINT_GETADAPTERSINFO;
		goto error;
	}

	pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info

	do
	{	
		char *pIP = pAdapterInfo->IpAddressList.IpAddress.String;
		UString usIP;
		
		memset(&usHash, 0x00, sizeof(UString));

		if (!strcmp(pIP, "127.0.0.1"))
			continue;

		// SHA256(IP)
		usIP.value = pIP;
		usIP.length = strlen(pIP);
		if((nRet = TK_Sha256Hash(&usIP, &usHash)) != TK_SUCCESS)
			goto error;
		// B64Encode(SHA256(IP))
		if((nRet = TK_Base64_Encode(&usHash, &pszHash_B64)) != TK_SUCCESS)
			goto error;
		// Compare B64Encode(SHA256(IP)) with agentHint		
		PRINT_DEBUG("compare B64(sha256(IP)) with agentHint");
		PRINT_DEBUG(pszHash_B64);
		PRINT_DEBUG(pContext->pConfig->agentHint);
		PRINT_DEBUG(pIP);

		if(strncmp(pContext->pConfig->agentHint, pszHash_B64, strlen(pContext->pConfig->agentHint)) == 0)
		{
			// Ȯ�� �� Client IP�� Config�� ����.			
			strncpy(pContext->pConfig->agentIP, pIP, sizeof(pContext->pConfig->agentIP) -1);
			PRINT_DEBUG("matched.");			
			break;
		}
		pAdapterInfo = pAdapterInfo->Next;		// Progress through linked list
		TK_MemFree((void**)&pszHash_B64);
		TK_Free_UString(&usHash);
	}
	while(pAdapterInfo);

	if(strlen(pContext->pConfig->agentIP) == 0)
	{
		// IP ȹ�� ����!!! Critical error!
		PRINT_DEBUG("*** IP Check Failed.");
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VERIFYAGENTHINT_INVALIDIP;
		goto error;
	}

#else

#ifdef AIX
	#define SIOCGIFCONF CSIOCGIFCONF
	PRINT_DEBUG("AIX");
#endif
	
	PRINT_DEBUG("KM_VerifyAgentHint Start.");

	if((sd = socket(PF_INET, SOCK_DGRAM, 0)) == 0)
	{
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VERIFYAGENTHINT_SOCKET;
		goto error;
	}

	ifc.ifc_len = sizeof(ifr);
	ifc.ifc_ifcu.ifcu_buf = (caddr_t)ifr;

	if (ioctl(sd, SIOCGIFCONF/*CSIOCGIFCONF*/, &ifc) != 0)
	{
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VERIFYAGENTHINT_IOCTL;
		goto error;
	}

	ifc_num = ifc.ifc_len / sizeof(struct ifreq);

	for (i = 0; i < ifc_num; ++i)
	{		
		UString usIP;

		if (ifc.ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr.sa_family != AF_INET) 
			continue;

		addr = ((struct sockaddr_in *)&ifc.ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr)->sin_addr; 
		strcpy(host, inet_ntoa(addr));

		if (!strcmp(host, "127.0.0.1"))
			continue;

		PRINT_DEBUG(host);
		// SHA256(IP)
		usIP.value = host;
		usIP.length = strlen(host);
		if((nRet = TK_Sha256Hash(&usIP, &usHash)) != TK_SUCCESS)
			goto error;
		// B64Encode(SHA256(IP))
		if((nRet = TK_Base64_Encode(&usHash, &pszHash_B64)) != TK_SUCCESS)
		{
			nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VERIFYAGENTHINT;
			goto error;
		}
		PRINT_DEBUG("compare B64(sha256(IP)) with agentHint");
		PRINT_DEBUG(pszHash_B64);
		PRINT_DEBUG(pContext->pConfig->agentHint);
		PRINT_DEBUG(host);

		if(strncmp(pContext->pConfig->agentHint, pszHash_B64, strlen(pContext->pConfig->agentHint)) == 0)
		{			
			// Ȯ�� �� Client IP�� Config�� ����.
			strncpy(pContext->pConfig->agentIP, host, sizeof(pContext->pConfig->agentIP) -1);
			PRINT_DEBUG("matched.");
			break;
		}			
		TK_MemFree((void**)&pszHash_B64);
		TK_Free_UString(&usHash);
	}

	if(strlen(pContext->pConfig->agentIP) == 0)
	{
		// IP ȹ�� ����!!! Critical error!
		PRINT_DEBUG("*** IP Check Failed.");
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VERIFYAGENTHINT_INVALIDIP;
		goto error;
	}
	close(sd);
#endif

error:

	TK_MemFree((void**)&pszHash_B64);
	TK_Free_UString(&usHash);
	PRINT_DEBUG("KM_VerifyAgentHint End.\n");

	return nRet;
}

int KM_VerifyIntegrity(Agent_Ctx *pContext)
{
	int nRet = 0;
	char IntegrityText[200] = {0};
	char *szHMACResult_B64 = NULL;
	UString HMACKey;
	UString HMACResult;

	memset(&HMACKey, 0x00, sizeof(UString));
	memset(&HMACResult, 0x00, sizeof(UString));
	
	PRINT_DEBUG("KM_VerifyIntegrity Start.");

	// HMAC Ű ����
	{
		UString agentIP;
		agentIP.value = pContext->pConfig->agentIP;
		agentIP.length = strlen(pContext->pConfig->agentIP);

		if((nRet = KM_Make_KeyFromMaterial(&agentIP, &HMACKey)) != TK_SUCCESS)
			goto error;
	}
	// HMAC ����	
	{
		// Integrity(HMAC) ���� �޽��� ����
		UString HMACText;
		
		sprintf(IntegrityText, "%s%s%s%s%s",pContext->pConfig->kmsIP, pContext->pConfig->kmsPort, pContext->pConfig->agentID, pContext->pConfig->agentType, pContext->pConfig->agentHint);

		HMACText.value = IntegrityText;
		HMACText.length = strlen(IntegrityText);
		
		if((nRet = TK_Make_HMAC(SHA256, &HMACKey, &HMACText, &HMACResult)) != TK_SUCCESS)
			goto error;
	}

	
	// Base64Encode	
 	if((nRet = TK_Base64_Encode(&HMACResult, &szHMACResult_B64)) != TK_SUCCESS)
		goto error;
	PRINT_DEBUG("Compare HMAC with Integrity");
	PRINT_DEBUG(szHMACResult_B64);
	PRINT_DEBUG(pContext->pConfig->Integrity);

	// Integrity���� HMAC�� ��	
	if(strncmp(pContext->pConfig->Integrity, szHMACResult_B64, sizeof(pContext->pConfig->Integrity)) != 0)
	{
		PRINT_DEBUG("Integrity Check Faild.");
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_VERIFYINTEGRITY_VERIFY;
		goto error;
	}	
	PRINT_DEBUG("Integrity Check Success.");

error:

	TK_Free_UString(&HMACKey);
	TK_Free_UString(&HMACResult);
	TK_MemFree((void**)&szHMACResult_B64);
	
	PRINT_DEBUG("KM_VerifyIntegrity End.\n");
	return nRet;
}

int KM_Make_Agent_HMAC(Agent_Ctx *pContext)
{
	int nRet = 0;
	UString hmacKey;
	UString agentHmac;
	char *szAgentHMAC_B64 = NULL;

	memset(&hmacKey, 0x00, sizeof(UString));
	memset(&agentHmac, 0x00, sizeof(UString));

	// HMAC Ű ����
	{
		UString agentIP;
		agentIP.value = pContext->pConfig->agentIP;
		agentIP.length = strlen(pContext->pConfig->agentIP);

		if((nRet = KM_Make_KeyFromMaterial(&agentIP, &hmacKey)) != TK_SUCCESS)
			goto error;
	}
	// HMAC ����
	{
		UString agentID;
		agentID.value = pContext->pConfig->agentID;
		agentID.length = strlen(pContext->pConfig->agentID);

		if((nRet = TK_Make_HMAC(SHA256, &hmacKey, &agentID, &agentHmac)) != TK_SUCCESS)
			goto error;
	}
	// Base64Encode
	if((nRet = TK_Base64_Encode(&agentHmac, &szAgentHMAC_B64)) != TK_SUCCESS)
		goto error;

	memcpy(pContext->pConfig->agentHMAC, szAgentHMAC_B64, strlen(szAgentHMAC_B64));

	PRINT_DEBUG("Agent HMAC");
	PRINT_DEBUG(pContext->pConfig->agentHMAC);

error:

	TK_Free_UString(&hmacKey);
	TK_Free_UString(&agentHmac);
	TK_MemFree((void**)&szAgentHMAC_B64);

	return nRet;
}

int KM_Make_KeyFromMaterial(UString *pMaterial, UString *pKey)
{
	int nRet = 0;
	char sha256Val[SHA_256_LEN] = {0};

	if(pMaterial->value == NULL || pMaterial->length == 0 || pKey == NULL)
	{
		nRet = g_nAgentErrorCode = TK_AGENT_ERROR_FAILED_MAKE_KEYFROMMATERIAL_ARG_ERROR;
		goto error;
	}	

	if((nRet = TK_Sha256Hash(pMaterial, pKey)) != TK_SUCCESS)
		goto error;

	memcpy(sha256Val, pKey->value, sizeof(sha256Val));

	// SHA256(in) �� 4,1,3,2 ������ �ɰ��� Ű�� �����.
	memcpy(&pKey->value[0],		&sha256Val[24],	8);
	memcpy(&pKey->value[8],		&sha256Val[0],	8);
	memcpy(&pKey->value[16],	&sha256Val[16],	8);
	memcpy(&pKey->value[24],	&sha256Val[8],	8);

error:

	return nRet;
}

int KM_Make_KEK_Material(Agent_Ctx *pContext)
{
	int		nRet = 0;
	char	randomNumber[32] = {0};

	if((nRet = TK_Make_Random(randomNumber, sizeof(randomNumber))) != TK_SUCCESS)
		goto error;

	memcpy(pContext->kekMaterial, randomNumber, sizeof(randomNumber));
	PRINT_DEBUG("KEK Material");	
	PRINT_DEBUG_BIN2STR(pContext->kekMaterial, sizeof(pContext->kekMaterial));

error:

	return nRet;
}

int KM_Make_KEK(Agent_Ctx *pContext, UString *pKek)
{
	int nRet = 0;
	UString kekMaterial;
	
	kekMaterial.value = pContext->kekMaterial;
	kekMaterial.length = sizeof(pContext->kekMaterial);

	if((nRet = KM_Make_KeyFromMaterial(&kekMaterial, pKek)) != TK_SUCCESS)
		goto error;

error:
	
	return nRet;
}

int KM_EncryptKey(Agent_Ctx *pContext, Key *pKey)
{
	int nRet = 0;
	UString kek;
	UString encKey;

	memset(&kek, 0x00, sizeof(UString));
	memset(&encKey, 0x00, sizeof(UString));

	PRINT_DEBUG("KM_EncryptKey Start.");

	// Make KEK
	if((nRet = KM_Make_KEK(pContext, &kek)) != TK_SUCCESS)
		goto error;
	// encrypt key
	{
		UString plainKey;
		plainKey.value = pKey->enc_key_value;
		plainKey.length = pKey->key_size;

		if((nRet = TK_Aes128_Encrypt(&encKey, &plainKey, &kek)) != TK_SUCCESS)
			goto error;
	}
	// Set Enc Key
	memset(pKey->enc_key_value, 0x00, sizeof(pKey->enc_key_value));
	memcpy(pKey->enc_key_value, encKey.value, encKey.length);
	pKey->key_size = encKey.length;

	PRINT_DEBUG("KM_EncryptKey Success.");

error:

	TK_Free_UString(&kek);
	TK_Free_UString(&encKey);

	PRINT_DEBUG("KM_EncryptKey End.\n");

	return nRet;
}

int KM_DecryptKey(Agent_Ctx *pContext, Key *pKey)
{
	int nRet = 0;
	UString kek;
	UString decKey;

	memset(&kek, 0x00, sizeof(UString));
	memset(&decKey, 0x00, sizeof(UString));

	// Make KEK
	if((nRet = KM_Make_KEK(pContext, &kek)) != TK_SUCCESS)
		goto error;
	// Decrypt key
	{
		UString encKey;
		encKey.value = pKey->enc_key_value;
		encKey.length = pKey->key_size;

		if((nRet = TK_Aes128_Decrypt(&decKey, &encKey, &kek)) != TK_SUCCESS)
			goto error;
	}

	// Set Dec Key
	memset(pKey->enc_key_value, 0x00, sizeof(pKey->enc_key_value));
	memcpy(pKey->enc_key_value, decKey.value, decKey.length);
	pKey->key_size = decKey.length;

error:
	TK_Free_UString(&kek);
	TK_Free_UString(&decKey);

	return nRet;
}

int KM_MakeKIR(char* I_ID, char* R_ID, UString usRandom2, UString *pusKIR)
{
	int nRet = 0;
	UString usHashI_ID, usHashR_ID, usResultXor;

	memset(&usHashI_ID, 0x00, sizeof(UString));
	memset(&usHashR_ID, 0x00, sizeof(UString));
	memset(&usResultXor, 0x00, sizeof(UString));

	{
		UString usI_ID, usR_ID;
		usI_ID.value = I_ID;
		usI_ID.length = strlen(I_ID);
		if((nRet = TK_Sha256Hash(&usI_ID, &usHashI_ID)) != TK_SUCCESS)
			goto error;

		usR_ID.value = R_ID;
		usR_ID.length = strlen(R_ID);
		if((nRet = TK_Sha256Hash(&usR_ID, &usHashR_ID)) != TK_SUCCESS)
			goto error;
	}	

	// Xor ��� ��� ����
	if((nRet = TK_Set_UString(&usResultXor, NULL, IND_BLOCK_SIZE)) != TK_SUCCESS)
		goto error;

	KM_ComputeXOR_IND_BLOCK(g_IND_PREFIX, usHashI_ID.value, usResultXor.value);
	KM_ComputeXOR_IND_BLOCK(usResultXor.value, usRandom2.value, usResultXor.value);
	KM_ComputeXOR_IND_BLOCK(usResultXor.value, usHashR_ID.value, usResultXor.value);
	KM_ComputeXOR_IND_BLOCK(usResultXor.value, g_IND_POSTFIX, usResultXor.value);

	if((nRet = KM_Make_KeyFromMaterial(&usResultXor, pusKIR)) != TK_SUCCESS)
		goto error;

error:

	TK_Free_UString(&usHashI_ID);
	TK_Free_UString(&usHashR_ID);
	TK_Free_UString(&usResultXor);

	return nRet;
}

int KM_MakeSessionKey(UString usRandom1, UString usRandom2, UString usKIR, UString *pusSessionKey)
{
	int nRet = 0;
	UString usResultXor;

	memset(&usResultXor, 0x00, sizeof(UString));

	if((nRet = TK_Set_UString(&usResultXor, NULL, IND_BLOCK_SIZE)) != TK_SUCCESS)
		goto error;
	KM_ComputeXOR_IND_BLOCK(usRandom1.value, usRandom2.value, usResultXor.value);
	
	if((nRet = TK_Aes128_Encrypt(pusSessionKey, &usResultXor, &usKIR)) != TK_SUCCESS)
		goto error;

error:

	TK_Free_UString(&usResultXor);

	return nRet;
}

void KM_ComputeXOR_IND_BLOCK(char *a, char *b, char *r)
{
	int i = 0;

	for(i = 0; i < IND_BLOCK_SIZE; i++)
	{
		r[i] = a[i] ^ b[i];
	}	
}
