#ifndef __HEARTLAND_SOAP_ELEMENT_HPP__
#define __HEARTLAND_SOAP_ELEMENT_HPP__

#include <string>
#include <list>
#include <map>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

using namespace std;

class SoapElement
{
public:
    typedef struct Attribute {
        string name;
        string value;

        const bool operator== (const struct Attribute& rhs) const {
            return name == rhs.name && value == rhs.value;
        }
    } Attribute;

    string soapTagName;
    string content;

    SoapElement(void) : soapTagName(""), content("") { }
    SoapElement(string soapTagName, string content = "") : soapTagName(soapTagName), content(content) { };

    const bool operator== (const SoapElement& rhs) const;

    void addSubElement(SoapElement element);
    SoapElement &addSubElement(string soapTagName, string content = "");
    void removeSubElement(SoapElement &element);
    void addSubElementList(list<SoapElement> &subElementList);
    void addAttribute(string name, string value);
    void printsubElementList(void);

    string generateSoapString(void);

    unsigned long subElementCount(void);
    unsigned long attributeCount(void);

protected:
    list<SoapElement> subElementList;
    list<Attribute> attributeList;

private:
    void generateSOAP(xmlNodePtr &soapNode);
    xmlNodePtr addCurrentNodeUnderSoap(xmlNodePtr &soapNode);
    void setAttributesOfSoap(xmlNodePtr &soapNode);
    void addSubElementsInSoap(xmlNodePtr &soapNode);
    void addSubElementsInSoap(SoapElement &soapNode);
};
typedef SoapElement& SoapElementRef;




#endif /* __HEARTLAND_SOAP_ELEMENT_HPP__ */
