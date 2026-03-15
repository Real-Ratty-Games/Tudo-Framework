/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef WINDOW_HPP_
#define WINDOW_HPP_
#include "SystemTypes.hpp"
#include <SDL3/SDL.h>

namespace Tudo
{
	class WindowCursor;

	/// <summary>
	/// Managing window creation and cleanup
	/// </summary>
	class Window
	{
	public:
		static void Initialize();
		static void Release();
		static void ShowSplashScreen(strgv filename = "Data/Splash.bmp");
		static void DestroySplashScreen();
		static bool IsSplashScreenVisible();
		static bool IsSDLInit();

		static void SetHardwareCursorImage(WindowCursor* cursor);
		static void ShowMessageBox(SDL_MessageBoxFlags flags, strgv header, strgv message, Window* window = nullptr);

		void Create(strgv title, uint width, uint height, bool fs);
		void Show(bool vl = true);
		void PollEvent();
		void Destroy();

		bool IsIconified();

		void SetSize(uint width, uint height);
		void Center();
		void SetFullscreen(bool vl);
		void SwitchFullscreen();

		vec2i GetSize();
		bool GetFullscreen();

		/// <summary>
		/// Returns native OS window handle
		/// </summary>
		/// <returns></returns>
		void* GetNativePtr();

	protected:
		virtual void EventCallback() = 0;

	protected:
		SDL_Window* mWndHandle;
		SDL_Event	mWndEvent;

		bool bIconified = false;

		static SDL_Window*		sSplashWndHandle;
		static SDL_Renderer*	sSplashWndRenderer;
		static bool				sbSplashWndVisible;

	private:
		static bool sbSDLInit;

		bool bFullscreen = false;
	};
}
#endif
