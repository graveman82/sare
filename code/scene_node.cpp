#include "scene_node.h"
#include "assert.h"


namespace sare {

//-----------------------------------------------------------------------------
SceneNode::SceneNode() :
    parent_(nullptr),
    next_sibling_(nullptr),
    prev_sibling_(nullptr),
    first_child_(nullptr)
{
    scale_.Set(1, 1, 1);
    flags_.set(kFlagBit_DirtyWorldTm);

    boolProperties_["visibility"] = true;
}

//-----------------------------------------------------------------------------
void SceneNode::LinkTo(SceneNode* parent) {
    SARE_ASSERT(parent);
    SARE_ASSERT(!Linked());
    SARE_ASSERT(!Cycling(parent));

    if (parent_->first_child_) {
        SARE_ASSERT(!parent_->first_child_->prev_sibling_);
        parent_->first_child_->prev_sibling_ = this;
        next_sibling_ = parent_->first_child_;

    }
    parent_->first_child_ = this;
    flags_.set(kFlagBit_DirtyWorldTm);
}

//-----------------------------------------------------------------------------
void SceneNode::Unlink() {
    SARE_ASSERT(Linked());

    if (parent_->first_child_ == this) {
        parent_->first_child_ = next_sibling_;
        if (next_sibling_)
            next_sibling_->prev_sibling_ = nullptr;
    }
    else {
        SARE_ASSERT(prev_sibling_);
        prev_sibling_->next_sibling_ = next_sibling_;
        if (next_sibling_)
            next_sibling_->prev_sibling_ = prev_sibling_;
    }
    next_sibling_ = prev_sibling_ = parent_ = nullptr;
    flags_.set(kFlagBit_DirtyWorldTm);
}

//-----------------------------------------------------------------------------
bool SceneNode::Cycling(SceneNode* parent) {
    SceneNode* node = parent;
    while (node) {
        if (node == this)
            return true;
        node = node->parent_;
    }
    return false;
}

//-----------------------------------------------------------------------------
SceneNode* SceneNode::Root() {
    SceneNode* root = this;
    while (root->parent_) {
        root = root->parent_;
    }
    return root;
}

//-----------------------------------------------------------------------------
bool SceneNode::DirtyWorldTm() const {
    return flags_.test(kFlagBit_DirtyWorldTm);
}

//-----------------------------------------------------------------------------
bool SceneNode::DirtyHierarchy() const {
    const SceneNode* node = this;
    while (node) {
        if (node->DirtyWorldTm())
            return true;
        node = node->parent();
    }
    return false;
}

//-----------------------------------------------------------------------------
void SceneNode::UpdateWorldMatrix() {

    if (parent() == 0) {
        world_tm_ = LocalMatrix();
    }
    else {
        D3DXMatrixMultiply(&world_tm_, &LocalMatrix(), &parent_->world_tm_);
    }
    flags_.reset(kFlagBit_DirtyWorldTm);

    for (SceneNode* child = first_child_; child; child = child->next_sibling()) {
        child->UpdateWorldMatrix();
    }
}

//-----------------------------------------------------------------------------
const D3DXMATRIX& SceneNode::world_tm() {
    if (DirtyHierarchy()) {
        Root()->UpdateWorldMatrix();
    }
    return world_tm_;
}
} // end of sare
