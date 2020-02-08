#version 330 core

in vec2 FragCoord;
out vec4 FragColor;

uniform vec2 uOffset;
uniform float uZoom;

vec3 hsv2rgb(vec3 c);

void main()
{
  vec2 startPos;

  vec2 originCoord = FragCoord.xy * uZoom;

  startPos.x = ((originCoord.x + 1.0)/2 * 2.5) - 2.;
  startPos.y = originCoord.y;
  startPos += uOffset;

  vec2 pos;
  int iteration = 0;
  int max_iterations = int(1000.);
  max_iterations = min(max_iterations, 10000);
  // int max_iterations = 100;

  while (pos.x*pos.x + pos.y*pos.y <= 2.0*2.0 && iteration < max_iterations)
  {
    float xtemp = pos.x*pos.x - pos.y*pos.y + startPos.x;
    pos.y = 2.0*pos.x*pos.y + startPos.y;
    pos.x = xtemp;
    iteration++;
  }

  float f_itr = float(iteration);
  float f_itr_max = float(max_iterations);

  float hue = f_itr/f_itr_max;
  vec3 color = hsv2rgb(vec3(hue, 0.8, 0.9));

  if (iteration == max_iterations) {
    color = vec3(0.);
  }


  FragColor = vec4(color.rgb, 1.0);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}
