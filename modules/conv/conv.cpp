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
 * NAME:        conv.cpp
 * PROJECT:     conv
 * Description: HLS implementation of the Convolutional Layer
 *******************************************************************************/

#include "headers/defines.h"
#include "headers/activations.h"
#include "headers/buffer.h"
#include <hls_video.h>

#ifndef __SYNTHESIS__
#include <stdio.h>
#endif



#include "ap_fixed.h"

#define FLOAT_WIDTH		24
#define INT_WIDTH	4

typedef ap_fixed<FLOAT_WIDTH, INT_WIDTH> float24_t;


#define BUFFER_SIZE (IMAGE_SIZE * IMAGE_CHANNELS * (CONV_KERNEL_SIZE -1) + CONV_KERNEL_SIZE * IMAGE_CHANNELS)

void conv(hls::stream<float24_t> &out, hls::stream<float24_t> &in, float24_t weight[CONV_KERNEL_SIZE][CONV_KERNEL_SIZE][CONV_CHANNELS][CONV_FILTERS], float24_t bias[CONV_BIAS_SIZE])
{
	int i,j,k,filter, contor = 0;
	float24_t sum, placeholder;
	int row_offset, col_offset, channel_offset;
	buffer<BUFFER_SIZE> conv_buff;

	/// DEBUG
	int attempted_reads1 = 0, reads1 = 0;
	int attempted_reads2 = 0, reads2 = 0;
	int attempted_reads3 = 0, reads3 = 0;

	/*
	 * Read the initial buffer
	 * */

	for(i = 0; i < BUFFER_SIZE;i++)
	{
		attempted_reads1++;
		if(in.empty() == 1)
		{
			#ifndef __SYNTHESIS__
			printf("\nInput stream is empty at \"Initial read state\"");
			#endif
		}
		else
		{
			in>>placeholder;
			conv_buff.InsertBack(placeholder);
			reads1++;
		}
	}

	conv_label5:for(i = 0; i < (IMAGE_SIZE - CONV_KERNEL_SIZE + 1); i += CONV_STRIDE)
	conv_label4:for(j = 0; j < (IMAGE_SIZE - CONV_KERNEL_SIZE + 1); j += CONV_STRIDE)
	{


	conv_label3:for(filter = 0; filter < CONV_FILTERS; filter++)
	{
		sum = 0;
		conv_label2:for(row_offset = 0; row_offset <CONV_KERNEL_SIZE; row_offset++)
			conv_label1:for(col_offset = 0; col_offset <CONV_KERNEL_SIZE; col_offset++)
				conv_label0:for(channel_offset = 0; channel_offset < CONV_CHANNELS; channel_offset++)
					sum += conv_buff.GetValue(row_offset*IMAGE_SIZE * IMAGE_CHANNELS +  col_offset * IMAGE_CHANNELS + channel_offset) * weight[row_offset][col_offset][channel_offset][filter];
		out<<relu(sum + bias[filter]);
	}

	if((j + CONV_STRIDE < (IMAGE_SIZE - CONV_KERNEL_SIZE + 1)))
		for(int p = 0 ; p<IMAGE_CHANNELS; p++)
		{
			attempted_reads3++;
			if(in.empty() == 1)
			{
				#ifndef __SYNTHESIS__
				printf("\nInput stream is empty at \"Next element state\"");
				#endif
			}
			else
			{
				reads3++;
				in>>placeholder;
				conv_buff.InsertBack(placeholder);
			}
		}
	else
		if((i + CONV_STRIDE < (IMAGE_SIZE - CONV_KERNEL_SIZE + 1)) && (j + CONV_STRIDE >= (IMAGE_SIZE - CONV_KERNEL_SIZE + 1)))
			for(int p = 0 ; p<CONV_KERNEL_SIZE * IMAGE_CHANNELS; p++)
			{
				attempted_reads2++;
				if(in.empty() == 1)
				{
					#ifndef __SYNTHESIS__
					printf("\nInput stream is empty at \"Endline\"");
					#endif
				}
				else
				{
					reads2++;
					in>>placeholder;
					conv_buff.InsertBack(placeholder);
				}
			}


	}
	#ifndef __SYNTHESIS__
	printf("\n===========");
	printf("\nStatistics:");
	printf("\n===========");

	printf("\nInitial buffer atempted to read %d and succeeded %d", attempted_reads1, reads1);
	printf("\nEndline buffer atempted to read %d and succeeded %d", attempted_reads2, reads2);
	printf("\nNextelm buffer atempted to read %d and succeeded %d", attempted_reads3, reads3);
	#endif

}
