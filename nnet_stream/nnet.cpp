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
 * NAME:        nnet.cpp
 * PROJECT:     nnet_stream
 * Description: HLS implementation of the Convolutional Neural Network
 *******************************************************************************/


#include "headers/weights.h"
#include "headers/defines.h"
#include "headers/activations.h"
#include <hls_video.h>

#include "ap_fixed.h"

#define EXP_WIDTH	16
#define INT_WIDTH	4

typedef ap_fixed<EXP_WIDTH, INT_WIDTH> float24_t;

#define CONV1_BUFFER_SIZE (IMAGE_SIZE * IMAGE_CHANNELS * (CONV1_KERNEL_SIZE -1) + CONV1_KERNEL_SIZE * IMAGE_CHANNELS)

void conv_layer1(hls::stream<float24_t> &out, hls::stream<float24_t> &in,
		float24_t weight[CONV1_KERNEL_SIZE][CONV1_KERNEL_SIZE][CONV1_CHANNELS][CONV1_FILTERS],
		float24_t bias[CONV1_BIAS_SIZE]) {
	int i, j, k, filter;
	float24_t sum, placeholder;
	int row_offset, col_offset, channel_offset;
	hls::LineBuffer<CONV1_BUFFER_SIZE, 1, float24_t> conv_buff;

	/*
	 * Read the initial buffer
	 * */

	for (i = 0; i < CONV1_BUFFER_SIZE; i++) {
		if (in.empty() == 0) {
			in >> placeholder;
			conv_buff.shift_up(0);
			conv_buff.insert_top(placeholder, 0);
		}
	}

	for (i = 0; i < (IMAGE_SIZE - CONV1_KERNEL_SIZE + 1); i += CONV1_STRIDE)
		conv_layer1_label9: for (j = 0;
				j < (IMAGE_SIZE - CONV1_KERNEL_SIZE + 1); j += CONV1_STRIDE)
				{
			conv_layer1_label2: for (filter = 0; filter < CONV1_FILTERS;
					filter++) {
				sum = 0;
				conv_layer1_label6: for (row_offset = 0;
						row_offset < CONV1_KERNEL_SIZE; row_offset++)
					conv_layer1_label7: for (col_offset = 0;
							col_offset < CONV1_KERNEL_SIZE; col_offset++)
						conv_layer1_label8: for (channel_offset = 0;
								channel_offset < CONV1_CHANNELS;
								channel_offset++) {
							int t1, t2;
							static float24_t val1, val2;
							t1 = row_offset * IMAGE_SIZE * IMAGE_CHANNELS;
							t2 = col_offset * IMAGE_CHANNELS;
							val1 = conv_buff.getval(t1 + t2 + channel_offset,
									0);
							val2 =
									weight[row_offset][col_offset][channel_offset][filter];
							sum += val1 * val2;
						}

				out << relu(sum + bias[filter]);
			}


			if ((j + CONV1_STRIDE < (IMAGE_SIZE - CONV1_KERNEL_SIZE + 1))) {
				conv_layer1_label1: for (int p = 0; p < IMAGE_CHANNELS; p++)
					if (in.empty() == 0) {
						in >> placeholder;
						conv_buff.shift_up(0);
						conv_buff.insert_top(placeholder, 0);
					}
			} else if ((i + CONV1_STRIDE < (IMAGE_SIZE - CONV1_KERNEL_SIZE + 1))
					&& (j + CONV1_STRIDE >= (IMAGE_SIZE - CONV1_KERNEL_SIZE + 1)))
				conv_layer1_label0: for (int p = 0;
						p < CONV1_KERNEL_SIZE * IMAGE_CHANNELS; p++)
					if (in.empty() == 0) {
						in >> placeholder;
						conv_buff.shift_up(0);
						conv_buff.insert_top(placeholder, 0);
					}
		}
}

#define CONV2_BUFFER_SIZE (P1_SIZE * P1_CHANNELS * (CONV2_KERNEL_SIZE -1) + CONV2_KERNEL_SIZE * P1_CHANNELS)

void conv_layer2(hls::stream<float24_t> &out, hls::stream<float24_t> &in,
		float24_t weight[CONV2_KERNEL_SIZE][CONV2_KERNEL_SIZE][CONV2_CHANNELS][CONV2_FILTERS],
		float24_t bias[CONV2_BIAS_SIZE]) {
	int i, j, k, filter;
	float24_t sum, placeholder;
	int row_offset, col_offset, channel_offset;
	hls::LineBuffer<CONV2_BUFFER_SIZE, 1, float24_t> conv_buff;

	/*
	 * Read the initial buffer
	 * */

	for (i = 0; i < CONV2_BUFFER_SIZE; i++) {
		if (in.empty() == 0) {
			in >> placeholder;
			conv_buff.shift_up(0);
			conv_buff.insert_top(placeholder, 0);
		}
	}

	for (i = 0; i < (P1_SIZE - CONV2_KERNEL_SIZE + 1); i += CONV2_STRIDE)
		conv_layer2_label5: for (j = 0; j < (P1_SIZE - CONV2_KERNEL_SIZE + 1);
				j += CONV2_STRIDE)
				{
			conv_layer2_label12: for (filter = 0; filter < CONV2_FILTERS;
					filter++) {
				sum = 0;
				conv_layer2_label13: for (row_offset = 0;
						row_offset < CONV2_KERNEL_SIZE; row_offset++)
					conv_layer2_label10: for (col_offset = 0;
							col_offset < CONV2_KERNEL_SIZE; col_offset++)
						conv_layer2_label11: for (channel_offset = 0;
								channel_offset < CONV2_CHANNELS;
								channel_offset++) {
							int t1, t2;
							t1 = row_offset * P1_SIZE * P1_CHANNELS;
							t2 = col_offset * P1_CHANNELS;
							sum +=
									conv_buff.getval(t1 + t2 + channel_offset,
											0)
											* weight[row_offset][col_offset][channel_offset][filter];
						}
				out << relu(sum + bias[filter]);
			}


			if ((j + CONV2_STRIDE < (P1_SIZE - CONV2_KERNEL_SIZE + 1))) {
				conv_layer2_label3: for (int p = 0; p < P1_CHANNELS; p++)
					if (in.empty() == 0) {
						in >> placeholder;
						conv_buff.shift_up(0);
						conv_buff.insert_top(placeholder, 0);
					}
			} else if ((i + CONV2_STRIDE < (P1_SIZE - CONV2_KERNEL_SIZE + 1))
					&& (j + CONV2_STRIDE >= (P1_SIZE - CONV2_KERNEL_SIZE + 1)))
				conv_layer2_label4: for (int p = 0;
						p < CONV2_KERNEL_SIZE * P1_CHANNELS; p++)
					if (in.empty() == 0) {
						in >> placeholder;
						conv_buff.shift_up(0);
						conv_buff.insert_top(placeholder, 0);
					}
		}
}

#define POOL1_BUFFER_SIZE (P1_SIZE*P1_CHANNELS)

void pool_layer1(hls::stream<float24_t>& out, hls::stream<float24_t>& in) {

	int i, j, k, l, m;
	float24_t read;
	hls::LineBuffer<POOL1_BUFFER_SIZE, 1, float24_t> pool_buff;

	for (i = 0; i < P1_SIZE; i++)
		pool_layer1_label6: for (l = 0; l < P1_KERNEL_SIZE; l++) {
			pool_layer1_label14: for (j = 0; j < P1_SIZE; j++)
				pool_layer1_label15: for (m = 0; m < P1_KERNEL_SIZE; m++)
					pool_layer1_label19: pool_layer1_label18: for (k = 0;
							k < P1_CHANNELS; k++) {
						in >> read;
						if (l == 0 && m == 0)
							pool_buff.val[j * P1_CHANNELS + k][0] = read;

						else
							pool_buff.val[j * P1_CHANNELS + k][0] =
									pool_buff.val[j * P1_CHANNELS + k][0]
											> read ?
											pool_buff.val[j * P1_CHANNELS + k][0] :
											read;

						if (l == (P1_KERNEL_SIZE - 1)
								&& m == (P1_KERNEL_SIZE - 1))
							out << pool_buff.val[j * P1_CHANNELS + k][0];
					}
			pool_layer1_label20: for (int skip = P1_SIZE * P1_STRIDE;
					skip < A1_SIZE; skip++)
				pool_layer1_label16: for (int channel = 0;
						channel < P1_CHANNELS; channel++)
					in >> read;
		}
	pool_layer1_label7: for (int skip_row = P1_SIZE * P1_STRIDE;
			skip_row < A1_SIZE; skip_row++)
		pool_layer1_label33: for (int skip_col = 0; skip_col < A1_SIZE;
				skip_col++)
			pool_layer1_label35: for (int skip_channel = 0;
					skip_channel < A1_CHANNELS; skip_channel++)
				in >> read;
}

#define POOL2_BUFFER_SIZE (P2_SIZE*P2_CHANNELS)

void pool_layer2(hls::stream<float24_t>& out, hls::stream<float24_t>& in) {

	int i, j, k, l, m;
	float24_t read;
	hls::LineBuffer<POOL2_BUFFER_SIZE, 1, float24_t> pool_buff;

	for (i = 0; i < P2_SIZE; i++)
		pool_layer2_label28: for (l = 0; l < P2_KERNEL_SIZE; l++) {
			pool_layer2_label0: for (j = 0; j < P2_SIZE; j++)
				pool_layer2_label26: for (m = 0; m < P2_KERNEL_SIZE; m++)
					pool_layer2_label36: for (k = 0; k < P2_CHANNELS; k++) {
						in >> read;
						if (l == 0 && m == 0)
							pool_buff.val[j * P2_CHANNELS + k][0] = read;

						else
							pool_buff.val[j * P2_CHANNELS + k][0] =
									pool_buff.val[j * P2_CHANNELS + k][0]
											> read ?
											pool_buff.val[j * P2_CHANNELS + k][0] :
											read;

						if (l == (P2_KERNEL_SIZE - 1)
								&& m == (P2_KERNEL_SIZE - 1))
							out << pool_buff.val[j * P2_CHANNELS + k][0];
					}
			pool_layer2_label38: for (int skip = P2_SIZE * P2_STRIDE;
					skip < A2_SIZE; skip++)
				pool_layer2_label37: for (int channel = 0;
						channel < P2_CHANNELS; channel++)
					in >> read;
		}
	pool_layer2_label9: for (int skip_row = P2_SIZE * P2_STRIDE;
			skip_row < A2_SIZE; skip_row++)
		pool_layer2_label16: for (int skip_col = 0; skip_col < A2_SIZE;
				skip_col++)
			pool_layer2_label39: for (int skip_channel = 0;
					skip_channel < A2_CHANNELS; skip_channel++)
				in >> read;
}

void fc_layer1(hls::stream<float24_t> &out, hls::stream<float24_t> &in,
		float24_t weight[FC1_WEIGHTS_H][FC1_WEIGHTS_W],
		float24_t bias[FC1_BIAS_SIZE]) {
	float24_t read;
	float24_t output[FC1_ACT_SIZE] = { 0 };

	in >> read;
	for (int i = 0; i < FC1_WEIGHTS_W; i++)
		output[i] = weight[0][i] * read;

	fc_layer1_label12: for (int j = 1; j < FC1_WEIGHTS_H; j++) {
		in >> read;
		fc_layer1_label40: for (int i = 0; i < FC1_WEIGHTS_W; i++) {
			output[i] += weight[j][i] * read;
		}
	}
	fc_layer1_label15: for (int i = 0; i < FC1_WEIGHTS_W; i++)
		out << relu(output[i] + bias[i]);

}

void fc_layer2(hls::stream<float24_t> &out, hls::stream<float24_t> &in,
		float24_t weight[FC2_WEIGHTS_H][FC2_WEIGHTS_W],
		float24_t bias[FC2_BIAS_SIZE]) {
	float24_t read;
	float24_t output[FC2_ACT_SIZE] = { 0 };

	in >> read;
	for (int i = 0; i < FC2_WEIGHTS_W; i++)
		output[i] = weight[0][i] * read;

	fc_layer2_label13: for (int j = 1; j < FC2_WEIGHTS_H; j++) {
		in >> read;
		fc_layer2_label41: for (int i = 0; i < FC2_WEIGHTS_W; i++) {
			output[i] += weight[j][i] * read;
		}
	}
	fc_layer2_label11: for (int i = 0; i < FC2_WEIGHTS_W; i++)
		out << relu(output[i] + bias[i]);

}

void fc_layer3(hls::stream<float24_t> &out, hls::stream<float24_t> &in,
		float24_t weight[FC3_WEIGHTS_H][FC3_WEIGHTS_W],
		float24_t bias[FC3_BIAS_SIZE]) {
	float24_t read;
	float24_t output[FC3_ACT_SIZE] = { 0 };

	in >> read;
	for (int i = 0; i < FC3_WEIGHTS_W; i++)
		output[i] = weight[0][i] * read;

	fc_layer3_label10: for (int j = 1; j < FC3_WEIGHTS_H; j++) {
		in >> read;
		fc_layer3_label42: for (int i = 0; i < FC3_WEIGHTS_W; i++) {
			output[i] += weight[j][i] * read;
		}
	}
	fc_layer3_label14: for (int i = 0; i < FC3_WEIGHTS_W; i++)
		out << relu(output[i] + bias[i]);

}

void nnet(hls::stream<float24_t> &fc3_out) {
	hls::stream<float24_t> conv1_out("conv1_out");
	hls::stream<float24_t> conv2_out("conv2_out");
	hls::stream<float24_t> pool1_out("pool1_out");
	hls::stream<float24_t> pool2_out("pool2_out");
	hls::stream<float24_t> fc1_out("fc1_out");
	hls::stream<float24_t> fc2_out("fc2_out");
	hls::stream<float24_t> image_in("image_in");

	for (int i = 0; i < IMAGE_SIZE * IMAGE_SIZE * IMAGE_CHANNELS; i++)
		image_in << image[i];

	conv_layer1(conv1_out, image_in, conv_layer1_weights, conv_layer1_bias);
	pool_layer1(pool1_out, conv1_out);

	conv_layer2(conv2_out, pool1_out, conv_layer2_weights, conv_layer2_bias);
	pool_layer2(pool2_out, conv2_out);

	fc_layer1(fc1_out, pool2_out, fc_layer1_weights, fc_layer1_bias);
	fc_layer2(fc2_out, fc1_out, fc_layer2_weights, fc_layer2_bias);
	fc_layer3(fc3_out, fc2_out, fc_layer3_weights, fc_layer3_bias);
}
