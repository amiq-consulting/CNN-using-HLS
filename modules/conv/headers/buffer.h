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
 * NAME:        buffer.h
 * PROJECT:     conv
 * Description: Used to buffer first elements from an image
 *******************************************************************************/
 
#ifndef _BUFFER_H_
#define _BUFFER_H_
#include "ap_fixed.h"

#define FLOAT_IDTH		24
#define INT_WIDTH	4

typedef ap_fixed<FLOAT_WIDTH, INT_WIDTH> float24_t;

template<int SIZE>
class buffer
{
public:
	buffer();
	void SetValue(float24_t val, int pos);
	void InsertFront(float24_t val);
	void InsertBack(float24_t val);
	float24_t GetValue(int pos);
private:
	float24_t array[SIZE];
};


/////////////////////////////////////////////////
// buffer.cpp
/////////////////////////////////////////////////

template<int SIZE>
buffer<SIZE>::buffer()
{
	for(int i = 0; i < SIZE; i++)
		array[i] = 0;
}


template<int SIZE>
void buffer<SIZE>::SetValue(float24_t val, int pos)
{
	array[pos] = val;
}

template<int SIZE>
void buffer<SIZE>::InsertFront(float24_t val)
{
	int i;
	for(i = SIZE-1; i > 0 ; i--)
		array[i] = array[i - 1];
	array[0] = val;
}


template<int SIZE>
void buffer<SIZE>::InsertBack(float24_t val)
{
	int i;
	for(i = 0; i < SIZE - 1; i++)
		array[i] = array[i + 1];
	array[SIZE - 1] = val;
}


template<int SIZE>
float24_t buffer<SIZE>::GetValue(int pos)
{
	return array[pos];
}

#endif
