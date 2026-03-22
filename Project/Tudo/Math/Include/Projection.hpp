/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef PROJECTION_HPP_
#define PROJECTION_HPP_
#include "SystemTypes.hpp"

namespace Tudo::Math
{
	template<typename T>
	Matrix4<T> LookAtLH(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& sup,
		Vector3<T>& forward, Vector3<T>& right, Vector3<T>& up)
	{
		// forward
		Vector3<T> zaxis = target - eye;
		zaxis = zaxis.Normalized();

		// right
		Vector3<T> xaxis = Vector3<T>::Cross(sup, zaxis);
		xaxis = xaxis.Normalized();

		// up
		Vector3<T> yaxis = Vector3<T>::Cross(zaxis, xaxis);

		forward = zaxis;
		right = xaxis;
		up = yaxis;

		return mat4(
			xaxis.X, yaxis.X, zaxis.X, 0,
			xaxis.Y, yaxis.Y, zaxis.Y, 0,
			xaxis.Z, yaxis.Z, zaxis.Z, 0,
			-Vector3<T>::Dot(xaxis, eye),
			-Vector3<T>::Dot(yaxis, eye),
			-Vector3<T>::Dot(zaxis, eye),
			1
		);
	}

	template<typename T>
	Matrix4<T> LookAtLH(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& sup)
	{
		// forward
		Vector3<T> zaxis = target - eye;
		zaxis = zaxis.Normalized();

		// right
		Vector3<T> xaxis = Vector3<T>::Cross(sup, zaxis);
		xaxis = xaxis.Normalized();

		// up
		Vector3<T> yaxis = Vector3<T>::Cross(zaxis, xaxis);

		return mat4(
			xaxis.X, xaxis.Y, xaxis.Z, 0,
			yaxis.X, yaxis.Y, yaxis.Z, 0,
			zaxis.X, zaxis.Y, zaxis.Z, 0,
			-Vector3<T>::Dot(xaxis, eye),
			-Vector3<T>::Dot(yaxis, eye),
			-Vector3<T>::Dot(zaxis, eye),
			1
		);
	}

	template<typename T>
	Matrix4<T> ProjectOrthoLH(T left, T right, T bottom, T top, T near, T far, T offset)
	{
		Matrix4<T> result = Matrix4<T>::Identity();

		const T rl = right - left;
		const T tb = top - bottom;
		const T fn = far - near;

		if (rl == 0 || tb == 0 || fn == 0) return result;

		const T invRl = 1 / rl;
		const T invTb = 1 / tb;
		const T invFn = 1 / fn;

		result(0, 0)	= 2 * invRl;
		result(1, 1)	= 2 * invTb;
		result(2, 2)	= 1 * invFn;
		result(3, 0)	= -(right + left) * invRl;
		result(3, 1)	= -(top + bottom) * invTb;
		result(3, 2)	= -near * invFn + offset;
		result(3, 3)	= 1;

		return result;
	}

	template<typename T>
	Matrix4<T> ProjectPerspLH(T fov, T aspect, T near, T far)
	{
		Matrix4<T> result = Matrix4<T>::Identity();

		T tanHalfFov = std::tan(fov / 2);

		result(0,0) = 1 / (aspect * tanHalfFov);
		result(1,1) = 1 / tanHalfFov;
		result(2,2) = far / (far - near);
		result(2,3) = 1;
		result(3,2) = -near * far / (far - near);
		result(3,3) = 0;

		return result;
	}
}
#endif