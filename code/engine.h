#ifndef SARE_ENGINE_H_
#define SARE_ENGINE_H_

#include "singleton.h"

#include <sdl.h>
#include <d3d9.h>
#include <memory>

#include "error.h"
#include "materialsystem.h"
#include "external_api.h"
#include "color.h"
#include "camera.h"
#include "mesh.h"
#include "world.h"

namespace sare {

//-----------------------------------------------------------------------------

class Engine : public Singleton<Engine> {
public:
    Engine();
    ~Engine();
    void Run(int argc, char* args[]);
    bool HandleError(ErrorCode error_code, ErrorLevel error_level,
                     ExternalApi external_api = kExternalApi_None,
                     long api_error_code = S_OK);

    operator bool() const;
    LPDIRECT3D9 direct3d9();
    LPDIRECT3DDEVICE9 direct3ddevice9();
    bool CanVertexShaderVersion(uint16_t major, uint16_t minor) const;
    bool CanPixelShaderVersion(uint16_t major, uint16_t minor) const;
    const D3DPRESENT_PARAMETERS& present_parameters() const { return present_parameters_; }
    uint32_t update_period() const { return update_period_; }
    MaterialSystem& material_system();
    Camera& camera();

private:
    bool Initialize();
    bool InitializeSdl();
    bool InitializeRender();
    bool InitializeWorld();
    void Terminate();
    void TerminateWorld();
    bool MainLoop();
    bool Frame();
    bool HandleEvent(const SDL_Event& event);

    bool Draw();
    bool DrawWorld();
    bool BeginRenderFrame();
    bool EndRenderFrame();
    bool RenderPresent();
    bool ClearBackBuffer(uint32_t clear_flags, RgbaColori color, float z, int stencil);

    SDL_Window* sdl_window_;
    SDL_Surface* sdl_screen_surface_;
    LPDIRECT3D9 direct3d9_;
    LPDIRECT3DDEVICE9 direct3ddevice9_;
    D3DCAPS9 d3d_caps9_;
    D3DDISPLAYMODE d3d_display_mode_;
    D3DPRESENT_PARAMETERS present_parameters_;

    uint32_t event_handling_start_time_;
    uint32_t frame_start_time_;
    bool in_render_frame_;
    uint32_t update_period_;

    MaterialSystem material_system_;
    std::shared_ptr<Mesh> mesh_;
    Camera camera_;
    World world_;
};

} // end of sare
#endif // SARE_ENGINE_H_

