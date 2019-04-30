/* 
 * File:   libstructdefined.h
 * Author: Michelin
 *
 * Created on 2017.06.29
 */

#ifndef __HEARTLAND_STRUCT_DEFINED_H__
#define __HEARTLAND_STRUCT_DEFINED_H__

#ifdef	__cplusplus
extern "C" {
#endif

    
#define TAG_NAME_CREDENTIAL_TOKEN               TAG_NAME_PREV"CredentialToken"
#define TAG_NAME_UNIQUE_DEVICE_ID               TAG_NAME_PREV"UniqueDeviceId"
#define TAG_NAME_SECRET_API_KEY                 TAG_NAME_PREV"SecretAPIKey"

#define TAG_NAME_GPS_COORDINATES                TAG_NAME_PREV"GPSCoordinates"
#define TAG_NAME_LATITUDE                       TAG_NAME_PREV"Latitude"
#define TAG_NAME_LONGITUDE                      TAG_NAME_PREV"Longitude"

#define TAG_NAME_DEBIT_SALE                     TAG_NAME_PREV"DebitSale"
#define TAG_NAME_DEBIT_REVERSAL                 TAG_NAME_PREV"DebitReversal"
#define TAG_NAME_CREDIT_VOID                    TAG_NAME_PREV"CreditVoid"
#define TAG_NAME_CREDIT_RETURN                  TAG_NAME_PREV"CreditReturn"
#define TAG_NAME_DEBIT_RETURN                   TAG_NAME_PREV"DebitReturn"


//#define TAG_NAME_TOKEN_MAPPING                  TAG_NAME_PREV"TokenMapping"
#define TAG_NAME_MAPPING                        TAG_NAME_PREV"Mapping"

#define TAG_NAME_RESTAURANT                     TAG_NAME_PREV"Restaurant"
#define TAG_NAME_GIFT_SHOP                      TAG_NAME_PREV"GiftShop"
#define TAG_NAME_MINI_BAR                       TAG_NAME_PREV"MiniBar"
#define TAG_NAME_TELEPHONE                      TAG_NAME_PREV"Telephone"
#define TAG_NAME_OTHER                          TAG_NAME_PREV"Other"
#define TAG_NAME_LAUNDRY                        TAG_NAME_PREV"Laundry"

#define TAG_NAME_PDL_REQUEST                    TAG_NAME_PREV"PDLRequest"
#define TAG_NAME_PDL_BLOCK_REQ                  TAG_NAME_PREV"PDLBlockReq"

#define TAG_NAME_PARAM_TYPE                     TAG_NAME_PREV"ParamType"
#define TAG_NAME_TABLE_ID                       TAG_NAME_PREV"TableID"
#define TAG_NAME_BLOCK_SEQUENCE                 TAG_NAME_PREV"BlockSequence"


/**
 * Error Code
**/
#define d_HSOAP_SUCCESS         0x0000
#define d_HSOAP_API_FAIL        0xAE00
#define d_HSOAP_API_NOT_INIT    0xAE01
#define d_HSOAP_NULL_POINT      0xAE02
#define d_HSOAP_BUF_NOT_ENOUGH  0xAE03
#define d_HSOAP_ERR_PARAMETER   0xAE04
#define d_HSOAP_ERR_DATA_SIZE   0xAE05

/**
 * szOther defined
**/
#define HSOAP_TRACK_DATA                0
#define HSOAP_MANUAL_ENTRY              1
#define HSOAP_TOKEN_DATA                2
#define HSOAP_ENCODE_BASE16             16
#define HSOAP_ENCODE_BASE64             64
#define HSOAP_TAG_VALUE_TYPE_MSD        10
#define HSOAP_TAG_VALUE_TYPE_CHIP       11

/**
 * Transaction type defined
**/
#define tt_TRANS_TYPE_START             100
#define tt_CREDIT_SALE                  (tt_TRANS_TYPE_START + 1)
#define tt_CREDIT_VOID                  (tt_TRANS_TYPE_START + 2)
#define tt_CREDIT_RETURN                (tt_TRANS_TYPE_START + 3)
#define tt_CREDIT_REVERSAL              (tt_TRANS_TYPE_START + 4)
#define tt_CREDIT_AUTH                  (tt_TRANS_TYPE_START + 5)
#define tt_CREDIT_ADD_TO_BATCH          (tt_TRANS_TYPE_START + 6)

#define tt_DEBIT_SALE                   (tt_TRANS_TYPE_START + 7)
#define tt_DEBIT_RETURN                 (tt_TRANS_TYPE_START + 8)
#define tt_DEBIT_REVERSAL               (tt_TRANS_TYPE_START + 9)

#define tt_BATCH_CLOSE                  (tt_TRANS_TYPE_START + 10)
#define tt_UTILITY_PARAMETER_DOWNLOAD   (tt_TRANS_TYPE_START + 11)
#define tt_CREDIT_TXN_EDIT              (tt_TRANS_TYPE_START + 12)
    
#define tt_TRANS_TYPE_END               (tt_TRANS_TYPE_START + 15)

/**
 * Transaction interface defined
**/
#define ti_SWIPE        1001
#define ti_CHIP         1002
#define ti_CTLS         1003

/**
 * Header optional field definitions
**/
#define d_HEADER_OPTIONAL_FIELD_CLERK_ID                    0x01
#define d_HEADER_OPTIONAL_FIELD_SITE_TRACE                  0x02
#define d_HEADER_OPTIONAL_FIELD_CREDENTIAL_TOKEN            0x03
#define d_HEADER_OPTIONAL_FIELD_CLIENT_TXN_ID               0x04
#define d_HEADER_OPTIONAL_FIELD_UNIQUE_DEVICE_ID            0x05
#define d_HEADER_OPTIONAL_FIELD_SECRET_API_KEY              0x06
#define d_HEADER_OPTIONAL_FIELD_LATITUDE                    0x07
#define d_HEADER_OPTIONAL_FIELD_LONGITUDE                   0x08
    
#define PARSE_TYPE_HEADER       10
#define PARSE_TYPE_TRANS        11
#define TAG_NAME_UNKNOWN        TAG_NAME_PREV"Unknown"
#define NULL_STRING             ""

#define ST_DATA_TYPE_NONE                       5000
#define ST_DATA_TYPE_HlData                     (ST_DATA_TYPE_NONE + 1)
#define ST_DATA_TYPE_CardData                   (ST_DATA_TYPE_NONE + 2)
#define ST_DATA_TYPE_ManualEntryData            (ST_DATA_TYPE_NONE + 3)
#define ST_DATA_TYPE_TokenData                  (ST_DATA_TYPE_NONE + 4)
#define ST_DATA_TYPE_EncryptionData             (ST_DATA_TYPE_NONE + 5)
#define ST_DATA_TYPE_TokenParametersData        (ST_DATA_TYPE_NONE + 6)
#define ST_DATA_TYPE_CardHolderData             (ST_DATA_TYPE_NONE + 7)
#define ST_DATA_TYPE_DirectMktData              (ST_DATA_TYPE_NONE + 8)
#define ST_DATA_TYPE_LodgingData                (ST_DATA_TYPE_NONE + 9)
#define ST_DATA_TYPE_LodgingDataEdit            (ST_DATA_TYPE_NONE + 10)
#define ST_DATA_TYPE_ExtraCharges               (ST_DATA_TYPE_NONE + 11)
#define ST_DATA_TYPE_AutoSubstantiation         (ST_DATA_TYPE_NONE + 12)
#define ST_DATA_TYPE_AllAdditionalAmtInfo       (ST_DATA_TYPE_NONE + 13) // 1~4 AdditionalAmtInfo
#define ST_DATA_TYPE_AddTxnFields               (ST_DATA_TYPE_NONE + 14)
#define ST_DATA_TYPE_OrigTxnRefData             (ST_DATA_TYPE_NONE + 15)
#define ST_DATA_TYPE_EmvData                    (ST_DATA_TYPE_NONE + 16)
#define ST_DATA_TYPE_SecureEC                   (ST_DATA_TYPE_NONE + 17)
#define ST_DATA_TYPE_TagData                    (ST_DATA_TYPE_NONE + 18)
#define ST_DATA_TYPE_PaymentMethodKeyData       (ST_DATA_TYPE_NONE + 19)
#define ST_DATA_TYPE_PDLRequest                 (ST_DATA_TYPE_NONE + 20)
#define ST_DATA_TYPE_TokenRequest               (ST_DATA_TYPE_NONE + 21)

#define _BASE_DATA_SIZE_   16

    
typedef struct 
{
    char szLicenseId[_BASE_DATA_SIZE_];
    char szSiteId[_BASE_DATA_SIZE_];
    char szDeviceId[_BASE_DATA_SIZE_];
    char szSiteTrace[_BASE_DATA_SIZE_ * 2];
    char szGatewayTxnId[_BASE_DATA_SIZE_];
    char szGatewayRspCode[_BASE_DATA_SIZE_];
    char szGatewayRspMsg[_BASE_DATA_SIZE_ * 17];
    char szRspDT[_BASE_DATA_SIZE_ * 2];
    char szClientTxnId[_BASE_DATA_SIZE_];
    char szUniqueDeviceId[_BASE_DATA_SIZE_];
    char szBatchId[_BASE_DATA_SIZE_];
    char szBatchSeqNbr[_BASE_DATA_SIZE_];
    char szPDLNotification[_BASE_DATA_SIZE_];
    char szTokenRspCode[_BASE_DATA_SIZE_];
    char szTokenRspMsg[_BASE_DATA_SIZE_ * 17];
    char szTokenValue[_BASE_DATA_SIZE_ * 4];
}ST_HSOAP_RESPONSE_GATEWAY;

typedef struct 
{
    // Common [CreditSale, CreditAuth, CreditReversal, DebitSale, DebitReversal, DebitReturn]
    char szRspCode[_BASE_DATA_SIZE_];
    char szRspText[_BASE_DATA_SIZE_ * 17];
    char szAuthCode[_BASE_DATA_SIZE_];
    char szAVSRsltCode[_BASE_DATA_SIZE_ * 2];
    char szCVVRsltCode[_BASE_DATA_SIZE_ * 2];
    char szCPCInd[_BASE_DATA_SIZE_];
    char szRefNbr[_BASE_DATA_SIZE_];
    char szAvailableBalance[_BASE_DATA_SIZE_];
    char szAuthAmt[_BASE_DATA_SIZE_];
    char szAVSResultCodeAction[_BASE_DATA_SIZE_];
    char szCVVResultCodeAction[_BASE_DATA_SIZE_];
    char szCardType[_BASE_DATA_SIZE_];
    char szAVSRsltText[_BASE_DATA_SIZE_ * 2];
    char szCVVRsltText[_BASE_DATA_SIZE_ * 2];
    char szTxnDescriptor[_BASE_DATA_SIZE_ * 2];
    char szRecurringDataCode[_BASE_DATA_SIZE_];
    char szEMVIssuerResp[_BASE_DATA_SIZE_ * 65];
    char szCAVVResultCode[_BASE_DATA_SIZE_];
    char szTokenPANLast4[_BASE_DATA_SIZE_];
    
    // BatchClose
    char szBatchId[_BASE_DATA_SIZE_];
    char szTxnCnt[_BASE_DATA_SIZE_];
    char szTotalAmt[_BASE_DATA_SIZE_];
    char szBatchSeqNbr[_BASE_DATA_SIZE_];
    
    // ParameterDownload
    char szPDLBlockRsp[_BASE_DATA_SIZE_ * 16];
    char szVersion[_BASE_DATA_SIZE_];
    char szBlockSequence[_BASE_DATA_SIZE_ * 7];
    char szTableID[_BASE_DATA_SIZE_];
    char szPdlCardType[_BASE_DATA_SIZE_]; // // PDLResponse->szCardType
    char szEndOfTableFlag[_BASE_DATA_SIZE_];
    char szConfirmationFlag[_BASE_DATA_SIZE_];
    char szTableBlock[_BASE_DATA_SIZE_ * 7];
    char szEMVEnabled[_BASE_DATA_SIZE_];
    char szTable30Version[_BASE_DATA_SIZE_];
    char szTable30Flag[_BASE_DATA_SIZE_];
    char szPdlCardTypes[_BASE_DATA_SIZE_]; // PDLResponse->TableData->Table10->CardTypes->szCardType
    char szTable40Version[_BASE_DATA_SIZE_];
    char szTable40Flag[_BASE_DATA_SIZE_];
    char szTable50Version[_BASE_DATA_SIZE_];
    char szTable50Flag[_BASE_DATA_SIZE_];
    char szTable60Version[_BASE_DATA_SIZE_];
    char szTable60Flag[_BASE_DATA_SIZE_];
    
    // Null [CreditAddToBatch, CreditVoid, CreditReturn]
}ST_HSOAP_RESPPONSE_TRANS;

typedef struct 
{
    char szExpMonth[_BASE_DATA_SIZE_]; // "1" ~ "12"
    char szExpYear[_BASE_DATA_SIZE_]; // "2000" ~ "9999"
    char szCardPresent[_BASE_DATA_SIZE_];
    char szReaderPresent[_BASE_DATA_SIZE_];
    char szCVV2[_BASE_DATA_SIZE_];
    char szCVV2Status[_BASE_DATA_SIZE_];
}ST_HSOAP_REQUEST_COMMONDATA;

typedef struct 
{
    char szCardNbr[_BASE_DATA_SIZE_ * 2];
    ST_HSOAP_REQUEST_COMMONDATA cCommData;
}ST_HSOAP_REQUEST_MANUALENTRY;

typedef struct 
{
    char szTokenValue[_BASE_DATA_SIZE_ * 4];
    ST_HSOAP_REQUEST_COMMONDATA cCommData;
}ST_HSOAP_REQUEST_TOKENDATA;

typedef struct 
{
    char szVersion[_BASE_DATA_SIZE_];
    char szEncryptedTrackNumber[_BASE_DATA_SIZE_];
    char szKTB[_BASE_DATA_SIZE_ * 20];
    char szKSN[_BASE_DATA_SIZE_ * 9];
}ST_HSOAP_REQUEST_ENCRYPTIONDATA;

typedef struct 
{
    char szMapping[_BASE_DATA_SIZE_];
}ST_HSOAP_REQUEST_TOKENPARAMETERS;

typedef struct 
{
    // Choice
    int nChoice; // HSOAP_MANUAL_ENTRY:ManualEntry, HSOAP_TOKEN_DATA:TokenData, HSOAP_TRACK_DATA:szTrackData
    char szTrackData[_BASE_DATA_SIZE_ * 16]; // method = swipe or proximity
    ST_HSOAP_REQUEST_MANUALENTRY cManualEntry;
    ST_HSOAP_REQUEST_TOKENDATA cTokenData;
    
    // Optional
    ST_HSOAP_REQUEST_ENCRYPTIONDATA cEncrypData;
    char szTokenRequest[_BASE_DATA_SIZE_];
    ST_HSOAP_REQUEST_TOKENPARAMETERS cTokenPara;
}ST_HSOAP_REQUEST_CARDDATA;

typedef struct 
{
    char szCardHolderFirstName[_BASE_DATA_SIZE_ * 2];
    char szCardHolderLastName[_BASE_DATA_SIZE_ * 2];
    char szCardHolderAddr[_BASE_DATA_SIZE_ * 4];
    char szCardHolderCity[_BASE_DATA_SIZE_ * 2];
    char szCardHolderState[_BASE_DATA_SIZE_ * 2];
    char szCardHolderZip[_BASE_DATA_SIZE_];
    char szCardHolderPhone[_BASE_DATA_SIZE_ * 2];
    char szCardHolderEmail[_BASE_DATA_SIZE_ * 7];
}ST_HSOAP_REQUEST_CARDHOLDERDATA;

typedef struct 
{
    char szDirectMktInvoiceNbr[_BASE_DATA_SIZE_ * 2];
    char szDirectMktShipMonth[_BASE_DATA_SIZE_]; // "1" ~ "12"
    char szDirectMktShipDay[_BASE_DATA_SIZE_]; // "1" ~ "31"
}ST_HSOAP_REQUEST_DIRECTMKTDATA;

typedef struct 
{
    char szRestaurant[_BASE_DATA_SIZE_];
    char szGiftShop[_BASE_DATA_SIZE_];
    char szMiniBar[_BASE_DATA_SIZE_];
    char szTelephone[_BASE_DATA_SIZE_];
    char szOther[_BASE_DATA_SIZE_];
    char szLaundry[_BASE_DATA_SIZE_];
}ST_HSOAP_REQUEST_EXTRACHARGES;

typedef struct 
{
    char FolioNumber[_BASE_DATA_SIZE_ * 2];
    char Duration[_BASE_DATA_SIZE_];
    char CheckInDate[_BASE_DATA_SIZE_];
    char CheckOutDate[_BASE_DATA_SIZE_];
    char Rate[_BASE_DATA_SIZE_]; // This field is required by AMEX.
    ST_HSOAP_REQUEST_EXTRACHARGES cExtraCharge;
    char szExtraChargeAmtInfo[_BASE_DATA_SIZE_];
}ST_HSOAP_REQUEST_LODGINGDATAEDIT;

typedef struct 
{
    char szPrestigiousPropertyLimit[_BASE_DATA_SIZE_ * 2];
    char szNoShow[_BASE_DATA_SIZE_];
    char szAdvancedDepositType[_BASE_DATA_SIZE_ * 2]; // For AMEX only.
    ST_HSOAP_REQUEST_LODGINGDATAEDIT cLodgingDataEdit;
    char szPreferredCustomer[_BASE_DATA_SIZE_];
}ST_HSOAP_EQUEST_LODGINGDATA;

typedef struct 
{
    char szAmtType[_BASE_DATA_SIZE_ * 2];
    char szAmt[_BASE_DATA_SIZE_];
}ST_HSOAP_REQUEST_AMTCOMM;

typedef struct 
{
    ST_HSOAP_REQUEST_AMTCOMM cFirstAdditionalAmtInfo;
    ST_HSOAP_REQUEST_AMTCOMM cSecondAdditionalAmtInfo;
    ST_HSOAP_REQUEST_AMTCOMM cThirdAdditionalAmtInfo;
    ST_HSOAP_REQUEST_AMTCOMM cFourthAdditionalAmtInfo;
    char szMerchantVerificationValue[_BASE_DATA_SIZE_];
    char szRealTimeSubstantiation[_BASE_DATA_SIZE_];
}ST_HSOAP_REQUEST_AUTOSUBSTANTIATION;

typedef struct 
{
    char szDescription[_BASE_DATA_SIZE_ * 16];
    char szInvoiceNbr[_BASE_DATA_SIZE_ * 4];
    char szCustomerID[_BASE_DATA_SIZE_ * 4];
}ST_HSOAP_REQUEST_ADDITIONALTXNFIELDS;

typedef struct 
{
    char szAuthCode[_BASE_DATA_SIZE_];
    char szCardNbrLastFour[_BASE_DATA_SIZE_];
}ST_HSOAP_REQUEST_ORIGTXNREFDATA;

typedef struct 
{
    char szEMVTagData[_BASE_DATA_SIZE_ * 65]; // EMV tag data in TLV format.
    char szEMVChipCondition[_BASE_DATA_SIZE_ * 2];
    char szPINBlock[_BASE_DATA_SIZE_ * 3];
}ST_HSOAP_REQUEST_EMVDATA;

typedef struct 
{
    char szPaymentDataSource[_BASE_DATA_SIZE_ * 2];
    char szTypeOfPaymentData[_BASE_DATA_SIZE_];
    
    int nPaymentDataEncode; // nPaymentDataEncode base16 = HSOAP_ENCODE_BASE16 and base64 = HSOAP_ENCODE_BASE64 (or other)
    char szPaymentData[_BASE_DATA_SIZE_ * 17]; // encoding ref. nPaymentDataEncode
    
    char szECommerceIndicator[_BASE_DATA_SIZE_];
    
    int nXidDataEncode; // nXidDataEncode = base16 = HSOAP_ENCODE_BASE16 and base64 = HSOAP_ENCODE_BASE64 (or other)
    char szXID[_BASE_DATA_SIZE_ * 17]; // encoding ref. nXidDataEncode
}ST_HSOAP_REQUEST_SECUREECOMMERCE;

typedef struct 
{
    int nTagValueType; // nTagValueType HSOAP_TAG_VALUE_TYPE_MSD = msd and HSOAP_TAG_VALUE_TYPE_CHIP(or other) = chip
    char szTagValues[_BASE_DATA_SIZE_ * 65]; // source = chip or msd
}ST_HSOAP_REQUEST_TAGDATA;

typedef struct 
{
    char szExpMonth[_BASE_DATA_SIZE_]; // "1" ~ "12"
    char szExpYear[_BASE_DATA_SIZE_]; // "2000" ~ "9999"
    char szCVV2[_BASE_DATA_SIZE_];
    char szCVV2Status[_BASE_DATA_SIZE_];
}ST_HSOAP_REQUEST_PAYMENTMETHODKEYDATA;

typedef struct 
{
    char szParamType[_BASE_DATA_SIZE_];
    char szTableID[_BASE_DATA_SIZE_];
    char szCardType[_BASE_DATA_SIZE_];
    char szVersion[_BASE_DATA_SIZE_];
    char szBlockSequence[_BASE_DATA_SIZE_ * 7];
}ST_HSOAP_REQUEST_PDLREQUEST;



/**
 * API Struc
 * Last byte must '\0'
**/
typedef struct 
{
    char szLicenseId[_BASE_DATA_SIZE_];
    char szSiteId[_BASE_DATA_SIZE_];
    char szDeviceId[_BASE_DATA_SIZE_];
    char szUserName[_BASE_DATA_SIZE_ * 7];
    char szPassword[_BASE_DATA_SIZE_ * 2];
    char szDeveloperID[_BASE_DATA_SIZE_];
    char szVersionNbr[_BASE_DATA_SIZE_];
    char szPosGatewayUrl[_BASE_DATA_SIZE_ * 8]; // Ex: "http://Hps.Exchange.PosGateway"
    
    // below optional
    char szClerkID[_BASE_DATA_SIZE_ * 4];
    char szSiteTrace[_BASE_DATA_SIZE_ * 2];
    char szCredentialToken[_BASE_DATA_SIZE_];
    char szClientTxnId[_BASE_DATA_SIZE_];
    char szUniqueDeviceId[_BASE_DATA_SIZE_];
    char szSecretAPIKey[_BASE_DATA_SIZE_];
    
    // GPS Coordinates
    char szLatitude[_BASE_DATA_SIZE_];
    char szLongitude[_BASE_DATA_SIZE_];
    
    char szRFU[_BASE_DATA_SIZE_];
    unsigned long long ullExtraData;
}ST_HSOAP_INIT;

typedef struct 
{
    int nTransactionType; // Ref. Transaction type defined
    int nTransactionInterface; // Ref. Transaction interface defined
    
    char szGatewayTxnId[_BASE_DATA_SIZE_];
    ST_HSOAP_REQUEST_CARDDATA cCardData;
    char szAmt[_BASE_DATA_SIZE_];
    char szGratuityAmtInfo[_BASE_DATA_SIZE_];
    char szCPCReq[_BASE_DATA_SIZE_];
    ST_HSOAP_REQUEST_CARDHOLDERDATA cCardHolderData;
    ST_HSOAP_REQUEST_DIRECTMKTDATA cDirMktData;
    char szAllowDup[_BASE_DATA_SIZE_];
    ST_HSOAP_EQUEST_LODGINGDATA cLodgingData;
    ST_HSOAP_REQUEST_AUTOSUBSTANTIATION cAutoSubstantiation;
    char szAllowPartialAuth[_BASE_DATA_SIZE_];
    char szEcommerce[_BASE_DATA_SIZE_];
    ST_HSOAP_REQUEST_ADDITIONALTXNFIELDS cAddTxnFields;
    ST_HSOAP_REQUEST_ORIGTXNREFDATA cOrigTxnRefData;
    char szConvenienceAmtInfo[_BASE_DATA_SIZE_];
    char szShippingAmtInfo[_BASE_DATA_SIZE_];
    char szTxnDescriptor[_BASE_DATA_SIZE_ * 2];
    char szSurchargeAmtInfo[_BASE_DATA_SIZE_];
    ST_HSOAP_REQUEST_EMVDATA cEmvData;
    ST_HSOAP_REQUEST_SECUREECOMMERCE cSecureEC;
    ST_HSOAP_REQUEST_TAGDATA cTagData;
    
    // CreditAuth
    char szPaymentMethodKey[_BASE_DATA_SIZE_ * 3];
    ST_HSOAP_REQUEST_PAYMENTMETHODKEYDATA cPayMethodKeyData;
    
    // CreditReversal
    char szAuthAmt[_BASE_DATA_SIZE_];
    char szClientTxnId[_BASE_DATA_SIZE_];
    
    // CreditReversal, CreditAddToBatch
    char szEMVTagData[_BASE_DATA_SIZE_];
    
    // DebitSale
    char szTrackData[_BASE_DATA_SIZE_ * 16];
    char szPinBlock[_BASE_DATA_SIZE_ * 3];
    char szTokenValue[_BASE_DATA_SIZE_ * 4];
    char szCashbackAmtInfo[_BASE_DATA_SIZE_];
    
    // ParameterDownload
    char szPDLBlockReq[_BASE_DATA_SIZE_ * 16];
    ST_HSOAP_REQUEST_PDLREQUEST cPDLRequest;
    
    char szRFU[_BASE_DATA_SIZE_];
    unsigned long long ullExtraData;
}ST_HSOAP_REQUEST;

typedef struct
{
    ST_HSOAP_RESPONSE_GATEWAY cGateway; // Gateway response
    ST_HSOAP_RESPPONSE_TRANS cTrans; // Transaction response
    
    char szRFU[_BASE_DATA_SIZE_];
    unsigned long long ullExtraData;
}ST_HSOAP_RESPONSE;

#ifdef	__cplusplus
}
#endif

#endif    /* __HEARTLAND__LIBHEARTLAND_H__ */

