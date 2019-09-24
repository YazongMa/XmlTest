/*
* gjobread.c : a small test program for gnome jobs XML format
*
* See Copyright for the status of this software.
*
* Daniel.Veillard@w3.org
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <curl/curl.h>

#include "libheartland.h"
#include "libstructdefined.h"

static char szData[2048] = { 0 };
static char szHttpRet[4];

static int TestHLPack();
static int TestHLXml();
static int TestHLUnPack();
static int TestHLCommunication();

int main()
{	
	TestHLPack();
	// TestHLCommunication();
	// TestHLXml();
	// TestHLUnPack();
	return 0;
}


int TestHLPack()
{
	HeartlandSoap	soap;
	ST_HSOAP_INIT	init = { 0 };

	strcpy(init.szDeviceId, "6399858");
	strcpy(init.szLicenseId, "142865");
	strcpy(init.szSiteId, "142952");
	strcpy(init.szUserName, "701389344");
	strcpy(init.szPassword, "$Test1234");
	strcpy(init.szPosGatewayUrl, "http://Hps.Exchange.PosGateway");
	soap.HeartlandSOAP_Init(&init);


	ST_HSOAP_REQUEST request = { 0 };
	request.nTransactionType = tt_DEBIT_SALE;
	request.nTransactionInterface = ti_SWIPE;
	strcpy(request.szAmt, "12.00");
	strcpy(request.szTrackData, ";6217500034254678912=201221103201132102F");
	strcpy(request.szAccountType, "SAVINGS");
	strcpy(request.szPosSequenceNbr, "123456789");

	int nDataLen = sizeof(szData);
	soap.HeartlandSOAP_Pack(&request, &nDataLen, szData);

	return 0;
}


int TestHLXml()
{
	SoapElement root("XmlTest");

	root.addAttribute("Attr_Root_1", "Attr_Root_Value1");
	root.addAttribute("Attr_Root_2", "Attr_Root_Value2");
	root.addAttribute("Attr_Root_3", "Attr_Root_Value3");
	root.addAttribute("Attr_Root_4", "Attr_Root_Value4");

	SoapElementRef SubTag1 = root.addSubElement("SubTag1");
	SoapElementRef SubTag2 = root.addSubElement("SubTag2", "SubTag2_Value");
	SoapElementRef SubTag3 = root.addSubElement("SubTag3", "SubTag3_Value");
	SoapElementRef SubTag4 = root.addSubElement("SubTag4");

	SubTag3.addAttribute("Attr_Sub3_1", "Attr_Sub3_Value1");
	SubTag3.addAttribute("Attr_Sub3_2", "Attr_Sub3_Value2");

	SubTag4.addAttribute("Attr_Sub4_1", "Attr_Sub4_Value1");
	SubTag4.addAttribute("Attr_Sub4_2", "Attr_Sub4_Value2");

	SoapElementRef SubTag4_1 = SubTag4.addSubElement("SubTag4_1", "SubTag4_1_Value");
	SoapElementRef SubTag4_2 = SubTag4.addSubElement("SubTag4_2", "SubTag4_2_Value");
	SoapElementRef SubTag4_3 = SubTag4.addSubElement("SubTag4_3", "SubTag4_3_Value");
	SoapElementRef SubTag4_4 = SubTag4.addSubElement("SubTag4_4", "SubTag4_4_Value");

	SoapElementRef SubTag4_1_1 = SubTag4_1.addSubElement("SubTag4_1_1", "SubTag4_1_1_Value");
	SoapElementRef SubTag4_1_2 = SubTag4_1.addSubElement("SubTag4_2_1", "SubTag4_1_2_Value");
	SoapElementRef SubTag4_1_3 = SubTag4_1.addSubElement("SubTag4_3_1", "SubTag4_1_3_Value");
	SoapElementRef SubTag4_1_4 = SubTag4_1.addSubElement("SubTag4_4_1", "SubTag4_1_4_Value");

	string result = root.generateSoapString();

	return 0;
}

#define d_TXN_RSP_CODE_REVERSAL                     "30"
#define d_TXN_RSP_CODE_SUCCESS                      "00"

int TestHLUnPack()
{
	HeartlandSoap		soap;
	ST_HSOAP_RESPONSE	response = { 0 };

#if 0
	char* resp = "<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"><soap:Body><PosResponse rootUrl=\"https://posgateway.cert.secureexchange.net/Hps.Exchange.PosGateway\" xmlns=\"http://Hps.Exchange.PosGateway\"><Ver1.0><Header><LicenseId>137846</LicenseId><SiteId>137907</SiteId><DeviceId>6378375</DeviceId><GatewayTxnId>1193129660</GatewayTxnId><GatewayRspCode>0</GatewayRspCode><GatewayRspMsg>Success</GatewayRspMsg><RspDT>2019-07-17T21:41:55.0593533</RspDT><UniqueDeviceId>2468</UniqueDeviceId></Header><Transaction><CreditSale><RspCode>00</RspCode><RspText>APPROVAL</RspText><AuthCode>007674</AuthCode><AVSRsltCode>0</AVSRsltCode><RefNbr>919817246151</RefNbr><CardType>Disc</CardType><AVSRsltText>AVS Not Requested.</AVSRsltText><CardBrandTxnId>071802415427628</CardBrandTxnId></CreditSale></Transaction></Ver1.0></PosResponse></soap:Body></soap:Envelope>";
#endif

#if 0
	char* resp = "<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"><soap:Body><PosResponse rootUrl=\"https://posgateway.cert.secureexchange.net/Hps.Exchange.PosGateway\" xmlns=\"http://Hps.Exchange.PosGateway\"><Ver1.0><Header><LicenseId>142865</LicenseId><SiteId>142952</SiteId><DeviceId>6399858</DeviceId><GatewayTxnId>1212533876</GatewayTxnId><GatewayRspCode>0</GatewayRspCode><GatewayRspMsg>Success</GatewayRspMsg><RspDT>2019-09-04T22:08:42.0112827</RspDT><UniqueDeviceId>2468</UniqueDeviceId></Header><Transaction><ReportTxnDetail><GatewayTxnId>1212612628</GatewayTxnId><SiteId>142952</SiteId><MerchName>POSGTEST714C CASTLEPAY SA</MerchName><DeviceId>6399858</DeviceId><UserName>701389344</UserName><ServiceName>CreditAuth</ServiceName><GatewayRspCode>0</GatewayRspCode><GatewayRspMsg>Success</GatewayRspMsg><ReqUtcDT>2019-09-05T03:08:19.337Z</ReqUtcDT><ReqDT>2019-09-05T03:08:19.337</ReqDT><RspUtcDT>2019-09-05T03:08:19.743Z</RspUtcDT><RspDT>2019-09-05T03:08:19.743</RspDT><SiteTrace /><OriginalGatewayTxnId>0</OriginalGatewayTxnId><MerchNbr>777701389344</MerchNbr><TermOrdinal>1</TermOrdinal><MerchAddr1>1   HEARTLAND   WAY</MerchAddr1><MerchAddr2 /><MerchCity>JEFFERSONVILL</MerchCity><MerchState>IN</MerchState><MerchZip>47130</MerchZip><MerchPhone>972-295-8700</MerchPhone><TzConversion>UTC</TzConversion><UniqueDeviceId>2468</UniqueDeviceId><Data><TxnStatus>A</TxnStatus><CardType>MC</CardType><MaskedCardNbr>222300******5780</MaskedCardNbr><CardPresent>Y</CardPresent><ReaderPresent>Y</ReaderPresent><CardSwiped>Y</CardSwiped><DebitCreditInd>C</DebitCreditInd><SaleReturnInd>S</SaleReturnInd><CVVReq>N</CVVReq><CVVRsltCode /><Amt>27.00</Amt><AuthAmt>27.00</AuthAmt><GratuityAmtInfo>0</GratuityAmtInfo><CashbackAmtInfo>0</CashbackAmtInfo><CardHolderLastName /><CardHolderFirstName /><CardHolderAddr /><CardHolderCity /><CardHolderState /><CardHolderZip /><CardHolderPhone /><CardHolderEmail /><AVSRsltCode>0</AVSRsltCode><CPCReq>N</CPCReq><CPCInd /><RspCode>00</RspCode><RspText>APPROVAL</RspText><AuthCode>093858</AuthCode><ReqACI>Y</ReqACI><RspACI /><MktSpecDataId /><TxnCode>54</TxnCode><AcctDataSrc>D</AcctDataSrc><AuthSrcCode>5</AuthSrcCode><IssTxnId>924718298866000</IssTxnId><IssValidationCode /><CardHolderIdCode>@</CardHolderIdCode><NetworkIdCode /><RefNbr>924718298866</RefNbr><TxnSeqNbr>72</TxnSeqNbr><DirectMktInvoiceNbr /><DirectMktShipMonth>0</DirectMktShipMonth><DirectMktShipDay>0</DirectMktShipDay><CPCCardHolderPONbr /><CPCTaxType /><CPCTaxAmt>0</CPCTaxAmt><SettlementAmt>27.00</SettlementAmt><AllowPartialAuth>Y</AllowPartialAuth><AVSRsltText>AVS Not Requested.</AVSRsltText><TokenizationMsg /><CryptoTypeIn>Clear</CryptoTypeIn><CryptoTypeOut>Clear</CryptoTypeOut><AllowDup>Y</AllowDup><EMVChipCondition /><EMVTagData /><EMVIssuerResp /><TypeOfPaymentData /><PaymentData /><ECommerceIndicator /><CAVVResultCode /><TokenPANLast4 /><Company /><CustomerFirstname /><CustomerLastname /><PaymentDataSource /><XID /><CardOnFileData /><CardBrandTxnId>MCC1019860905</CardBrandTxnId></Data><x_global_transaction_id /><MerchAddr3 /><AuthenticatedSiteId>142952</AuthenticatedSiteId><AuthenticatedDeviceId>6399858</AuthenticatedDeviceId></ReportTxnDetail></Transaction></Ver1.0></PosResponse></soap:Body></soap:Envelope>";
#endif

	char* resp = "<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"><soap:Body><PosResponse rootUrl=\"https://posgateway.cert.secureexchange.net/Hps.Exchange.PosGateway\" xmlns=\"http://Hps.Exchange.PosGateway\"><Ver1.0><Header><LicenseId>142704</LicenseId><SiteId>142789</SiteId><DeviceId>664</DeviceId><GatewayTxnId>1220629879</GatewayTxnId><GatewayRspCode>30</GatewayRspCode><GatewayRspMsg>Transaction has timed out.  You may need to submit a reversal for the transaction.</GatewayRspMsg><RspDT>2019-09-17T21:09:56.7721269</RspDT><UniqueDeviceId>2468</UniqueDeviceId></Header><Transaction><CreditSale /></Transaction></Ver1.0></PosResponse></soap:Body></soap:Envelope>";
	soap.HeartlandSOAP_Unpack(&response, resp);


	char chTxnResult = 0x05;
	if (memcmp(response.cGateway.szGatewayRspCode, d_TXN_RSP_CODE_REVERSAL, 2) == 0)
		chTxnResult = 0x1B;
	else if (memcmp(response.cTrans.szRspCode, d_TXN_RSP_CODE_SUCCESS, 2) != 0)
		chTxnResult = 0x00;


	return 0;
}



/* holder for curl fetch */
struct curl_fetch_st {
	char *payload;
	size_t size;
};

size_t curl_cbHeader(void *ptr, size_t size, size_t nmemb, void  *userdata)
{
	int ret = 0;
	if (strncmp((char *)ptr, "HTTP/1.1", 8) == 0) // get http response code
	{
		strtok((char *)ptr, " ");
		userdata = (strtok(NULL, " \n")); // http response code

		printf("*userdata :%s \n", (char *)userdata);
		memcpy(szHttpRet, userdata, strlen((char *)userdata));


		printf("szHttpRet :%s\n", szHttpRet);
	}

	printf("\n Get head Call pack*ptr :%s\n", (char *)ptr);
	return nmemb;
}


size_t curl_callback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;                             /* calculate buffer size */
	struct curl_fetch_st *p = (struct curl_fetch_st *) userp;   /* cast pointer to fetch struct */

																/* expand buffer */
	p->payload = (char *)realloc(p->payload, p->size + realsize + 1);

	/* check buffer */
	if (p->payload == NULL) {
		/* this isn't good */

		printf("ERROR: Failed to expand buffer in curl_callback");
		/* free buffer */
		free(p->payload);
		/* return */
		return -1;
	}

	/* copy contents to buffer */
	memcpy(&(p->payload[p->size]), contents, realsize);

	/* set new buffer size */
	p->size += realsize;

	/* ensure null termination */
	p->payload[p->size] = 0;

	/* return size */
	return realsize;
}

int TestHLCommunication()
{
	CURLcode rcode = curl_global_init(CURL_GLOBAL_DEFAULT);
	if (rcode != CURLE_OK)
	{
		return 1;
	}

	struct curl_slist* pHeaders = NULL;
	pHeaders = curl_slist_append(pHeaders, "Accept-Encoding: gzip,deflate");
	pHeaders = curl_slist_append(pHeaders, "Content-Type: text/xml;charset=UTF-8");
	pHeaders = curl_slist_append(pHeaders, "SOAPAction: DoTransaction");

	CURL* pCurlHandle = curl_easy_init();
	if (pCurlHandle == NULL)
	{
		return 1;
	}
	struct curl_fetch_st curl_fetch = { 0 };

	curl_easy_setopt(pCurlHandle, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(pCurlHandle, CURLOPT_CAPATH, "D:\\TimProject\\09_XmlTest\\x64\\Debug");
	curl_easy_setopt(pCurlHandle, CURLOPT_CAINFO, "D:\\TimProject\\09_XmlTest\\x64\\Debug\\ca-certificates.crt");
	curl_easy_setopt(pCurlHandle, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(pCurlHandle, CURLOPT_HTTPHEADER, pHeaders);
	curl_easy_setopt(pCurlHandle, CURLOPT_POSTFIELDS, szData);
	curl_easy_setopt(pCurlHandle, CURLOPT_URL, "https://cert.api2-c.heartlandportico.com/Hps.Exchange.PosGateway/PosGatewayService.asmx");
	curl_easy_setopt(pCurlHandle, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
	curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, curl_callback);
	curl_easy_setopt(pCurlHandle, CURLOPT_HEADERFUNCTION, curl_cbHeader);
	curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, (void *)&curl_fetch);
	curl_easy_setopt(pCurlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_setopt(pCurlHandle, CURLOPT_TIMEOUT, 100);
	curl_easy_setopt(pCurlHandle, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(pCurlHandle, CURLOPT_MAXREDIRS, 1);

	// curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, false);
	// printf("Info Use CURLOPT_SSL_VERIFYPEER");

	rcode = curl_easy_perform(pCurlHandle);
	curl_easy_cleanup(pCurlHandle);
	curl_slist_free_all(pHeaders);

	printf("http status:%s\n", szHttpRet);
	if (rcode != CURLE_OK || curl_fetch.size < 1) 
	{
		printf("err rcode :%04X\n", rcode);
		printf("ERROR: Failed to perform curl said: %s\n",curl_easy_strerror(rcode));
	}
	else
	{
		printf("Rresponse size:%s\n", curl_fetch.size);
		if (curl_fetch.payload)
		{
			printf("Rresponse data:%s\n", curl_fetch.payload);
			curl_fetch.payload = NULL;
		}
	}

	return 0;
}




#if 0
/*
* This example should compile and run indifferently with libxml-1.8.8 +
* and libxml2-2.1.0 +
* Check the COMPAT comments below
*/

/*
* COMPAT using xml-config --cflags to get the include path this will
* work with both
*/
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#define DEBUG(x) printf(x)

/*
* A person record
* an xmlChar * is really an UTF8 encoded char string (0 terminated)
*/
typedef struct person
{
	xmlChar *name;
	xmlChar *email;
	xmlChar *company;
	xmlChar *organisation;
	xmlChar *smail;
	xmlChar *webPage;
	xmlChar *phone;
} person, *personPtr;

/*
* And the code needed to parse it
*/
static personPtr
parsePerson(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur)
{
	personPtr ret = NULL;

	DEBUG("parsePerson\n");
	/*
	* allocate the struct
	*/
	ret = (personPtr)malloc(sizeof(person));
	if (ret == NULL)
	{
		fprintf(stderr, "out of memory\n");
		return (NULL);
	}
	memset(ret, 0, sizeof(person));

	/* We don't care what the top level element name is */
	/* COMPAT xmlChildrenNode is a macro unifying libxml1 and libxml2 names */
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"Person")) &&
			(cur->ns == ns))
			ret->name = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"Email")) &&
			(cur->ns == ns))
			ret->email = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		cur = cur->next;
	}

	return (ret);
}

/*
* and to print it
*/
static void
printPerson(personPtr cur)
{
	if (cur == NULL)
		return;
	printf("------ Person\n");
	if (cur->name)
		printf("	name: %s\n", cur->name);
	if (cur->email)
		printf("	email: %s\n", cur->email);
	if (cur->company)
		printf("	company: %s\n", cur->company);
	if (cur->organisation)
		printf("	organisation: %s\n", cur->organisation);
	if (cur->smail)
		printf("	smail: %s\n", cur->smail);
	if (cur->webPage)
		printf("	Web: %s\n", cur->webPage);
	if (cur->phone)
		printf("	phone: %s\n", cur->phone);
	printf("------\n");
}

/*
* a Description for a Job
*/
typedef struct job
{
	xmlChar *projectID;
	xmlChar *application;
	xmlChar *category;
	personPtr contact;
	int nbDevelopers;
	personPtr developers[100]; /* using dynamic alloc is left as an exercise */
} job, *jobPtr;

/*
* And the code needed to parse it
*/
static jobPtr
parseJob(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur)
{
	jobPtr ret = NULL;

	DEBUG("parseJob\n");
	/*
	* allocate the struct
	*/
	ret = (jobPtr)malloc(sizeof(job));
	if (ret == NULL)
	{
		fprintf(stderr, "out of memory\n");
		return (NULL);
	}
	memset(ret, 0, sizeof(job));

	/* We don't care what the top level element name is */
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{

		if ((!xmlStrcmp(cur->name, (const xmlChar *)"Project")) &&
			(cur->ns == ns))
		{
			ret->projectID = xmlGetProp(cur, (const xmlChar *)"ID");
			if (ret->projectID == NULL)
			{
				fprintf(stderr, "Project has no ID\n");
			}
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"Application")) &&
			(cur->ns == ns))
			ret->application =
			xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"Category")) &&
			(cur->ns == ns))
			ret->category =
			xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"Contact")) &&
			(cur->ns == ns))
			ret->contact = parsePerson(doc, ns, cur);
		cur = cur->next;
	}

	return (ret);
}

/*
* and to print it
*/
static void
printJob(jobPtr cur)
{
	int i;

	if (cur == NULL)
		return;
	printf("=======  Job\n");
	if (cur->projectID != NULL)
		printf("projectID: %s\n", cur->projectID);
	if (cur->application != NULL)
		printf("application: %s\n", cur->application);
	if (cur->category != NULL)
		printf("category: %s\n", cur->category);
	if (cur->contact != NULL)
		printPerson(cur->contact);
	printf("%d developers\n", cur->nbDevelopers);

	for (i = 0; i < cur->nbDevelopers; i++)
		printPerson(cur->developers[i]);
	printf("======= \n");
}

/*
* A pool of Gnome Jobs
*/
typedef struct gjob
{
	int nbJobs;
	jobPtr jobs[500]; /* using dynamic alloc is left as an exercise */
} gJob, *gJobPtr;

static gJobPtr
parseGjobFile(char *filename)
{
	xmlDocPtr doc;
	gJobPtr ret;
	jobPtr curjob;
	xmlNsPtr ns;
	xmlNodePtr cur;

#ifdef LIBXML_SAX1_ENABLED
	/*
	* build an XML tree from a the file;
	*/
	doc = xmlParseFile(filename);
	if (doc == NULL)
		return (NULL);
#else
	/*
	* the library has been compiled without some of the old interfaces
	*/
	return (NULL);
#endif /* LIBXML_SAX1_ENABLED */

	/*
	* Check the document is of the right kind
	*/

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL)
	{
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
		return (NULL);
	}
	ns = xmlSearchNsByHref(doc, cur,
		(const xmlChar *)"http://www.gnome.org/some-location");
	if (ns == NULL)
	{
		fprintf(stderr,
			"document of the wrong type, GJob Namespace not found\n");
		xmlFreeDoc(doc);
		return (NULL);
	}
	if (xmlStrcmp(cur->name, (const xmlChar *)"Helping"))
	{
		fprintf(stderr, "document of the wrong type, root node != Helping");
		xmlFreeDoc(doc);
		return (NULL);
	}

	/*
	* Allocate the structure to be returned.
	*/
	ret = (gJobPtr)malloc(sizeof(gJob));
	if (ret == NULL)
	{
		fprintf(stderr, "out of memory\n");
		xmlFreeDoc(doc);
		return (NULL);
	}
	memset(ret, 0, sizeof(gJob));

	/*
	* Now, walk the tree.
	*/
	/* First level we expect just Jobs */
	cur = cur->xmlChildrenNode;
	while (cur && xmlIsBlankNode(cur))
	{
		cur = cur->next;
	}
	if (cur == 0)
	{
		xmlFreeDoc(doc);
		free(ret);
		return (NULL);
	}
	if ((xmlStrcmp(cur->name, (const xmlChar *)"Jobs")) || (cur->ns != ns))
	{
		fprintf(stderr, "document of the wrong type, was '%s', Jobs expected",
			cur->name);
		fprintf(stderr, "xmlDocDump follows\n");
#ifdef LIBXML_OUTPUT_ENABLED
		xmlDocDump(stderr, doc);
		fprintf(stderr, "xmlDocDump finished\n");
#endif /* LIBXML_OUTPUT_ENABLED */
		xmlFreeDoc(doc);
		free(ret);
		return (NULL);
	}

	/* Second level is a list of Job, but be laxist */
	cur = cur->xmlChildrenNode;
	while (cur != NULL)
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"Job")) &&
			(cur->ns == ns))
		{
			curjob = parseJob(doc, ns, cur);
			if (curjob != NULL)
				ret->jobs[ret->nbJobs++] = curjob;
			if (ret->nbJobs >= 500)
				break;
		}
		cur = cur->next;
	}

	return (ret);
}

static void
handleGjob(gJobPtr cur)
{
	int i;

	/*
	* Do whatever you want and free the structure.
	*/
	printf("%d Jobs registered\n", cur->nbJobs);
	for (i = 0; i < cur->nbJobs; i++)
		printJob(cur->jobs[i]);
}

int main(int argc, char **argv)
{
	int i;
	gJobPtr cur;

	/* COMPAT: Do not genrate nodes for formatting spaces */
	LIBXML_TEST_VERSION
		xmlKeepBlanksDefault(0);

	for (i = 1; i < argc; i++)
	{
		cur = parseGjobFile(argv[i]);
		if (cur)
			handleGjob(cur);
		else
			fprintf(stderr, "Error parsing file '%s'\n", argv[i]);
	}

	/* Clean up everything else before quitting. */
	xmlCleanupParser();

	return (0);
}

#endif