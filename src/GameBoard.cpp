/**
 * \file GameBoard.cpp
 * \brief Implements the GameBoard class
 */

#include "GameBoard.h"

GameBoard::GameBoard(wxFrame *parent)
       : wxPanel(parent, wxID_ANY, wxDefaultPosition,
             wxDefaultSize, wxBORDER_NONE)
{
    timer = new wxTimer(this, 1);

    m_stsbar = parent->GetStatusBar();
    isFallingFinished = false;
    isStarted = false;
    isPaused = false;
    numLinesRemoved = 0;
    curX = 0;
    curY = 0;

    ClearGameBoard();  

    Connect(wxEVT_PAINT, wxPaintEventHandler(GameBoard::OnPaint));
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(GameBoard::OnKeyDown));
    Connect(wxEVT_TIMER, wxCommandEventHandler(GameBoard::OnTimer));
}


void GameBoard::Start()
{
    if (isPaused)
        return;

    isStarted = true;
    isFallingFinished = false;
    numLinesRemoved = 0;
    ClearGameBoard();

    NewPiece();
    timer->Start(300);
}

void GameBoard::Pause()
{
    if (!isStarted)
        return;

    isPaused = !isPaused;
    if (isPaused) {
        timer->Stop();
        m_stsbar->SetStatusText(wxT("paused"));
    } else {
        timer->Start(300);
        wxString str;
        str.Printf(wxT("%d"), numLinesRemoved);
        m_stsbar->SetStatusText(str);
    }
    Refresh();
}

void GameBoard::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    wxSize size = GetClientSize();
    int boardTop = size.GetHeight() - GameBoardHeight * SquareHeight();


    for (int i = 0; i < GameBoardHeight; ++i) {
        for (int j = 0; j < GameBoardWidth; ++j) {
            Tetrominoes shape = GameShapeAt(j, GameBoardHeight - i - 1);
            if (shape != NoGameShape)
                DrawSquare(dc, 0 + j * SquareWidth(),
                           boardTop + i * SquareHeight(), shape);
        }
    }

    if (curPiece.GetGameShape() != NoGameShape) {
        for (int i = 0; i < 4; ++i) {
            int x = curX + curPiece.x(i);
            int y = curY - curPiece.y(i);
            DrawSquare(dc, 0 + x * SquareWidth(),
                       boardTop + (GameBoardHeight - y - 1) * SquareHeight(),
                       curPiece.GetGameShape());
        }
    }
}

void GameBoard::OnKeyDown(wxKeyEvent& event)
{
    if (!isStarted || curPiece.GetGameShape() == NoGameShape) {  
        event.Skip();
        return;
    }

    int keycode = event.GetKeyCode();

    if (keycode == 'p' || keycode == 'P') {
	Pause();
        return;
    }

    if (isPaused)
        return;

    switch (keycode) {
    case WXK_LEFT:
        TryMove(curPiece, curX - 1, curY);
        break;
    case WXK_RIGHT:
        TryMove(curPiece, curX + 1, curY);
        break;
    case WXK_DOWN:
        TryMove(curPiece.RotateRight(), curX, curY);
        break;
    case WXK_UP:
        TryMove(curPiece.RotateLeft(), curX, curY);
        break;
    case WXK_SPACE:
        DropDown();
        break;
    case 'd':
        OneLineDown();
        break;
    case 'D':
        OneLineDown();
        break;
    default:
        event.Skip();
    }

}

void GameBoard::OnTimer(wxCommandEvent& event)
{
    if (isFallingFinished) {
        isFallingFinished = false;
        NewPiece();
    } else {
        OneLineDown();
    }
}

void GameBoard::ClearGameBoard()
{
    for (int i = 0; i < GameBoardHeight * GameBoardWidth; ++i)
        board[i] = NoGameShape;
}

void GameBoard::DropDown()
{
    int newY = curY;
    while (newY > 0) {
        if (!TryMove(curPiece, curX, newY - 1))
            break;
        --newY;
    }
    PieceDropped();
}

void GameBoard::OneLineDown()
{
    if (!TryMove(curPiece, curX, curY - 1))
        PieceDropped();
}

void GameBoard::PieceDropped()
{
    for (int i = 0; i < 4; ++i) {
        int x = curX + curPiece.x(i);
        int y = curY - curPiece.y(i);
        GameShapeAt(x, y) = curPiece.GetGameShape();
    }

    RemoveFullLines();

    if (!isFallingFinished)
        NewPiece();
}

void GameBoard::RemoveFullLines()
{
     int numFullLines = 0;

     for (int i = GameBoardHeight - 1; i >= 0; --i) {
         bool lineIsFull = true;

         for (int j = 0; j < GameBoardWidth; ++j) {
             if (GameShapeAt(j, i) == NoGameShape) {
                 lineIsFull = false;
                 break;
             }
         }

         if (lineIsFull) {
             ++numFullLines;
             for (int k = i; k < GameBoardHeight - 1; ++k) {
                 for (int j = 0; j < GameBoardWidth; ++j)
                     GameShapeAt(j, k) = GameShapeAt(j, k + 1);
             }
         }
     }

     if (numFullLines > 0) {
         numLinesRemoved += numFullLines;
         wxString str;
	 str.Printf(wxT("%d"), numLinesRemoved);
         m_stsbar->SetStatusText(str);

         isFallingFinished = true;
         curPiece.SetGameShape(NoGameShape);
         Refresh();
     }
 }

void GameBoard::NewPiece()
{
    curPiece.SetRandomGameShape();
    curX = GameBoardWidth / 2 + 1;
    curY = GameBoardHeight - 1 + curPiece.MinY();

    if (!TryMove(curPiece, curX, curY)) {
        curPiece.SetGameShape(NoGameShape);
        timer->Stop();
        isStarted = false;
        m_stsbar->SetStatusText(wxT("game over"));
    }
}

bool GameBoard::TryMove(const GameShape& newPiece, int newX, int newY)
{
    for (int i = 0; i < 4; ++i) {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);
        if (x < 0 || x >= GameBoardWidth || y < 0 || y >= GameBoardHeight)
            return false;
        if (GameShapeAt(x, y) != NoGameShape)
            return false;
    }

    curPiece = newPiece;
    curX = newX;
    curY = newY;
    Refresh();
    return true;
}

void GameBoard::DrawSquare(wxPaintDC& dc, int x, int y, Tetrominoes shape)
{
    static wxColour colors[] = { wxColour(0, 0, 0), wxColour(204, 102, 102), 
             wxColour(102, 204, 102), wxColour(102, 102, 204), wxColour(204, 204, 102), 
             wxColour(204, 102, 204), wxColour(102, 204, 204), wxColour(218, 170, 0) };

    static wxColour light[] = { wxColour(0, 0, 0), wxColour(248, 159, 171),
             wxColour(121, 252, 121), wxColour(121, 121, 252), wxColour(252, 252, 121), 
             wxColour(252, 121, 252), wxColour(121, 252, 252), wxColour(252, 198, 0) };

    static wxColour dark[] = { wxColour(0, 0, 0), wxColour(128, 59, 59), wxColour(59, 128, 59), 
             wxColour(59, 59, 128), wxColour(128, 128, 59), wxColour(128, 59, 128), 
             wxColour(59, 128, 128), wxColour(128, 98, 0) };


    wxPen pen(light[int(shape)]);
    pen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(pen);

    dc.DrawLine(x, y + SquareHeight() - 1, x, y);
    dc.DrawLine(x, y, x + SquareWidth() - 1, y);

    wxPen darkpen(dark[int(shape)]);
    darkpen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(darkpen);

    dc.DrawLine(x + 1, y + SquareHeight() - 1,
        x + SquareWidth() - 1, y + SquareHeight() - 1);
    dc.DrawLine(x + SquareWidth() - 1, 
        y + SquareHeight() - 1, x + SquareWidth() - 1, y + 1);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(colors[int(shape)])); 
    dc.DrawRectangle(x + 1, y + 1, SquareWidth() - 2, 
        SquareHeight() - 2);
}
