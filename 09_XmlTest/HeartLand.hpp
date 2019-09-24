#ifndef __HEART_LAND_HPP__
#define __HEART_LAND_HPP__

#include <string>
#include <sstream>
#include <map>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include "SoapElement.hpp"
#include "NestedMap.hpp"

using namespace std;

class HeartLandRequest: public SoapElement
{
public:
    string generateSoapString(string hpsGateway);
};

class HeartLandResponse
{
public:
    NestedMap header;
    NestedMap transactionResult;

    bool parseResponseSoap(char *xmlString);

private:
    bool paresResponseXmlDoc(xmlDocPtr doc);
    bool generateHeader(xmlDocPtr doc);
    bool generateTransaction(xmlDocPtr doc);
    bool generateResponseList(xmlDocPtr doc, char *xpath, NestedMap &list);
    xmlNodeSetPtr getNodeSet (xmlDocPtr doc, xmlChar *xpath);
    bool is_leaf(xmlNodePtr node);
    void constructMap(NestedMap &superMap, xmlNodePtr node);
};


#define TAG_NAME_PREV_STRING                    "hps"
#define TAG_NAME_PREV                           TAG_NAME_PREV_STRING":"

#define TAG_NAME_POS_REQUEST                    TAG_NAME_PREV"PosRequest"
#define TAG_NAME_VER_1_0                        TAG_NAME_PREV"Ver1.0"
/// Header
#define TAG_NAME_HEADER                         TAG_NAME_PREV"Header"
#define TAG_NAME_LICENSE_ID                     TAG_NAME_PREV"LicenseId"
#define TAG_NAME_SITE_ID                        TAG_NAME_PREV"SiteId"
#define TAG_NAME_DEVICE_ID                      TAG_NAME_PREV"DeviceId"
#define TAG_NAME_USER_NAME                      TAG_NAME_PREV"UserName"
#define TAG_NAME_PASSWORD                       TAG_NAME_PREV"Password"
#define TAG_NAME_SITE_TRACE                     TAG_NAME_PREV"SiteTrace"
#define TAG_NAME_DEVELOPER_ID                   TAG_NAME_PREV"DeveloperID"
#define TAG_NAME_VERSION_NUMBER                 TAG_NAME_PREV"VersionNbr"
#define TAG_NAME_CLERK_ID                       TAG_NAME_PREV"ClerkID"

/// Transaction
#define TAG_NAME_TRANSACTION                    TAG_NAME_PREV"Transaction"
#define TAG_NAME_BLOCK_1                        TAG_NAME_PREV"Block1"
#define TAG_NAME_GATEWAY_TXN_ID                 TAG_NAME_PREV"GatewayTxnId"

/// PinBlock
#define TAG_NAME_PIN_BLOCK                      TAG_NAME_PREV"PinBlock"
/// CardData
#define TAG_NAME_CARD_DATA                      TAG_NAME_PREV"CardData"
#define TAG_NAME_TRACK_DATA                     TAG_NAME_PREV"TrackData"
/// ManualEntry
#define TAG_NAME_MANUAL_ENTRY                   TAG_NAME_PREV"ManualEntry"
#define TAG_NAME_CARD_NBR                       TAG_NAME_PREV"CardNbr"
#define TAG_NAME_EXP_MONTH                      TAG_NAME_PREV"ExpMonth"
#define TAG_NAME_EXP_YAER                       TAG_NAME_PREV"ExpYear"
#define TAG_NAME_CARD_PRESENT                   TAG_NAME_PREV"CardPresent"
#define TAG_NAME_READER_PRESENT                 TAG_NAME_PREV"ReaderPresent"
#define TAG_NAME_CVV2                           TAG_NAME_PREV"CVV2"
#define TAG_NAME_CVV2_STATUS                    TAG_NAME_PREV"CVV2Status"
/// TokenData
#define TAG_NAME_TOKEN_DATA                     TAG_NAME_PREV"TokenData"
#define TAG_NAME_TOKEN_VALUE                    TAG_NAME_PREV"TokenValue"
/// EncryptionData
#define TAG_NAME_ENCRYPTION_DATA                TAG_NAME_PREV"EncryptionData"
#define TAG_NAME_VERSION                        TAG_NAME_PREV"Version"
#define TAG_NAME_ENCRYPTED_TRACK_NUMBER         TAG_NAME_PREV"EncryptedTrackNumber"
#define TAG_NAME_KTB                            TAG_NAME_PREV"KTB"
#define TAG_NAME_KSN                            TAG_NAME_PREV"KSN"
#define TAG_NAME_TOKEN_REQUEST                  TAG_NAME_PREV"TokenRequest"
/// TokenParameters
#define TAG_NAME_TOKEN_PARAMETERS               TAG_NAME_PREV"TokenParameters"
#define TAG_NAME_TOKEN_MAPPING                  TAG_NAME_PREV"TokenMapping"

#define TAG_NAME_AMT                            TAG_NAME_PREV"Amt"
#define TAG_NAME_AMT_TYPE                       TAG_NAME_PREV"AmtType"
#define TAG_NAME_GRATUITY_AMT_INFO              TAG_NAME_PREV"GratuityAmtInfo"
#define TAG_NAME_CPC_REQ                        TAG_NAME_PREV"CPCReq"

/// CardHolderData
#define TAG_NAME_CARD_HOLDER_DATA               TAG_NAME_PREV"CardHolderData"
#define TAG_NAME_CARD_HOLDER_FISRT_NAME         TAG_NAME_PREV"CardHolderFirstName"
#define TAG_NAME_CARD_HOLDER_LAST_NAME          TAG_NAME_PREV"CardHolderLastName"
#define TAG_NAME_CARD_HOLDER_ADDR               TAG_NAME_PREV"CardHolderAddr"
#define TAG_NAME_CARD_HOLDER_CITY               TAG_NAME_PREV"CardHolderCity"
#define TAG_NAME_CARD_HOLDER_STATE              TAG_NAME_PREV"CardHolderState"
#define TAG_NAME_CARD_HOLDER_ZIP                TAG_NAME_PREV"CardHolderZip"
#define TAG_NAME_CARD_HOLDER_PHONE              TAG_NAME_PREV"CardHolderPhone"
#define TAG_NAME_CARD_HOLDER_EMAIL              TAG_NAME_PREV"CardHolderEmail"

/// DirectMktData
#define TAG_NAME_DIRECT_MKT_DATA                TAG_NAME_PREV"DirectMktData"
#define TAG_NAME_DIRECT_MKT_INVOICE_NBR         TAG_NAME_PREV"DirectMktInvoiceNbr"
#define TAG_NAME_DIRECT_MKT_SHIP_MONTH          TAG_NAME_PREV"DirectMktShipMonth"
#define TAG_NAME_DIRECT_MKT_SHIP_DAY            TAG_NAME_PREV"DirectMktShipDay"
#define TAG_NAME_ALLOW_DUP                      TAG_NAME_PREV"AllowDup"

/// LodgingData
#define TAG_NAME_LODGING_DATA                   TAG_NAME_PREV"LodgingData"
#define TAG_NAME_PRESTIGIOUS_PROPERTY_LIMIT     TAG_NAME_PREV"PrestigiousPropertyLimit"
#define TAG_NAME_NO_SHOW                        TAG_NAME_PREV"NoShow"
#define TAG_NAME_ADVANCE_DEPOSIT_TYPE           TAG_NAME_PREV"AdvancedDepositType"
#define TAG_NAME_PREFERRED_CUSTOMER             TAG_NAME_PREV"PreferredCustomer"
/// LodgingDataEdit
#define TAG_NAME_LODGING_DATA_EDIT              TAG_NAME_PREV"LodgingDataEdit"
#define TAG_NAME_FOLIO_NUMBER                   TAG_NAME_PREV"FolioNumber"
#define TAG_NAME_DURATION                       TAG_NAME_PREV"Duration"
#define TAG_NAME_CHECK_IN_DATE                  TAG_NAME_PREV"CheckInDate"
#define TAG_NAME_CHECK_OUT_DATE                 TAG_NAME_PREV"CheckOutDate"
#define TAG_NAME_RATE                           TAG_NAME_PREV"Rate"
#define TAG_NAME_EXTRA_CHARGES                  TAG_NAME_PREV"ExtraCharges"
#define TAG_NAME_EXTRA_CHARGE_AMT_INFO          TAG_NAME_PREV"ExtraChargeAmtInfo"

/// AutoSubstantiation
#define TAG_NAME_AUTO_SUBSTANTIATION            TAG_NAME_PREV"AutoSubstantiation"
#define TAG_NAME_FIRST_ADDITINOAL_AMT_INFO      TAG_NAME_PREV"FirstAdditionalAmtInfo"
#define TAG_NAME_SECOND_ADDITIONAL_AMT_INFO     TAG_NAME_PREV"SecondAdditionalAmtInfo"
#define TAG_NAME_THIRD_ADDITIONAL_AMT_INFO      TAG_NAME_PREV"ThirdAdditionalAmtInfo"
#define TAG_NAME_FOURTH_ADDITINOAL_AMT_INFO     TAG_NAME_PREV"FourthAdditionalAmtInfo"
#define TAG_NAME_MERCHANT_VERIFICATION_VALUE    TAG_NAME_PREV"MerchantVerificationValue"
#define TAG_NAME_REAL_TIME_SUBSTANTIATION       TAG_NAME_PREV"RealTimeSubstantiation"

#define TAG_NAME_ALLOW_PARTIAL_AUTH             TAG_NAME_PREV"AllowPartialAuth"
#define TAG_NAME_ECOMMERCE                      TAG_NAME_PREV"Ecommerce"

/// AdditionalTxnFields
#define TAG_NAME_ADDITIONAL_TXN_FIELDS          TAG_NAME_PREV"AdditionalTxnFields"
#define TAG_NAME_DESCRIPTION                    TAG_NAME_PREV"Description"
#define TAG_NAME_INVOICE_NBR                    TAG_NAME_PREV"InvoiceNbr"
#define TAG_NAME_CUSTOMER_ID                    TAG_NAME_PREV"CustomerID"

/// OrigTxnRefData
#define TAG_NAME_ORIG_TXN_REF_DATA              TAG_NAME_PREV"OrigTxnRefData"
#define TAG_NAME_AUTH_CODE                      TAG_NAME_PREV"AuthCode"
#define TAG_NAME_CARD_NBR_LAST_FOUR             TAG_NAME_PREV"CardNbrLastFour"
#define TAG_NAME_CONVENIENCE_AMT_INFO           TAG_NAME_PREV"ConvenienceAmtInfo"
#define TAG_NAME_SHIPPING_AMT_INFO              TAG_NAME_PREV"ShippingAmtInfo"
#define TAG_NAME_TXN_DESCRIPTOR                 TAG_NAME_PREV"TxnDescriptor"
#define TAG_NAME_SURCHARGE_AMT_INFO             TAG_NAME_PREV"SurchargeAmtInfo"

/// EMVData
#define TAG_NAME_EMV_DATA                       TAG_NAME_PREV"EMVData"
#define TAG_NAME_EMV_TAG_DATA                   TAG_NAME_PREV"EMVTagData"
#define TAG_NAME_EMV_CHIP_CONDITION             TAG_NAME_PREV"EMVChipCondition"
#define TAG_NAME_EMV_DATA_PIN_BLOCK             TAG_NAME_PREV"PINBlock"

/// SecureECommerce
#define TAG_NAME_SECURE_ECOMMERCE               TAG_NAME_PREV"SecureECommerce"
#define TAG_NAME_PAYMENT_DATA_SOURCE            TAG_NAME_PREV"PaymentDataSource"
#define TAG_NAME_TYPE_OF_PAYMENT_DATA           TAG_NAME_PREV"TypeOfPaymentData"
#define TAG_NAME_PAYMENT_DATA                   TAG_NAME_PREV"PaymentData"
#define TAG_NAME_ECOMMERCE_INDICATOR            TAG_NAME_PREV"ECommerceIndicator"
#define TAG_NAME_XID                            TAG_NAME_PREV"XID"

#define TAG_NAME_TAG_DATA                       TAG_NAME_PREV"TagData"
#define TAG_NAME_TAG_VALUES                     TAG_NAME_PREV"TagValues"

#define TAG_NAME_PAYMENT_METHOD_KEY             TAG_NAME_PREV"PaymentMethodKey"
#define TAG_NAME_PAYMENT_METHOD_KEY_DATA        TAG_NAME_PREV"PaymentMethodKeyData"
#define TAG_NAME_AUTH_AMT                       TAG_NAME_PREV"AuthAmt"
#define TAG_NAME_CLIENT_TXN_ID                  TAG_NAME_PREV"ClientTxnId"

#define TAG_NAME_CASH_BACK_AMT_INFO             TAG_NAME_PREV"CashbackAmtInfo"
#define TAG_NAME_ACCOUNT_TYPE                   TAG_NAME_PREV"AccountType"
#define TAG_NAME_POS_SEQUENCE_NBR               TAG_NAME_PREV"PosSequenceNbr"

/// Response
#define TAG_NAME_RSP_CODE                       TAG_NAME_PREV"RspCode"
#define TAG_NAME_RSP_TEXT                       TAG_NAME_PREV"RspText"
#define TAG_NAME_AVS_RSLT_CODE                  TAG_NAME_PREV"AVSRsltCode"
#define TAG_NAME_CVV_RSLT_CODE                  TAG_NAME_PREV"CVVRsltCode"
#define TAG_NAME_CPC_IND                        TAG_NAME_PREV"CPCInd"
#define TAG_NAME_REF_NBR                        TAG_NAME_PREV"RefNbr"
#define TAG_NAME_AVAILABLE_BALANCE              TAG_NAME_PREV"AvailableBalance"
#define TAG_NAME_AVS_RESULT_CODE_ACTION         TAG_NAME_PREV"AVSResultCodeAction"
#define TAG_NAME_CVV_RESULT_CODE_ACTION         TAG_NAME_PREV"CVVResultCodeAction"
#define TAG_NAME_CARD_TYPE                      TAG_NAME_PREV"CardType"
#define TAG_NAME_AVS_RSLT_TEXT                  TAG_NAME_PREV"AVSRsltText"
#define TAG_NAME_CVS_RSLT_TEXT                  TAG_NAME_PREV"CVVRsltText"
#define TAG_NAME_RECURRING_DATA_CODE            TAG_NAME_PREV"RecurringDataCode"
#define TAG_NAME_EMV_ISSUER_RESP                TAG_NAME_PREV"EMVIssuerResp"
#define TAG_NAME_CAVV_RESULT_CODE               TAG_NAME_PREV"CAVVResultCode"
#define TAG_NAME_TOKEN_PAN_LAST_4               TAG_NAME_PREV"TokenPANLast4"
#define TAG_NAME_BATCH_ID                       TAG_NAME_PREV"BatchId"
#define TAG_NAME_TXN_CNT                        TAG_NAME_PREV"TxnCnt"
#define TAG_NAME_TOTAL_AMT                      TAG_NAME_PREV"TotalAmt"
#define TAG_NAME_BATCH_SEQ_NBR                  TAG_NAME_PREV"BatchSeqNbr"

/// Commands
#define TAG_NAME_CREDIT_AUTH                    TAG_NAME_PREV"CreditAuth"
#define TAG_NAME_CREDIT_ADD_TO_BATCH            TAG_NAME_PREV"CreditAddToBatch"
#define TAG_NAME_CREDIT_REVERSAL                TAG_NAME_PREV"CreditReversal"
#define TAG_NAME_CREDIT_SALE                    TAG_NAME_PREV"CreditSale"
#define TAG_NAME_BATCH_CLOSE                    TAG_NAME_PREV"BatchClose"
#define TAG_NAME_CREDIT_TXN_EDIT                TAG_NAME_PREV"CreditTxnEdit"

/// Attribute Method
#define ATTRIBUTE_NAME_METHOD       "method"
#define METHOD_VALUE_SWIPE          "swipe"
#define METHOD_VALUE_PROXIMITY      "proximity"

/// Attribute Source
#define ATTRIBUTE_NAME_SOURCE       "source"
#define SOURCE_VALUE_MSD            "msd"
#define SOURCE_VALUE_CIP            "chip"

/// Attribute encoding
#define ATTRIBUTE_NAME_ENCODING     "encoding"
#define ENCODING_VALUE_BASE16       "base16"
#define ENCODING_VALUE_BASE64       "base64"

#endif /*__HEART_LAND_HPP__*/
