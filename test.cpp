#include "raylib.h"

int main()
{
  InitWindow(800, 450, "¡Mi primera ventana en Linux!");
  SetTargetFPS(60);

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("¡Lo logramos! Raylib funciona en C++", 190, 200, 20, LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}