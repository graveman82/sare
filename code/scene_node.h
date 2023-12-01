#ifndef SARE_SCENE_NODE_H_
#define SARE_SCENE_NODE_H_

#include <memory>
#include <unordered_map>
#include <bitset>
#include "rotator.h"
#include "object.h"


namespace sare
{

//-----------------------------------------------------------------------------
//
class SceneNode : public Object
{
public:
    using BoolProperties = std::unordered_map<std::string, bool>;


    enum FlagBit {
        kFlagBit_DirtyLocalTm,
        kFlagBit_DirtyWorldTm,
        kFlagBit_Count
    };
    SceneNode();
    void LinkTo(SceneNode* parent);
    void Unlink();
    bool Linked() const { return nullptr != parent_; }
    SceneNode* Root();
    SceneNode* parent() const {return parent_; }
    SceneNode* next_sibling() { return next_sibling_; }
    const D3DXMATRIX& world_tm();

protected:
    SceneNode(const SceneNode&);
    SceneNode& operator=(const SceneNode&);

    bool Cycling(SceneNode* parent);

    bool GetBoolProperty(const char* property, bool& value);
    bool SetBoolProperty(const char* property, bool value);

    void MoveAlongX(float dx);
    void MoveAlongY(float dy);
    void MoveAlongZ(float dz);
    void MoveAlongLocalX(float dx);
    void MoveAlongLocalY(float dy);
    void MoveAlongLocalZ(float dz);
    void SetPosition(float x, float y, float z);
    void SetScale(float x, float y, float z);
    void SetRotation(float p, float y, float r);
    void Rotate(float dp, float dy, float dr);
    const D3DXMATRIX& LocalMatrix();

    bool DirtyWorldTm() const;
    bool DirtyHierarchy() const;
    void UpdateWorldMatrix();
private:
    bool FindBoolProperty(const char* property, BoolProperties::iterator& iter);
protected:
    SceneNode* parent_;
    SceneNode* next_sibling_;
    SceneNode* prev_sibling_;
    SceneNode* first_child_;


    D3dxVector3Ex position_;
    D3dxVector3Ex scale_;
    Rotator rotator_;
    D3DXMATRIX local_tm_;
    D3DXMATRIX world_tm_;
    std::bitset<kFlagBit_Count> flags_;
private:
    BoolProperties boolProperties_;
};

} // end of sare
#endif // SARE_SCENE_NODE_H_
