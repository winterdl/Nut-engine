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

#ifdef WRAPPER_EXPORTS
#define WRAPPER_API __declspec(dllexport)
#else
#define WRAPPER_API __declspec(dllimport)
#endif
#include"stdafx.h"
#include"chessboard.h"
#include"searcher.h"
#include<memory>
#include<iostream>
#include<string>


class WRAPPER_API Cwrapper {
public:
	Cwrapper(void);
	std::string About()
	{
		return "Nutengine v0.3.5 RC";
	}
	void Resetboard()
	{
		board.reset();
	}
	bool Fullboard()
	{
		return board.Fullboard();
	}
	auto Getboard()
	{
		return board;
	}
	bool Undoboard(int row, int col)
	{
		return board.undo(row, col);
	}
	int Checkwin(const int row, const int col)
	{
		return board.checkpoint(row, col);
	}
	auto Putcomputer(int turn, int timeout)
	{
		std::tuple<int, int8_t, int8_t> y;
		if (timeout >= 4000)
			y = searchman.alpha_beta_search(turn, std::ref(board), 7, timeout);
		else
			y = searchman.alpha_beta_search(turn, std::ref(board), 6, timeout);
		return y;
	}
	bool Put(int row, int col, int turn)
	{
		return board.put(row, col, turn);
	}
	bool IsFree(int8_t i, int8_t ii)
	{
		return board.get(i, ii) == 0;
	}
private:
	chessboard board;
	searcher searchman;
};