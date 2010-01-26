/**
 * \file Game.cpp
 * \brief Why is this here?
 */

#include "Game.h"
#include "GameBoard.h"

Game::Game(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(180, 380))
{
  wxStatusBar *sb = CreateStatusBar();
  sb->SetStatusText(wxT("0"));
  GameBoard *board = new GameBoard(this);
  board->SetFocus();
  board->Start();
}
