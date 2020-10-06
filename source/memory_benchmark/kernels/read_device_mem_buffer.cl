// Copyright (c) 2009-2020 Intel Corporation
// All rights reserved.
//
// WARRANTY DISCLAIMER
//
// THESE MATERIALS ARE PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THESE
// MATERIALS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Intel Corporation is the author of the Materials, and requests that all
// problem reports or change requests be submitted to it directly


__kernel void ClearCaches(__global unsigned int* pBuffer, unsigned int buffSize )
{
	unsigned int i = 0;
	while( i++ < buffSize ) {
		*(pBuffer++) = 0xDEADBEAF;
	}
}
#define VECTOR_SIZE 4
#define SEND_SIZE	(sizeof(float)*VECTOR_SIZE)
#define THREAD_TILE_SIZE 2048
#define SUBGROUP_SIZE 8

__attribute__((intel_reqd_sub_group_size(SUBGROUP_SIZE)))
__kernel void ReadOnly (__global float4* const pSrcBuffer, 
	__global float4* pDstBuffer, 
	unsigned int sliceMask, 
	unsigned int sliceSize, 
	unsigned int slotMask)
{
	const uint	gid			= get_global_id(0); 
	const uint 	sid			= ((get_group_id(0) * get_num_sub_groups() ) + get_sub_group_id()) % slotMask ;
	uint		startOffset	=  0;

	float4 _out_data = { 0.0f, 0.0f, 0.0f, 0.0f };
	float4 _input_data;
	uint j = 0, i = 0;
	
	for(j = 0; j < KERNEL_LOOP_ITERATIONS; j++)
	{
		i = 0; 
		uint slotOffset = sliceSize * ( j & sliceMask);
		startOffset	=  (((THREAD_TILE_SIZE * sid)  + slotOffset ) / SEND_SIZE );
		
		__attribute__((opencl_unroll_hint(NUM_SENDS)))
		do
		{ 
			_input_data = as_float4(intel_sub_group_block_read4((__global const uint *)((__global uint* const)(&pSrcBuffer[startOffset]) )));
			_out_data += _input_data;
			startOffset += SUBGROUP_SIZE;
		}
		while(++i < NUM_SENDS );
	}
#if 0
	//if(gid == 0 )	
	//if(gid == 0 || gid == ((1024*1024)-8) || gid == (1024*1024) || gid == ((1024*1024)+8) || ( sid == 167 && (gid % 8 ) == 0  && gid < 4006 )  )	
	//if(gid == 0 || gid == 8 || gid == 64 || gid == 56  || gid == 1336 || gid == 1344 || gid == 16376 )
	if(gid %8 == 0 )
	//if(gid == 0 || gid == 7 || gid == 8 || gid == 15 )	
	{
		j = 0;
		uint slotOffset = sliceSize * ( j & sliceMask);
		startOffset	=  (((THREAD_TILE_SIZE * sid)  + slotOffset ) / ( sizeof(float) * 4 ) );
		//(get_group_id(0) * 2 + get_sub_group_id()) * 8 * 4;
		printf("\n  gid %d, sid %d ,group %d, sub %d, sliceM %x, sSize %x, iterMask %x, i = %d, j=%d, numS = %d, numL = %d, Odata %f, Odata[3] %f, inD %f, inD %f, Offset %x\n",  gid, sid,get_group_id(0), get_sub_group_id()  , 
			sliceMask, sliceSize, slotMask,i, j, NUM_SENDS, KERNEL_LOOP_ITERATIONS, _out_data[0], _out_data[3], _input_data[0], _input_data[3], startOffset );
		
		//printf("Size %X, num groups %x, max Size %x", get_sub_group_size(), get_num_sub_groups(), get_max_sub_group_size() );
		//startOffset += (SEND_SIZE/sizeof(float));
		//printf("new off %x  ", startOffset);
		//startOffset += (SEND_SIZE/sizeof(float));
		//printf("new off %x  \n", startOffset);
		j++; slotOffset = sliceSize * ( j & sliceMask);  startOffset	=  (((THREAD_TILE_SIZE * sid)  + slotOffset ) / ( sizeof(float) * 4 ) ); printf("gid %d ,j %d,  new off %x  \n", gid, j,startOffset);
		_input_data = as_float4(intel_sub_group_block_read4((__global const uint *)((__global uint* const)(&pSrcBuffer[startOffset]) )));
		printf("gid %d , j %d, new off %x  inD[0] = %f, inD[3] = %f \n", gid, j,startOffset, _input_data[0], _input_data[3]);
		j=15;slotOffset = sliceSize * ( j & sliceMask);  startOffset	=  (((THREAD_TILE_SIZE * sid)  + slotOffset ) / ( sizeof(float) * 4 ) ); printf("gid %d , j %d, new off %x  \n", gid, j,startOffset);
		j=16;slotOffset = sliceSize * ( j & sliceMask);  startOffset	=  (((THREAD_TILE_SIZE * sid)  + slotOffset ) / ( sizeof(float) * 4 ) ); printf("gid %d , j %d, new off %x  \n", gid, j,startOffset);
	}
#endif
	if (_out_data.x < 0.0f)											// Prevent compiler optimization from throwing away read
	{
		pDstBuffer[gid] = _out_data;									// Write data out	
	}	
}