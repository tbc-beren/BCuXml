/**
*
* The Black Codex Library: Chapter: BCXML - A minimal XML implementation
*
* https://github.com/tbc-beren/BCXML
*
* Mozilla Public License Version 2.0
* https://github.com/tbc-beren/BCJson/blob/master/LICENSE
*
*/
#pragma once

#include <list>
#include <map>
#include <string>

namespace BlackCodex {
namespace uXml {

class BCuXmlNode
{
private:
    typedef std::map<std::string, std::string>      BCuXmlPropList;

    typedef std::list<BCuXmlNode>                   BCuXmlNodeList;
    typedef BCuXmlNodeList::iterator                xmlnodelist_iterator_t;

    std::string     mName;
    std::string     mValue;
    BCuXmlPropList  mProperties;
    BCuXmlNodeList  mChildren;

public:
    BCuXmlNode();
    BCuXmlNode(const char* node_name);

    const char*         value() const;
    const std::string&  getValue() const;
    void                addNode(const BCuXmlNode& node);
    BCuXmlNode&         getLastNode();

    void                appendValue(const std::string& str);

    const std::string&  getName() const;
    size_t              getChildCount() const;
    size_t              getPropertyCount() const;

    const std::string&  getPropertyByName(const char* propname) const;
    const BCuXmlNode&   getChild(size_t index) const;
    BCuXmlNode&         getChild(size_t index);
    BCuXmlNode&         getChild(const std::string& tagName);
    BCuXmlNode&         getChildByPath(const std::string& path);

    void addProperty(const std::string& propName,
                     const std::string& propValue);

    void                clear();

protected:
    static BCuXmlNode& getEmptyNode();
};


} // uXml
} // BlackCodex
