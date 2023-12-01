#include "scene_node.h"

namespace sare
{
//-----------------------------------------------------------------------------
//
bool SceneNode::FindBoolProperty(const char* property, BoolProperties::iterator& iter)
{
    static std::string s;
    s = property;
    iter = boolProperties_.find(s);
    if (boolProperties_.end() != iter)
    {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
//
bool SceneNode::GetBoolProperty(const char* property, bool& value)
{
    BoolProperties::iterator iter;
    if (FindBoolProperty("visibility", iter))
    {
        value = iter->second;
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
//
bool SceneNode::SetBoolProperty(const char* property, bool value)
{
    BoolProperties::iterator iter;
    if (FindBoolProperty("visibility", iter))
    {
        iter->second = value;
        return true;
    }
    return false;
}
//
} // end of sare
