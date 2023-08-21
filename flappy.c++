#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <conio.h>
#include <windows.h>
#include <vector>

using namespace std;

class Bird
{
public:
  int x;
  int y;

  Bird() : x(10), y(11) {}

  int getCoordX() const
  {
    return x;
  }

  int getCoordY() const
  {
    return y;
  }

  void moveDown()
  {
    y++;
  }

  void moveUp()
  {
    y -= 2;
    if (y < 0)
    {
      y = 0;
    }
  }
};

class Obstacle
{
public:
  int x;
  int gapY;
  int width;

  Obstacle(int x, int gapY, int width) : x(x), gapY(gapY), width(width) {}
};

vector<Obstacle> obstacles;

const int screenWidth = 100;
const int screenHeight = 25;
char screen[screenHeight][screenWidth];
const int obstacleSpacing = 25;  // Espacio entre obstáculos

// void gotoxy(int x, int y) {
//   HANDLE hCon;
//   hCon = GetStdHandle(STD_OUTPUT_HANDLE);
//   COORD dwPos;
//   dwPos.X = x;
//   dwPos.Y = y;

//   SetConsoleCursorPosition(hCon, dwPos);
// }

void generateObstacle() {
    int gapY = rand() % (screenHeight - 10) + 5;  // Generar altura del espacio entre los tubos
    obstacles.emplace_back(screenWidth - 1, gapY, 6);  // Agregar obstáculo al final de la lista
}

void updateObstacles() {
    for (size_t i = 0; i < obstacles.size(); i++) {
        obstacles[i].x--;

        // Eliminar obstáculos que salen de la pantalla
        if (obstacles[i].x + obstacles[i].width < 0) {
            obstacles.erase(obstacles.begin() + i);
            i--;
        }
    }
}

void drawObstacles() {
    for (const Obstacle& obstacle : obstacles) {
        for (int i = 0; i < screenHeight; i++) {
            if (i < obstacle.gapY || i > obstacle.gapY + 8) {
                screen[i][obstacle.x] = '|';
                screen[i][obstacle.x + 1] = '|';
            }
        }
    }
}

bool checkCollision(Bird& bird) {
    for (const Obstacle& obstacle : obstacles) {
        if (bird.getCoordX() + 4 >= obstacle.x && bird.getCoordX() <= obstacle.x + obstacle.width) {
            if (bird.getCoordY() <= obstacle.gapY || bird.getCoordY() + 1 >= obstacle.gapY + 8) {
                return true;  // Colisión detectada
            }
        }
    }
    return false;  // No hay colisión
}

void clearScreen()
{
  system("cls"); // Limpiar la consola
  // Dibujar borde con #
  for (int i = 0; i < screenHeight; i++)
  {
    for (int j = 0; j < screenWidth; j++)
    {
      screen[i][j] = ' ';
    }
  }
}

void drawBorder()
{
  // Dibujar borde con #
  for (int i = 0; i < screenWidth; i++)
  {
    screen[0][i] = '#';
    screen[screenHeight - 1][i] = '#';
  }
  for (int i = 1; i < screenHeight - 1; i++)
  {
    screen[i][0] = '#';
    screen[i][screenWidth - 1] = '#';
  }
}

void updateScreen(Bird &bird)
{
  // Dibujar el pájaro en la posición actual
  int birdX = bird.getCoordX();
  int birdY = bird.getCoordY();
  if (birdY >= 0 && birdY < screenHeight - 1)
  {
    screen[birdY][birdX] = '/';
    screen[birdY][birdX + 1] = '-';
    screen[birdY][birdX + 2] = '-';
    screen[birdY][birdX + 3] = '0';
    screen[birdY][birdX + 4] = '\\';
    if (birdY + 1 < screenHeight - 1)
    {
      screen[birdY + 1][birdX] = '|';
      screen[birdY + 1][birdX + 1] = 'u';
      screen[birdY + 1][birdX + 2] = 'u';
      screen[birdY + 1][birdX + 3] = '_';
      screen[birdY + 1][birdX + 4] = '>';
    }
  }
}

void cursor(bool game)
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(hConsole, &cursorInfo);
  cursorInfo.bVisible = game;
  SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void showGameOver()
{
  system("cls");
  cout << "Game Over" << endl;
  cursor(true);
}

int main()
{
  Bird bird;
  auto startTime = chrono::steady_clock::now();
  bool gameOver = false;

  cursor(gameOver);

  while (!gameOver)
  {
    if (_kbhit())
    {
      char key = _getch();
      if (key == ' ')
      {
        bird.moveUp();
      }
    }

    if (bird.getCoordY() == 0 || bird.getCoordY() == screenHeight - 2)
    {
      showGameOver();
      gameOver = true; // Cambiar el estado del juego para salir del bucle
    }

    auto currentTime = chrono::steady_clock::now();
    auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(currentTime - startTime).count();

    if (elapsedTime >= 100 && !gameOver)
    { // Evitar mover el pájaro si el juego ha terminado
      bird.moveDown();
      updateObstacles();
      if (obstacles.empty() || screenWidth - obstacles.back().x >= obstacleSpacing) {
                generateObstacle();
      }
      startTime = currentTime;
    }

    clearScreen();
    drawBorder();
    if (!gameOver)
    {
      updateScreen(bird);
      drawObstacles();
    }

    if (checkCollision(bird)) {
            showGameOver();
            gameOver = true;
        }

    // Imprimir el contenido actualizado del recuadro
    for (int i = 0; i < screenHeight; i++)
    {
      for (int j = 0; j < screenWidth; j++)
      {
        cout << screen[i][j];
      }
      cout << endl;
    }

    Sleep(100);
  }

  showGameOver();

  getch();
}