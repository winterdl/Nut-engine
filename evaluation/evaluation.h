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

#define DllExport   __declspec( dllexport )  

class DllExport evaluation
{
	friend class searcher;
	friend class chessboard;
public:
	evaluation()noexcept;
	int evaluate(std::array<std::array<int8_t, 15>, 15>&, int turn)noexcept;
private:
	int pos[15][15];
	void reset()noexcept;
	int analyse_line(int line[30], int record[30], int num, int pos)noexcept;
	int analysis_horizon(std::array<std::array<int8_t, 15>, 15>&, int i, int j)noexcept;
	int analysis_vertical(std::array<std::array<int8_t, 15>, 15>&, int i, int j)noexcept;
	int analysis_left(std::array<std::array<int8_t, 15>, 15>&, int i, int j)noexcept;
	int analysis_right(std::array<std::array<int8_t, 15>, 15>&, int i, int j)noexcept;
	int __evaluate(std::array<std::array<int8_t, 15>, 15>&, int turn)noexcept;
	const int STWO = 1, STHREE = 2, SFOUR = 3, TWO = 4, THREE = 5, FOUR = 6, FIVE = 7, DFOUR = 8,
		FOURT = 9, DTHREE = 10, NOTYPE = 11, ANALYSED = 255, TODO = 0;
	int result[30];
	int line[30];
	int record[15][15][4];//[row][col][direction]
	int count[3][20];
};
