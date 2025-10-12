#pragma once

#include "RenderPass.hpp"
#include "DoubleTexture2D.hpp"
#include "DoubleTexture3D.hpp"
#include "Texture2D.hpp"
#include "Texture3D.hpp"

// Might want to make 

struct NodeTexture2DWrite {
};

struct NodeTexture2DRead {
};

struct NodeTexture2DReadLamda
{

};

struct NodeDrawTriangles {
	RenderPass* render_pass;

};