#ifndef SARE_KEY_TRIGGER_H_
#define SARE_KEY_TRIGGER_H_

#include <sdl.h>

namespace sare {

//-----------------------------------------------------------------------------
class PressKeyTrigger {
public:
    PressKeyTrigger(SDL_Keycode sdl_key_code = SDLK_UNKNOWN);
    void SetKeyCode(SDL_Keycode sdl_key_code);
    bool OnEvent(SDL_Keycode sdl_key_code, bool pressed);
    virtual void Frame() = 0;

protected:
    bool on() const { return on_; }

    bool on_;
    SDL_Keycode sdl_key_code_;
};

} // end of sare
#endif // SARE_KEY_TRIGGER_H_

