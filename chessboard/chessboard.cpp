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

chessboard::chessboard() noexcept
{
	reset();
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			pos[i][ii] = 7 - max(abs(i - 7), abs(ii - 7));
		}
	}
}

//reset the chess board

void chessboard::reset() noexcept
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
	for (int x = 0; x < 4; ++x)
	{
		for (int i = 0; i < 29; ++i)
		{
			for (int ii = 0; ii < 15; ++ii)
			{
				layer_2[x][i][ii] = 0;
			}
		}
	}
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			for (int iii = 0; iii < 4; ++iii)
			{
				layer_3[i][ii][iii] = 0;
			}
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		for (int ii = 0; ii < 20; ++ii)
		{
			layer_4[i][ii] = 0;
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
		update_layer_2(row, col, true);
		++number;
		return true;
	}
	else
	{
		return false;
	}
}

int chessboard::undo(const int row, const int col) noexcept
{
	if (row < 0 || row >= 15 || col < 0 || col >= 15 || board[row][col] == 0)
		return 2;
	board[row][col] = 0;
	--number;
	update_remotecell(row, col, false);
	update_layer_2(row, col, false);
	return 0;
}

int chessboard::check() const noexcept
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
		for (; x < 15 && y < 15; ++x, ++y)
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
		if ((countx != 0 || counto != 0) && board[row][ii] == 0)
		{
			countx = counto = 0;
		}
		if (board[row][ii] == 1)
		{
			++counto;
			countx = 0;
		}
		else if (board[row][ii] == 2)
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
		if ((countx != 0 || counto != 0) && board[ii][col] == 0)
		{
			countx = counto = 0;
		}
		if (board[ii][col] == 1)
		{
			++counto;
			countx = 0;
		}
		else if (board[ii][col] == 2)
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
	int x, y;
	if (row < col)
	{
		y = col - row;
		x = 0;
	}
	else
	{
		y = 0;
		x = row - col;
	}
	for (; x < 15 && y < 15; ++x, ++y)
	{
		if ((countx != 0 || counto != 0) && board[x][y] == 0)
		{
			countx = counto = 0;
		}
		if (board[x][y] == 1)
		{
			++counto;
			countx = 0;
		}
		else if (board[x][y] == 2)
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
	if (14 - row < col)
	{
		y = col - 14 + row;
		x = 14;
	}
	else
	{
		y = 0;
		x = row + col;
	}
	for (; x >= 0 && y < 15; --x, ++y)
	{
		if ((countx != 0 || counto != 0) && board[x][y] == 0)
		{
			countx = counto = 0;
		}
		if (board[x][y] == 1)
		{
			++counto;
			countx = 0;
		}
		else if (board[x][y] == 2)
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

void chessboard::update_remotecell(const int row, const int col, const bool add) noexcept
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

void chessboard::update_layer_2(const int row, const int col, const bool add) noexcept
{
	// 0 - horizontal
	// 1 - vertical
	int xx;
	if (add == false)
		xx = 0;
	else
		xx = board[row][col];
	layer_2[0][row][col] = xx;
	layer_2[1][col][row] = xx;
	int x, y;
	if (row < col)
	{
		x = col - row;
		y = 0;
	}
	else
	{
		x = 0;
		y = row - col;
	}
	layer_2[2][row - col + 14][col - x] = xx;
	if (14 - row < col)
	{
		x = col - 14 + row;
		y = 14;
	}
	else
	{
		x = 0;
		y = row + col;
	}
	layer_2[3][row + col][col - x] = xx;
}

std::vector<std::tuple<int, int8_t, int8_t>> chessboard::genmove() const noexcept
{
	std::vector<std::tuple<int, int8_t, int8_t>> moves;
	moves.reserve(64);
	int score;
	for (int i = 0; i < 15; ++i)
	{
		for (int ii = 0; ii < 15; ++ii)
		{
			if (remote_cell[i][ii] != 0 && board[i][ii] == 0)
			{
				score = pos[i][ii];
				moves.emplace_back(std::make_tuple(score, i, ii));
			}
		}
	}
	if (moves.empty())
		moves.push_back(std::make_tuple(3, 7, 7));
	std::sort(moves.rbegin(), moves.rend());
	return moves;
}

bool chessboard::Fullboard() const noexcept
{
	return number == 15 * 15;
}
