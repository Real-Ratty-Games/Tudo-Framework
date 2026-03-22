/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include "SystemTypes.hpp"
#include <vector>

#define M3DC_VERSION	100
#define M3DC_MAGIC		0x156AF

namespace Tudo
{
	struct M3DCHeader
	{
		uint64 Magic;
		uint64 Version;
		uint64 Amount;
	};

	/// When changing this, don't forget to change
	/// the vertex layout TUDO_RENDERER_M3D_VERTEXLAYOUT
	/// in GraphicsDevice.cpp.
	/// Also adapt in M3DC!
	struct MeshVertex
	{
		// Position
		float X;
		float Y;
		float Z;

		// Normal
		float NX;
		float NY;
		float NZ;

		// TexCoord
		float U;
		float V;

		// Tangent
		float TX;
		float TY;
		float TZ;

		// Bitangent
		float BX;
		float BY;
		float BZ;
	};

	struct MeshData
	{
		std::vector<MeshVertex>	Vertices;
		std::vector<uint16>		Indices;
		uint64					VSize;
		uint64					ISize;
	};
}
#endif