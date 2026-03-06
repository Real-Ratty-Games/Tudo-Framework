
#include <Renderer.hpp>
#include <DrawSurface3D.hpp>
#include <Viewport3D.hpp>
#include <DrawMath.hpp>

static Shader			_MeshShader;
static DrawSurface3D*	_3DSurface;
static Viewport3D 		_viewport;
static Model3D 			_model;
static quat 			_rot = Math::QuatFromAxisAngle(vec3(), 0);
static Texture 			_tex;
static Sprite* 			_3dsprite;

void OnResize(vec2i& size)
{
	_3DSurface->OnResize(size);
}

bool Initialize()
{
	_3DSurface 	= new DrawSurface3D(1, resolution, nullptr, false);
	_3dsprite 	= new Sprite(&_3DSurface->GetTexture());
	
	_MeshShader.Initialize("Mesh3D");
	_MeshShader.InitUniform("s_texColor", bgfx::UniformType::Sampler);
	
	_viewport.CreateView();
	
	Renderer::LoadModelFromFile(_model, "box.m3d");
	
	Renderer::LoadTextureFromFile(_tex, "box.png", BGFX_SAMPLER_MIN_POINT |
		BGFX_SAMPLER_MAG_POINT |
		BGFX_SAMPLER_U_CLAMP |
		BGFX_SAMPLER_V_CLAMP, "tex", false, true);
}

void Tick()
{
	EulerRotation er;
	
	if (Keyboard::KeyDown(KeyboardKey::SPACE))
	{
		er.Pitch = Math::ToRadians(1.0f);
		_rot = Math::QuatAddEulerRotation(_rot, er, false);
	}
	else if (Keyboard::KeyDown(KeyboardKey::X))
	{
		er.Yaw = Math::ToRadians(1.0f);
		_rot = Math::QuatAddEulerRotation(_rot, er, false);
	}
	else if (Keyboard::KeyDown(KeyboardKey::Y))
	{
		er.Roll = Math::ToRadians(1.0f);
		_rot = Math::QuatAddEulerRotation(_rot, er, false);
	}
}

void Draw()
{
	Renderer::BeginDraw();
	if (!mWindow->IsIconified())
	{
		Renderer::BeginDrawMesh(_3DSurface, _viewport);
			Renderer::SetActiveShader(&_MeshShader);
			
			mat4 mdl 	= mat4::Identity();
			mdl 		= Math::Translate(mdl, vec3(0.0f, -5.0f, 10.0f), false);
			mdl 		= Math::Scale(mdl, vec3(10.0f), false);
			mdl 		= Math::Rotate(mdl, _rot, false);
			Renderer::SetTransform(mdl);
			
			for (auto& it : _model.Meshes)
			{
				Renderer::SetState(
					BGFX_STATE_WRITE_RGB
					| BGFX_STATE_WRITE_A
					| BGFX_STATE_WRITE_Z
					| BGFX_STATE_DEPTH_TEST_LESS
					| BGFX_STATE_BLEND_ALPHA
					| BGFX_STATE_CULL_CCW
				);

				Renderer::SetMesh(0, it);
				Renderer::GetActiveShader()->SetTexture(0, "s_texColor", _tex);
				Renderer::DrawMesh();
			}
		Renderer::EndDrawMesh();
		
		Renderer::BeginDrawSprite(mBackBufferSurface, mCamera);
			Renderer::SetActiveShader(&mSprite2DShader);
			
			Transform2D transf;
			Renderer::DrawSprite(_3dsprite, transf);
		Renderer::EndDrawSprite();
	}
	Renderer::EndDraw();
}

void Cleanup()
{
	Renderer::FreeModel(_model);
	Renderer::FreeTexture(_tex);
	
	if(_3dsprite != nullptr)
		delete _3dsprite;
	
	if (_3DSurface != nullptr)
	{
		_3DSurface->Release();
		delete _3DSurface;
	}
	
	_MeshShader.Release();
}
