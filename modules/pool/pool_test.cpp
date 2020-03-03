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
 * NAME:        pool_test.cpp
 * PROJECT:     pool
 * Description: Basic test that sends an image to the MaxPooling Layer
 *              and checks that the output matches the python reference
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <hls_video.h>
#include "headers/defines.h"

#define eps 0.0002


#include "ap_fixed.h"

#define EXP_WIDTH	28
#define INT_WIDTH	10

typedef ap_fixed<EXP_WIDTH, INT_WIDTH> float24_t;


void pool(hls::stream<float24_t>& out, hls::stream<float24_t>& in);

int main()
{
	float24_t pool_out[P_SIZE * P_SIZE * P_CHANNELS];
	float24_t pool_ref[P_SIZE][P_SIZE][P_CHANNELS];
	float24_t image[A_SIZE * A_SIZE * A_CHANNELS];
	float a;

	int i,j,k;
	int correct_values = 0, total_values = 0;
	hls::stream<float24_t> out("output_stream");
	hls::stream<float24_t> in("input_stream");

	FILE* conv_content = fopen("../../../../ref/conv_py.out","r");
	if(conv_content == NULL)
	{
		printf("Couldn't open ../../../../ref/conv_py.out");
		exit(1);
	}

	FILE* pool_content = fopen("../../../../ref/pool_py.out","r");
	if(pool_content == NULL)
	{
		printf("Couldn't open ../../../../ref/pool_py.out");
		exit(1);
	}

	for(k = 0; k < A_CHANNELS; k++)
		for(i = 0; i < A_SIZE; i++)
			for(j = 0; j < A_SIZE; j++)
			{
				fscanf(conv_content,"%f",&a);
				image[i * A_SIZE * A_CHANNELS + j * A_CHANNELS + k] = (float24_t)a;
			}
	for(k = 0; k < P_CHANNELS; k++)
		for(i = 0; i < P_SIZE; i++)
			for(j = 0; j < P_SIZE; j++)
			{
				fscanf(pool_content,"%f",&a);
				pool_ref[i][j][k] = (float24_t) a;
			}

	/// Send stream
	for(i = 0 ; i < A_SIZE * A_SIZE * A_CHANNELS; i++)
			in<<image[i];

	pool(out, in);

	/// Receive stream
	for(i = 0 ; i < P_SIZE * P_SIZE * P_CHANNELS; i++)
			out>>pool_out[i];

	printf("Checking POOL  ...\n");

	for(k = 0; k < P_CHANNELS; k++)
		for(i = 0; i < P_SIZE; i++)
			for(j = 0; j < P_SIZE; j++)
			{
				total_values++;
				if(pool_out[i * P_SIZE * P_CHANNELS + j * P_CHANNELS + k] - pool_ref[i][j][k] > eps || pool_ref[i][j][k] - pool_out[i * P_SIZE * P_CHANNELS + j * P_CHANNELS + k] > eps)
				{
					if(correct_values + 1 == total_values)
						printf("Missmatch in POOL check\n");
				}
				else
					correct_values++;
			}
	printf("DONE: %d out of %d are correct\n\n", correct_values, total_values);

	return 0;
}

