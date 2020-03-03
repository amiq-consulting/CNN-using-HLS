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
 * NAME:        fc_test.cpp
 * PROJECT:     fully_connected
 * Description: Basic test that sends an image to the Fully Connected Layer
 *              and checks that the output matches the python reference
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "headers/defines.h"
#include "headers/weights.h"
#include <hls_video.h>

#define eps 0.00002

void fc(hls::stream<float> &out, hls::stream<float> &in, float weight[FC_WEIGHTS_H][FC_WEIGHTS_W], float bias[FC_BIAS_SIZE]);

int main()
{

	float input[FLATTEN_SIZE];
	float fc_out[FC_ACT_SIZE];
	float fc_ref[FC_ACT_SIZE];

	int i;
	int correct_values = 0, total_values = 0;

	hls::stream<float> out;
	hls::stream<float> in;

	FILE* flatten_content = fopen("../../../../ref/flatten_py.out","r");
	if(flatten_content == NULL)
	{
		printf("Couldn't open ../../../../ref/flatten_py.out");
		exit(1);
	}

	FILE* fc_content = fopen("../../../../ref/fc_py.out","r");
	if(fc_content == NULL)
	{
		printf("Couldn't open ../../../../ref/fc_py.out");
		exit(1);
	}


    for(i = 0; i < FLATTEN_SIZE; i++)
    	fscanf(flatten_content,"%f",&input[i]);

    for(i = 0; i < FC_ACT_SIZE; i++)
    	fscanf(fc_content,"%f",&fc_ref[i]);

    for(i = 0; i < FLATTEN_SIZE; i++)
    	in<<input[i];

	fc(out, in, fc_weights, fc_bias);

    for(i = 0; i < FC_ACT_SIZE; i++)
    	out>>fc_out[i];
    printf("Checking FC Layer ...\n");

    for(i = 0; i < FC_ACT_SIZE; i++)
    {
    	total_values++;
    	if(fc_out[i] - fc_ref[i] > eps || fc_ref[i] - fc_out[i] > eps)
    	{
    		if(correct_values + 1 == total_values)
    			printf("Missmatch in FC check\n");
    	}
    	else
    		correct_values++;
    	}
   	printf("DONE: %d out of %d are correct\n\n", correct_values, total_values);
	return 0;
}
