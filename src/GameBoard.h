/**
 * \file GameBoard.h    
 * \brief Headers for GameBoard.cpp
 */

#ifndef BOARD_H
#define BOARD_H

#include "GameShape.h"
#include <wx/wx.h>

class GameBoard : public wxPanel
{

public:
    GameBoard(wxFrame *parent);
    void Start();
    void Pause();
    void linesRemovedChanged(int numLines);

protected:
    void OnPaint(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnTimer(wxCommandEvent& event);

private:
    enum { GameBoardWidth = 10, GameBoardHeight = 22 };

    Tetrominoes & GameShapeAt(int x, int y) { return board[(y * GameBoardWidth) + x]; }

    int SquareWidth() { return GetClientSize().GetWidth() / GameBoardWidth; }
    int SquareHeight() { return GetClientSize().GetHeight() / GameBoardHeight; }
    void ClearGameBoard();
    void DropDown();
    void OneLineDown();
    void PieceDropped();
    void RemoveFullLines();
    void NewPiece();
    bool TryMove(const GameShape& newPiece, int newX, int newY);
    void DrawSquare(wxPaintDC &dc, int x, int y, Tetrominoes shape);

    wxTimer *timer;
    bool isStarted;
    bool isPaused;
    bool isFallingFinished;
    GameShape curPiece;
    int curX;
    int curY;
    int numLinesRemoved;
    Tetrominoes board[GameBoardWidth * GameBoardHeight];
    wxStatusBar *m_stsbar;
};

#endif
