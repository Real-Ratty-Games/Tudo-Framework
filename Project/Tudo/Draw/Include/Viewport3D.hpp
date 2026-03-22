/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef VIEWPORT3D_HPP_
#define VIEWPORT3D_HPP_
#include "SystemTypes.hpp"

namespace Tudo
{
	/// +X is right
	/// +Y is up
	/// +Z is forward
	class Viewport3D
	{
	public:
		Viewport3D();
		void CreateView();
		const vec3& Forward() const;
		const vec3& Right() const;
		const mat4& View() const;
		const vec3& Up() const;

	public:
		float Fov;
		float Near; // =-1000 for ortho
		float Far;

		vec3 Eye;
		vec3 Target;
		vec3 SceneUp;

	private:
		mat4 mView;
		vec3 mForward;
		vec3 mRight;
		vec3 mUp;
	};

	class ViewportOrtho3D : public Viewport3D
	{
	public:
		float Offset	= 0.0f;
		float Bottom	= -5.0f;
		float Right		= 10.0f;
		float Left		= -10.0f;
		float Top		= 5.0f;
	};
}
#endif