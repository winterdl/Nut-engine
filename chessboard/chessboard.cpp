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
	number = 0;
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			remote_cell[i][ii] = 0;
			board[i][ii] = 0;
		}
	}
}

int chessboard::get(const int row, const int col) const noexcept
{
	return board[row][col];
}

bool chessboard::put(const int row, const int col, const int x) noexcept
{
	if (row >= 0 && row < 15 && col >= 0 && col < 15 && board[row][col] == 0)
	{
		board[row][col] = x;
		update_remotecell(row, col, true);
		++number;
		return true;
	}
	else
	{
		return false;
	}
}

int chessboard::undo(const int row, const int col)noexcept
{
	if (row < 0 || row >= 15 || col < 0 || col >= 15 || board[row][col] == 0)
		return 2;
	board[row][col] = 0;
	--number;
	update_remotecell(row, col, false);
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
	for (int i = -10; i <= 10; ++i)
	{
		int x = i, y = 0;
		while (x < 0)
			++x, ++y;
		for (; x < 15; ++x, ++y)
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
		while (y >= 15)
		{
			++x;
			--y;
		}
		for (; x < 15 && y >= 0; ++x, --y)
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
	return 0;
}

int chessboard::checkpoint(const int row, const int col) const noexcept
{
	if (row < 0 || row >= 15 || col < 0 || col >= 15)
		return 0;
	int counto = 0, countx = 0;
	for (int ii = 0; ii < 15; ++ii)
	{
		if (board[row][ii] == 0)
		{
			countx = counto = 0;
		}
		if (board[row][ii] == 1)
		{
			++counto;
			countx = 0;
		}
		if (board[row][ii] == 2)
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
	for (int ii = 0; ii < 15; ++ii)
	{
		if (board[ii][col] == 0)
		{
			countx = counto = 0;
		}
		if (board[ii][col] == 1)
		{
			++counto;
			countx = 0;
		}
		if (board[ii][col] == 2)
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
	int x = row - col, y = 0;
	while (x < 0)
		++x, ++y;
	for (; x < 15; ++x, ++y)
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
	int i = row + col;
	x = 0;
	y = i;
	while (y >= 15)
	{
		++x;
		--y;
	}
	for (; x < 15 && y >= 0; ++x, --y)
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
	return 0;
}

void chessboard::update_remotecell(const int row, const  int col, const bool add) noexcept
{
	for (int i = row - 2; i <= row + 2; ++i)
	{
		if (i < 0 || i >= 15)
			continue;
		for (int j = col - 2; j <= col + 2; ++j)
		{
			if (j < 0 || j >= 15)
				continue;
			if (add == true)
				++remote_cell[i][j];
			else
				--remote_cell[i][j];
		}
	}
}

std::vector<std::tuple<int, int8_t, int8_t>> chessboard::genmove() const noexcept
{
	std::vector<std::tuple<int, int8_t, int8_t>>moves;
	int score;
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			if (board[i][ii] == 0 && remote_cell[i][ii] != 0)
			{
				score = evaluator.pos[i][ii];
				moves.push_back(std::make_tuple(score, i, ii));
			}
		}
	}
	if (moves.empty())
		moves.push_back(std::make_tuple(3, 7, 7));
	std::sort(moves.rbegin(), moves.rend());
	return moves;
}

bool chessboard::Fullboard()const noexcept
{
	return number == 15 * 15;
}
