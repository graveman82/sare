#ifndef SARE_FILE_UTIL_H_
#define SARE_FILE_UTIL_H_

#include <string>
#include <d3dx9math.h>
#include <sdl.h>

#include "vertex.h"

namespace sare {

bool Uint16_Load(uint16_t& u, SDL_RWops* file);
bool Uint32_Load(uint32_t& u, SDL_RWops* file);
bool StdString_Load(std::string& str, SDL_RWops* file);
bool Matrix4x4_Load(D3DXMATRIX& matrix, SDL_RWops* file);
bool Vector2D_Load(D3DXVECTOR2& v, SDL_RWops* file);
bool Vector3D_Load(D3DXVECTOR3& v, SDL_RWops* file);
bool Vector4D_Load(D3DXVECTOR4& v, SDL_RWops* file);
bool D3dxVector3Ex_Load(D3dxVector3Ex* v, uint32_t count, SDL_RWops* file);
bool VertexArray_Load(Vertex* v, uint32_t count, SDL_RWops* file);
bool Uint16Array_Load(uint16_t* v, uint32_t count, SDL_RWops* file);

} // end of sare
#endif // SARE_FILE_UTIL_H_
