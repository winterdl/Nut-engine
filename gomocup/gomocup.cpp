/*
   Copyright 2017 Chijun Sima

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "stdafx.h"
#include "pisqpipe.h"
#include "wrapper.h"
#include <windows.h>

const char *infotext = "name=\"Gomoku AI\", author=\"Chijun Sima\", version=\"1.0.0\", country=\"China\", www=\"https://github.com/NutshellySima\"";

Cwrapper AI;

/* create the board and call pipeOut("OK"); or pipeOut("ERROR Maximal board size is .."); */
void brain_init()
{
	if (width != 15 || height != 15)
	{
		pipeOut("ERROR Supported board size is 15");
		return;
	}
	pipeOut("OK");
}

/* delete old board, create new board, call pipeOut("OK"); */
void brain_restart()
{
	AI.Resetboard();
	pipeOut("OK");
}

int isFree(int x, int y)
{
	return AI.IsFree(x, y);
}

/* put your move to the board */
void brain_my(int x, int y)
{
	if (isFree(x, y))
	{
		AI.Put(x, y, 2);
	}
	else
	{
		pipeOut("ERROR my move [%d,%d]", x, y);
	}
}

/* put opponent's move to the board */
void brain_opponents(int x, int y)
{
	if (isFree(x, y))
	{
		AI.Put(x, y, 1);
	}
	else
	{
		pipeOut("ERROR opponents's move [%d,%d]", x, y);
	}
}

/* square [x,y] belongs to a winning line (when info_continuous is 1) */
void brain_block(int x, int y)
{
	if (isFree(x, y))
	{
		AI.Put(x, y, 3);
	}
	else
	{
		pipeOut("ERROR winning move [%d,%d]", x, y);
	}
}

/* clear one square; return value: 0:success, 1:not supported, 2:error */
int brain_takeback(int x, int y)
{
	return AI.Undoboard(x, y);
}

/* choose your move and call do_mymove(x,y);
0<=x<width, 0<=y<height */
void brain_turn()
{
	int x, y;
	std::tuple<int, int8_t, int8_t>putter = AI.Putcomputer(2, info_timeout_turn);
	x = std::get<1>(putter);
	y = std::get<2>(putter);
	do_mymove(x, y);
}

/* delete temporary files, free resources */
void brain_end()
{

}

#ifdef DEBUG_EVAL
#include <windows.h>

void brain_eval(int x, int y)
{
	HDC dc;
	HWND wnd;
	RECT rc;
	char c;
	wnd = GetForegroundWindow();
	dc = GetDC(wnd);
	GetClientRect(wnd, &rc);
	c = (char)(board[x][y] + '0');
	TextOut(dc, rc.right - 15, 3, &c, 1);
	ReleaseDC(wnd, dc);
}

#endif
