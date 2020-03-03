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
 * NAME:        fc.cpp
 * PROJECT:     fully_connected
 * Description: HLS implementation of the Fully Connected Layer
 *******************************************************************************/

#include "headers/defines.h"
#include "headers/activations.h"
#include <hls_video.h>


void fc(hls::stream<float> &out, hls::stream<float> &in, float weight[FC_WEIGHTS_H][FC_WEIGHTS_W], float bias[FC_BIAS_SIZE])
{
	float read;
	float output[FC_ACT_SIZE] = {0};
	for(int j = 0; j < FC_WEIGHTS_H; j++)
	{
		in>>read;
		for(int i = 0; i < FC_WEIGHTS_W; i++)
		{
			output[i] += weight[j][i] * read;
		}
	}
	for(int i = 0; i < FC_WEIGHTS_W; i++)
		out<< relu(output[i] + bias[i]);

}
