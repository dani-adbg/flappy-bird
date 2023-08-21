#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <conio.h>
#include <windows.h>
#include <vector>

using namespace std;

const int screenWidth = 115;
const int winWidth = 95;
const int screenHeight = 25;
const int obstacleSpacing = 25; 
int score = 0;

class Bird
{
public:
  int y;

  Bird() : y(screenHeight / 2) {}

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

void gotoxy(int x, int y)
{
  HANDLE hCon;
  hCon = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD dwPos;
  dwPos.X = x;
  dwPos.Y = y;

  SetConsoleCursorPosition(hCon, dwPos);
}

void generateObstacle()
{
  int gapY = rand() % (screenHeight - 10) + 5;   // Generar altura del espacio entre los tubos
  obstacles.emplace_back(winWidth - 3, gapY, 6); // Agregar obstáculo al final de la lista
}

void updateObstacles()
{
  for (size_t i = 0; i < obstacles.size(); i++)
  {
    obstacles[i].x--;

    if (obstacles[i].x == 1)
    {
      obstacles.erase(obstacles.begin() + i);
      i--;
    }
  }
}

void drawObstacles()
{
  for (const Obstacle &obstacle : obstacles)
  {
    for (int i = 1; i < screenHeight; i++)
    {
      if (i < obstacle.gapY || i > obstacle.gapY + 8)
      {
        gotoxy(obstacle.x, i);
        cout << "|||";
      }
    }
  }
}
void eraseObstacles()
{
  for (const Obstacle &obstacle : obstacles)
  {
    for (int i = 1; i < screenHeight; i++)
    {
      if (i < obstacle.gapY || i > obstacle.gapY + 8)
      {
        gotoxy(obstacle.x, i);
        cout << "   ";
      }
    }
  }
}

bool collision(Bird &bird)
{
  int birdX = 11;
  for (const Obstacle &obstacle : obstacles)
  {
    if (birdX >= obstacle.x && birdX <= obstacle.x + obstacle.width)
    {
      if (bird.getCoordY() <= obstacle.gapY || bird.getCoordY() + 1 >= obstacle.gapY + 8)
      {
        return true;
      }
    }
  }
  return false;
}

void drawBorder()
{
  // Dibujar borde con #
  for (int i = 0; i < screenWidth; i++)
  {
    gotoxy(i, 0);
    cout << "#";
    gotoxy(i, screenHeight);
    cout << "#";
  }

  for (int i = 0; i < screenHeight; i++)
  {
    gotoxy(0, i);
    cout << "#";
    gotoxy(screenWidth, i);
    cout << "#";
  }

  for (int i = 0; i < screenHeight; i++)
  {
    gotoxy(winWidth, i);
    cout << "#";
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

void drawBird(Bird &bird)
{
  int birdY = bird.getCoordY();

  gotoxy(5, birdY - 1);
  cout << "/--0\\";
  gotoxy(5, birdY);
  cout << "|uu__>";
}
void eraseBird(Bird &bird)
{
  int birdY = bird.getCoordY();

  gotoxy(5, birdY - 1);
  cout << "     ";
  gotoxy(5, birdY);
  cout << "      ";
}

void gameOver()
{
  system("cls");
  gotoxy((screenWidth / 2) - 25, (screenHeight / 2) - 1);
  cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -";
  gotoxy(screenWidth / 2, screenHeight / 2);
  cout << "Game Over";
  gotoxy((screenWidth / 2) - 1, screenHeight / 2);
  cout << "Puntaje: " << score;
  gotoxy((screenWidth / 2) - 25, (screenHeight / 2) + 1);
  cout << "Presiona ESPACIO para jugar otra vez o ENTER para salir...";
  gotoxy((screenWidth / 2) - 25, (screenHeight / 2) + 2);
  cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -";
}

bool scoreCollision()
{
  int birdX = 9;
  for (const Obstacle &obstacle : obstacles)
  {
    if (birdX == obstacle.x)
    {
      return true;
    }
  }
  return false;
}

void updateScore() {
  gotoxy(winWidth + 6, 5);
  cout << "Puntos: " << score << endl; 
}

int main()
{
  bool playAgain = true;

  while (playAgain)
  {
    Bird bird;
    cursor(false);
    drawBorder();
    obstacles.clear();
    bool game = true;

    updateScore();

    gotoxy(winWidth + 5, 2);cout<<"FLAPPY BIRD";
	  gotoxy(winWidth + 5, 4);cout<<"-----------";
	  gotoxy(winWidth + 5, 6);cout<<"-----------";
	  gotoxy(winWidth + 7, 12);cout<<"Control ";
	  gotoxy(winWidth + 6, 13);cout<<"---------";
	  gotoxy(winWidth + 2, 14);cout<<" Espacio = Salta";

    while (game)
    {
      if (_kbhit())
      {
        char key = _getch();
        if (key == ' ')
        {
          bird.moveUp();
        }
      }

      drawBird(bird);

      int birdY = bird.getCoordY();

      if (birdY == screenHeight || birdY - 1 == 0 || collision(bird) == true)
      {
        game = false;
      }
      updateObstacles();

      if (obstacles.empty() || winWidth - obstacles.back().x >= obstacleSpacing)
      {
        generateObstacle();
      }
      drawObstacles();

      Sleep(100);
      eraseBird(bird);
      eraseObstacles();
      bird.moveDown();

      if(scoreCollision() == true) {
        score++;
        updateScore();
      }

    }

    gameOver();
    score = 0;
    Sleep(1000);
    char key;
    do
    {
      key = _getch();
    } while (key != ' ' && key != 13); 
    if (key == 13)
    {
      playAgain = false;
    }
    system("cls"); 
  }
  return 0;
}
