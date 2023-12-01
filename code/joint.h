#ifndef SARE_JOINT_H_
#define SARE_JOINT_H_

#include <SDL.h>
#include "scene_node.h"

#include <string>


namespace sare {

class Joint : public SceneNode {
public:
    bool Load(SDL_RWops* file);

    std::string name_;
    int32_t parent_index_;
    uint32_t child_count_;
};

} // end of sare
#endif // SARE_JOINT_H_

