#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<complex.h>
#include<raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MAX_ITER 1000


double mandelbrot_minX = -2;
double mandelbrot_minY = -2;

double mandelbrot_maxX = 2;
double mandelbrot_maxY = 2;


double map(double value, double min_input, double max_input, double min_output, double max_output){
  return (value - min_input) * (max_output - min_output) / (max_input - min_input) + min_output; 
}

void getMandelbrotSet(double complex z, double complex c, int *iter_count, Vector2 *points){

  if(cabs(z) > 2) return; //magnitude of the complex number, mag = sqrt(r**r + img**img)
  
  if(*iter_count == MAX_ITER) return;
  
  double complex z_ = (z * z) + c;
  
  points[*iter_count] = (Vector2) {.x = map(creal(z_),mandelbrot_minX,mandelbrot_maxX,0,SCREEN_WIDTH) , .y = map(cimag(z_),mandelbrot_minY,mandelbrot_maxY,0,SCREEN_HEIGHT) }; 
  
  *(iter_count) += 1;
  getMandelbrotSet(z_,c, iter_count,points);
}

int getIterationCount(double complex c){
  double complex z = 0.0 + 0.0 * I;
  int iter_cnt = -1; // if the iteration count goes beyond MAX_ITER

  for(int i = 0; i < MAX_ITER; ++i){
    double complex z_ = (z * z) + c;
    z = z_;
    if(cabs(z) > 2) return i; //magnitude of the complex number, mag = sqrt(r**r + img**img)
  }
  return iter_cnt;
}

void drawMandelbrotFractal(Color *pixels){

  for(int i = 0; i < SCREEN_WIDTH; ++i){
    for(int j = 0; j < SCREEN_HEIGHT; ++j){
        //DrawPixel(i,j,pixels[i * SCREEN_WIDTH + j]);
        DrawRectangle(i,j,1,1,pixels[i * SCREEN_WIDTH + j]);
      }
  }
}


void generateMandelbrotFractal(Color *pixels){
  for(int i = 0; i < SCREEN_WIDTH; ++i){
    for(int j = 0; j < SCREEN_HEIGHT; ++j){
      double x_m = map(i,0,SCREEN_WIDTH,mandelbrot_minX,mandelbrot_maxX);
      double y_m = map(j,0,SCREEN_HEIGHT,mandelbrot_minY,mandelbrot_maxY);

      double complex c = x_m + y_m * I;

      int iter_count = getIterationCount(c);
      Color color;
      if(iter_count == -1){
        color = (Color){0,0,0,255}; // black
      }
      else{
      // based the iter count, i need to give a color to the pixels
        color = (Color) {.r = (unsigned int) (8 * iter_count) % 255, .b = (unsigned int) (5 * iter_count) % 255, .g = (unsigned int) (3 * iter_count) % 255, .a = 255 };
      }
      pixels[i * SCREEN_WIDTH + j] = color;
    }
  }
}



void drawMandelbrotPoints(Vector2 *points, int iter_cnt){
  for(int i = 0; i < iter_cnt; ++i){
    Color c = {.r = 155, .g = 128, 50, 255};
    DrawCircleV(points[i], 3, c);
  }

}

void zoomMandelbrot(double mouseX, double mouseY, float zoom_factor){
  double mouseX_m = map(mouseX,0,SCREEN_WIDTH,mandelbrot_minX,mandelbrot_maxX);
  double mouseY_m = map(mouseY,0,SCREEN_HEIGHT,mandelbrot_minY,mandelbrot_maxY);

  double x_range = (mandelbrot_maxX - mandelbrot_minX) * zoom_factor;
  double y_range = (mandelbrot_maxY - mandelbrot_minY) * zoom_factor;

  mandelbrot_minX = mouseX_m - x_range/2;
  mandelbrot_minY = mouseY_m - y_range/2;

  mandelbrot_maxX = mouseX_m + x_range/2;
  mandelbrot_maxY = mouseY_m + y_range/2;

}

int main(void){
  InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"Mandelbrot");
  SetTargetFPS(60);

  // two complex values z and c
  double complex z = 0 + 0 * I;

  int iter_cnt = 0;
  Vector2 points[MAX_ITER] = {0};

  Color *pixels = calloc(SCREEN_HEIGHT * SCREEN_WIDTH, sizeof(Color));

  generateMandelbrotFractal(pixels);


  // creal(z) -> real value/component cimag(z) -> complex value

  while(!WindowShouldClose()){
    BeginDrawing();
      ClearBackground(BLACK);
      DrawLine(0,SCREEN_WIDTH/2, SCREEN_WIDTH,SCREEN_HEIGHT/2, BLUE); // Horizontal
      DrawLine(SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2,SCREEN_HEIGHT, BLUE);
      
      DrawCircleLines(400,400,400,WHITE);

      if(IsMouseButtonDown(0)){
        iter_cnt = 0;
        Vector2 mouse_pos = GetMousePosition(); // in image grid coordinates
      //
        double mouse_pos_mX = map(mouse_pos.x,0,SCREEN_WIDTH,mandelbrot_minX,mandelbrot_maxX);
        double mouse_pos_mY = map(mouse_pos.y,0,SCREEN_HEIGHT,mandelbrot_minY,mandelbrot_maxY);

        double complex c = mouse_pos_mX+ mouse_pos_mY * I;
        getMandelbrotSet(z,c,&iter_cnt, points);

        DrawText(TextFormat("c = %f + %f I", mouse_pos_mX, mouse_pos_mY),mouse_pos.x,mouse_pos.y,20,WHITE);
        DrawText(TextFormat("Iter = %d",iter_cnt),600, 50,20,WHITE);
      }

      if(IsKeyPressed(KEY_UP)){
        Vector2 mouse_pos = GetMousePosition();
        float zoom_factor = 0.5f;
        zoomMandelbrot(mouse_pos.x,mouse_pos.y,zoom_factor);
        generateMandelbrotFractal(pixels);
    }
      if(IsKeyPressed(KEY_DOWN)){
        Vector2 mouse_pos = GetMousePosition();
        float zoom_factor = 2.0f;
        zoomMandelbrot(mouse_pos.x,mouse_pos.y,zoom_factor);
        generateMandelbrotFractal(pixels);
    }
    drawMandelbrotFractal(pixels);
    drawMandelbrotPoints(points,iter_cnt);
    EndDrawing();
  }
  CloseWindow();
}

