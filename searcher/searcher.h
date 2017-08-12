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

#pragma once
#include"stdafx.h"
#include"evaluation.h"
#include"chessboard.h"

#define DllExport   __declspec( dllexport )  

class DllExport searcher
{
public:
	std::tuple<int, int8_t, int8_t> alpha_beta_search(int turn, chessboard board, int depth, int timeout)noexcept;
private:
	evaluation evaluator;
	std::vector<std::tuple<int, int8_t, int8_t>> smart_genmove(int turn, chessboard, int depth, int current)noexcept;
	std::tuple<int, int8_t, int8_t> max_value(int turn, chessboard board, long long alpha, long long beta, int depth, int i, int ii, int ply)noexcept;
	std::tuple<int, int8_t, int8_t> min_value(int turn, chessboard board, long long alpha, long long beta, int depth, int i, int ii, int ply)noexcept;
	int timeoutnum = 30000;
};
