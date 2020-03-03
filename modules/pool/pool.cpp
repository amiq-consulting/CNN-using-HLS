/******************************************************************************
 * (C) Copyright 2020 AMIQ Consulting
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * NAME:        pool.cpp
 * PROJECT:     pool
 * Description: HLS implementation of the MaxPooling Layer
 *******************************************************************************/

#include <hls_video.h>
#include "headers/defines.h"

#ifndef __SYNTHESIS__
#include <stdio.h>
#endif

#include "ap_fixed.h"

#define EXP_WIDTH	28
#define INT_WIDTH	10

typedef ap_fixed<EXP_WIDTH, INT_WIDTH> float24_t;

#define BUFFER_SIZE (P_SIZE*P_CHANNELS)

void pool(hls::stream<float24_t>& out, hls::stream<float24_t>& in)
{

	int i,j,k,l,m;
	float24_t read;
	hls::LineBuffer<BUFFER_SIZE,1,float24_t> pool_buff;

	for(i = 0 ; i < P_SIZE; i++)
		for(l = 0; l < P_KERNEL_SIZE; l++)
		{	for(j = 0 ; j < P_SIZE; j++)
				for(m = 0; m < P_KERNEL_SIZE; m++)
					for(k = 0 ; k < P_CHANNELS; k++)
					{
						in>>read;
						if(l == 0 && m == 0)
							pool_buff.val[j*P_CHANNELS + k][0] = read;

						else
							pool_buff.val[j*P_CHANNELS + k][0] = pool_buff.val[j*P_CHANNELS + k][0] > read? pool_buff.val[j*P_CHANNELS + k][0] : read;


						if(l == (P_KERNEL_SIZE - 1) && m == (P_KERNEL_SIZE - 1))
							out<<pool_buff.val[j*P_CHANNELS + k][0];
					}
		for(int skip = P_SIZE * P_STRIDE ; skip < A_SIZE; skip++)
			for(int channel = 0; channel < P_CHANNELS; channel++)
					in>>read;
	}


	for(int skip_row = P_SIZE * P_STRIDE ; skip_row < A_SIZE; skip_row++)
		for(int skip_col = 0 ; skip_col < A_SIZE; skip_col++)
			for(int skip_channel = 0 ; skip_channel < A_CHANNELS; skip_channel++)
				in>>read;
	int count = 0;
	while(!in.empty())
		{
			in>>read;
			count++;
		}
#ifndef __SYNTHESIS__
	printf("%d ",count);
#endif

}


