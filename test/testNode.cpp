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

#include <gtest/gtest.h>

namespace {
    BlackCodex::uXml::BCuXmlNode getTestNode() {
        using namespace BlackCodex::uXml;
        
        BCuXmlNode node;

        BCuXmlNode node1("test:subnode");

        BCuXmlNode node1_1("test:subnode1_1");
        node1.addNode(node1_1);

        node.addNode( node1 );

        {
            const auto& nodeLast = node.getLastNode();
            EXPECT_EQ       ("test:subnode", nodeLast.getName());
        }

        BCuXmlNode nodeWithProp("test:subnode2");
        nodeWithProp.addProperty("prop1", "prop1value");
        nodeWithProp.addProperty("prop2", "prop2value");

        node.addNode( nodeWithProp );

        return node;
    }

}

TEST(testUXmlNode, appendValue) {
    using namespace BlackCodex::uXml;
    
    BCuXmlNode node;
    node.appendValue("Test");
    node.appendValue("Value");

    EXPECT_STRCASEEQ("TestValue", node.value());
}

TEST(testUXmlNode, nodeName) {
    using namespace BlackCodex::uXml;
    
    BCuXmlNode node("test:node");
    node.appendValue("Test");
    node.appendValue("Value");

    EXPECT_EQ       ("test:node", node.getName());
    EXPECT_STRCASEEQ("TestValue", node.value());

    node.clear();
    EXPECT_TRUE(node.getName().empty());
    EXPECT_TRUE(node.getValue().empty());
}

TEST(testUXmlNode, nodeChildNotFoundByName) {
    using namespace BlackCodex::uXml;
    
    BCuXmlNode nodeRoot = getTestNode();

    const BCuXmlNode& node = nodeRoot.getChild("test:childDoesNotExist");
    EXPECT_TRUE(node.getName().empty());
    EXPECT_TRUE(node.getValue().empty());
}

TEST(testUXmlNode, nodeChildNotFoundByPathInRoot) {
    using namespace BlackCodex::uXml;
    
    BCuXmlNode nodeRoot = getTestNode();

    const BCuXmlNode& node = nodeRoot.getChildByPath("test:thisNodeDoesNoExist/test:subNode");
    EXPECT_TRUE(node.getName().empty());
    EXPECT_TRUE(node.getValue().empty());
}

TEST(testUXmlNode, nodeChildByName) {
    using namespace BlackCodex::uXml;
    
    BCuXmlNode nodeRoot = getTestNode();

    const auto& node2 = nodeRoot.getChild("test:subnode2");
    EXPECT_EQ       ("test:subnode2",   node2.getName());
    EXPECT_EQ       (2,                 node2.getPropertyCount());
    EXPECT_EQ       ("prop1value",      node2.getPropertyByName("prop1"));
    EXPECT_EQ       ("prop2value",      node2.getPropertyByName("prop2"));
    EXPECT_TRUE     (node2.getPropertyByName("prop3").empty());
}

TEST(testUXmlNode, nodeChildByPathLevel1) {
    using namespace BlackCodex::uXml;
    
    BCuXmlNode nodeRoot = getTestNode();

    const auto& node2 = nodeRoot.getChildByPath("test:subnode2");
    EXPECT_EQ       ("test:subnode2",   node2.getName());
    EXPECT_EQ       (2,                 node2.getPropertyCount());
    EXPECT_EQ       ("prop1value",      node2.getPropertyByName("prop1"));
    EXPECT_EQ       ("prop2value",      node2.getPropertyByName("prop2"));
    EXPECT_TRUE     (node2.getPropertyByName("prop3").empty());
}

TEST(testUXmlNode, nodeChildByPathLevel2) {
    using namespace BlackCodex::uXml;
    
    BCuXmlNode nodeRoot = getTestNode();

    const auto& node2 = nodeRoot.getChildByPath("test:subnode/test:subnode1_1");
    EXPECT_EQ       ("test:subnode1_1", node2.getName());
    EXPECT_EQ       (0,                 node2.getPropertyCount());
    EXPECT_EQ       (0,                 node2.getChildCount());
}

TEST(testUXmlNode, nodeChildWithProperties) {
    using namespace BlackCodex::uXml;
    
    BCuXmlNode mnode = getTestNode();
    const BCuXmlNode& node = getTestNode();

    {
        const auto& nodeLast = mnode.getLastNode();
        EXPECT_EQ       ("test:subnode2", nodeLast.getName());
    }

    EXPECT_EQ       (2, node.getChildCount());
    EXPECT_EQ       (0, node.getPropertyCount());

    const BCuXmlNode& node2 = node.getChild(1);
    EXPECT_EQ       ("test:subnode2",   node2.getName());
    EXPECT_EQ       (2,                 node2.getPropertyCount());
    EXPECT_EQ       ("prop1value",      node2.getPropertyByName("prop1"));
    EXPECT_EQ       ("prop2value",      node2.getPropertyByName("prop2"));
    EXPECT_TRUE     (node2.getPropertyByName("prop3").empty());

    const auto& node3 = node.getChild(2);
    EXPECT_TRUE(node3.getName().empty());
    EXPECT_TRUE(node3.getValue().empty());
}

TEST(testUXmlNode, nodeChildAdd) {
    using namespace BlackCodex::uXml;
    
    BCuXmlNode mnode = getTestNode();
    const BCuXmlNode& node = mnode;

    {
        const auto& nodeLast = mnode.getLastNode();
        EXPECT_EQ       ("test:subnode2", nodeLast.getName());

        mnode.getChild(1).addNode("test:grandchild");       // getChild() on mutable node
        EXPECT_TRUE(mnode.getChild(2).getName().empty());   // getChild() [Non-existing] on mutable node
    }

    EXPECT_EQ       (2, node.getChildCount());
    EXPECT_EQ       (0, node.getPropertyCount());

    const BCuXmlNode& node2 = node.getChild(1);
    EXPECT_EQ       ("test:subnode2",   node2.getName());
    EXPECT_EQ       (2,                 node2.getPropertyCount());
    EXPECT_EQ       ("prop1value",      node2.getPropertyByName("prop1"));
    EXPECT_EQ       ("prop2value",      node2.getPropertyByName("prop2"));
    EXPECT_TRUE     (node2.getPropertyByName("prop3").empty());

    const auto& node3 = node2.getChild(0);
    EXPECT_EQ       ("test:grandchild", node3.getName());

    const auto& node4 = node3.getChild(0);
    EXPECT_TRUE(node4.getName().empty());
    EXPECT_TRUE(node4.getValue().empty());
}
