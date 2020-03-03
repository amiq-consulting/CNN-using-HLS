#################################################################################
 # (C) Copyright 2020 AMIQ Consulting
 
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 
 # http://www.apache.org/licenses/LICENSE-2.0
 
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 
 # NAME:        gen_proj.tcl
 # PROJECT:     fully_connected
 # Description: .tcl script to generate the HLS project
 
#################################################################################

open_project fully_connected
set_top fc
add_files fc.cpp
add_files -tb fc_test.cpp
open_solution "solution1"
set_part {xc7z020clg400-1} -tool vivado
create_clock -period 10 -name default
source "directives.tcl"

