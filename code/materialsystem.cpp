#include "materialsystem.h"
#include <string.h>
#include "engine.h"
#include "shared_functions.h"
#include "log.h"

namespace sare {


//-----------------------------------------------------------------------------
MaterialSystem::Shader::Shader() :
    d3dx_constant_table(nullptr),
    direct3d_vertex_shader9(nullptr) {}

//-----------------------------------------------------------------------------
MaterialSystem::Shader::~Shader() {
    SafeReleaseCom(direct3d_vertex_shader9);
    SafeReleaseCom(d3dx_constant_table);
    Logger& logger = Logger::instance();
    LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Info);
    logger << "MaterialSystem::Shader::~Shader()\n";
}

//-----------------------------------------------------------------------------
bool MaterialSystem::CreateVertexShader(const char* shader_name) {

    std::string shader_path = "../Data/Shaders/";
    shader_path += shader_name;
    shader_path += ".txt";
    const char* shader_version = "";
    if (!strcmp("regular", shader_name)) {
        if (vertex_shaders_.find(shader_name) != vertex_shaders_.end()) {
            SARE_ASSERT_MSG(0, "Already created");
            return false;
        }

        shader_version = "vs_1_1";
    }
    if (!shader_version) {
        SARE_ASSERT_MSG(0, "Shader not found");
        return false;
    }

    LPD3DXBUFFER error_message = nullptr;
    LPD3DXBUFFER compiled_code = nullptr;
    HRESULT  hr = D3DXCompileShaderFromFile(shader_path.c_str(), nullptr, nullptr,
                                            "Main", shader_version, D3DXSHADER_DEBUG, &compiled_code,
        &error_message, &vertex_shaders_[shader_name].d3dx_constant_table);

    if (FAILED(hr)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "D3DXCompileShaderFromFile() failed!\n";
        logger << (const char*) error_message->GetBufferPointer() << "\n";
        SafeReleaseCom(error_message);
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal,
                                       kExternalApi_DirectX9, hr);
        return false;
    }
    SafeReleaseCom(error_message);
    hr = Engine::instance().direct3ddevice9()->CreateVertexShader(
        (DWORD*)compiled_code->GetBufferPointer(), &vertex_shaders_[shader_name].direct3d_vertex_shader9);
    SafeReleaseCom(compiled_code);
    if (FAILED(hr)) {
        Logger& logger = Logger::instance();
        LoggerAutoFlush laf = logger.StartMessage(Logger::kMessageType_Error);
        logger << "IDirect3DDevice9::CreateVertexShader() failed!\n";
        Engine::instance().HandleError(kErrorCode_ApiInternal, kErrorLevel_Fatal,
                                       kExternalApi_DirectX9, hr);
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool MaterialSystem::SetVertexShader(const char* shader_name) {
    std::map<std::string, Shader>::iterator it = vertex_shaders_.end();
    if (!strcmp("regular", shader_name)) {
        it = vertex_shaders_.find(shader_name);
        SARE_ASSERT(it != vertex_shaders_.end());
    }
    else {
        SARE_ASSERT_MSG(0, "Shader not found");
        return false;
    }
    Shader& shader = it->second;
    D3DXHANDLE world_view_proj_matrix_handle = shader.d3dx_constant_table->GetConstantByName(
        0, "world_view_proj_matrix");
    D3DXHANDLE world_matrix_handle = shader.d3dx_constant_table->GetConstantByName(
        0, "world_matrix");
    D3DXHANDLE ambient_color_handle = shader.d3dx_constant_table->GetConstantByName(
        0, "ambient_color");
    D3DXHANDLE specular_exp_handle = shader.d3dx_constant_table->GetConstantByName(
        0, "specular_exp");
    D3DXHANDLE material_coef_handle = shader.d3dx_constant_table->GetConstantByName(
        0, "material_coef");
    D3DXHANDLE light0_dir_handle = shader.d3dx_constant_table->GetConstantByName(
        0, "light0_dir");
    D3DXHANDLE light0_camera_half_handle = shader.d3dx_constant_table->GetConstantByName(
        0, "light0_camera_half");

    LPDIRECT3DDEVICE9 direct3ddevice9 = Engine::instance().direct3ddevice9();
    Camera& camera = Engine::instance().camera();

    D3DXMATRIX world_matrix;
    D3DXMatrixIdentity(&world_matrix);
    shader.d3dx_constant_table->SetMatrix(direct3ddevice9, world_matrix_handle,
                                            &world_matrix);

    D3DXMATRIX world_view_proj_matrix;
    D3DXMatrixMultiply(&world_view_proj_matrix, &world_matrix, &camera.view_matrix());
    D3DXMatrixMultiply(&world_view_proj_matrix, &world_view_proj_matrix, &camera.projection_matrix());
    shader.d3dx_constant_table->SetMatrix(direct3ddevice9, world_view_proj_matrix_handle,
                                            &world_view_proj_matrix);
    // Направление на источник света:
    D3DXVECTOR4 light0_dir(-0.3f, 0.57f, -0.3f, 0.0f);
    shader.d3dx_constant_table->SetVector(direct3ddevice9, light0_dir_handle,
                                            &light0_dir);

    D3DXVECTOR4 light0_camera_half(camera.eye().x, camera.eye().y, camera.eye().z, 0.0f);
    light0_camera_half += light0_dir;
    D3DXVec4Normalize(&light0_camera_half, &light0_camera_half);
    shader.d3dx_constant_table->SetVector(direct3ddevice9, light0_camera_half_handle,
                                            &light0_camera_half);

     // Материалы:
    D3DXVECTOR4 ambientMtrl(0.05f, 0.05f, 0.05f, 1.0f);
    D3DXVECTOR4 diffuseMtrl(0.7f, 0.7f, 0.7f, 1.0f);
    shader.d3dx_constant_table->SetVector(direct3ddevice9, ambient_color_handle, &ambientMtrl);
    shader.d3dx_constant_table->SetVector(direct3ddevice9, material_coef_handle, &diffuseMtrl);
    float specular_exp = 2.0f;
    shader.d3dx_constant_table->SetFloat(direct3ddevice9, specular_exp_handle, specular_exp);

    direct3ddevice9->SetVertexShader(shader.direct3d_vertex_shader9);
    return true;
}

//-----------------------------------------------------------------------------
bool MaterialSystem::Initialize() {
    if (!CreateVertexShader("regular")) {
        return false;
    }
    return true;
}

} // end of sare
