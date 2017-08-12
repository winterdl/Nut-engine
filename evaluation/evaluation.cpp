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
#include "evaluation.h"

evaluation::evaluation()noexcept
{
	std::fill(result.begin(), result.end(), 0);
	std::fill(line.begin(), line.end(), 0);
	memset(count, 0, sizeof(count));
	memset(record, 0, sizeof(record));
}

void evaluation::reset()noexcept
{
	memset(count, 0, sizeof(count));
	memset(record, 0, sizeof(record));
}

int evaluation::analyse_line(std::array<int, 30>& line, std::array<int, 30>& record, int num, int pos)noexcept
{
	std::fill_n(record.begin(), num, TODO);
	if (num < 5)
	{
		std::fill_n(record.begin(), num, ANALYSED);
		return 0;
	}
	std::fill_n(line.rbegin(), 30 - num, 0xf);
	int8_t stone = line[pos];
	int8_t inverse = nturn[stone];

	--num;
	int xl = pos, xr = pos;
	while (xl > 0)
	{
		if (line[xl - 1] != stone)
			break;
		--xl;
	}
	while (xr < num)
	{
		if (line[xr + 1] != stone)
			break;
		++xr;
	}
	int left_range = xl, right_range = xr;
	while (left_range > 0)
	{
		if (line[left_range - 1] == inverse)
			break;
		--left_range;
	}
	while (right_range < num)
	{
		if (line[right_range + 1] == inverse)
			break;
		++right_range;
	}
	if (right_range - left_range < 4)
	{
		for (int k = left_range; k <= right_range; ++k)
			record[k] = ANALYSED;
		return 0;
	}
	for (int k = xl; k <= xr; ++k)
		record[k] = ANALYSED;
	int srange = xr - xl;
	if (srange >= 4)
	{
		record[pos] = FIVE;
		return FIVE;
	}
	if (srange == 3)
	{
		bool leftfour = false;
		if (xl > 0)
		{
			if (line[xl - 1] == 0)
				leftfour = true;
		}
		if (xr < num)
		{
			if (line[xr + 1] == 0)
			{
				if (leftfour)
					record[pos] = FOUR;
				else
					record[pos] = SFOUR;
			}
			else if (leftfour)
				record[pos] = SFOUR;
			return record[pos];
		}
	}
	if (srange == 2)
	{
		bool left3 = false;
		if (xl > 0)
		{
			if (line[xl - 1] == 0)
			{
				if (xl > 1 && line[xl - 2] == stone)
				{
					record[xl] = SFOUR;
					record[xl - 2] = ANALYSED;
				}
				else
					left3 = true;
			}
			else if (xr == num || line[xr + 1] != 0)
				return 0;
		}
		if (xr < num)
		{
			if (line[xr + 1] == 0)
			{
				if (xr < num - 1 && line[xr + 2] == stone)
				{
					record[xr] = SFOUR;
					record[xr + 2] = ANALYSED;
				}
				else if (left3)
					record[xr] = THREE;
				else
					record[xr] = STHREE;
			}
			else if (record[xl] == SFOUR)
				return record[xl];
			else if (left3)
				record[pos] = STHREE;
		}
		else
		{
			if (record[xl] == SFOUR)
				return record[xl];
			if (left3)
				record[pos] = STHREE;
		}
		return record[pos];
	}
	if (srange == 1)
	{
		bool left2 = false;
		if (xl > 2)
		{
			if (line[xl - 1] == 0)
			{
				if (line[xl - 2] == stone)
				{
					if (line[xl - 3] == stone)
					{
						record[xl - 3] = ANALYSED;
						record[xl - 2] = ANALYSED;
						record[xl] = SFOUR;
					}
					else if (line[xl - 3] == 0)
					{
						record[xl - 2] = ANALYSED;
						record[xl] = STHREE;
					}
				}
				else
					left2 = true;
			}
		}
		if (xr < num)
		{
			if (line[xr + 1] == 0)
			{
				if (xr < num - 2 && line[xr + 2] == stone)
				{
					if (line[xr + 3] == stone)
					{
						record[xr + 3] = ANALYSED;
						record[xr + 2] = ANALYSED;
						record[xr] = SFOUR;
					}
					else if (line[xr + 3] == 0)
					{
						record[xr + 2] = ANALYSED;
						if (left2)
							record[xr] = THREE;
						else
							record[xr] = STHREE;
					}
				}
				else
				{
					if (record[xl] == SFOUR)
						return record[xl];
					if (record[xl] == STHREE)
					{
						record[xl] = THREE;
						return record[xl];
					}
					if (left2)
						record[pos] = TWO;
					else
						record[pos] = STWO;
				}
			}
			else
			{
				if (record[xl] == SFOUR)
					return record[xl];
				if (left2)
					record[pos] = STWO;
			}
		}
		return record[pos];
	}
	return 0;
}

void evaluation::analysis_horizon(std::array<std::array<int8_t, 15>, 15>&board, int i, int j)noexcept
{
	for (int x = 0; x < 15; ++x)
		line[x] = board[i][x];
	analyse_line(line, result, 15, j);
	for (int x = 0; x < 15; ++x)
	{
		if (result[x] != TODO)
			record[i][x][0] = result[x];
	}
}

void evaluation::analysis_vertical(std::array<std::array<int8_t, 15>, 15>&board, int i, int j)noexcept
{
	for (int x = 0; x < 15; ++x)
		line[x] = board[x][j];
	analyse_line(line, result, 15, i);
	for (int x = 0; x < 15; ++x)
	{
		if (result[x] != TODO)
			record[x][j][1] = result[x];
	}
}

void evaluation::analysis_left(std::array<std::array<int8_t, 15>, 15>&board, int i, int j)noexcept
{
	int x, y, k = 0;
	if (i < j)
	{
		x = j - i;
		y = 0;
	}
	else
	{
		x = 0;
		y = i - j;
	}
	while (k < 15)
	{
		if (x + k > 14 || y + k > 14)
			break;
		line[k] = board[y + k][x + k];
		++k;
	}
	analyse_line(line, result, k, j - x);
	for (int s = 0; s < k; ++s)
	{
		if (result[s] != TODO)
			record[y + s][x + s][2] = result[s];
	}
}

void evaluation::analysis_right(std::array<std::array<int8_t, 15>, 15>&board, int i, int j)noexcept
{
	int x, y, k = 0;
	if (14 - i < j)
	{
		x = j - 14 + i;
		y = 14;
	}
	else
	{
		x = 0;
		y = i + j;
	}
	while (k < 15)
	{
		if (x + k > 14 || y - k < 0)
			break;
		line[k] = board[y - k][x + k];
		++k;
	}
	analyse_line(line, result, k, j - x);
	for (int s = 0; s < k; ++s)
	{
		if (result[s] != TODO)
			record[y - s][x + s][3] = result[s];
	}
}

int evaluation::__evaluate(std::array<std::array<int8_t, 15>, 15>&board, int turn)noexcept
{
	reset();
	for (int i = 0; i < 15; ++i)
	{
		auto boardrow = board[i];
		auto recordrow = record[i];
		for (int j = 0; j < 15; ++j)
		{
			if (boardrow[j] != 0)
			{
				if (recordrow[j][0] == TODO)
					analysis_horizon(board, i, j);
				if (recordrow[j][1] == TODO)
					analysis_vertical(board, i, j);
				if (recordrow[j][2] == TODO)
					analysis_left(board, i, j);
				if (recordrow[j][3] == TODO)
					analysis_right(board, i, j);
			}
		}
	}
	for (int i = 0; i < 15; ++i)
	{
		for (int j = 0; j < 15; ++j)
		{
			int stone = board[i][j];
			if (stone != 0)
			{
				for (int k = 0; k < 4; ++k)
				{
					int ch = record[i][j][k];
					if (ch >= STWO&&ch <= FIVE)
						++count[stone][ch];
				}
			}
		}
	}
	if (count[BLACK][FIVE])
		return 9999 * checkturn(BLACK, turn);
	if (count[WHITE][FIVE])
		return 9999 * checkturn(WHITE, turn);
	if (count[WHITE][SFOUR] >= 2)
		++count[WHITE][FOUR];
	if (count[BLACK][SFOUR] >= 2)
		++count[BLACK][FOUR];
	int wvalue = 0, bvalue = 0;
	if (turn == WHITE)
	{
		if (count[WHITE][FOUR] > 0)
			return 9990;
		if (count[WHITE][SFOUR] > 0)
			return 9980;
		if (count[BLACK][FOUR] > 0)
			return -9970;
		if (count[BLACK][SFOUR] && count[BLACK][THREE])
			return -9960;
		if (count[WHITE][THREE] && count[BLACK][SFOUR] == 0)
			return 9950;
		if (count[BLACK][THREE] > 1 &&
			count[WHITE][SFOUR] == 0 &&
			count[WHITE][THREE] == 0 &&
			count[WHITE][STHREE] == 0)
			return -9940;
		if (count[WHITE][THREE] > 1)
			wvalue += 2000;
		else if (count[WHITE][THREE])
			wvalue += 200;
		if (count[BLACK][THREE] > 1)
			bvalue += 500;
		else if (count[BLACK][THREE])
			bvalue += 100;
		if (count[WHITE][STHREE])
			wvalue += count[WHITE][STHREE] * 10;
		if (count[BLACK][STHREE])
			bvalue += count[BLACK][STHREE] * 10;
		if (count[WHITE][TWO])
			wvalue += count[WHITE][TWO] * 4;
		if (count[BLACK][TWO])
			bvalue += count[BLACK][TWO] * 4;
		if (count[WHITE][STWO])
			wvalue += count[WHITE][STWO];
		if (count[BLACK][STWO])
			bvalue += count[BLACK][STWO];
	}
	else
	{
		if (count[BLACK][FOUR] > 0)
			return 9990;
		if (count[BLACK][SFOUR] > 0)
			return 9980;
		if (count[WHITE][FOUR] > 0)
			return -9970;
		if (count[WHITE][SFOUR] && count[WHITE][THREE])
			return -9960;
		if (count[BLACK][THREE] && count[WHITE][SFOUR] == 0)
			return 9950;
		if (count[WHITE][THREE] > 1 &&
			count[BLACK][SFOUR] == 0 &&
			count[BLACK][THREE] == 0 &&
			count[BLACK][STHREE] == 0)
			return -9940;
		if (count[BLACK][THREE] > 1)
			bvalue += 2000;
		else if (count[BLACK][THREE])
			bvalue += 200;
		if (count[WHITE][THREE] > 1)
			wvalue += 500;
		else if (count[WHITE][THREE])
			wvalue += 100;
		if (count[BLACK][STHREE])
			bvalue += count[BLACK][STHREE] * 10;
		if (count[WHITE][STHREE])
			wvalue += count[WHITE][STHREE] * 10;
		if (count[BLACK][TWO])
			bvalue += count[BLACK][TWO] * 4;
		if (count[WHITE][TWO])
			wvalue += count[WHITE][TWO] * 4;
		if (count[BLACK][STWO])
			bvalue += count[BLACK][STWO];
		if (count[WHITE][STWO])
			wvalue += count[WHITE][STWO];
	}
	if (turn == WHITE)
		return wvalue - bvalue;
	return bvalue - wvalue;
}

int evaluation::evaluate(std::array<std::array<int8_t, 15>, 15>&board, int turn)noexcept
{
	int score = __evaluate(board, turn);
	if (score < -9000)
	{
		int stone;
		if (turn == 1)
			stone = 2;
		else
			stone = 1;
		for (int i = 0; i < 20; ++i)
			if (count[stone][i] > 0)
				score -= i;
	}
	else if (score > 9000)
	{
		int stone;
		if (turn == 1)
			stone = 2;
		else
			stone = 1;
		for (int i = 0; i < 20; ++i)
			if (count[stone][i] > 0)
				score += i;
	}
	return score;
}
