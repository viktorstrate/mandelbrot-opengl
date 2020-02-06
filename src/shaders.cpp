#include "shaders.h"

// "buffer" should be at least the size of "length+1"
void readGeneratedShader(unsigned char* generated_shader, unsigned int length, char* buffer)
{
  for(int i = 0; i < length+1; i++) {
    buffer[i] = generated_shader[i];
  }
  buffer[length] = 0x00; // null terminator
}