#include <string.h>
#include <time.h>
//#include "libfuns.h"
#include "libheartland.h"


#define HEARTLAND_SOAP_VERSION  "1.0.0.20190221"



ST_HSOAP_INIT hsoapHeaderData;
static unsigned char hsoapHeaderFlag = 0;
bool gDebugEnabled = false;

typedef unsigned short (* ExtraFunction_HeartlandSOAP_Init)(ST_HSOAP_INIT *hsoapData);
typedef unsigned short (* ExtraFunction_HeartlandSOAP_Pack)(ST_HSOAP_REQUEST *hsoapData, int *pDataLen, char *pData);
typedef unsigned short (* ExtraFunction_HeartlandSOAP_Unpack)(ST_HSOAP_RESPONSE *hsoapData, char *pData);


void HeartlandSoap::HeartlandSOAP_DebugEnable(void) {
    gDebugEnabled = true;
}
void HeartlandSoap::HeartlandSOAP_DebugDisable(void) {
    gDebugEnabled = false;
}

unsigned short HeartlandSoap::HeartlandSOAP_GetVersion(char *pcVersion, int *piLength) {
    if (pcVersion == NULL || piLength == NULL) {
        HLND_SOAP_DEBUG_MSG("ERR: pcVersion == NULL || piLength == NULL");
        return d_HSOAP_NULL_POINT;
    }
    if (*piLength < (strlen(HEARTLAND_SOAP_VERSION) + 1)) {
        HLND_SOAP_DEBUG_MSG("ERR: pcVersion buffer length(*piLength:%d) < %d", *piLength, (strlen(HEARTLAND_SOAP_VERSION) + 1));
        return d_HSOAP_ERR_PARAMETER;
    }
    *piLength = strlen(HEARTLAND_SOAP_VERSION);
    memcpy(pcVersion, HEARTLAND_SOAP_VERSION, *piLength);
    pcVersion[*piLength] = '\0';
    return d_HSOAP_SUCCESS;
}

unsigned short HeartlandSoap::HeartlandSOAP_Init(ST_HSOAP_INIT *hsoapData) {
    if (hsoapData->ullExtraData != 0)
    {
        HLND_SOAP_DEBUG_MSG("INFO: hsoapData has extra data");
        ExtraFunction_HeartlandSOAP_Init pExtraFunction = (ExtraFunction_HeartlandSOAP_Init)(hsoapData->ullExtraData);
        return pExtraFunction(hsoapData);
    }
    
    hsoapHeaderFlag = 0;
    memset(&hsoapHeaderData, 0x00, sizeof(ST_HSOAP_INIT));
    if (hsoapData == NULL) {
        HLND_SOAP_DEBUG_MSG("ERR: hsoapData == NULL");
        return d_HSOAP_NULL_POINT;
    }
    memcpy(&hsoapHeaderData, hsoapData, sizeof(ST_HSOAP_INIT));
    hsoapHeaderFlag = 1;
    
    return d_HSOAP_SUCCESS;
}

unsigned short HeartlandSoap::HeartlandSOAP_Pack(ST_HSOAP_REQUEST *hsoapData, int *pDataLen, char *pData) {
    if (hsoapData->ullExtraData != 0)
    {
        HLND_SOAP_DEBUG_MSG("INFO: hsoapData has extra data");
        ExtraFunction_HeartlandSOAP_Pack pExtraFunction = (ExtraFunction_HeartlandSOAP_Pack)(hsoapData->ullExtraData);
        return pExtraFunction(hsoapData, pDataLen, pData);
    }
    
    unsigned short usRet;
    string soapStr;
    HeartLandRequest heartland;
    
    do {
        if (hsoapData == NULL || pDataLen == NULL || pData == NULL) {
            HLND_SOAP_DEBUG_MSG("ERR: hsoapData == NULL || pDataLen == NULL || pData == NULL");
            usRet = d_HSOAP_NULL_POINT;
            break;
        }
        
        if (!hsoapHeaderFlag) {
            usRet = d_HSOAP_API_NOT_INIT;
            break;
        }
        
        usRet = HeartlandPackHeader(heartland, &hsoapHeaderData);
        if (usRet != d_HSOAP_SUCCESS) {
            break;
        }
        
        if (!HeartlandHeaderDataLengthCheck(strlen(hsoapHeaderData.szPosGatewayUrl), sizeof(hsoapHeaderData.szPosGatewayUrl))) {
            HLND_SOAP_DEBUG_MSG("ERR: No szPosGatewayUrl");
            usRet = d_HSOAP_ERR_PARAMETER;
            break;
        }
        
        usRet = HeartlandPackTransaction(heartland, hsoapData);
        if (usRet != d_HSOAP_SUCCESS) {
            break;
        }
        soapStr = heartland.generateSoapString(hsoapHeaderData.szPosGatewayUrl);
        if (soapStr.size() <= 0) {
            HLND_SOAP_DEBUG_MSG("ERR: generateSoapString fail");
            usRet = d_HSOAP_API_FAIL;
            break;
        }
        if (*pDataLen < (soapStr.size() + 1)) {
            HLND_SOAP_DEBUG_MSG("ERR: *pDataLen(%d) < %d", *pDataLen, (soapStr.size() + 1));
            usRet = d_HSOAP_BUF_NOT_ENOUGH;
            break;
        }
        *pDataLen = soapStr.size();
        memcpy(pData, soapStr.c_str(), *pDataLen);
        pData[*pDataLen] = '\0';
    }while (0);
    HeartlandSOAP_ResetHeaderOptionalFields();
    return usRet;
}

unsigned short HeartlandSoap::HeartlandSOAP_Unpack(ST_HSOAP_RESPONSE *hsoapData, char *pData) {
    if (hsoapData->ullExtraData != 0)
    {
        HLND_SOAP_DEBUG_MSG("INFO: hsoapData has extra data");
        ExtraFunction_HeartlandSOAP_Unpack pExtraFunction = (ExtraFunction_HeartlandSOAP_Unpack)(hsoapData->ullExtraData);
        return pExtraFunction(hsoapData, pData);
    }
    
    HeartLandResponse response;
    if (hsoapData == NULL || pData == NULL) {
        HLND_SOAP_DEBUG_MSG("ERR: hsoapData == NULL || pData == NULL");
        return d_HSOAP_NULL_POINT;
    }
    if (response.parseResponseSoap(pData) == false) {
        HLND_SOAP_DEBUG_MSG("ERR: parseResponseSoap fail");
        return d_HSOAP_API_FAIL;
    }
    
    if (response.header.size() > 0) {
        HeartlandUnpackHeader(response, hsoapData);
    }
    
    if (response.transactionResult.size() > 0) {
        HeartlandUnpackTransaction(response, hsoapData);
    }
    return d_HSOAP_SUCCESS;
}

unsigned short HeartlandSoap::HeartlandSOAP_SetHeaderOptionalFields(unsigned char ucFieldType, unsigned char *pucData, unsigned short usDataLen) {
    unsigned short usRet = d_HSOAP_SUCCESS;
    
    do {
        if (pucData == NULL) {
            HLND_SOAP_DEBUG_MSG("ERR: pucData == NULL");
            usRet = d_HSOAP_NULL_POINT;
            break;
        }
        switch (ucFieldType) {
            case d_HEADER_OPTIONAL_FIELD_CLERK_ID:
                if (usDataLen > (sizeof(hsoapHeaderData.szClerkID) - 1)) {
                    HLND_SOAP_DEBUG_MSG("ERR: usDataLen(%d) > hsoapHeaderData.szClerkID buffer len(%d)", usDataLen, (sizeof(hsoapHeaderData.szClerkID) - 1));
                    usRet = d_HSOAP_ERR_PARAMETER;
                    break;
                }
                memcpy(hsoapHeaderData.szClerkID, pucData, usDataLen);
                hsoapHeaderData.szClerkID[usDataLen] = '\0';
                break;
            case d_HEADER_OPTIONAL_FIELD_SITE_TRACE:
                if (usDataLen > (sizeof(hsoapHeaderData.szSiteTrace) - 1)) {
                    HLND_SOAP_DEBUG_MSG("ERR: usDataLen(%d) > hsoapHeaderData.szSiteTrace buffer len(%d)", usDataLen, (sizeof(hsoapHeaderData.szSiteTrace) - 1));
                    usRet = d_HSOAP_ERR_PARAMETER;
                    break;
                }
                memcpy(hsoapHeaderData.szSiteTrace, pucData, usDataLen);
                hsoapHeaderData.szSiteTrace[usDataLen] = '\0';
                break;
            case d_HEADER_OPTIONAL_FIELD_CREDENTIAL_TOKEN:
                if (usDataLen > (sizeof(hsoapHeaderData.szCredentialToken) - 1)) {
                    HLND_SOAP_DEBUG_MSG("ERR: usDataLen(%d) > hsoapHeaderData.szCredentialToken buffer len(%d)", usDataLen, (sizeof(hsoapHeaderData.szCredentialToken) - 1));
                    usRet = d_HSOAP_ERR_PARAMETER;
                    break;
                }
                memcpy(hsoapHeaderData.szCredentialToken, pucData, usDataLen);
                hsoapHeaderData.szCredentialToken[usDataLen] = '\0';
                break;
            case d_HEADER_OPTIONAL_FIELD_CLIENT_TXN_ID:
                if (usDataLen > (sizeof(hsoapHeaderData.szClientTxnId) - 1)) {
                    HLND_SOAP_DEBUG_MSG("ERR: usDataLen(%d) > hsoapHeaderData.szClientTxnId buffer len(%d)", usDataLen, (sizeof(hsoapHeaderData.szClientTxnId) - 1));
                    usRet = d_HSOAP_ERR_PARAMETER;
                    break;
                }
                memcpy(hsoapHeaderData.szClientTxnId, pucData, usDataLen);
                hsoapHeaderData.szClientTxnId[usDataLen] = '\0';
                break;
            case d_HEADER_OPTIONAL_FIELD_UNIQUE_DEVICE_ID:
                if (usDataLen > (sizeof(hsoapHeaderData.szUniqueDeviceId) - 1)) {
                    HLND_SOAP_DEBUG_MSG("ERR: usDataLen(%d) > hsoapHeaderData.szUniqueDeviceId buffer len(%d)", usDataLen, (sizeof(hsoapHeaderData.szUniqueDeviceId) - 1));
                    usRet = d_HSOAP_ERR_PARAMETER;
                    break;
                }
                memcpy(hsoapHeaderData.szUniqueDeviceId, pucData, usDataLen);
                hsoapHeaderData.szUniqueDeviceId[usDataLen] = '\0';
                break;
            case d_HEADER_OPTIONAL_FIELD_SECRET_API_KEY:
                if (usDataLen > (sizeof(hsoapHeaderData.szSecretAPIKey) - 1)) {
                    HLND_SOAP_DEBUG_MSG("ERR: usDataLen(%d) > hsoapHeaderData.szSecretAPIKey buffer len(%d)", usDataLen, (sizeof(hsoapHeaderData.szSecretAPIKey) - 1));
                    usRet = d_HSOAP_ERR_PARAMETER;
                    break;
                }
                memcpy(hsoapHeaderData.szSecretAPIKey, pucData, usDataLen);
                hsoapHeaderData.szSecretAPIKey[usDataLen] = '\0';
                break;
            case d_HEADER_OPTIONAL_FIELD_LATITUDE:
                if (usDataLen > (sizeof(hsoapHeaderData.szLatitude) - 1)) {
                    HLND_SOAP_DEBUG_MSG("ERR: usDataLen(%d) > hsoapHeaderData.szLatitude buffer len(%d)", usDataLen, (sizeof(hsoapHeaderData.szLatitude) - 1));
                    usRet = d_HSOAP_ERR_PARAMETER;
                    break;
                }
                memcpy(hsoapHeaderData.szLatitude, pucData, usDataLen);
                hsoapHeaderData.szLatitude[usDataLen] = '\0';
                break;
            case d_HEADER_OPTIONAL_FIELD_LONGITUDE:
                if (usDataLen > (sizeof(hsoapHeaderData.szLongitude) - 1)) {
                    HLND_SOAP_DEBUG_MSG("ERR: usDataLen(%d) > hsoapHeaderData.szLongitude buffer len(%d)", usDataLen, (sizeof(hsoapHeaderData.szLongitude) - 1));
                    usRet = d_HSOAP_ERR_PARAMETER;
                    break;
                }
                memcpy(hsoapHeaderData.szLongitude, pucData, usDataLen);
                hsoapHeaderData.szLongitude[usDataLen] = '\0';
                break;
            default:
                HLND_SOAP_DEBUG_MSG("ERR: typr not defined(0x%02X)", ucFieldType);
                usRet = d_HSOAP_ERR_PARAMETER;
                break;
        }
    }while (0);
    return usRet;
}

unsigned short HeartlandSoap::HeartlandSOAP_ResetHeaderOptionalFields(void) {
    // below optional
    memset(hsoapHeaderData.szClerkID, 0x00, sizeof(hsoapHeaderData.szClerkID));
    memset(hsoapHeaderData.szSiteTrace, 0x00, sizeof(hsoapHeaderData.szSiteTrace));
    memset(hsoapHeaderData.szCredentialToken, 0x00, sizeof(hsoapHeaderData.szCredentialToken));
    memset(hsoapHeaderData.szClientTxnId, 0x00, sizeof(hsoapHeaderData.szClientTxnId));
    memset(hsoapHeaderData.szUniqueDeviceId, 0x00, sizeof(hsoapHeaderData.szUniqueDeviceId));
    memset(hsoapHeaderData.szSecretAPIKey, 0x00, sizeof(hsoapHeaderData.szSecretAPIKey));
    memset(hsoapHeaderData.szLatitude, 0x00, sizeof(hsoapHeaderData.szLatitude));
    memset(hsoapHeaderData.szLongitude, 0x00, sizeof(hsoapHeaderData.szLongitude));
    memset(hsoapHeaderData.szRFU, 0x00, sizeof(hsoapHeaderData.szRFU));
    return d_HSOAP_SUCCESS;
}


unsigned short HeartlandSoap::HeartlandPackHeader(SoapElementRef root, ST_HSOAP_INIT *hsoapHeaderData) {
    unsigned short usRet;
    
    SoapElementRef header = root.addSubElement(TAG_NAME_HEADER);
    usRet = HeartlandPackHeaderMandatory(header, hsoapHeaderData);
    if (usRet == d_HSOAP_SUCCESS) {
        HeartlandPackHeaderOptional(header, hsoapHeaderData);
    }
    return usRet;
}

unsigned short HeartlandSoap::HeartlandPackHeaderMandatory(SoapElementRef header, ST_HSOAP_INIT *hsoapHeaderData) {
    if (HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_LICENSE_ID, hsoapHeaderData->szLicenseId, sizeof(hsoapHeaderData->szLicenseId), true)) {
        HLND_SOAP_DEBUG_MSG("ERR: Header data [szLicenseId] is empty");
        return d_HSOAP_ERR_DATA_SIZE;
    }
    
    if (HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_SITE_ID, hsoapHeaderData->szSiteId, sizeof(hsoapHeaderData->szSiteId), true)) {
        HLND_SOAP_DEBUG_MSG("ERR: Header data [szSiteId] is empty");
        return d_HSOAP_ERR_DATA_SIZE;
    }
    
    if (HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_DEVICE_ID, hsoapHeaderData->szDeviceId, sizeof(hsoapHeaderData->szDeviceId), true)) {
        HLND_SOAP_DEBUG_MSG("ERR: Header data [szDeviceId] is empty");
        return d_HSOAP_ERR_DATA_SIZE;
    }
    
    if (HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_USER_NAME, hsoapHeaderData->szUserName, sizeof(hsoapHeaderData->szUserName), true)) {
        HLND_SOAP_DEBUG_MSG("ERR: Header data [szUserName] is empty");
        return d_HSOAP_ERR_DATA_SIZE;
    }
    
    if (HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_PASSWORD, hsoapHeaderData->szPassword, sizeof(hsoapHeaderData->szPassword), true)) {
        HLND_SOAP_DEBUG_MSG("ERR: Header data [szPassword] is empty");
        return d_HSOAP_ERR_DATA_SIZE;
    }
    
    if (HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_DEVELOPER_ID, hsoapHeaderData->szDeveloperID, sizeof(hsoapHeaderData->szDeveloperID), true)) {
        HLND_SOAP_DEBUG_MSG("ERR: Header data [szDeveloperID] is empty");
        return d_HSOAP_ERR_DATA_SIZE;
    }
    
    if (HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_VERSION_NUMBER, hsoapHeaderData->szVersionNbr, sizeof(hsoapHeaderData->szVersionNbr), true)) {
        HLND_SOAP_DEBUG_MSG("ERR: Header data [szVersionNbr] is empty");
        return d_HSOAP_ERR_DATA_SIZE;
    }
    return d_HSOAP_SUCCESS;
}

void HeartlandSoap::HeartlandPackHeaderOptional(SoapElementRef header, ST_HSOAP_INIT *hsoapHeaderData) {
    HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_CLERK_ID, hsoapHeaderData->szClerkID, sizeof(hsoapHeaderData->szClerkID), false);
    HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_SITE_TRACE, hsoapHeaderData->szSiteTrace, sizeof(hsoapHeaderData->szSiteTrace), false);
    HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_CREDENTIAL_TOKEN, hsoapHeaderData->szCredentialToken, sizeof(hsoapHeaderData->szCredentialToken), false);
    HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_CLIENT_TXN_ID, hsoapHeaderData->szClientTxnId, sizeof(hsoapHeaderData->szClientTxnId), false);
    HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_UNIQUE_DEVICE_ID, hsoapHeaderData->szUniqueDeviceId, sizeof(hsoapHeaderData->szUniqueDeviceId), false);
    HeartlandAddSubElement(header, NULL_STRING, NULL_STRING, TAG_NAME_SECRET_API_KEY, hsoapHeaderData->szSecretAPIKey, sizeof(hsoapHeaderData->szSecretAPIKey), false);
    
    if(strlen(hsoapHeaderData->szLatitude) > 0 || strlen(hsoapHeaderData->szLongitude) > 0) {
        SoapElementRef gpsCoordinates = header.addSubElement(TAG_NAME_GPS_COORDINATES);
        HeartlandAddSubElement(gpsCoordinates, NULL_STRING, NULL_STRING, TAG_NAME_LATITUDE, hsoapHeaderData->szLatitude, sizeof(hsoapHeaderData->szLatitude), false);
        HeartlandAddSubElement(gpsCoordinates, NULL_STRING, NULL_STRING, TAG_NAME_LONGITUDE, hsoapHeaderData->szLongitude, sizeof(hsoapHeaderData->szLongitude), false);
    }
}

unsigned short HeartlandSoap::HeartlandPackTransaction(SoapElementRef root, ST_HSOAP_REQUEST *hsoapData) {
    unsigned short usRet = d_HSOAP_ERR_PARAMETER;
    
    if (hsoapData->nTransactionType > tt_TRANS_TYPE_START && hsoapData->nTransactionType < tt_TRANS_TYPE_END) {
        SoapElementRef transType = HeartlandPackTransType(root, hsoapData->nTransactionType);
        
        switch (hsoapData->nTransactionType) {
            case tt_CREDIT_SALE:
            case tt_CREDIT_RETURN:
            case tt_CREDIT_REVERSAL:
            case tt_CREDIT_AUTH:
            case tt_DEBIT_SALE:
            case tt_DEBIT_RETURN:
            case tt_DEBIT_REVERSAL:
            case tt_UTILITY_PARAMETER_DOWNLOAD:
                usRet = HeartlandPackProcessEntryEx(transType, hsoapData, true);
            break;
            
            case tt_CREDIT_VOID:
            case tt_CREDIT_ADD_TO_BATCH:
            case tt_CREDIT_TXN_EDIT:
                usRet = HeartlandPackProcessEntryEx(transType, hsoapData, false);
                break;
                
            default: // empty such as tt_BATCH_CLOSE
                usRet = d_HSOAP_SUCCESS;
                break;
        }
        HeartlandPackProcessRemove(true, root, transType);
    }
    return usRet;
}

SoapElementRef HeartlandSoap::HeartlandPackTransType(SoapElementRef root, int transactionType) {
    string txnTypeStr;
    SoapElementRef transaction = root.addSubElement(TAG_NAME_TRANSACTION);
    switch (transactionType) {
        case tt_CREDIT_SALE:
            txnTypeStr = TAG_NAME_CREDIT_SALE;
            break;
        case tt_CREDIT_VOID:
            txnTypeStr = TAG_NAME_CREDIT_VOID;
            break;
        case tt_CREDIT_RETURN:
            txnTypeStr = TAG_NAME_CREDIT_RETURN;
            break;
        case tt_CREDIT_REVERSAL:
            txnTypeStr = TAG_NAME_CREDIT_REVERSAL;
            break;
        case tt_CREDIT_AUTH:
            txnTypeStr = TAG_NAME_CREDIT_AUTH;
            break;
        case tt_CREDIT_ADD_TO_BATCH:
            txnTypeStr = TAG_NAME_CREDIT_ADD_TO_BATCH;
            break;
            
        case tt_DEBIT_SALE:
            txnTypeStr = TAG_NAME_DEBIT_SALE;
            break;
        case tt_DEBIT_RETURN:
            txnTypeStr = TAG_NAME_DEBIT_RETURN;
            break;
        case tt_DEBIT_REVERSAL:
            txnTypeStr = TAG_NAME_DEBIT_REVERSAL;
            break;

        case tt_BATCH_CLOSE:
            txnTypeStr = TAG_NAME_BATCH_CLOSE;
            break;

        case tt_CREDIT_TXN_EDIT:
            txnTypeStr = TAG_NAME_CREDIT_TXN_EDIT;
            break;

        default:
            txnTypeStr = TAG_NAME_UNKNOWN;
            break;
    }
    return transaction.addSubElement(txnTypeStr);
}

unsigned short HeartlandSoap::HeartlandPackProcessEntryEx(SoapElementRef startNode, ST_HSOAP_REQUEST *hsoapData, bool enableBolck1Node) {
    
    if (enableBolck1Node) {
        SoapElementRef block1 = HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_HlData, TAG_NAME_BLOCK_1, false);
        HeartlandPackProcessRemove(true, startNode, block1);
        return HeartlandPackProcessEntry(block1, hsoapData);
    } else {
        HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_HlData, NULL_STRING, false);
        return HeartlandPackProcessEntry(startNode, hsoapData);
    }
}

unsigned short HeartlandSoap::HeartlandPackProcessEntry(SoapElementRef startNode, ST_HSOAP_REQUEST *hsoapData) {
    int txnType = hsoapData->nTransactionType;
    
    if (txnType == tt_CREDIT_VOID) {
        return d_HSOAP_SUCCESS;
    }
    
    if (txnType == tt_UTILITY_PARAMETER_DOWNLOAD) {
        SoapElementRef pdlRequest = HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_PDLRequest, TAG_NAME_PDL_REQUEST, false);
        HeartlandPackProcessRemove(true, startNode, pdlRequest);
        return d_HSOAP_SUCCESS;
    }
    
    if ((txnType == tt_CREDIT_ADD_TO_BATCH) || (txnType == tt_CREDIT_TXN_EDIT)) {
        SoapElementRef cLodgingDataEdit = HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_LodgingDataEdit, TAG_NAME_LODGING_DATA_EDIT, false);
        HeartlandPackProcessEx(cLodgingDataEdit, hsoapData, ST_DATA_TYPE_ExtraCharges, TAG_NAME_EXTRA_CHARGES, true);
        HeartlandPackProcessRemove(true, startNode, cLodgingDataEdit);
        HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_DirectMktData, TAG_NAME_DIRECT_MKT_DATA, true);
        HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_TagData, TAG_NAME_TAG_DATA, true);
        return d_HSOAP_SUCCESS;
    }
    
    if(txnType == tt_CREDIT_AUTH) {
        HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_PaymentMethodKeyData, TAG_NAME_PAYMENT_METHOD_KEY_DATA, true);
    }
    
    if (txnType == tt_DEBIT_SALE || txnType == tt_DEBIT_RETURN || txnType == tt_DEBIT_REVERSAL) {
        HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_EncryptionData, TAG_NAME_ENCRYPTION_DATA, true);
    } else {
        SoapElementRef cCardData = HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_CardData, TAG_NAME_CARD_DATA, false);
        HeartlandPackProcessEx(cCardData, hsoapData, ST_DATA_TYPE_ManualEntryData, TAG_NAME_MANUAL_ENTRY, true);
        HeartlandPackProcessEx(cCardData, hsoapData, ST_DATA_TYPE_TokenData, TAG_NAME_TOKEN_DATA, true);
        HeartlandPackProcessEx(cCardData, hsoapData, ST_DATA_TYPE_EncryptionData, TAG_NAME_ENCRYPTION_DATA, true);
        HeartlandPackProcessEx(cCardData, hsoapData, ST_DATA_TYPE_TokenParametersData, TAG_NAME_TOKEN_PARAMETERS, true);
        HeartlandPackProcessEx(cCardData, hsoapData, ST_DATA_TYPE_TokenRequest, NULL_STRING, true);
        HeartlandPackProcessRemove(true, startNode, cCardData);
    }

    if (txnType != tt_CREDIT_REVERSAL && txnType != tt_DEBIT_REVERSAL) {
            HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_CardHolderData, TAG_NAME_CARD_HOLDER_DATA, true);
            
    if(txnType != tt_DEBIT_SALE && txnType != tt_DEBIT_RETURN) {
        HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_DirectMktData, TAG_NAME_DIRECT_MKT_DATA, true);
    }
            
    if (txnType != tt_CREDIT_VOID && txnType != tt_DEBIT_RETURN) {
        SoapElementRef cLodgingData = HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_LodgingData, TAG_NAME_LODGING_DATA, false);
            SoapElementRef cLodgingDataEdit = HeartlandPackProcessEx(cLodgingData, hsoapData, ST_DATA_TYPE_LodgingDataEdit, TAG_NAME_LODGING_DATA_EDIT, false);
            HeartlandPackProcessEx(cLodgingDataEdit, hsoapData, ST_DATA_TYPE_ExtraCharges, TAG_NAME_EXTRA_CHARGES, true);
            HeartlandPackProcessRemove(true, cLodgingData, cLodgingDataEdit);
        HeartlandPackProcessRemove(true, startNode, cLodgingData);
        
    if(txnType != tt_DEBIT_SALE) {
        SoapElementRef cAutoSubstantiation = HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_AutoSubstantiation, TAG_NAME_AUTO_SUBSTANTIATION, false);
        HeartlandPackProcessEx(cAutoSubstantiation, &hsoapData->cAutoSubstantiation.cFirstAdditionalAmtInfo, ST_DATA_TYPE_AllAdditionalAmtInfo, TAG_NAME_FIRST_ADDITINOAL_AMT_INFO, true);
        HeartlandPackProcessEx(cAutoSubstantiation, &hsoapData->cAutoSubstantiation.cSecondAdditionalAmtInfo, ST_DATA_TYPE_AllAdditionalAmtInfo, TAG_NAME_SECOND_ADDITIONAL_AMT_INFO, true);
        HeartlandPackProcessEx(cAutoSubstantiation, &hsoapData->cAutoSubstantiation.cThirdAdditionalAmtInfo, ST_DATA_TYPE_AllAdditionalAmtInfo, TAG_NAME_THIRD_ADDITIONAL_AMT_INFO, true);
        HeartlandPackProcessEx(cAutoSubstantiation, &hsoapData->cAutoSubstantiation.cFourthAdditionalAmtInfo, ST_DATA_TYPE_AllAdditionalAmtInfo, TAG_NAME_FOURTH_ADDITINOAL_AMT_INFO, true);
        HeartlandPackProcessRemove(true, startNode, cAutoSubstantiation);

        HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_OrigTxnRefData, TAG_NAME_ORIG_TXN_REF_DATA, true);
        HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_SecureEC, TAG_NAME_SECURE_ECOMMERCE, true);
    }
        
    }
    if(txnType != tt_DEBIT_SALE && txnType != tt_DEBIT_RETURN) {
        HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_EmvData, TAG_NAME_EMV_DATA, true);
    }
        
    }
        HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_AddTxnFields, TAG_NAME_ADDITIONAL_TXN_FIELDS, true);
        
    if(txnType != tt_DEBIT_SALE && txnType != tt_DEBIT_RETURN && txnType != tt_DEBIT_REVERSAL) {
        HeartlandPackProcessEx(startNode, hsoapData, ST_DATA_TYPE_TagData, TAG_NAME_TAG_DATA, true);
    }
    return d_HSOAP_SUCCESS;
}

SoapElementRef HeartlandSoap::HeartlandPackProcessEx(SoapElementRef node, void *pData, int dataType, string newNodeTagName, bool checkData) {
    if (newNodeTagName.size() > 0) {
        SoapElementRef newNode = node.addSubElement(newNodeTagName);
        HeartlandPackProcess(newNode, pData, dataType);
        HeartlandPackProcessRemove(checkData, node, newNode);
        return newNode;
    } else {
        HeartlandPackProcess(node, pData, dataType);
        return node;
    }
}

void HeartlandSoap::HeartlandPackProcess(SoapElementRef node, void *pData, int dataType) {
    ST_HSOAP_REQUEST *hsoapData;
    if (pData == NULL) {
        return;
    }
    
    hsoapData = (ST_HSOAP_REQUEST*) pData;
    switch (dataType) {
        case ST_DATA_TYPE_HlData:
            HeartlandPackElement_HlData(node, hsoapData);
            break;
        case ST_DATA_TYPE_CardData:
            HeartlandPackElement_CardData(node, &hsoapData->cCardData, hsoapData->nTransactionInterface);
            break;
        case ST_DATA_TYPE_ManualEntryData:
            if (hsoapData->cCardData.nChoice == HSOAP_MANUAL_ENTRY) {
                HeartlandPackElement_ManualEntryData(node, &hsoapData->cCardData.cManualEntry);
            }
            break;
        case ST_DATA_TYPE_TokenData:
            if (hsoapData->cCardData.nChoice == HSOAP_TOKEN_DATA) {
                HeartlandPackElement_TokenData(node, &hsoapData->cCardData.cTokenData);
            }
            break;
        case ST_DATA_TYPE_EncryptionData:
            HeartlandPackElement_EncryptionData(node, &hsoapData->cCardData.cEncrypData);
            break;
        case ST_DATA_TYPE_TokenParametersData:
            HeartlandPackElement_TokenParametersData(node, &hsoapData->cCardData.cTokenPara);
            break;
        case ST_DATA_TYPE_CardHolderData:
            HeartlandPackElement_CardHolderData(node, &hsoapData->cCardHolderData);
            break;
        case ST_DATA_TYPE_DirectMktData:
            HeartlandPackElement_DirectMktData(node, &hsoapData->cDirMktData);
            break;
        case ST_DATA_TYPE_LodgingData:
            HeartlandPackElement_LodgingData(node, &hsoapData->cLodgingData);
            break;
        case ST_DATA_TYPE_LodgingDataEdit:
            HeartlandPackElement_LodgingDataEdit(node, &hsoapData->cLodgingData.cLodgingDataEdit);
            break;
        case ST_DATA_TYPE_ExtraCharges:
            HeartlandPackElement_ExtraCharges(node, &hsoapData->cLodgingData.cLodgingDataEdit.cExtraCharge);
            break;
        case ST_DATA_TYPE_AutoSubstantiation:
            HeartlandPackElement_AutoSubstantiation(node, &hsoapData->cAutoSubstantiation);
            break;
        case ST_DATA_TYPE_AllAdditionalAmtInfo:
            HeartlandPackElement_AllAdditionalAmtInfo(node, (ST_HSOAP_REQUEST_AMTCOMM *) pData);
            break;
        case ST_DATA_TYPE_AddTxnFields:
            HeartlandPackElement_AddTxnFields(node, &hsoapData->cAddTxnFields);
            break;
        case ST_DATA_TYPE_OrigTxnRefData:
            HeartlandPackElement_OrigTxnRefData(node, &hsoapData->cOrigTxnRefData);
            break;
        case ST_DATA_TYPE_EmvData:
            HeartlandPackElement_EmvData(node, &hsoapData->cEmvData);
            break;
        case ST_DATA_TYPE_SecureEC:
            HeartlandPackElement_SecureEC(node, &hsoapData->cSecureEC);
            break;
        case ST_DATA_TYPE_TagData:
            HeartlandPackElement_TagData(node, &hsoapData->cTagData);
            break;
        case ST_DATA_TYPE_PaymentMethodKeyData:
            HeartlandPackElement_PayMethodKeyData(node, &hsoapData->cPayMethodKeyData);
            break;
        case ST_DATA_TYPE_PDLRequest:
            HeartlandPackElement_PDLRequest(node, &hsoapData->cPDLRequest);
        case ST_DATA_TYPE_TokenRequest:
            HeartlandPackElement_TokenRequest(node, &hsoapData->cCardData);
            break;
        default:
            // not process.....
            break;
    }
}

void HeartlandSoap::HeartlandPackProcessRemove(bool check, SoapElementRef node, SoapElementRef checkNode) {
    if (check && checkNode.subElementCount() == 0) {
        node.removeSubElement(checkNode);
    }
}

void HeartlandSoap::HeartlandPackElement_HlData(SoapElementRef node, ST_HSOAP_REQUEST *pData) {
    if (pData->nTransactionType == tt_UTILITY_PARAMETER_DOWNLOAD) {
        HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_PDL_BLOCK_REQ, pData->szPDLBlockReq, sizeof(pData->szPDLBlockReq), false);
        return;
    }
    
    if (pData->nTransactionType == tt_DEBIT_SALE || pData->nTransactionType == tt_DEBIT_RETURN || pData->nTransactionType == tt_DEBIT_REVERSAL) {
        HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_TRACK_DATA, pData->szTrackData, sizeof(pData->szTrackData), false);
        
        if (pData->nTransactionType != tt_DEBIT_REVERSAL) {
            HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_PIN_BLOCK, pData->szPinBlock, sizeof(pData->szPinBlock), false);
            HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_TOKEN_VALUE, pData->szTokenValue, sizeof(pData->szTokenValue), false);
        }
        
        if (pData->nTransactionType != tt_DEBIT_RETURN && pData->nTransactionType != tt_DEBIT_REVERSAL) {
            HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CASH_BACK_AMT_INFO, pData->szCashbackAmtInfo, sizeof(pData->szCashbackAmtInfo), false);
        }
    } else {
        HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_GATEWAY_TXN_ID, pData->szGatewayTxnId, sizeof(pData->szGatewayTxnId), false);
    }
    
    if (pData->nTransactionType == tt_DEBIT_RETURN || pData->nTransactionType == tt_DEBIT_REVERSAL) {
        HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_GATEWAY_TXN_ID, pData->szGatewayTxnId, sizeof(pData->szGatewayTxnId), false);
    }
    
    if (pData->nTransactionType == tt_CREDIT_VOID) {
        return;
    }
    
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_AMT, pData->szAmt, sizeof(pData->szAmt), false);
    
    if (pData->nTransactionType == tt_CREDIT_REVERSAL || pData->nTransactionType == tt_DEBIT_REVERSAL) {
        HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_AUTH_AMT, pData->szAuthAmt, sizeof(pData->szAuthAmt), false);
        
        if (pData->nTransactionType != tt_DEBIT_REVERSAL) {
            HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CLIENT_TXN_ID, pData->szClientTxnId, sizeof(pData->szClientTxnId), false);
            HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_EMV_TAG_DATA, pData->szEMVTagData, sizeof(pData->szEMVTagData), false);
        }
    } else {
        if (pData->nTransactionType != tt_CREDIT_VOID && pData->nTransactionType != tt_DEBIT_RETURN) {
            HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_GRATUITY_AMT_INFO, pData->szGratuityAmtInfo, sizeof(pData->szGratuityAmtInfo), false);
        }
        
        if (pData->nTransactionType != tt_DEBIT_SALE && pData->nTransactionType != tt_DEBIT_RETURN) {
            HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_SURCHARGE_AMT_INFO, pData->szSurchargeAmtInfo, sizeof(pData->szSurchargeAmtInfo), false);
        }
        
        if ((pData->nTransactionType == tt_CREDIT_ADD_TO_BATCH) || (pData->nTransactionType == tt_CREDIT_TXN_EDIT)) {
            HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_EMV_TAG_DATA, pData->szEMVTagData, sizeof(pData->szEMVTagData), false);
        } else {
            if (pData->nTransactionType != tt_CREDIT_VOID && pData->nTransactionType != tt_DEBIT_RETURN) {
                if(pData->nTransactionType != tt_DEBIT_SALE) {
                    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CPC_REQ, pData->szCPCReq, sizeof(pData->szCPCReq), false);
                    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CONVENIENCE_AMT_INFO, pData->szConvenienceAmtInfo, sizeof(pData->szConvenienceAmtInfo), false);
                    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_SHIPPING_AMT_INFO, pData->szShippingAmtInfo, sizeof(pData->szShippingAmtInfo), false);
                    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_TXN_DESCRIPTOR, pData->szTxnDescriptor, sizeof(pData->szTxnDescriptor), false);
                }
                HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_ALLOW_PARTIAL_AUTH, pData->szAllowPartialAuth, sizeof(pData->szAllowPartialAuth), false);
            }
            HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_ALLOW_DUP, pData->szAllowDup, sizeof(pData->szAllowDup), false);
        }
    }
    
    if (pData->nTransactionType == tt_DEBIT_RETURN || pData->nTransactionType == tt_DEBIT_REVERSAL) {
        return;
    }

    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_ECOMMERCE, pData->szEcommerce, sizeof(pData->szEcommerce), false);
    
    if (pData->nTransactionType == tt_CREDIT_AUTH) {
        HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_PAYMENT_METHOD_KEY, pData->szPaymentMethodKey, sizeof(pData->szPaymentMethodKey), false);
    }
}

void HeartlandSoap::HeartlandPackElement_CardData(SoapElementRef node, ST_HSOAP_REQUEST_CARDDATA *pData, int nTransactionInterface) {
    string value = METHOD_VALUE_SWIPE;
    if (nTransactionInterface != ti_SWIPE) {
        value = METHOD_VALUE_PROXIMITY;
    }
    if (pData->nChoice == HSOAP_TRACK_DATA) {
        HeartlandAddSubElement(node, ATTRIBUTE_NAME_METHOD, value, TAG_NAME_TRACK_DATA, pData->szTrackData, sizeof(pData->szTrackData), false);
    }
    //HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_TOKEN_REQUEST, pData->szTokenRequest, sizeof(pData->szTokenRequest), false);
}

void HeartlandSoap::HeartlandPackElement_ManualEntryData(SoapElementRef node, ST_HSOAP_REQUEST_MANUALENTRY *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CARD_NBR, pData->szCardNbr, sizeof(pData->szCardNbr), false);
    HeartlandPackElement_CommonData(node, &pData->cCommData);
}

void HeartlandSoap::HeartlandPackElement_TokenData(SoapElementRef node, ST_HSOAP_REQUEST_TOKENDATA *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_TOKEN_VALUE, pData->szTokenValue, sizeof(pData->szTokenValue), false);
    HeartlandPackElement_CommonData(node, &pData->cCommData);
}

void HeartlandSoap::HeartlandPackElement_EncryptionData(SoapElementRef node, ST_HSOAP_REQUEST_ENCRYPTIONDATA *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_VERSION, pData->szVersion, sizeof(pData->szVersion), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_ENCRYPTED_TRACK_NUMBER, pData->szEncryptedTrackNumber, sizeof(pData->szEncryptedTrackNumber), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_KTB, pData->szKTB, sizeof(pData->szKTB), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_KSN, pData->szKSN, sizeof(pData->szKSN), false);
}

void HeartlandSoap::HeartlandPackElement_TokenParametersData(SoapElementRef node, ST_HSOAP_REQUEST_TOKENPARAMETERS *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_MAPPING, pData->szMapping, sizeof(pData->szMapping), false);
}

void HeartlandSoap::HeartlandPackElement_CardHolderData(SoapElementRef node, ST_HSOAP_REQUEST_CARDHOLDERDATA *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CARD_HOLDER_FISRT_NAME, pData->szCardHolderFirstName, sizeof(pData->szCardHolderFirstName), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CARD_HOLDER_LAST_NAME, pData->szCardHolderLastName, sizeof(pData->szCardHolderLastName), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CARD_HOLDER_ADDR, pData->szCardHolderAddr, sizeof(pData->szCardHolderAddr), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CARD_HOLDER_CITY, pData->szCardHolderCity, sizeof(pData->szCardHolderCity), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CARD_HOLDER_STATE, pData->szCardHolderState, sizeof(pData->szCardHolderState), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CARD_HOLDER_ZIP, pData->szCardHolderZip, sizeof(pData->szCardHolderZip), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CARD_HOLDER_PHONE, pData->szCardHolderPhone, sizeof(pData->szCardHolderPhone), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CARD_HOLDER_EMAIL, pData->szCardHolderEmail, sizeof(pData->szCardHolderEmail), false);
}

void HeartlandSoap::HeartlandPackElement_DirectMktData(SoapElementRef node, ST_HSOAP_REQUEST_DIRECTMKTDATA *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_DIRECT_MKT_INVOICE_NBR, pData->szDirectMktInvoiceNbr, sizeof(pData->szDirectMktInvoiceNbr), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_DIRECT_MKT_SHIP_MONTH, pData->szDirectMktShipMonth, sizeof(pData->szDirectMktShipMonth), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_DIRECT_MKT_SHIP_DAY, pData->szDirectMktShipDay, sizeof(pData->szDirectMktShipDay), false);
}

void HeartlandSoap::HeartlandPackElement_LodgingData(SoapElementRef node, ST_HSOAP_EQUEST_LODGINGDATA *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_PRESTIGIOUS_PROPERTY_LIMIT, pData->szPrestigiousPropertyLimit, sizeof(pData->szPrestigiousPropertyLimit), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_NO_SHOW, pData->szNoShow, sizeof(pData->szNoShow), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_ADVANCE_DEPOSIT_TYPE, pData->szAdvancedDepositType, sizeof(pData->szAdvancedDepositType), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_PREFERRED_CUSTOMER, pData->szPreferredCustomer, sizeof(pData->szPreferredCustomer), false);
}

void HeartlandSoap::HeartlandPackElement_LodgingDataEdit(SoapElementRef node, ST_HSOAP_REQUEST_LODGINGDATAEDIT *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_FOLIO_NUMBER, pData->FolioNumber, sizeof(pData->FolioNumber), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_DURATION, pData->Duration, sizeof(pData->Duration), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CHECK_IN_DATE, pData->CheckInDate, sizeof(pData->CheckInDate), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CHECK_OUT_DATE, pData->CheckOutDate, sizeof(pData->CheckOutDate), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_RATE, pData->Rate, sizeof(pData->Rate), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_EXTRA_CHARGE_AMT_INFO, pData->szExtraChargeAmtInfo, sizeof(pData->szExtraChargeAmtInfo), false);
}

void HeartlandSoap::HeartlandPackElement_ExtraCharges(SoapElementRef node, ST_HSOAP_REQUEST_EXTRACHARGES *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_RESTAURANT, pData->szRestaurant, sizeof(pData->szRestaurant), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_GIFT_SHOP, pData->szGiftShop, sizeof(pData->szGiftShop), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_MINI_BAR, pData->szMiniBar, sizeof(pData->szMiniBar), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_TELEPHONE, pData->szTelephone, sizeof(pData->szTelephone), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_OTHER, pData->szOther, sizeof(pData->szOther), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_LAUNDRY, pData->szLaundry, sizeof(pData->szLaundry), false);
}

void HeartlandSoap::HeartlandPackElement_AutoSubstantiation(SoapElementRef node, ST_HSOAP_REQUEST_AUTOSUBSTANTIATION *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_MERCHANT_VERIFICATION_VALUE, pData->szMerchantVerificationValue, sizeof(pData->szMerchantVerificationValue), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_REAL_TIME_SUBSTANTIATION, pData->szRealTimeSubstantiation, sizeof(pData->szRealTimeSubstantiation), false);
}

void HeartlandSoap::HeartlandPackElement_AllAdditionalAmtInfo(SoapElementRef node, ST_HSOAP_REQUEST_AMTCOMM *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_AMT_TYPE, pData->szAmtType, sizeof(pData->szAmtType), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_AMT, pData->szAmt, sizeof(pData->szAmt), false);
}

void HeartlandSoap::HeartlandPackElement_AddTxnFields(SoapElementRef node, ST_HSOAP_REQUEST_ADDITIONALTXNFIELDS *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_DESCRIPTION, pData->szDescription, sizeof(pData->szDescription), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_INVOICE_NBR, pData->szInvoiceNbr, sizeof(pData->szInvoiceNbr), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CUSTOMER_ID, pData->szCustomerID, sizeof(pData->szCustomerID), false);
}

void HeartlandSoap::HeartlandPackElement_OrigTxnRefData(SoapElementRef node, ST_HSOAP_REQUEST_ORIGTXNREFDATA *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_AUTH_CODE, pData->szAuthCode, sizeof(pData->szAuthCode), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CARD_NBR_LAST_FOUR, pData->szCardNbrLastFour, sizeof(pData->szCardNbrLastFour), false);
}

void HeartlandSoap::HeartlandPackElement_EmvData(SoapElementRef node, ST_HSOAP_REQUEST_EMVDATA *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_EMV_TAG_DATA, pData->szEMVTagData, sizeof(pData->szEMVTagData), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_EMV_CHIP_CONDITION, pData->szEMVChipCondition, sizeof(pData->szEMVChipCondition), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_EMV_DATA_PIN_BLOCK, pData->szPINBlock, sizeof(pData->szPINBlock), false);
}

void HeartlandSoap::HeartlandPackElement_SecureEC(SoapElementRef node, ST_HSOAP_REQUEST_SECUREECOMMERCE *pData) {
    string encodeType = NULL_STRING;
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_PAYMENT_DATA_SOURCE, pData->szPaymentDataSource, sizeof(pData->szPaymentDataSource), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_TYPE_OF_PAYMENT_DATA, pData->szTypeOfPaymentData, sizeof(pData->szTypeOfPaymentData), false);
    
    encodeType = ENCODING_VALUE_BASE64;
    if (pData->nPaymentDataEncode == HSOAP_ENCODE_BASE16) {
        encodeType = ENCODING_VALUE_BASE16;
    }
    HeartlandAddSubElement(node, ATTRIBUTE_NAME_ENCODING, encodeType, TAG_NAME_PAYMENT_DATA, pData->szPaymentData, sizeof(pData->szPaymentData), false);
    
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_ECOMMERCE_INDICATOR, pData->szECommerceIndicator, sizeof(pData->szECommerceIndicator), false);
    
    encodeType = ENCODING_VALUE_BASE64;
    if (pData->nXidDataEncode == HSOAP_ENCODE_BASE16) {
        encodeType = ENCODING_VALUE_BASE16;
    }
    HeartlandAddSubElement(node, ATTRIBUTE_NAME_ENCODING, encodeType, TAG_NAME_XID, pData->szXID, sizeof(pData->szXID), false);
}

void HeartlandSoap::HeartlandPackElement_TagData(SoapElementRef node, ST_HSOAP_REQUEST_TAGDATA *pData) {
    string valueType = SOURCE_VALUE_CIP;
    if (pData->nTagValueType == HSOAP_TAG_VALUE_TYPE_MSD) {
        valueType = SOURCE_VALUE_MSD;
    }
    HeartlandAddSubElement(node, ATTRIBUTE_NAME_SOURCE, valueType, TAG_NAME_TAG_VALUES, pData->szTagValues, sizeof(pData->szTagValues), false);
}

void HeartlandSoap::HeartlandPackElement_PayMethodKeyData(SoapElementRef node, ST_HSOAP_REQUEST_PAYMENTMETHODKEYDATA *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_EXP_MONTH, pData->szExpMonth, sizeof(pData->szExpMonth), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_EXP_YAER, pData->szExpYear, sizeof(pData->szExpYear), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CVV2, pData->szCVV2, sizeof(pData->szCVV2), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CVV2_STATUS, pData->szCVV2Status, sizeof(pData->szCVV2Status), false);
}

void HeartlandSoap::HeartlandPackElement_PDLRequest(SoapElementRef node, ST_HSOAP_REQUEST_PDLREQUEST *pData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_PARAM_TYPE, pData->szParamType, sizeof(pData->szParamType), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_TABLE_ID, pData->szTableID, sizeof(pData->szTableID), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CARD_TYPE, pData->szCardType, sizeof(pData->szCardType), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_VERSION, pData->szVersion, sizeof(pData->szVersion), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_BLOCK_SEQUENCE, pData->szBlockSequence, sizeof(pData->szBlockSequence), false);
}

void HeartlandSoap::HeartlandPackElement_TokenRequest(SoapElementRef node, ST_HSOAP_REQUEST_CARDDATA *pData)
{
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_TOKEN_REQUEST, pData->szTokenRequest, sizeof(pData->szTokenRequest), false);
}

void HeartlandSoap::HeartlandPackElement_CommonData(SoapElementRef node, ST_HSOAP_REQUEST_COMMONDATA *cCommData) {
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_EXP_MONTH, cCommData->szExpMonth, sizeof(cCommData->szExpMonth), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_EXP_YAER, cCommData->szExpYear, sizeof(cCommData->szExpYear), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CARD_PRESENT, cCommData->szCardPresent, sizeof(cCommData->szCardPresent), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_READER_PRESENT, cCommData->szReaderPresent, sizeof(cCommData->szReaderPresent), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CVV2, cCommData->szCVV2, sizeof(cCommData->szCVV2), false);
    HeartlandAddSubElement(node, NULL_STRING, NULL_STRING, TAG_NAME_CVV2_STATUS, cCommData->szCVV2Status, sizeof(cCommData->szCVV2Status), false);
}

void HeartlandSoap::HeartlandUnpackHeader(HeartLandResponse response, ST_HSOAP_RESPONSE *hsoapData) {
    HeartlandParseResponse("LicenseId", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szLicenseId, sizeof(hsoapData->cGateway.szLicenseId));
    HeartlandParseResponse("SiteId", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szSiteId, sizeof(hsoapData->cGateway.szSiteId));
    HeartlandParseResponse("DeviceId", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szDeviceId, sizeof(hsoapData->cGateway.szDeviceId));
    HeartlandParseResponse("SiteTrace", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szSiteTrace, sizeof(hsoapData->cGateway.szSiteTrace));
    HeartlandParseResponse("GatewayTxnId", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szGatewayTxnId, sizeof(hsoapData->cGateway.szGatewayTxnId));
    HeartlandParseResponse("GatewayRspCode", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szGatewayRspCode, sizeof(hsoapData->cGateway.szGatewayRspCode));
    HeartlandParseResponse("GatewayRspMsg", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szGatewayRspMsg, sizeof(hsoapData->cGateway.szGatewayRspMsg));
    HeartlandParseResponse("RspDT", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szRspDT, sizeof(hsoapData->cGateway.szRspDT));
    HeartlandParseResponse("ClientTxnId", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szClientTxnId, sizeof(hsoapData->cGateway.szClientTxnId));
    HeartlandParseResponse("UniqueDeviceId", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szUniqueDeviceId, sizeof(hsoapData->cGateway.szUniqueDeviceId));
    HeartlandParseResponse("BatchId", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szBatchId, sizeof(hsoapData->cGateway.szBatchId));
    HeartlandParseResponse("BatchSeqNbr", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szBatchSeqNbr, sizeof(hsoapData->cGateway.szBatchSeqNbr));
    HeartlandParseResponse("PDLNotification", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szPDLNotification, sizeof(hsoapData->cGateway.szPDLNotification));
    
    HeartlandParseResponse("TokenData", "TokenRspCode", NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szTokenRspCode, sizeof(hsoapData->cGateway.szTokenRspCode));
    HeartlandParseResponse("TokenData", "TokenRspMsg", NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szTokenRspMsg, sizeof(hsoapData->cGateway.szTokenRspMsg));
    HeartlandParseResponse("TokenData", "TokenValue", NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_HEADER, response, hsoapData->cGateway.szTokenValue, sizeof(hsoapData->cGateway.szTokenValue));
}

void HeartlandSoap::HeartlandUnpackTransaction(HeartLandResponse response, ST_HSOAP_RESPONSE *hsoapData) {
    HeartlandParseResponse("RspCode", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szRspCode, sizeof(hsoapData->cTrans.szRspCode));
    HeartlandParseResponse("RspText", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szRspText, sizeof(hsoapData->cTrans.szRspText));
    HeartlandParseResponse("AuthCode", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szAuthCode, sizeof(hsoapData->cTrans.szAuthCode));
    HeartlandParseResponse("AVSRsltCode", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szAVSRsltCode, sizeof(hsoapData->cTrans.szAVSRsltCode));
    HeartlandParseResponse("CVVRsltCode", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szCVVRsltCode, sizeof(hsoapData->cTrans.szCVVRsltCode));
    HeartlandParseResponse("CPCInd", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szCPCInd, sizeof(hsoapData->cTrans.szCPCInd));
    HeartlandParseResponse("RefNbr", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szRefNbr, sizeof(hsoapData->cTrans.szRefNbr));
    HeartlandParseResponse("AvailableBalance", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szAvailableBalance, sizeof(hsoapData->cTrans.szAvailableBalance));
    HeartlandParseResponse("AuthAmt", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szAuthAmt, sizeof(hsoapData->cTrans.szAuthAmt));
    HeartlandParseResponse("AVSResultCodeAction", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szAVSResultCodeAction, sizeof(hsoapData->cTrans.szAVSResultCodeAction));
    HeartlandParseResponse("CVVResultCodeAction", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szCVVResultCodeAction, sizeof(hsoapData->cTrans.szCVVResultCodeAction));
    HeartlandParseResponse("CardType", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szCardType, sizeof(hsoapData->cTrans.szCardType));
    HeartlandParseResponse("AVSRsltText", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szAVSRsltText, sizeof(hsoapData->cTrans.szAVSRsltText));
    HeartlandParseResponse("CVVRsltText", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szCVVRsltText, sizeof(hsoapData->cTrans.szCVVRsltText));
    HeartlandParseResponse("TxnDescriptor", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTxnDescriptor, sizeof(hsoapData->cTrans.szTxnDescriptor));
    HeartlandParseResponse("RecurringDataCode", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szRecurringDataCode, sizeof(hsoapData->cTrans.szRecurringDataCode));
    HeartlandParseResponse("EMVIssuerResp", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szEMVIssuerResp, sizeof(hsoapData->cTrans.szEMVIssuerResp));
    HeartlandParseResponse("CAVVResultCode", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szCAVVResultCode, sizeof(hsoapData->cTrans.szCAVVResultCode));
    HeartlandParseResponse("TokenPANLast4", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTokenPANLast4, sizeof(hsoapData->cTrans.szTokenPANLast4));
    
    HeartlandParseResponse("BatchId", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szBatchId, sizeof(hsoapData->cTrans.szBatchId));
    HeartlandParseResponse("TxnCnt", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTxnCnt, sizeof(hsoapData->cTrans.szTxnCnt));
    HeartlandParseResponse("TotalAmt", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTotalAmt, sizeof(hsoapData->cTrans.szTotalAmt));
    HeartlandParseResponse("BatchSeqNbr", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szBatchSeqNbr, sizeof(hsoapData->cTrans.szBatchSeqNbr));
    
    HeartlandParseResponse("PDLBlockRsp", NULL_STRING, NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szPDLBlockRsp, sizeof(hsoapData->cTrans.szPDLBlockRsp));
    
    HeartlandParseResponse("PDLResponse", "Version", NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szVersion, sizeof(hsoapData->cTrans.szVersion));
    HeartlandParseResponse("PDLResponse", "BlockSequence", NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szBlockSequence, sizeof(hsoapData->cTrans.szBlockSequence));
    HeartlandParseResponse("PDLResponse", "TableID", NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTableID, sizeof(hsoapData->cTrans.szTableID));
    HeartlandParseResponse("PDLResponse", "CardType", NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szPdlCardType, sizeof(hsoapData->cTrans.szPdlCardType));
    HeartlandParseResponse("PDLResponse", "EndOfTableFlag", NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szEndOfTableFlag, sizeof(hsoapData->cTrans.szEndOfTableFlag));
    HeartlandParseResponse("PDLResponse", "ConfirmationFlag", NULL_STRING, NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szConfirmationFlag, sizeof(hsoapData->cTrans.szConfirmationFlag));
    HeartlandParseResponse("PDLResponse", "TableData", "TableBlock", NULL_STRING, NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTableBlock, sizeof(hsoapData->cTrans.szTableBlock));
    HeartlandParseResponse("PDLResponse", "TableData", "Table10", "EMVEnabled", NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szEMVEnabled, sizeof(hsoapData->cTrans.szEMVEnabled));
    HeartlandParseResponse("PDLResponse", "TableData", "Table10", "Table30Version", NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTable30Version, sizeof(hsoapData->cTrans.szTable30Version));
    HeartlandParseResponse("PDLResponse", "TableData", "Table10", "Table30Flag", NULL_STRING, PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTable30Flag, sizeof(hsoapData->cTrans.szTable30Flag));
    HeartlandParseResponse("PDLResponse", "TableData", "Table10", "CardTypes", "CardType", PARSE_TYPE_TRANS, response, hsoapData->cTrans.szPdlCardTypes, sizeof(hsoapData->cTrans.szPdlCardTypes));
    HeartlandParseResponse("PDLResponse", "TableData", "Table10", "CardTypes", "Table40Version", PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTable40Version, sizeof(hsoapData->cTrans.szTable40Version));
    HeartlandParseResponse("PDLResponse", "TableData", "Table10", "CardTypes", "Table40Flag", PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTable40Flag, sizeof(hsoapData->cTrans.szTable40Flag));
    HeartlandParseResponse("PDLResponse", "TableData", "Table10", "CardTypes", "Table50Version", PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTable50Version, sizeof(hsoapData->cTrans.szTable50Version));
    HeartlandParseResponse("PDLResponse", "TableData", "Table10", "CardTypes", "Table50Flag", PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTable50Flag, sizeof(hsoapData->cTrans.szTable50Flag));
    HeartlandParseResponse("PDLResponse", "TableData", "Table10", "CardTypes", "Table60Version", PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTable60Version, sizeof(hsoapData->cTrans.szTable60Version));
    HeartlandParseResponse("PDLResponse", "TableData", "Table10", "CardTypes", "Table60Flag", PARSE_TYPE_TRANS, response, hsoapData->cTrans.szTable60Flag, sizeof(hsoapData->cTrans.szTable60Flag));
}

void HeartlandSoap::HeartlandParseResponse(string tag1, string tag2, string tag3, string tag4, string tag5, int parseType, HeartLandResponse response, char *pResponse, int responseBufLen) {
    string tmpStr;
    int tmpStrLength;
    
    if (pResponse != NULL && responseBufLen > 0 && tag1.size() > 0) {
        switch (parseType) {
            case PARSE_TYPE_HEADER:
                if (tag2.size() > 0) {
                    tmpStr = response.header[tag1][tag2];
                } else {
                    tmpStr = response.header[tag1];
                }
                break;
            case PARSE_TYPE_TRANS:
                if (tag2.size() > 0) {
                    if (tag3.size() > 0) {
                        if (tag4.size() > 0) {
                            if (tag5.size() > 0) {
                                tmpStr = response.transactionResult[tag1][tag2][tag3][tag4][tag5];
                            } else {
                                tmpStr = response.transactionResult[tag1][tag2][tag3][tag4];
                            }
                        } else {
                            tmpStr = response.transactionResult[tag1][tag2][tag3];
                        }
                    } else {
                        tmpStr = response.transactionResult[tag1][tag2];
                    }
                } else {
                    tmpStr = response.transactionResult[tag1];
                }
                break;
            default:
                return;
        }
        
        tmpStrLength = tmpStr.size();
        if (tmpStrLength > (responseBufLen - 1)) {
            tmpStrLength = (responseBufLen - 1);
        }
        
        if (tmpStrLength > 0) {
            memcpy(pResponse, tmpStr.c_str(), tmpStrLength);
        }
        pResponse[tmpStrLength] = '\0';
    }
}

bool HeartlandSoap::HeartlandHeaderDataLengthCheck(int dataLen, int dataMaxLen) {
    if (dataLen != 0 && dataMaxLen > dataLen) {
        return true; // ok
    }
    return false; // fail
}

bool HeartlandSoap::HeartlandAddSubElement(SoapElementRef ref, string attributeName, string attributeValue, string tagName, char *value, int valueMaxLen, bool check) {
    bool isOk = HeartlandHeaderDataLengthCheck(strlen(value), valueMaxLen);
    
    if (check && !isOk) {
        return true; // fail
    }
    if (isOk) {
        SoapElementRef attributeRef = ref.addSubElement(tagName, value);
        
        if(attributeName.size() > 0)
            attributeRef.addAttribute(attributeName, attributeValue);
    }
    return false; // ok
}


extern bool gDebugEnabled;
void HeartlandDebugMessage(const char* pszFileName, const int nLineNum, const char * pFormat, ...)
{                                                                       
    if (!gDebugEnabled)                                                  
        return ;
    
    time_t t;                                                       
    time (&t);                                                      
    struct tm *lt = localtime (&t);
    
    va_list argList;
    va_start(argList, pFormat);
    
    int nLen = vsnprintf(NULL, 0, pFormat, argList) + 1;
    char *pszBuf = (char *) calloc(1, nLen);
    if (NULL == pszBuf)
        return;

    vsprintf(pszBuf, pFormat, argList);
    va_end(argList);

    printf("%02d:%02d:%02d %s-%05d, %s\n", 
           lt->tm_hour, lt->tm_min, lt->tm_sec, 
           pszFileName, nLineNum, pszBuf);     

    free(pszBuf);
    pszBuf = NULL;
}