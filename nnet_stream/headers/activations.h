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
 * NAME:        activations.h
 * PROJECT:     nnet_stream
 * Description: definition and implementation of activation functions
 *******************************************************************************/

#ifndef EXP_WIDTH

#include "ap_fixed.h"

#define EXP_WIDTH	16
#define INT_WIDTH	4

typedef ap_fixed<EXP_WIDTH, INT_WIDTH> float24_t;

#endif


#ifndef __ACTIVATIONS_H
#define __ACTIVATIONS_H



float24_t relu(float24_t a)
{
	return a > (float24_t)0 ? a : (float24_t)0;
}

#endif
