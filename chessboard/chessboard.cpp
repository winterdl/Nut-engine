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
#include "chessboard.h"

chessboard::chessboard()noexcept
{
	reset();
}

//reset the chess board

void chessboard::reset()noexcept
{
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
			board[i][ii] = 0;
	}
}

int chessboard::get(int row, int col) const noexcept
{
	return board[row][col];
}

bool chessboard::put(int row, int col, int x) noexcept
{
	if (row >= 0 && row < 15 && col >= 0 && col < 15 && board[row][col] == 0)
	{
		board[row][col] = x;
		return true;
	}
	else
	{
		return false;
	}
}


int chessboard::undo(int row,int col)noexcept
{
	if (row < 0 || row >= 15 || col < 0 || col >= 15|| board[row][col] == 0)
		return 2;
	board[row][col] = 0;
	return 0;
}

int chessboard::check()const noexcept
{
	int counto = 0, countx = 0;
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			if (board[i][ii] == 0)
			{
				countx = counto = 0;
			}
			if (board[i][ii] == 1)
			{
				++counto;
				countx = 0;
			}
			if (board[i][ii] == 2)
			{
				++countx;
				counto = 0;
			}
			if (counto == 5)
			{
				return 1;
			}
			if (countx == 5)
			{
				return 2;
			}
		}
		counto = countx = 0;
	}
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			if (board[ii][i] == 0)
			{
				countx = counto = 0;
			}
			if (board[ii][i] == 1)
			{
				++counto;
				countx = 0;
			}
			if (board[ii][i] == 2)
			{
				++countx;
				counto = 0;
			}
			if (counto == 5)
			{
				return 1;
			}
			if (countx == 5)
			{
				return 2;
			}
		}
		counto = countx = 0;
	}
	for (int i = 0; i <= 10; ++i)
	{
		int x = i;
		for (int y = 0; x < 15; ++x, ++y)
		{
			if (board[x][y] == 0)
			{
				countx = counto = 0;
			}
			if (board[x][y] == 1)
			{
				++counto;
				countx = 0;
			}
			if (board[x][y] == 2)
			{
				++countx;
				counto = 0;
			}
			if (counto == 5)
			{
				return 1;
			}
			if (countx == 5)
			{
				return 2;
			}
		}
		counto = countx = 0;
	}
	for (int i = 0; i <= 10; ++i)
	{
		int y = i;
		for (int x = 0; y < 15; ++x, ++y)
		{
			if (board[x][y] == 0)
			{
				countx = counto = 0;
			}
			if (board[x][y] == 1)
			{
				++counto;
				countx = 0;
			}
			if (board[x][y] == 2)
			{
				++countx;
				counto = 0;
			}
			if (counto == 5)
			{
				return 1;
			}
			if (countx == 5)
			{
				return 2;
			}
		}
		counto = countx = 0;
	}
	for (int i = 4; i <= 24; ++i)
	{
		int x = 0;
		int y = i;
		for (; x < 15 && y >= 0; ++x, --y)
		{
			while (y >= 15)
			{
				++x;
				--y;
			}
			if (board[x][y] == 0)
			{
				countx = counto = 0;
			}
			if (board[x][y] == 1)
			{
				++counto;
				countx = 0;
			}
			if (board[x][y] == 2)
			{
				++countx;
				counto = 0;
			}
			if (counto == 5)
			{
				return 1;
			}
			if (countx == 5)
			{
				return 2;
			}
		}
		counto = countx = 0;
	}
	return 0;
}

bool chessboard::Fullboard()const noexcept
{
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			if (board[i][ii] == 0)
				return false;
		}
	}
	return true;
}
