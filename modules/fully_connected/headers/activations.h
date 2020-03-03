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
 * PROJECT:     fully_connected
 * Description: definition and implementation of activation functions
 *******************************************************************************/

#ifndef __ACTIVATIONS_H
#define __ACTIVATIONS_H

float relu(float a)
{
	return a > 0 ? a : 0;
}

#endif
