#!/bin/sh

cd $(dirname $0)
xxd -i shaders/fragment.glsl > src/shaders_generated.h
xxd -i shaders/vertex.glsl >> src/shaders_generated.h
