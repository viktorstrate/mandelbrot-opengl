#version 330 core

in vec2 FragCoord;
out vec4 FragColor;

uniform vec2 uOffset;
uniform float uZoom;

void main()
{
  vec2 startPos;

  vec2 originCoord = FragCoord.xy * uZoom;

  startPos.x = ((originCoord.x + 1.0)/2 * 2.5) - 2.;
  startPos.y = originCoord.y;
  startPos += uOffset;

  vec2 pos;
  int iteration = 0;
  int max_iterations = int(100. / (uZoom/2.+1.));
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

  float color = f_itr / f_itr_max;

  FragColor = vec4(color, color, color, 1.0);
}
