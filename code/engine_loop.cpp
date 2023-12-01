#include "engine.h"

namespace sare {

//-----------------------------------------------------------------------------
bool Engine::MainLoop() {
    bool to_quit_loop = false;
    SDL_Event event;

    while (!to_quit_loop) {
        event_handling_start_time_ = SDL_GetTicks();
        //Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                to_quit_loop = true;
            }
            //User presses a key
            else {
                HandleEvent(event);
            }

        }
        if (!to_quit_loop) {
            frame_start_time_ = SDL_GetTicks();
            uint32_t event_handling_time = frame_start_time_ - event_handling_start_time_;
            if (!Frame()) {
                return false;
            }
            uint32_t sleep_time = (SDL_GetTicks() - frame_start_time_) + event_handling_time;
            if (sleep_time < update_period_) {
                SDL_Delay(update_period_ - sleep_time);
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Engine::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN ||
        event.type == SDL_KEYUP ||
        event.type == SDL_MOUSEMOTION) {

        if (event.key.keysym.sym == SDLK_c && event.type == SDL_KEYDOWN) {
            SDL_SetRelativeMouseMode((SDL_bool)!SDL_GetRelativeMouseMode());
        }
        else if (event.type == SDL_KEYDOWN) {
            camera_.GetMoveTrigger(kDirection_Front).OnEvent(event.key.keysym.sym, true);
            camera_.GetMoveTrigger(kDirection_Back).OnEvent(event.key.keysym.sym, true);
            camera_.GetMoveTrigger(kDirection_Right).OnEvent(event.key.keysym.sym, true);
            camera_.GetMoveTrigger(kDirection_Left).OnEvent(event.key.keysym.sym, true);
            camera_.GetMoveTrigger(kDirection_Up).OnEvent(event.key.keysym.sym, true);
            camera_.GetMoveTrigger(kDirection_Down).OnEvent(event.key.keysym.sym, true);
        }
        else if (event.type == SDL_KEYUP) {
            camera_.GetMoveTrigger(kDirection_Front).OnEvent(event.key.keysym.sym, false);
            camera_.GetMoveTrigger(kDirection_Back).OnEvent(event.key.keysym.sym, false);
            camera_.GetMoveTrigger(kDirection_Right).OnEvent(event.key.keysym.sym, false);
            camera_.GetMoveTrigger(kDirection_Left).OnEvent(event.key.keysym.sym, false);
            camera_.GetMoveTrigger(kDirection_Up).OnEvent(event.key.keysym.sym, false);
            camera_.GetMoveTrigger(kDirection_Down).OnEvent(event.key.keysym.sym, false);
        }
        else if (event.type == SDL_MOUSEMOTION) {
            camera_.mouse_rotator().OnMouseMotion(event.motion.xrel, event.motion.yrel);
        }

    }
    return true;
}

//-----------------------------------------------------------------------------
bool Engine::Frame() {
    camera_.Frame();
    if (!Draw()) {
        return false;
    }
    return true;
}

} // end of sare
