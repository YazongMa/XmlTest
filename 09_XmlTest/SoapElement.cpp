#include "SoapElement.hpp"

const bool SoapElement::operator== (const SoapElement& rhs) const {
    return soapTagName == rhs.soapTagName &&
           content == rhs.content &&
           attributeList == rhs.attributeList &&
           subElementList == rhs.subElementList;
}

void SoapElement::addSubElement(SoapElement element) {
    subElementList.push_back(element);
}

SoapElement &SoapElement::addSubElement(string soapTagName, string content) {
    SoapElement element(soapTagName, content);
    subElementList.push_back(element);
    
    return subElementList.back();
}

void SoapElement::removeSubElement(SoapElement &element) {
    subElementList.remove(element);
}

void SoapElement::addSubElementList(list<SoapElement> &subElementList) {
    //std::copy(subElementList.begin(), subElementList.end(),
    //          std::back_insert_iterator<std::list<SoapElement> >(this->subElementList));

	this->subElementList.insert(this->subElementList.end(), subElementList.begin(), subElementList.end());
}

void SoapElement::addAttribute(string name, string value) {
    Attribute attr;
    attr.name = name;
    attr.value = value;
    attributeList.push_back(attr);
}

void SoapElement::printsubElementList(void)
{
    printf("%s-%05d size:[%d] current nodeName:[%s]\n", __FILE__, __LINE__, subElementList.size(), soapTagName.c_str());

    for(list<SoapElement>::iterator itr = subElementList.begin(); itr != subElementList.end(); ++itr)
    {
        SoapElementRef ref = *itr;
        printf("%s-%05d subNodeName:[%s]\n", __FILE__, __LINE__, ref.soapTagName.c_str());
    }

    printf("%s-%05d\n", __FILE__, __LINE__);
}

string SoapElement::generateSoapString(void) {
    xmlNodePtr soapRoot = xmlNewNode(NULL, BAD_CAST soapTagName.c_str());
    // Set Attributes
    setAttributesOfSoap(soapRoot);
    // Add element in subElementList.
    addSubElementsInSoap(soapRoot);
    
    xmlBufferPtr buf = xmlBufferCreate();
    if (buf == NULL) {
        return "";
    }
    xmlNodeDump(buf, NULL, soapRoot, 0, 0);
    string result = (char *)buf->content;
    xmlBufferFree(buf);
    
    xmlFreeNodeList(soapRoot); // All the children are freed too.
    
    return result;
}

void SoapElement::generateSOAP(xmlNodePtr &soapNode)  {
    // Add a tag into soap
    xmlNodePtr currentNode = addCurrentNodeUnderSoap(soapNode);
    // Set Attributes
    setAttributesOfSoap(currentNode);
    // Add element in subElementList.
    addSubElementsInSoap(currentNode);
}

xmlNodePtr SoapElement::addCurrentNodeUnderSoap(xmlNodePtr &soapNode) {
    return xmlNewChild(soapNode, NULL, BAD_CAST soapTagName.c_str(), BAD_CAST content.c_str());
}

void SoapElement::setAttributesOfSoap(xmlNodePtr &soapNode)  {
    // Set Attributes
    list<Attribute>::iterator attributeIter;
    for (attributeIter = attributeList.begin(); attributeIter != attributeList.end(); ++attributeIter) {
        xmlNewProp(soapNode, BAD_CAST attributeIter->name.c_str(), BAD_CAST attributeIter->value.c_str());
    }
}

void SoapElement::addSubElementsInSoap(xmlNodePtr &soapNode)  {
    // Add element in element list.
    list<SoapElement>::iterator subElementIter;
    for (subElementIter = subElementList.begin(); subElementIter != subElementList.end(); ++subElementIter) {
        subElementIter->generateSOAP(soapNode);
    }
}

unsigned long SoapElement::subElementCount(void) {
    return subElementList.size();
}

unsigned long SoapElement::attributeCount(void) {
    return attributeList.size();
}
