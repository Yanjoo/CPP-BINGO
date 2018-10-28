/*
제목: 빙고 게임 만들기
만든이: 이한주
만든 날짜: 2018-10-28
*/

#include <iostream>
#include <ctime>
#include <Windows.h>

using namespace std;

enum AI_MODE { EASY = 1, HARD };	// AI 난이도

enum LINE// 빙고 라인 12개
{
	LN_H1, LN_H2, LN_H3, LN_H4, LN_H5,	// 가로 빙고 라인
	LN_V1, LN_V2, LN_V3, LN_V4, LN_V5,	// 세로 빙고 라인
	LN_LT, LN_RT	// 대각선 빙고 라인
};

const int STAR = -1;	// * 표시를 위한 변수 STAR

// 함수 분리
int SelectMode();
void SetBingo(int bingo[]);
void PrintBingo(int bingo[]);
int SelectNumber(bool &userSelect, bool &finish, AI_MODE mode);
bool CheckOverlap(int bingo[], int input);
int CheckBingoV(int bingo[]);
int CheckBingoH(int bingo[]);
int CheckBingoLD(int bingo[]);
int CheckBingoRD(int bingo[]);

// 유저, AI 빙고판
int iNumber[25];
int AINumber[25];

int main(void)
{
	// 유저, AI 빙고판 초기화
	for (int i = 0; i < 25; i++)
		AINumber[i] = iNumber[i] = i + 1;

	srand((unsigned)time(NULL));

	// AI 난이도 선택
	int mode = SelectMode();
	
	// 빙고판 초기화
	SetBingo(iNumber);
	SetBingo(AINumber);
	
	bool finish = false; //반복문 조건
	int bingoLine = 0, AIbingoLine = 0;	// user와 AI의 빙고 수
	bool userSelect = true; // user입력 판별

	while (!finish)
	{
		system("cls");

		// 유저 빙고 출력
		cout << "================User===============" << endl;
		PrintBingo(iNumber);
		cout << "User Bingo Line: " << bingoLine << endl << endl;
		if (bingoLine >= 5)
		{
			cout << "User 승리!" << endl;
			break;
		}

		// AI 빙고 출력
		cout << "=================AI================" << endl;
		PrintBingo(AINumber);
		cout << "AI Bingo Line: " << AIbingoLine << endl << endl;
		if (AIbingoLine >= 5)
		{
			cout << "AI 승리!" << endl;
			break;
		}

		int input = SelectNumber(userSelect, finish, (AI_MODE)mode);	// 숫자 입력 변수

		// 예외 처리
		if (input < 1 || input > 25)
		{
			userSelect = !userSelect;
			continue;
		}

		// 유저 중복 체크
		bool bAcc = CheckOverlap(iNumber, input);
		if (bAcc)
		{
			cout << "User 선택 중복, 재 선택" << endl;
			userSelect = !userSelect;
			Sleep(2000);
			continue;
		}
		
		// AI 중복 체크
		bAcc = CheckOverlap(AINumber, input);
		if (bAcc)
		{
			cout << "AI 선택 중복, 재 선택" << endl;
			userSelect = !userSelect;
			Sleep(2000);
			continue;
		}

		// 빙고 체크
		bingoLine = CheckBingoV(iNumber) + CheckBingoH(iNumber) + CheckBingoLD(iNumber) + CheckBingoRD(iNumber);
		AIbingoLine = CheckBingoV(AINumber) + CheckBingoH(AINumber) + CheckBingoLD(AINumber) + CheckBingoRD(AINumber);
	}

	return 0;
}

// 모드 선택
int SelectMode()
{
	int mode;
	while (true)
	{
		system("cls");
		cout << "AI 난이도 선택(1:EASY, 2:HARD): ";
		cin >> mode;
		if (mode == 1 || mode == 2)
			break;
	}
	return mode;
}

// 빙고판 세팅
void SetBingo(int bingo[])
{
	// 빙고판의 순서를 섞어 준다
	int idx1, idx2, temp;
	for (int i = 0; i < 100; i++)
	{
		// 유저의 빙고판
		idx1 = rand() % 25;
		idx2 = rand() % 25;
		temp = bingo[idx1];
		bingo[idx1] = bingo[idx2];
		bingo[idx2] = temp;
	}
}

// 빙고판 출력
void PrintBingo(int bingo[])
{
	for (int i = 0; i < 25; i++)
	{
		if (bingo[i] == STAR)
			cout << '*' << '\t';
		else
			cout << bingo[i] << '\t';

		if ((i + 1) % 5 == 0)
			cout << '\n';
	}
}

// 숫자 선택 함수
int SelectNumber(bool &userSelect, bool &finish, AI_MODE mode)
{
	int input;
	if (userSelect) // user 선택
	{
		// 숫자 입력
		cout << "숫자를 입력하세요(0 종료)> ";
		cin >> input;

		// 종료조건
		if (input == 0)
			finish = true;
		// 유저가 한번 선택 한 후 AI가 선택
		userSelect = false;
	}
	else // AI 선택
	{
		if (mode == EASY)	// 선택하지 않은 수에서 랜덤하게 선택
		{
			int count = 0;
			int arr[25];
			for (int i = 0; i < 25; i++)
			{
				if (AINumber[i] != STAR)
					arr[count++] = AINumber[i];
			}
			input = arr[rand() % count];
		}

		if (mode == HARD)	// 가장 빙고 가능성이 높은 라인에서 선택
		{
			int count = 0;
			int max_star = 0;	// 5개 이하중에서 가장 많은 별의 개수
			int line = 0;		// max_star의 라인 (enum LINE에서 선택)

			for (int i = 0; i < 5; i++)
			{
				count = 0;
				for (int j = 0; j < 5; j++)
				{
					// 가로 라인 체크
					if (AINumber[i * 5 + j] == STAR)
					{
						++count;
					}
				}

				if (max_star <= count && count < 5)
				{
					line = i;
					max_star = count;
				}
			}

			for (int i = 0; i < 5; i++)
			{
				count = 0;
				for (int j = 0; j < 5; j++)
				{
					// 세로 라인 체크
					if (AINumber[i + j * 5] == STAR)
					{
						++count;
					}
				}

				if (max_star < count && count < 5)
				{
					line = i + 5;
					max_star = count;
				}
			}

			// 왼쪽 -> 오른쪽 대각선 체크
			count = 0;
			for (int i = 0; i < 25; i += 6)
			{
				if (AINumber[i] == STAR)
				{
					++count;
				}
			}
			if (max_star < count && count < 5)
			{
				line = LN_LT;
				max_star = count;
			}

			// 오른쪽 -> 왼쪽 대각선 체크
			count = 0;
			for (int i = 4; i <= 20; i += 4)
			{
				if (AINumber[i] == STAR)
				{
					++count;
				}
			}
			if (max_star < count && count < 5)
			{
				line = LN_RT;
				max_star = count;
			}

			if (line <= LN_H5) // 가로 줄이 최대 일 경우
			{
				for (int i = 0; i < 5; i++)
				{
					if (AINumber[line * 5 + i] != STAR)
					{
						input = AINumber[line * 5 + i];
						break;
					}
				}
			}
			else if (line <= LN_V5) // 세로 줄이 최대일 경우
			{
				for (int i = 0; i < 5; i++)
				{
					if (AINumber[(line - 5) + i * 5] != STAR)
					{
						input = AINumber[(line - 5) + 5 * i];
						break;
					}
				}
			}
			else if (line == LN_LT) // 왼쪽 -> 오른쪽 대각선이 최대일 경우
			{
				for (int i = 0; i < 25; i += 6)
				{
					if (AINumber[i] != STAR)
					{
						input = AINumber[i];
						break;
					}
				}
			}
			else    // 오른쪽 -> 왼쪽 대각선이 최대일 경우
			{
				for (int i = 4; i <= 20; i += 4)
				{
					if (AINumber[i] != STAR)
					{
						input = AINumber[i];
						break;
					}
				}
			}

		}

		userSelect = true; // AI가 선택 후 user가 선택
		cout << "AI 선택: " << input << endl;
		Sleep(2000);
	}
	return input;
}

// 숫자 중복 체크
bool CheckOverlap(int bingo[], int input)
{
	bool bAcc = true;
	for (int i = 0; i < 25; i++)
	{
		if (input == bingo[i])
		{
			// 숫자를 찾으면 중복이 아니므로
			bAcc = false;
			// *로 바꾸기 위해 STAR로 저장한다
			bingo[i] = STAR;
			// 더 이상 찾을 필요가 없으므로 break
			break;
		}
	}
	return bAcc;
}

// 가로 줄 빙고 체크
int CheckBingoH(int bingo[])
{
	// 빙고 체크
	int star;
	int bingoLine = 0;

	// 가로 빙고 체크
	for (int i = 0; i < 5; i++)
	{
		// 유저 빙고 체크
		star = 0;
		for (int j = 0; j < 5; j++)
		{
			if (bingo[i * 5 + j] == STAR)
				star++;
		}

		if (star == 5)
			bingoLine++;
	}
	return bingoLine;
}

// 세로 빙고 줄 체크
int CheckBingoV(int bingo[])
{
	// 빙고 체크
	int star;
	int bingoLine = 0;

	// 세로 빙고 체크
	for (int i = 0; i < 5; i++)
	{
		// 유저 빙고 체크
		star = 0;
		for (int j = 0; j < 5; j++)
		{
			if (bingo[i + j * 5] == STAR)
				star++;
		}

		if (star == 5)
			bingoLine++;
	}

	return bingoLine;
}

// 좌 대각선 빙고 체크
int CheckBingoLD(int bingo[])
{
	// 대각선(좌상단 -> 우하단) 빙고 체크
	int star = 0, bingoLine = 0;
	for (int i = 0; i < 25; i += 6)
	{
		// 유저 빙고 체크
		if (bingo[i] == STAR)
			star++;

		if (star == 5)
			bingoLine++;
	}
	return bingoLine;
}

// 우 대각선 빙고 체크
int CheckBingoRD(int bingo[])
{
	// 대각선(우상단 -> 좌하단) 빙고 체크
	int star = 0, bingoLine = 0;
	for (int i = 4; i <= 20; i += 4)
	{
		// 유저 빙고 체크
		if (bingo[i] == STAR)
			star++;

		if (star == 5)
			bingoLine++;
	}
	return bingoLine;
}
