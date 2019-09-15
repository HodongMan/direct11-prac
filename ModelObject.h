#pragma once

class ModelObject : public AlignedAllocationPolicy<16>
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
};