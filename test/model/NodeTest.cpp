//
// Created by Anton O. on 2/23/22.
//

#include "model/Node.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::AtLeast;
using ::testing::Return;

class NodeTest : public ::testing::Test
{
public:
    Core::TaskID parent_id_;
    Core::TaskID child_id_;

    void SetUp() override {
        parent_id_.set_value(1);
        child_id_.set_value(2);
    }
};

TEST_F(NodeTest, shouldCreateWithNoParent)
{
    Node n{};
    EXPECT_FALSE(n.parent());
}

TEST_F(NodeTest, shouldCreateWithAParent)
{
    Node n{parent_id_};
    EXPECT_EQ(parent_id_, n.parent());
}

TEST_F(NodeTest, shouldAddChildren)
{
    Node n{parent_id_};
    n.AddChild(child_id_);
    ASSERT_EQ(1, n.children().size());
    EXPECT_EQ(child_id_, n.children()[0]);
    child_id_.set_value(42);
    n.AddChild(child_id_);
    ASSERT_EQ(2, n.children().size());
    EXPECT_EQ(child_id_, n.children()[1]);
}

TEST_F(NodeTest, shouldRemoveChild)
{
    Node n{parent_id_};
    n.AddChild(child_id_);
    auto new_id{child_id_};
    new_id.set_value(42);
    n.AddChild(new_id);
    n.RemoveChild(new_id);
    ASSERT_EQ(1, n.children().size());
    EXPECT_EQ(child_id_, n.children()[0]);

}

TEST_F(NodeTest, shouldRemoveChildren)
{
    Node n{parent_id_};
    n.AddChild(child_id_);
    auto new_id{child_id_};
    new_id.set_value(42);
    n.AddChild(new_id);
    n.RemoveChildren();
    ASSERT_EQ(0, n.children().size());
}

TEST_F(NodeTest, shouldRemoveParent)
{
    Node n{parent_id_};
    n.RemoveParent();
    ASSERT_EQ(std::nullopt, n.parent());
}

TEST_F(NodeTest, shouldSetParent)
{
    Node n{parent_id_};
    n.SetParent(child_id_);
    ASSERT_EQ(child_id_, n.parent());
}
