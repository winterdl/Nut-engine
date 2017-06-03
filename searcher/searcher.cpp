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
#include "searcher.h"
#include"chessboard.h"
using namespace std;

bool searcher::remotecell(std::array<std::array<int8_t, 15>, 15> board, int8_t row, int8_t col) const noexcept
{
	for (int i = row - 2; i <= row + 2; ++i)
	{
		if (i < 0 || i >= 15)
			continue;
		for (int j = col - 2; j <= col + 2; ++j)
		{
			if (j < 0 || j >= 15)
				continue;
			if (board[i][j] > 0)
				return false;
		}
	}
	return true;
}

std::vector<std::tuple<int, int8_t, int8_t>> searcher::genmove(std::array<std::array<int8_t, 15>, 15>board)const noexcept
{
	int min_r = INT_MAX, min_c = INT_MAX, max_r = INT_MIN, max_c = INT_MIN;
	for (int r = 0; r < 15; ++r)
	{
		for (int c = 0; c < 15; ++c)
		{
			if (board[r][c] != 0)
			{
				if (r < min_r)
					min_r = r;
				if (c < min_c)
					min_c = c;
				if (r > max_r)
					max_r = r;
				if (c > max_c)
					max_c = c;
			}
		}
	}
	if (min_r - 2 < 0)
		min_r = 2;
	if (min_c - 2 < 0)
		min_c = 2;
	if (max_r + 2 >= 15)
		max_r = 12;
	if (max_c + 2 >= 15)
		max_c = 12;
	std::vector<std::tuple<int, int8_t, int8_t>>moves;
	for (int i = min_r - 2; i <= max_r + 2; ++i)
	{
		for (int ii = min_c - 2; ii <= max_c + 2; ++ii)
		{
			if (board[i][ii] == 0)
			{
				int score;
				if (!remotecell(std::ref(board), i, ii))
				{
					score = evaluator.pos[i][ii];
					moves.push_back(std::make_tuple(score, i, ii));
				}
			}
		}
	}
	if (moves.empty())
		moves.push_back(std::make_tuple(3, 7, 7));
	std::sort(moves.rbegin(), moves.rend());
	return moves;
}

std::vector<std::tuple<int, int8_t, int8_t>> searcher::smart_genmove(int turn, chessboard board, int depth) noexcept
{
	int alpha = -0x7fffffff, beta = 0x7fffffff;
	std::vector<std::tuple<int, int8_t, int8_t>> ress;
	auto moves = genmove(ref(board.board));
	std::tuple<int, int, int> v = std::make_tuple(-0x7fffffff, -1, -1);
	for (auto&x : moves)
	{
		board.board[std::get<1>(x)][std::get<2>(x)] = turn;
		int nturn;
		if (turn == 1)
			nturn = 2;
		else
			nturn = 1;
		auto temp = min_value(nturn, ref(board), alpha, beta, depth - 2, std::get<1>(x), std::get<2>(x), 0);
		auto com = make_tuple(std::get<0>(temp), std::get<1>(x), std::get<2>(x));
		if (std::get<0>(v) < std::get<0>(com))
			v = com;
		ress.push_back(com);
		board.board[std::get<1>(x)][std::get<2>(x)] = 0;
		alpha = max(alpha, std::get<0>(v));
	}
	sort(ress.rbegin(), ress.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
	return ress;
}

std::tuple<int, int8_t, int8_t> searcher::alpha_beta_search(int turn, chessboard board, int depth, int timeout)noexcept
{
	timeoutnum = timeout;
	return max_value(turn, ref(board), -0x7fffffff, 0x7fffffff, depth, -1, -1, 0);
}

std::tuple<int, int8_t, int8_t> searcher::max_value(int turn, chessboard board, long long alpha, long long beta, int depth, int i, int ii, int ply)noexcept
{
	bool changed = false;
	std::chrono::time_point<std::chrono::steady_clock> timeout;
	if (depth == 4)
		timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutnum);
	int nturn;
	if (turn == 1)
		nturn = 2;
	else
		nturn = 1;
	int res = evaluator.evaluate(ref(board.board), turn);
	int checker = board.check();
	if (checker == turn)
		return std::make_tuple(10000000 - ply, i, ii);
	else if (checker == nturn)
		return std::make_tuple(0 - 10000000 + ply, i, ii);
	else if (depth <= 0)
		return std::make_tuple(res, i, ii);
	std::vector<std::tuple<int, int8_t, int8_t>> moves;
	if (depth >= 4)
		moves = smart_genmove(turn, ref(board), 2);
	else
		moves = genmove(ref(board.board));
	std::tuple<int, int, int> v = std::make_tuple(-0x7fffffff, -1, -1);
	for (auto&x : moves)
	{
		board.board[std::get<1>(x)][std::get<2>(x)] = turn;
		std::tuple<int, int8_t, int8_t> temp;
		if (depth == 4)
		{
			if (changed == false)
			{
				v = std::make_tuple(-0x7fffffff, std::get<1>(x), std::get<2>(x));
				changed = true;
			}
			auto the_answer = std::async(std::launch::async, &searcher::min_value, this, nturn, ref(board), alpha, beta, depth - 1, std::get<1>(x), std::get<2>(x), ply + 1);
			while (true)
			{
				if (the_answer.wait_until(timeout) == std::future_status::timeout)
					return v;
				else if (the_answer.wait_until(timeout) == std::future_status::ready)
				{
					temp = the_answer.get();
					break;
				}
			}
		}
		else
			temp = min_value(nturn, ref(board), alpha, beta, depth - 1, std::get<1>(x), std::get<2>(x), ply + 1);
		auto com = make_tuple(std::get<0>(temp), std::get<1>(x), std::get<2>(x));
		if (std::get<0>(v) < std::get<0>(com))
			v = com;
		board.board[std::get<1>(x)][std::get<2>(x)] = 0;
		if (std::get<0>(v) >= beta)
			return v;
		alpha = max(alpha, std::get<0>(v));
	}
	return v;
}

std::tuple<int, int8_t, int8_t> searcher::min_value(int turn, chessboard board, long long alpha, long long beta, int depth, int i, int ii, int ply)noexcept
{
	int nturn;
	if (turn == 1)
		nturn = 2;
	else
		nturn = 1;
	int res = 0 - evaluator.evaluate(ref(board.board), turn);
	int checker = board.check();
	if (checker == turn)
		return std::make_tuple(0 - 10000000 + ply, i, ii);
	else if (checker == nturn)
		return std::make_tuple(10000000 - ply, i, ii);
	else if (depth <= 0)
		return std::make_tuple(res, i, ii);
	auto moves = genmove(ref(board.board));
	std::tuple<int, int, int> v = std::make_tuple(0x7fffffff, -1, -1);
	for (auto&x : moves)
	{
		board.board[std::get<1>(x)][std::get<2>(x)] = turn;
		auto temp = max_value(nturn, ref(board), alpha, beta, depth - 1, std::get<1>(x), std::get<2>(x), ply + 1);
		auto com = make_tuple(std::get<0>(temp), std::get<1>(x), std::get<2>(x));
		if (get<0>(v) > get<0>(com))
			v = com;
		board.board[std::get<1>(x)][std::get<2>(x)] = 0;
		if (std::get<0>(v) <= alpha)
			return v;
		beta = min(beta, std::get<0>(v));
	}
	return v;
}
