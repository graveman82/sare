#ifndef SARE_MATERIALSYSTEM_H_
#define SARE_MATERIALSYSTEM_H_

#include <memory>
#include <map>
#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include "shared_constants.h"

namespace sare {

//-----------------------------------------------------------------------------

class MaterialSystem {
public:
    struct Shader {
        Shader();
        ~Shader();

        LPD3DXCONSTANTTABLE d3dx_constant_table;
        LPDIRECT3DVERTEXSHADER9 direct3d_vertex_shader9;
    };
    bool Initialize();
    bool CreateVertexShader(const char* shader_name);
    bool SetVertexShader(const char* shader_name);

private:
    std::map<std::string, Shader> vertex_shaders_;

};

} // end of sare
#endif // SARE_MATERIALSYSTEM_H_

