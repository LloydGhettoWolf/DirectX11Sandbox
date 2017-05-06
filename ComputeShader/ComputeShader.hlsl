struct Value
{
	float val;
};

StructuredBuffer<Value> Buffer0: register(t0);
RWStructuredBuffer<Value> BufferOut: register(u0);

[numthreads(100, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	BufferOut[DTid.x].val = 10.0f;
}