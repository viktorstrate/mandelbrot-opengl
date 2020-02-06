#version 330 core

in vec2 FragCoord;
out vec4 FragColor;

void main()
{
  vec2 startPos;
  startPos.x = ((FragCoord.x + 1.0)/2 * 3.5) - 2.5;
  startPos.y = FragCoord.y;

  vec2 pos;
  int iteration = 0;
  int max_iterations = 100;

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
