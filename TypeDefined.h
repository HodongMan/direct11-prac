#pragma once

#include "pch.h"

#define KeyTypeLength			256
#define FULL_SCREEN				false
#define VSYNC_ENABLE			true
#define SCREEN_DEPTH			1000.0f
#define SCREEN_NEAR				0.1f

#define VERTEX_COUNT			3
#define INDEX_COUNT				3



using DeviceContextComPtr = Microsoft::WRL::ComPtr<ID3D11DeviceContext>;