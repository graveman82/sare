#ifndef SARE_OBJECT_H
#define SARE_OBJECT_H

#include <string>

namespace sare
{

class Object
{
public:

    Object();
    virtual ~Object();

    const std::string& name() const { return name_; }
    const char* c_name() const { return name_.c_str(); }
    void Rename(const char* new_name);
    bool NameContains(const char* text) const;
protected:
    std::string name_;
};

//
} // end of sare

#endif // SARE_OBJECT_H

