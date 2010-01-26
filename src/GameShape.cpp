/**
 * \file GameShape.cpp
 * \brief Implements GameShape class
 */

#include <stdlib.h>
#include <algorithm>
#include "GameShape.h"

using namespace std;

void GameShape::SetGameShape(Tetrominoes shape)
{
    static const int coordsTable[8][4][2] = {
        { { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, -1 },  { 0, 0 },   { -1, 0 },  { -1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 1, 0 },   { 1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 0, 1 },   { 0, 2 } },
        { { -1, 0 },  { 0, 0 },   { 1, 0 },   { 0, 1 } },
        { { 0, 0 },   { 1, 0 },   { 0, 1 },   { 1, 1 } },
        { { -1, -1 }, { 0, -1 },  { 0, 0 },   { 0, 1 } },
        { { 1, -1 },  { 0, -1 },  { 0, 0 },   { 0, 1 } }
    };

    for (int i = 0; i < 4 ; i++) {
        for (int j = 0; j < 2; ++j)
            coords[i][j] = coordsTable[shape][i][j];
    }
    pieceGameShape = shape;
}



void GameShape::SetRandomGameShape()
{
  int x = rand() % 7 + 1;
  SetGameShape(Tetrominoes(x));
}

int GameShape::MinX() const
{
  int m = coords[0][0];
  for (int i=0; i<4; i++) {
      m = min(m, coords[i][0]);
  }
  return m;
}

int GameShape::MaxX() const
{
  int m = coords[0][0];
  for (int i=0; i<4; i++) {
      m = max(m, coords[i][0]);
  }
  return m;
}

int GameShape::MinY() const
{
  int m = coords[0][1];
  for (int i=0; i<4; i++) {
      m = min(m, coords[i][1]);
  }
  return m;
}

int GameShape::MaxY() const
{
  int m = coords[0][1];
  for (int i=0; i<4; i++) {
      m = max(m, coords[i][1]);
  }
  return m;
}

GameShape GameShape::RotateLeft() const
{
    if (pieceGameShape == SquareGameShape)
        return *this;

    GameShape result;
    result.pieceGameShape = pieceGameShape;
    for (int i = 0; i < 4; ++i) {
        result.SetX(i, y(i));
        result.SetY(i, -x(i));
    }
    return result;
}

GameShape GameShape::RotateRight() const
{
    if (pieceGameShape == SquareGameShape)
        return *this;

    GameShape result;
    result.pieceGameShape = pieceGameShape;
    for (int i = 0; i < 4; ++i) {
        result.SetX(i, -y(i));
        result.SetY(i, x(i));
    }
    return result;
}
