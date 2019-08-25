#pragma once

template<size_t Allignment>
class AlignedAllocationPolicy
{
public:
	static void* operator new( size_t size )
	{
		return _aligned_malloc( size, Allignment );
	}

	static void operator delete( void* memory )
	{
		_aligned_free( memory );
	}
};