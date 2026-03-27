/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Viewport3D.hpp"
#include "Projection.hpp"

using namespace Tudo;

Viewport3D::Viewport3D()
{
	Fov		= 60.0f;
	Near	= 0.001f;
	Far		= 1000.0f;
	Target	= vec3(0.0f, 0.0f, 0.0f);
	Eye		= vec3(0.0f, 0.0f, 1.0f);
	SceneUp = vec3(0.0f, 1.0f, 0.0f);
}

void Viewport3D::CreateView()
{
	mView = Math::LookAtLH(Eye, Target, SceneUp, mForward, mRight, mUp);
}

const vec3& Viewport3D::Forward() const
{
	return mForward;
}

const vec3& Viewport3D::Right() const
{
	return mRight;
}

const mat4& Viewport3D::View() const
{
	return mView;
}

const vec3& Viewport3D::Up() const
{
	return mUp;
}
