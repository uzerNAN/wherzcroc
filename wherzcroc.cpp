// wherzcroc.cpp : définit le point d'entrée pour l'application console.
//
#include "game_env.h"
#include "stdafx.h"
#include "CrocGame.h"
#include <Windows.h>
#pragma comment(lib, "wherescrocengine")

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	bool OK;
	//wchar_t* Name("3");
	//Name = {"3"};
	
	const wstring GroupName(L"3");
	CrocSession _session = CrocSession(GroupName, OK);
	GameEnv _game(&_session);
	_game.startGame();
	_game.updateGameInfo();
	_game.findCroc();
	_game.findPath();
	//_game.~GameEnv();
	return 0;
}

