############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_unroll "conv/conv_label0"
set_directive_unroll "conv/conv_label1"
set_directive_unroll "conv/conv_label2"
set_directive_unroll "conv/conv_label3"
set_directive_unroll "conv/conv_label4"
set_directive_stream -depth 1024 -dim 1 "main" in
set_directive_stream -depth 1024 -dim 1 "main" out
set_directive_stream -depth 1024 -dim 1 "conv" out
set_directive_stream -depth 1024 -dim 1 "conv" in
set_directive_unroll "conv/conv_label0"
set_directive_unroll "conv/conv_label1"
set_directive_unroll "conv/conv_label2"
set_directive_unroll "conv/conv_label3"
set_directive_unroll "conv/conv_label4"
set_directive_pipeline "conv/conv_label5"
