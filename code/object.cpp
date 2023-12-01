#include "object.h"

namespace sare
{
//-----------------------------------------------------------------------------
//
Object::Object()
{

}

//-----------------------------------------------------------------------------
//
Object::~Object()
{

}

//-----------------------------------------------------------------------------
//
void Object::Rename(const char* new_name)
{
    name_ = new_name;
}

//-----------------------------------------------------------------------------
//
bool Object::NameContains(const char* text) const
{
    return name().find(text) != std::string::npos;
}
//
} // end of sare
