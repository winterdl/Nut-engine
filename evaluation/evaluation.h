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
public:
	evaluation()noexcept {};
	int evaluate(chessboard&, const int turn)noexcept;
private:
	inline void reset()noexcept
	{
		memset(count, 0, sizeof(count));
		memset(record, 0, sizeof(record));
	}
	void analyse_line(const std::array<uint8_t, 15>& line, std::array<int, 15>& record, int num, const int pos)noexcept;
	void analysis_horizon(chessboard&, int i, int j)noexcept;
	void analysis_vertical(chessboard&, int i, int j)noexcept;
	void analysis_left(chessboard&, int i, int j)noexcept;
	void analysis_right(chessboard&, int i, int j)noexcept;
	inline int checkturn(const int &side, const int &turn)const noexcept
	{
		if (side == turn)
			return 1;
		return -1;
	}
	inline int sign(int number)const noexcept
	{
		if (number < 0)
			return -1;
		return 1;
	}
	int __evaluate(chessboard&, int turn)noexcept;
	const int STWO = 1, STHREE = 2, SFOUR = 3, TWO = 4, THREE = 5, FOUR = 6, FIVE = 7, DFOUR = 8,
		FOURT = 9, DTHREE = 10, NOTYPE = 11, ANALYSED = 255, TODO = 0;
	const int BLACK = 1, WHITE = 2;
	const std::array<int, 3>nturn = { 0,2,1 };
	std::array<int, 15>result;
	int record[15][15][4];//[row][col][direction]
	int count[3][20];
};
