#include <Renderer.hpp>

static Renderer 			_Renderer;
static SpriteInstanceData	_SimpleTextData;
static Texture 				_ftex;
static Sprite* 				_spriteFont;
static SpriteFont 			_ffont;

bool Initialize()
{
	_Renderer.LoadTextureFromFile(_ftex, "Data/Font.png", BGFX_SAMPLER_MIN_POINT |
		BGFX_SAMPLER_MAG_POINT |
		BGFX_SAMPLER_U_CLAMP |
		BGFX_SAMPLER_V_CLAMP, "Font", false, false);
		
	_spriteFont = new Sprite(&_ftex);
	
	_ffont.pSprite = _spriteFont;
	_ffont.GlyphSize = vec2(18, 30);
	_ffont.Glyphs =
		"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
		"ÄäÖöÜü"
		"0123456789"
		",;.:_-!?\"§$%&/()=*+~'#|<>²³{[]}\\";
		
	Transform2D transf;
	transf.Location 	= vec2(100, 100);
	transf.Scale 		= vec2(1);
	transf.Rotation 	= 0.0f;
	transf.ImageColor 	= Color(1);
	_Renderer.PrepareSpriteFontText(_ffont, transf, _SimpleTextData, "Look at this cool box!\nPretty neat, right?");
}

void Draw()
{
	_Renderer.BeginDraw();
	if (!mWindow->IsIconified())
	{
		_Renderer.BeginDrawSprite(mBackBufferSurface, mCamera);
			_Renderer.SetActiveShader(&mSprite2DAtlasIShader);
			_Renderer.DrawSpriteFontText(_ffont, _SimpleTextData);
		_Renderer.EndDrawSprite();
	}
	_Renderer.EndDraw();	
}

void Cleanup()
{
	delete _spriteFont;
}
