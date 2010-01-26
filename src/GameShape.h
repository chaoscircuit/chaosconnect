/**
 * \file GameShape.h
 * \brief Headers for GameShape.cpp
 */

#ifndef SHAPE_H
#define SHAPE_H

enum Tetrominoes { NoGameShape, ZGameShape, SGameShape, LineGameShape, 
                  TGameShape, SquareGameShape, LGameShape, MirroredLGameShape };

class GameShape
{
public:
    GameShape() { SetGameShape(NoGameShape); }
    void SetGameShape(Tetrominoes shape);
    void SetRandomGameShape();

    Tetrominoes GetGameShape() const { return pieceGameShape; }
    int x(int index) const { return coords[index][0]; }
    int y(int index) const { return coords[index][1]; }

    int MinX() const;
    int MaxX() const;
    int MinY() const;
    int MaxY() const;

    GameShape RotateLeft() const;
    GameShape RotateRight() const;

private:
    void SetX(int index, int x) { coords[index][0] = x; }
    void SetY(int index, int y) { coords[index][1] = y; }
    Tetrominoes pieceGameShape;
    int coords[4][2];
};

#endif
