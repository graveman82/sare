#include "key_trigger.h"

namespace sare {

//-----------------------------------------------------------------------------
PressKeyTrigger::PressKeyTrigger(SDL_Keycode sdl_key_code) :
    on_(false), sdl_key_code_(sdl_key_code) {}

//-----------------------------------------------------------------------------
void PressKeyTrigger::SetKeyCode(SDL_Keycode sdl_key_code) { sdl_key_code_ = sdl_key_code; }

//-----------------------------------------------------------------------------
bool PressKeyTrigger::OnEvent(SDL_Keycode sdl_key_code, bool pressed) {
    if (sdl_key_code_ != sdl_key_code)
        return false;
    if (!on() && pressed) {
        on_ = true;
        return true;
    }
    else if (on() && !pressed) {
        on_ = false;
        return true;
    }
    return false;
}

} // end of sare
