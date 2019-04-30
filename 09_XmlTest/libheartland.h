/* 
 * File:   libheartland.h
 * Author: Michelin
 *
 * Created on 2017.06.22
 * 
 * Modified by: Tim Ma
 * Modified at: 2019.03.06
 */

#ifndef __HEARTLAND__LIBHEARTLAND_H__
#define __HEARTLAND__LIBHEARTLAND_H__

#include "HeartLand.hpp"
#include "libstructdefined.h"


class HeartlandSoap
{
public:
    virtual void HeartlandSOAP_DebugEnable(void);
    virtual void HeartlandSOAP_DebugDisable(void);
    virtual unsigned short HeartlandSOAP_GetVersion(char *pcVersion, int *piLength);
    virtual unsigned short HeartlandSOAP_Init(ST_HSOAP_INIT *hsoapData);
    virtual unsigned short HeartlandSOAP_Pack(ST_HSOAP_REQUEST *hsoapData, int *pDataLen, char *pData);
    virtual unsigned short HeartlandSOAP_Unpack(ST_HSOAP_RESPONSE *hsoapData, char *pData);
    virtual unsigned short HeartlandSOAP_SetHeaderOptionalFields(unsigned char ucFieldType, unsigned char *pucData, unsigned short usDataLen);
    virtual unsigned short HeartlandSOAP_ResetHeaderOptionalFields(void);


    virtual unsigned short HeartlandPackHeader(SoapElementRef root, ST_HSOAP_INIT *hsoapHeaderData);
    virtual unsigned short HeartlandPackHeaderMandatory(SoapElementRef header, ST_HSOAP_INIT *hsoapHeaderData);
    virtual void HeartlandPackHeaderOptional(SoapElementRef header, ST_HSOAP_INIT *hsoapHeaderData);

    virtual unsigned short HeartlandPackTransaction(SoapElementRef root, ST_HSOAP_REQUEST *hsoapData);
    virtual SoapElementRef HeartlandPackTransType(SoapElementRef root, int transactionType);

    virtual unsigned short HeartlandPackProcessEntryEx(SoapElementRef startNode, ST_HSOAP_REQUEST *hsoapData, bool enableBolck1Node);
    virtual unsigned short HeartlandPackProcessEntry(SoapElementRef startNode, ST_HSOAP_REQUEST *hsoapData);

    // if newNodeTagName size > 0 return newNode, other return node
    // if checkData = true, will remove null element data
    virtual SoapElementRef HeartlandPackProcessEx(SoapElementRef node, void *pData, int dataType, string newNodeTagName, bool checkData);
     
    virtual void HeartlandPackProcess(SoapElementRef node, void *pData, int dataType);
    virtual void HeartlandPackProcessRemove(bool check, SoapElementRef node, SoapElementRef checkNode);
    virtual void HeartlandPackElement_HlData(SoapElementRef node, ST_HSOAP_REQUEST *pData);
    virtual void HeartlandPackElement_CardData(SoapElementRef node, ST_HSOAP_REQUEST_CARDDATA *pData, int nTransactionInterface);
    virtual void HeartlandPackElement_ManualEntryData(SoapElementRef node, ST_HSOAP_REQUEST_MANUALENTRY *pData);
    virtual void HeartlandPackElement_TokenData(SoapElementRef node, ST_HSOAP_REQUEST_TOKENDATA *pData);
    virtual void HeartlandPackElement_EncryptionData(SoapElementRef node, ST_HSOAP_REQUEST_ENCRYPTIONDATA *pData);
    virtual void HeartlandPackElement_TokenParametersData(SoapElementRef node, ST_HSOAP_REQUEST_TOKENPARAMETERS *pData);
    virtual void HeartlandPackElement_CardHolderData(SoapElementRef node, ST_HSOAP_REQUEST_CARDHOLDERDATA *pData);
    virtual void HeartlandPackElement_DirectMktData(SoapElementRef node, ST_HSOAP_REQUEST_DIRECTMKTDATA *pData);
    virtual void HeartlandPackElement_LodgingData(SoapElementRef node, ST_HSOAP_EQUEST_LODGINGDATA *pData);
    virtual void HeartlandPackElement_LodgingDataEdit(SoapElementRef node, ST_HSOAP_REQUEST_LODGINGDATAEDIT *pData);
    virtual void HeartlandPackElement_ExtraCharges(SoapElementRef node, ST_HSOAP_REQUEST_EXTRACHARGES *pData);
    virtual void HeartlandPackElement_AutoSubstantiation(SoapElementRef node, ST_HSOAP_REQUEST_AUTOSUBSTANTIATION *pData);
    virtual void HeartlandPackElement_AllAdditionalAmtInfo(SoapElementRef node, ST_HSOAP_REQUEST_AMTCOMM *pData);
    virtual void HeartlandPackElement_AddTxnFields(SoapElementRef node, ST_HSOAP_REQUEST_ADDITIONALTXNFIELDS *pData);
    virtual void HeartlandPackElement_OrigTxnRefData(SoapElementRef node, ST_HSOAP_REQUEST_ORIGTXNREFDATA *pData);
    virtual void HeartlandPackElement_EmvData(SoapElementRef node, ST_HSOAP_REQUEST_EMVDATA *pData);
    virtual void HeartlandPackElement_SecureEC(SoapElementRef node, ST_HSOAP_REQUEST_SECUREECOMMERCE *pData);
    virtual void HeartlandPackElement_TagData(SoapElementRef node, ST_HSOAP_REQUEST_TAGDATA *pData);
    virtual void HeartlandPackElement_PayMethodKeyData(SoapElementRef node, ST_HSOAP_REQUEST_PAYMENTMETHODKEYDATA *pData);
    virtual void HeartlandPackElement_PDLRequest(SoapElementRef node, ST_HSOAP_REQUEST_PDLREQUEST *pData);
    virtual void HeartlandPackElement_TokenRequest(SoapElementRef node, ST_HSOAP_REQUEST_CARDDATA *pData);
    virtual void HeartlandPackElement_CommonData(SoapElementRef node, ST_HSOAP_REQUEST_COMMONDATA *cCommData);
    
    virtual void HeartlandUnpackHeader(HeartLandResponse response, ST_HSOAP_RESPONSE *hsoapData);
    virtual void HeartlandUnpackTransaction(HeartLandResponse response, ST_HSOAP_RESPONSE *hsoapData);
    virtual void HeartlandParseResponse(string tag1, string tag2, string tag3, string tag4, string tag5, int parseType, HeartLandResponse response, char *pResponse, int responseBufLen);
    
    virtual bool HeartlandHeaderDataLengthCheck(int dataLen, int dataMaxLen);
    virtual bool HeartlandAddSubElement(SoapElementRef ref, string attributeName, string attributeValue, string tagName, char *value, int valueMaxLen, bool check);
};


extern "C" void HeartlandDebugMessage(const char* pszFileName, const int nLineNum, const char * pFormat, ...);
#define HLND_SOAP_DEBUG_MSG(fmt, ...) HeartlandDebugMessage(__FILE__, __LINE__, fmt, ## __VA_ARGS__);


#endif    /* LIBHEARTLAND_H */

