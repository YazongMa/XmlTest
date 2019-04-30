#include "HeartLand.hpp"

//----------------
// HeartLandRequest
//----------------
string HeartLandRequest::generateSoapString(string hpsGateway) {
    SoapElement soapRoot("soapenv:Envelope");
    soapRoot.addAttribute("xmlns:soapenv", "http://schemas.xmlsoap.org/soap/envelope/");
    soapRoot.addAttribute("xmlns:" TAG_NAME_PREV_STRING, hpsGateway);

        soapRoot.addSubElement("soapenv:Header");
        SoapElementRef body = soapRoot.addSubElement("soapenv:Body");

            SoapElementRef posRequest = body.addSubElement(TAG_NAME_POS_REQUEST);

            posRequest.addAttribute("clientType", "");
            posRequest.addAttribute("clientVer", "");

                SoapElementRef ver10 = posRequest.addSubElement(TAG_NAME_VER_1_0);

                    // Add element in subElementList.
                    ver10.addSubElementList(subElementList);

    string result = soapRoot.generateSoapString();

    return result;
}


//----------------
// HeartLandResponse
//----------------
bool HeartLandResponse::parseResponseSoap(char *xmlString) {
    if (xmlString == NULL) {
        return false;
    }

    header.clear();
    transactionResult.clear();

    xmlDocPtr doc = xmlParseDoc(BAD_CAST xmlString);
    if (doc == NULL) {
        return false;
    }

    bool parseResult = paresResponseXmlDoc(doc);

    xmlFreeDoc(doc);

    return parseResult;
}

bool HeartLandResponse::paresResponseXmlDoc(xmlDocPtr doc) {
    if (generateHeader(doc) == false) {
        return false;
    }
    generateTransaction(doc);

    return true;
}

bool HeartLandResponse::generateHeader(xmlDocPtr doc) {
    if (doc == NULL) {
        return false;
    }

    return generateResponseList(doc, (char *)"//*[name()='Header']", header);
}

bool HeartLandResponse::generateTransaction(xmlDocPtr doc) {
    if (doc == NULL) {
        return false;

    }

    return generateResponseList(doc, (char *)"//*[name()='Transaction']/*", transactionResult);
}

bool HeartLandResponse::generateResponseList(xmlDocPtr doc, char *xpath, NestedMap &list) {
    if (doc == NULL || xpath == NULL) {
        return false;
    }

    xmlNodeSetPtr transactionNode = getNodeSet(doc, BAD_CAST xpath);
    if (transactionNode == NULL) {
        return false;
    }

    xmlNodePtr nodePtr = transactionNode->nodeTab[0]->children;
    constructMap(list, nodePtr);

//    while (childPtr != NULL) {
//        xmlChar *content = xmlNodeGetContent(childPtr);
//        list[(char *)childPtr->name] = (char *)content;
//        xmlFree(content);
//
//        childPtr = childPtr->next;
//    }

    return true;
}

xmlNodeSetPtr HeartLandResponse::getNodeSet (xmlDocPtr doc, xmlChar *xpath) {
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;

    /* Create xpath evaluation context */
    context = xmlXPathNewContext(doc);
    if (context == NULL) {
        printf("Error in xmlXPathNewContext\n");
        return NULL;
    }

    /* Evaluate xpath expression */
    result = xmlXPathEvalExpression(xpath, context);
    if (result == NULL) {
        printf("Error in xmlXPathEvalExpression\n");
        return NULL;
    }

    /* Cleanup */
    xmlXPathFreeContext(context);

    if (result == NULL) {
        printf("Error in xmlXPathEvalExpression\n");
        return NULL;
    }

    if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
        xmlXPathFreeObject(result);
        printf("No result\n");
        return NULL;
    }

    return result->nodesetval;
}

bool HeartLandResponse::is_leaf(xmlNodePtr node) {
    xmlNodePtr child = node->children;
    while (child) {
        if(child->type == XML_ELEMENT_NODE) {
            return false;
        }

        child = child->next;
    }

    return true;
}

void HeartLandResponse::constructMap(NestedMap &superMap, xmlNodePtr node) {
    while (node) {
        if (node->type == XML_ELEMENT_NODE) {
            if (is_leaf(node)) {
                superMap[(char *)node->name] = (char *)xmlNodeGetContent(node);
            }
            else {
                NestedMap subMap;
                constructMap(subMap, node->children);
                superMap[(char *)node->name] = subMap;
            }
        }
        node = node->next;
    }
}
