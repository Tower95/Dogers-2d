#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <algorithm>

struct Meteor
{
  Vector2 position;
  float speed;
  float radius;
  bool active;
};

struct Laser
{
  Vector2 position;
  float speed;
  float radius;
  bool active;
};

// Functions declarations
// Player functions
void HandlePlayerInput(Vector2 &position, float speed, std::vector<Laser> &lasers);
void KeepPlayerInBounds(Vector2 &position, float radius, int screenWidth, int screenHeight);
// Meteor functions
void UpdateMeteors(std::vector<Meteor> &meteors, int screenWidth, int screenHeight);
void DrawMeteors(const std::vector<Meteor> &meteors);
// Game Over/Reset fucntions
bool CheckPlayerCollision(Vector2 playerPos, float playerRadius, const std::vector<Meteor> &meteors);
void ResetGame(Vector2 &playerPos, std::vector<Meteor> &meteors, std::vector<Laser> &lasers, bool &gameOver, int &score, int screenWidth, int screenHeight);
// Laser Functions
void ShootLaser(std::vector<Laser> &lasers, Vector2 playerPos);
void UpdateLasers(std::vector<Laser> &lasers);
void DrawLasers(const std::vector<Laser> &lasers);
void CheckLaserMeteorCollisions(std::vector<Laser> &lasers, std::vector<Meteor> &meteors, int &score, int screenWidth);

int main()
{
  // Dimension de la screen
  const int screenWidth = 800;
  const int screenHeight = 600;

  // Open window: InitWindow
  InitWindow(screenWidth, screenHeight, "Spaces Dodgers 2D");

  // Set target frames per second
  SetTargetFPS(60);

  // Player variables
  Vector2 playerPosition = {
      (float)screenWidth / 2,
      (float)screenHeight - 50};
  float playerSpeed = 6.0f;
  float playerRadius = 20.0f;

  // Initialize Meteors
  int maxMeteors = 8;
  std::vector<Meteor> meteors;

  std::vector<Laser> lasers;
  int score = 0;

  for (int i = 0; i < maxMeteors; i++)
  {
    Meteor m;
    m.position = {(float)(rand() % screenWidth), (float)(rand() % -screenHeight)}; // Aparecen mas aya de la pantalla
    m.speed = (float)(rand() % 4 + 2);                                             // rand velocidad
    m.radius = (float)(rand() % 15 + 15);                                          // size rand
    m.active = true;

    meteors.push_back(m); // lo guardamos
  }
  // game over variable
  bool gameOver = false;

  // Game Loop
  while (!WindowShouldClose())
  {
    // 1 Update / Logic
    if (!gameOver)
    {

      // Solo actualizamos si no perdimos
      HandlePlayerInput(playerPosition, playerSpeed, lasers);
      KeepPlayerInBounds(playerPosition, playerRadius, screenWidth, screenHeight);

      CheckLaserMeteorCollisions(lasers, meteors, score, screenWidth);
      UpdateLasers(lasers);
      UpdateMeteors(meteors, screenWidth, screenHeight);

      // Revisa si chocamos en este frame.
      if (CheckPlayerCollision(playerPosition, playerRadius, meteors))
      {
        gameOver = true;
      }
    }
    else
    {
      if (IsKeyPressed(KEY_R))
      {
        ResetGame(playerPosition, meteors, lasers, gameOver, score, screenWidth, screenHeight);
      }
    }

    // 2 Drawing / render
    BeginDrawing();

    // paint  all the backspace black
    ClearBackground(BLACK);

    DrawMeteors(meteors);
    DrawLasers(lasers);

    // Player draw
    DrawCircleV(playerPosition, playerRadius, SKYBLUE);

    // Text test
    // DrawText("Game loop Active!", (screenWidth - (18 * 30)) / 2, screenHeight - (screenHeight - 20), 30, RAYWHITE);
    DrawText(TextFormat("Score: %i", score), 20, 20, 20, GREEN);

    if (gameOver)
    {
      DrawText("GAME OVER", screenWidth / 2 - 110, screenHeight / 2 - 30, 40, RED);
      DrawText("Presiona 'R' para reiniciar", screenWidth / 2 - 130, screenHeight / 2 + 20, 20, RAYWHITE);
    }

    EndDrawing();
  }

  // Clean up and exit
  CloseWindow();

  return 0;
}

// Function Inplementation
void HandlePlayerInput(Vector2 &position, float speed, std::vector<Laser> &lasers)
{
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    position.x += speed;
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    position.x -= speed;
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    position.y += speed;
  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    position.y -= speed;
  if (IsKeyPressed(KEY_SPACE))
    ShootLaser(lasers, position);
}
void KeepPlayerInBounds(Vector2 &position, float radius, int screenWidth, int screenHeight)
{
  if (position.x < radius)
    position.x = radius;
  if (position.x > screenWidth - radius)
    position.x = screenWidth - radius;
  if (position.y < radius)
    position.y = radius;
  if (position.y > screenHeight - radius)
    position.y = screenHeight - radius;
}

void UpdateMeteors(std::vector<Meteor> &meteors, int screenWidth, int screenHeight)
{
  for (auto &m : meteors)
  {
    // Hacemos que caiga sumado a su Y
    m.position.y += m.speed;

    // Si el meteoro sale por debajo de la pantall lo reaparecemos arriba
    if (m.position.y > (screenHeight + m.radius))
    {
      m.position.y = -m.radius;
      m.position.x = (rand() % screenWidth);
    }
  }
}
void DrawMeteors(const std::vector<Meteor> &meteors)
{
  for (const auto &m : meteors)
  {
    DrawCircleV(m.position, m.radius, RED);
  }
}

bool CheckPlayerCollision(Vector2 playerPos, float playerRadius, const std::vector<Meteor> &meteors)
{

  for (auto &m : meteors)
  {

    if (m.active)
    {
      if (CheckCollisionCircles(playerPos, playerRadius, m.position, m.radius))
      {
        return true;
      }
    }
  }
  return false;
}

void ResetGame(Vector2 &playerPos, std::vector<Meteor> &meteors, std::vector<Laser> &lasers, bool &gameOver, int &score, int screenWidth, int screenHeight)
{
  // Regresa al jugador al centro
  playerPos = {
      (float)screenWidth / 2,
      (float)screenHeight - 50};

  for (auto &m : meteors)
  {
    m.position = {(float)(rand() % screenWidth), (float)(rand() % -400)};
  }

  lasers.clear();
  score = 0;
  gameOver = false;
}

void ShootLaser(std::vector<Laser> &lasers, Vector2 playerPos)
{
  Laser l;
  l.position = playerPos;
  l.speed = 10.0f;
  l.radius = 4.0f;
  l.active = true;

  lasers.push_back(l);
}

void UpdateLasers(std::vector<Laser> &lasers)
{
  for (auto &l : lasers)
  {
    if (l.active)
    {
      l.position.y -= l.speed;

      if (l.position.y < -l.radius)
      {
        l.active = false;
      }
    }
  }
  // LIMPIEZA DE MEMORIA: Borrar del vector todo los laser con active == false
  lasers.erase(std::remove_if(lasers.begin(), lasers.end(), [](const Laser &l)
                              { return !l.active; }),
               lasers.end());
}
void DrawLasers(const std::vector<Laser> &lasers)
{
  for (const auto &l : lasers)
  {
    if (l.active)
    {

      DrawCircleV(l.position, l.radius, YELLOW);
    }
  }
}
void CheckLaserMeteorCollisions(std::vector<Laser> &lasers, std::vector<Meteor> &meteors, int &score, int screenWidth)
{
  for (auto &l : lasers)
  {
    if (!l.active)
      continue;

    for (auto &m : meteors)
    {
      if (!m.active)
        continue;

      if (CheckCollisionCircles(l.position, l.radius, m.position, m.radius))
      {
        l.active = false;

        // reinicia el metero arriba
        m.position.y = -m.radius;
        m.position.x = rand() % screenWidth;

        score += 100;
      }
    }
  }
}
