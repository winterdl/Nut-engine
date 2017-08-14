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

std::vector<std::tuple<int, int8_t, int8_t>> searcher::smart_genmove(int turn, chessboard board, int depth, int current) noexcept
{
	auto moves = board.genmove();
	if (current <= 2)
		return moves;
	const int alpha = -0x7fffffff, beta = 0x7fffffff;
	std::vector<std::tuple<int, int8_t, int8_t>> ress;
	decltype(ress) realretval;
	ress.reserve(1 + moves.size());
	bool check1 = false; // Searched normal results.
	bool check2 = false; // Searched very good result.
	bool check3 = false; // Searched ultra good result.
	bool check4 = false; // Win signal.
	for (auto&x : moves)
	{
		board.put(ref(std::get<1>(x)), ref(std::get<2>(x)), ref(turn));
		int nturn;
		if (turn == 1)
			nturn = 2;
		else
			nturn = 1;
		auto temp = min_value(nturn, ref(board), alpha, beta, depth - 1, std::get<1>(x), std::get<2>(x), 0);
		auto com = make_tuple(std::get<0>(temp), std::get<1>(x), std::get<2>(x));
		if (!check1&&std::get<0>(temp) >= 0)
			check1 = true;
		if (!check2&&std::get<0>(temp) >= 9000)
			check2 = true;
		if (!check3&&std::get<0>(temp) >= 9970)
			check3 = true;
		if (!check4&&std::get<0>(temp) >= 50000)
			check4 = true;
		if (check1&&std::get<0>(temp) <= -2000)
		{
			;
		}
		else if (check2&&std::get<0>(temp) < 2000)
		{
			;
		}
		else if (check3&&std::get<0>(temp) <= 9900)
		{
			;
		}
		else if (check4&&std::get<0>(temp) <= 15000)
		{
			;
		}
		else
			ress.emplace_back(com);
		board.undo(ref(std::get<1>(x)), ref(std::get<2>(x)));
		evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
	}
	if (check1 || check2|| check3||check4)
	{
		if (check4)
		{
			for (auto &xxx : ress)
			{
				if (std::get<0>(xxx) >= 15000)
					realretval.emplace_back(xxx);
			}
			sort(realretval.rbegin(), realretval.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
			const int maxnum = 16;
			size_t sizer = realretval.size();
			if (sizer > maxnum)
				realretval.resize(maxnum);
			return realretval;
		}
		else if (check3)
		{
			for (auto &xxx : ress)
			{
				if (std::get<0>(xxx) > 9900)
					realretval.emplace_back(xxx);
			}
			sort(realretval.rbegin(), realretval.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
			const int maxnum = 16;
			size_t sizer = realretval.size();
			if (sizer > maxnum)
				realretval.resize(maxnum);
			return realretval;
		}
		else if (check2)
		{
			for (auto &xxx : ress)
			{
				if (std::get<0>(xxx) >= 2000)
					realretval.emplace_back(xxx);
			}
			sort(realretval.rbegin(), realretval.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
			const int maxnum = 16;
			size_t sizer = realretval.size();
			if (sizer > maxnum)
				realretval.resize(maxnum);
			return realretval;
		}
		else// check1
		{
			for (auto &xxx : ress)
			{
				if (std::get<0>(xxx) > -2000)
					realretval.emplace_back(xxx);
			}
			sort(realretval.rbegin(), realretval.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
			const int maxnum = 16;
			size_t sizer = realretval.size();
			if (sizer > maxnum)
				realretval.resize(maxnum);
			return realretval;
		}
	}
	else
	{
		sort(ress.rbegin(), ress.rend(), [](const auto &i, const auto &ii) {return get<0>(i) < get<0>(ii); });
		const int maxnum = 16;
		size_t sizer = ress.size();
		if (sizer > maxnum)
			ress.resize(maxnum);
		return ress;
	}
}

std::tuple<int, int8_t, int8_t> searcher::alpha_beta_search(int turn, chessboard board, int depth, int timeout)noexcept
{
	timeoutnum = timeout;
	start = clock.now();
	evaluator.evaluate(ref(board), turn, -1, -1, true);
	return max_value(turn, ref(board), -0x7fffffff, 0x7fffffff, depth, -1, -1, 0);
}

std::tuple<int, int8_t, int8_t> searcher::max_value(int turn, chessboard board, long long alpha, long long beta, int depth, int i, int ii, int ply)noexcept
{
	bool changed = false;
	std::chrono::time_point<std::chrono::steady_clock> timeout;
	if (depth == 7)
		timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutnum);
	int nturn;
	if (turn == 1)
		nturn = 2;
	else
		nturn = 1;
	int res = evaluator.evaluate(ref(board), turn, i, ii, true);
	res = evaluator.evaluate(ref(board), turn, i, ii, false);
	//int checker = board.checkpoint(i, ii);
	if (res == 100000)
		return std::make_tuple(10000000 - ply, i, ii);
	else if (res == -100000)
		return std::make_tuple(0 - 10000000 + ply, i, ii);
	else if (depth <= 0)
	{
		//res = evaluator.evaluate(ref(board), turn, i, ii, false);
		return std::make_tuple(res, i, ii);
	}
	std::vector<std::tuple<int, int8_t, int8_t>> moves;
	moves = smart_genmove(turn, ref(board), 1, depth);
	std::tuple<int, int, int> v = std::make_tuple(-0x7fffffff, -1, -1);
	for (auto&x : moves)
	{
		board.put(ref(std::get<1>(x)), ref(std::get<2>(x)), ref(turn));
		std::tuple<int, int8_t, int8_t> temp;
		if (depth == 7)
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
		board.undo(ref(std::get<1>(x)), ref(std::get<2>(x)));
		evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
		if (std::get<0>(v) >= beta)
			return v;
		alpha = max(alpha, std::get<0>(v));
		if (chrono::duration_cast<chrono::milliseconds>(clock.now() - start).count() >= timeoutnum)
			break;
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
	int res = evaluator.evaluate(ref(board), turn, i, ii, true);
	res = 0 - evaluator.evaluate(ref(board), turn, i, ii, false);
	//int checker = board.checkpoint(i, ii);
	if (res == -100000)
		return std::make_tuple(0 - 10000000 + ply, i, ii);
	else if (res == 100000)
		return std::make_tuple(10000000 - ply, i, ii);
	else if (depth <= 0)
	{
		//res = 0 - evaluator.evaluate(ref(board), turn, i, ii, false);
		return std::make_tuple(res, i, ii);
	}
	auto moves = smart_genmove(turn, ref(board), 1, depth);
	std::tuple<int, int, int> v = std::make_tuple(0x7fffffff, -1, -1);
	for (auto&x : moves)
	{
		board.put(ref(std::get<1>(x)), ref(std::get<2>(x)), ref(turn));
		auto temp = max_value(nturn, ref(board), alpha, beta, depth - 1, std::get<1>(x), std::get<2>(x), ply + 1);
		auto com = make_tuple(std::get<0>(temp), std::get<1>(x), std::get<2>(x));
		if (get<0>(v) > get<0>(com))
			v = com;
		board.undo(ref(std::get<1>(x)), ref(std::get<2>(x)));
		evaluator.evaluate(ref(board), turn, std::get<1>(x), std::get<2>(x), true);
		if (std::get<0>(v) <= alpha)
			return v;
		beta = min(beta, std::get<0>(v));
		if (chrono::duration_cast<chrono::milliseconds>(clock.now() - start).count() >= timeoutnum)
			break;
	}
	return v;
}
