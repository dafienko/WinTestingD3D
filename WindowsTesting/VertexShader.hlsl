struct VSOut {
	float3 color : Color;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 c : Color)
{
	VSOut vso;
	vso.pos = float4(pos, 1.0f);
	vso.color = c;
	
	return vso;
}