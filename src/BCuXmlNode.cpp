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
#include <BCuXml/BCuXmlNode.hpp>

namespace BlackCodex {
namespace uXml {

BCuXmlNode::BCuXmlNode() {}

BCuXmlNode::BCuXmlNode(const char* node_name)
    : mName(node_name)
{}

//static
BCuXmlNode& BCuXmlNode::getEmptyNode() {
    static BCuXmlNode EMPTY_NODE;
    return EMPTY_NODE; 
}
const char*         BCuXmlNode::value() const                       {   return mValue.c_str();          }
const std::string&  BCuXmlNode::getValue() const {
    return mValue;
}
void                BCuXmlNode::addNode(const BCuXmlNode& node)     {   mChildren.push_back(node);      }
BCuXmlNode&         BCuXmlNode::getLastNode()                       {   return mChildren.back();        }

void                BCuXmlNode::appendValue(const std::string& str) {   mValue.append(str);             }

const std::string&  BCuXmlNode::getName() const                     {   return mName;                   }
size_t              BCuXmlNode::getChildCount() const               {   return mChildren.size();        }
size_t              BCuXmlNode::getPropertyCount() const            {   return mProperties.size();      }

const std::string& BCuXmlNode::getPropertyByName(const char* propname) const
{
    auto it = mProperties.find(propname);
    if (it == this->mProperties.end()) {
        static const std::string EMPTY_STRING;
        return EMPTY_STRING;
    } else {
        return it->second;
    }
}

const BCuXmlNode& BCuXmlNode::getChild(size_t index) const {
    if (index >= mChildren.size()) {
        return getEmptyNode();
    }

    auto it = mChildren.begin();
    while( it != mChildren.end() && index--!=0) {
        ++it;
    }

    return *it;
}

BCuXmlNode& BCuXmlNode::getChild(size_t index) {
    if (index >= mChildren.size()) {
        return getEmptyNode();
    }

    auto it = mChildren.begin();
    while( it != mChildren.end() && index--!=0) {
        ++it;
    }

    return *it;
}

BCuXmlNode& BCuXmlNode::getChild(const std::string& name)
{
    xmlnodelist_iterator_t  it=this->mChildren.begin();

    while(it!= this->mChildren.end())
    {
        if (name == it->mName)
        {
            BCuXmlNode& obj = *it;
            return obj;
        }

        ++it;
    }

    return getEmptyNode();
}

BCuXmlNode& BCuXmlNode::getChildByPath(const std::string& path)
{
    BCuXmlNode*              retval = nullptr;
    xmlnodelist_iterator_t  it=this->mChildren.begin();

    size_t      pos = path.find_first_of('/');

    if (std::string::npos == pos) {
        return getChild(path);
    }

    std::string nodebase = path.substr(0, pos);

    std::string str(path);
    str = str.substr(pos+1);

    while(it!= this->mChildren.end()) {
        if (nodebase.compare(it->mName.c_str())==0) {
            return it->getChildByPath(str.c_str());
        }

        ++it;
    }

    return getEmptyNode();
}

void BCuXmlNode::addProperty(const std::string& propName,
                             const std::string& propValue) {
    mProperties[propName] = propValue;
}

void BCuXmlNode::clear() {
    this->mChildren.clear();
    this->mName.clear();
    this->mProperties.clear();
    this->mValue.clear();
}


} // uXml
} // BlackCodex
