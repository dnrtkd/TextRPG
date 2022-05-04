#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string>
#include <time.h>
#include <Windows.h>
#include <conio.h>

//씬 변환을 위한 변수
int SceneState = 0;
//씬 변환 상수
const int Scene_Logo = 0;
const int Scene_Menu = 1;
const int Scene_NewGame = 2;
const int Scene_Menu2 = 3;
const int Scene_SelectField = 4;
const int Scene_InField = 5;
const int Scene_Battle = 6;
const int Scene_Exit = 7;
//현재 맵
int MapState = 0;
//맵 상수
const int Map_Samgir = 0;
const int Map_SmallCave = 1;
const int Map_seaBottom = 2;

const int Job_sword = 0;
const int Job_bow = 1;
const int Job_gun = 2;

typedef struct tagMap
{
	char* name;
	int levelLimit;
	int apearMonster[3];
}Map; //맵 정보 구조체

//스텟
typedef struct tagStatus
{
	int hp;
	int att;
	int def;
	int quick;
}STATUS;
//방어구
typedef struct tagProtection
{
	int limitLevel;
	char* name;
	int def;
}PROTECTION;

typedef struct tagWeapon
{
	int limitLevel;
	char* name;
	int att;
}WEAPON;
typedef struct tagPlayer //플레이어 구조체
{
	int level;
	char* name;
	int exp;
	int gold;
	int job;
	STATUS status;
	WEAPON weapon;
	PROTECTION protection;
}Player;

typedef struct tagInventory
{
	int size;

}INVENTORY;

typedef struct tagMonster
{
	char* name;
	int level;
	int Hp;
	int att;
	int def;
	int quick;
	int exp;
	int gold;
}MONSTER;


Map maps[3];
WEAPON weapons[9];
PROTECTION protections[3];
MONSTER monsters[9];

void HideCursor();
void SetPosition(int _x, int _y, char* _str, int _Color);
void SetColor(int _Color);
void LogoScene();
void SceneManager();
void MenuScene();
void MenuScene2();
char* SetName();
void SelectFieldScene();
void BattleScene();
void InField();
void showFrame(int w, int h, int startX, int startY);
void allotPoints();
void CreatePlayerScene();
void showPlayer();
void SetCursor(int _x, int _y);
void winUi(MONSTER* p);
void rewardGain(MONSTER* p);
int PlayerAttack(const MONSTER* p);
int monsAttack(const MONSTER* p);
void WeaponDataInput(int index, int limitLevel, const char* name, int att)
{
	weapons[index].limitLevel = limitLevel;
	weapons[index].name = (char*)name;
	weapons[index].att = att;
}
void ProtectionDataInput(int index, int limitLevel, const char* name, int def)
{
	protections[index].limitLevel = limitLevel;
	protections[index].name = (char*)name;
	protections[index].def = def;
}
void MonsterDataInput(int index, const char* name, int level, int hp, int att, int def, int quick, int exp, int gold)
{
	monsters[index].name = (char*)name;
	monsters[index].level = level;
	monsters[index].Hp = hp;
	monsters[index].att = att;
	monsters[index].def = def;
	monsters[index].quick = quick;
	monsters[index].exp = exp;
	monsters[index].gold = gold;
}
void MapDataInput(int index, const char* name, int levelLimit, int appearMon1, int appearMon2, int appearMon3)
{
	maps[index].name = (char*)name;
	maps[index].levelLimit = levelLimit;
	maps[index].apearMonster[0] = appearMon1;
	maps[index].apearMonster[1] = appearMon2;
	maps[index].apearMonster[2] = appearMon3;
}
MONSTER* CreateMonster(int index)
{
	MONSTER* p = (MONSTER*)malloc(sizeof(MONSTER));
	p->name = monsters[index].name;
	p->att = monsters[index].att;
	p->def = monsters[index].def;
	p->exp = monsters[index].exp;
	p->Hp = monsters[index].Hp;
	p->level = monsters[index].level;
	p->quick = monsters[index].quick;
	p->exp = monsters[index].exp;
	p->gold = monsters[index].gold;

	return p;
}

Player player;
//const int CursorX;
//const int CursorY;

void EquipWeapon(WEAPON w)
{
	if (w.limitLevel <= player.level)
	{
		player.weapon = w;
		player.status.att += w.att;
	}
	else
		printf_s("장착 불가!");
}

void EquipProtection(PROTECTION p)
{
	if (p.limitLevel <= player.level)
	{
		player.protection = p;
		player.status.def += p.def;
	}
	else
		printf_s("장착 불가!");
}

int main(void)
{
	srand(time(0));

	//장비 초기화
	WeaponDataInput(0, 1, "죽도", 5);
	WeaponDataInput(1, 5, "환도", 8);
	WeaponDataInput(2, 10, "청도", 12);
	WeaponDataInput(3, 1, "수련용활", 7);
	WeaponDataInput(4, 5, "목궁", 10);
	WeaponDataInput(5, 10, "철궁", 15);
	WeaponDataInput(6, 1, "수렵 총", 10);
	WeaponDataInput(7, 5, "조총", 15);
	WeaponDataInput(8, 10, "화승총", 20);

	ProtectionDataInput(0, 1, "각목 갑옷", 5);
	ProtectionDataInput(1, 5, "청동 갑옷", 10);
	ProtectionDataInput(2, 10, "철 갑옷", 15);

	MonsterDataInput(0, "꼬꼬닭", 1, 30, 5, 5, 5, 30, 20);
	MonsterDataInput(1, "상경쥐", 2, 35, 6, 5, 5, 40, 25);
	MonsterDataInput(2, "구리왕", 3, 60, 7, 7, 5, 60, 50);
	MonsterDataInput(3, "철퇴병사", 5, 100, 10, 10, 5, 80, 35);
	MonsterDataInput(4, "남한궁병", 6, 70, 12, 5, 9, 90, 45);
	MonsterDataInput(5, "도부수", 7, 150, 12, 8, 6, 150, 90);
	MonsterDataInput(6, "조총수", 10, 110, 15, 5, 7, 120, 60);
	MonsterDataInput(7, "칼잡이", 11, 130, 13, 8, 6, 130, 80);
	MonsterDataInput(8, "사무라이", 12, 250, 16, 12, 10, 300, 200);

	MapDataInput(0, "한양 부근", 1, 0, 1, 2);
	MapDataInput(1, "남한 산성", 5, 3, 4, 5);
	MapDataInput(2, "대마도", 10, 6, 7, 8);
	// ** 커서를 안보이게 함.
	HideCursor();

	// ** 콘솔창의 사이즈를 설정.
	system("mode con:cols=120 lines=30");

	// ** 콜솔창의 이름을 설정
	system("title 홍길동 Framework v0.6");

	// ** 전체 배경색을 변경함.
	//system("color 70");

	DWORD dwTime = GetTickCount(); // 1/1000 (1000분의 1초)
	int Delay = 1000;

	int UpCount = 0;

	while (true)
	{
		if (dwTime + Delay < GetTickCount())
		{
			dwTime = GetTickCount();

			// ** 콘솔창에 있는 모든 내용을 지움.
			system("cls");
			// ** 게임 루프
			SceneManager();
		}
	}

	return 0;
}

void HideCursor()
{
	CONSOLE_CURSOR_INFO Info;

	Info.dwSize = 1;// 커서의 크기
	Info.bVisible = false; //커서의 표시여부

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);//인수: 콘솔 기본 핸들, 인포 값
}

void SceneManager()
{
	switch (SceneState)
	{
	case Scene_Logo:
		LogoScene();
		break;
	case Scene_Menu:
		MenuScene();
		break;
	case Scene_NewGame:
		CreatePlayerScene();
		break;
	case Scene_Menu2:
		MenuScene2();
		break;
	case Scene_SelectField:
		SelectFieldScene();
		break;
	case Scene_InField:
		InField();
		break;
	case Scene_Battle:
		BattleScene();
		break;
	case Scene_Exit:
		exit(NULL);// ** 프로그램 종료
		break;
	}
}

void LogoScene()
{
	int Width = (120 / 2) - (strlen("      ,gggg,     _,gggggg,_        ,gg,         _,gggggg,_      ") / 2);
	int Height = 10;

	SetPosition(Width, Height + 1, (char*)"      ,gggg,     _,gggggg,_        ,gg,         _,gggggg,_      ", 1);
	SetPosition(Width, Height + 2, (char*)"      d8` `8I   ,d8P``d8P`Y8b,     i8``8i      ,d8P``d8P`Y8b,   ", 2);
	SetPosition(Width, Height + 3, (char*)"      88  ,dP  ,d8'   Y8   `8b,dP  `8,,8'     ,d8'   Y8   `8b,dP", 3);
	SetPosition(Width, Height + 4, (char*)"   8888888P`   d8'    `Ybaaad88P'   `Y88aaad8 d8'    `Ybaaad88P'", 4);
	SetPosition(Width, Height + 5, (char*)"      88       8P       `````Y8      d8````Y8,8P       `````Y8  ", 5);
	SetPosition(Width, Height + 6, (char*)"      88       8b            d8     ,8P     8b8b            d8  ", 6);
	SetPosition(Width, Height + 7, (char*)" ,aa,_88       Y8,          ,8P     dP      Y8Y8,          ,8P  ", 7);
	SetPosition(Width, Height + 8, (char*)"dP` `88P       `Y8,        ,8P' _ ,dP'      I8`Y8,        ,8P'  ", 8);
	SetPosition(Width, Height + 9, (char*)"Yb,_,d88b,,_    `Y8b,,__,,d8P'  `888,,_____,dP `Y8b,,__,,d8P'   ", 9);
	SetPosition(Width, Height + 10, (char*)" `Y8P`  `Y88888   ``Y8888P`'    a8P`Y888888P`    ``Y8888P`'     ", 10);
	SetColor(1);

	Sleep(3000);
	SetPosition(Width + 10, Height + 12, (char*)"게임을 시작하려면, Enter키를 눌러주세요.", 2);
	char c;
	while (true)
	{
		c = _getch();
		if (c == 13)break;
	}

	SceneState++;
}


void MenuScene()
{
	printf_s("MenuScene\n\n");

	//printf_s("다음 씬 ㄱㄱ??\n1. 게임시작\n2. 종료\n입력 : ");
	printf_s("1. 새로시작\n");
	printf_s("2. 이어하기\n");
	printf_s("3. 게임종료\n");
	int i = 0;
	scanf("%d", &i);

	if (i == 1)
		SceneState++;
	else if (i == 2) SceneState += 2;
	else if (i == 3)
		SceneState = Scene_Exit;
}
//새로 하기 씬
void CreatePlayerScene()
{
	printf_s("이름 입력: ");
	player.name = SetName();
	player.level = 1;
	player.gold = 0;
	player.exp = 0;

	printf_s("\n\n직업 선택\n1. 검술사\n2. 궁술사\n3. 총술사");
	int i = 0;
	scanf("%d", &i);

	if (i == 1) player.job = 0;
	if (i == 2) player.job = 1;
	if (i == 3) player.job = 2;

	EquipWeapon(weapons[0]);
	EquipProtection(protections[0]);

	allotPoints();
	SceneState++;
}
//신규 캐릭터 스텟 분배
void allotPoints()
{
	if (player.job == Job_sword)
	{
		player.status.att = 8;
		player.status.def = 8;
		player.status.quick = 5;
		player.status.hp = 50;
	}
	else if (player.job == Job_bow)
	{
		player.status.att = 8;
		player.status.def = 5;
		player.status.quick = 6;
		player.status.hp = 40;
	}
	else if (player.job == Job_gun)
	{
		player.status.att = 10;
		player.status.def = 4;
		player.status.quick = 5;
		player.status.hp = 35;
	}
}


void MenuScene2()
{
	printf_s("MenuScene2\n");

	printf_s("\n1. 필드탐험\n2. 보유페트\n3. 인벤토리\n4. 플레이어 정보");

	int i = 0;
	scanf("%d", &i);

	if (i == 1)
		SceneState++;
	else if (i == 2) {}
	else if (i == 3) {}
	else if (i == 4)
	{
		showPlayer();
	}

}

void showPlayer()
{
	short startX = 40;
	short startY = 5;


	showFrame(40, 18, startX, startY);
	SetCursor(startX + 3, startY + 2);
	printf_s("플레이어 닉네임: %s", player.name);

	SetCursor(startX + 2, startY + 3);
	printf_s("레벨          : %d", player.level);

	SetCursor(startX + 2, startY + 4);
	printf_s("경험치        : %d", player.exp);

	SetCursor(startX + 2, startY + 5);
	printf_s("직업          : %d", player.job);

	SetCursor(startX + 2, startY + 6);
	printf_s("골드          : %d", player.gold);

	SetCursor(startX + 2, startY + 7);
	printf_s("체력          : %d", player.status.hp);

	SetCursor(startX + 2, startY + 8);
	printf_s("공격력        : %d", player.status.att);

	SetCursor(startX + 2, startY + 9);
	printf_s("방어력        : %d", player.status.def);

	SetCursor(startX + 2, startY + 10);
	printf_s("순발력        : %d", player.status.quick);

	SetCursor(startX + 2, startY + 11);
	printf_s("장착 무기     : %s", player.weapon.name);

	SetCursor(startX + 2, startY + 12);
	printf_s("장착 방어구   : %s", player.protection.name);

	SetCursor(startX + 2, startY + 14);
	printf_s("1. 캐릭터 정보창 닫기");

	int i = 0;
	scanf("%d", &i);


}

void SelectFieldScene()
{
	printf_s("SelectFieldScene\n\n");

	for (int i = 0; i < 3; i++)
	{
		if (!(maps[i].name == nullptr))
			printf_s("%d. %s\n", i + 1, maps[i].name);
	}

	printf_s("4. 뒤로 가기\n");
	int i = 0;
	scanf("%d", &i);
	if (i > 0 && i < 4)
	{
		if (maps[i - 1].levelLimit <= player.level)
		{
			MapState = i - 1;
			SceneState++;
		}
		else
			printf_s("레벨 제한");
	}
	if (i == 4) SceneState--;


}
void InField()
{
	system("cls");

	printf("%s\n", maps[MapState].name);

	printf("1. 사냥\n2. 인벤토리\n3. 뒤로가기");

	int i = 0;
	scanf("%d", &i);

	if (i == 1)
	{
		SceneState++;
	}
	else if (i == 2)
	{

	}
	else if (i == 3)
	{
		SceneState--;
	}
	else if (i == 4)
	{
		SceneState--;
	}
}
void BattleScene()
{
	int hidden = rand() % 100;
	MONSTER* p = NULL;

	if (hidden < 10) p = CreateMonster(maps[MapState].apearMonster[2]); //가장 강한 몬스터
	else if (hidden < 50) p = CreateMonster(maps[MapState].apearMonster[1]); //중간
	else p = CreateMonster(maps[MapState].apearMonster[0]); //가장 약함

	int monsCurrHp = p->Hp;
	int PlayCurrHp = player.status.hp;
	while (true)
	{
		system("cls");
		printf_s("몬스터 이름: %s\t레벨: %d\tHP: %d/%d\n\n\n", p->name, p->level, monsCurrHp, p->Hp);
		printf_s("플레이어: %s\t레벨: %d\tHP: %d/%d\n\n", player.name, player.level, PlayCurrHp, player.status.hp);
		printf_s("1. 공격\n2. 도망");

		int i = 0;
		scanf_s("%d", &i);

		if (i == 1)
		{
			if (player.status.quick >= p->quick)
			{
				monsCurrHp -= PlayerAttack(p);

				if (monsCurrHp <= 0)
				{
					monsCurrHp = 0;
					rewardGain(p);
					winUi(p);
					break;
				}

				PlayCurrHp -= monsAttack(p);

				if (PlayCurrHp <= 0)
				{
					PlayCurrHp = 0;
					break;
				}
			}
			else
			{
				PlayCurrHp -= monsAttack(p);

				if (PlayCurrHp <= 0)
				{
					PlayCurrHp = 0;
					break;
				}

				monsCurrHp -= PlayerAttack(p);

				if (monsCurrHp <= 0)
				{
					monsCurrHp = 0;
					rewardGain(p);
					winUi(p);
					break;
				}
			}
		}
		else if (i == 2)
		{
			SceneState--;
			break;
		}
	}
	free(p);
	SceneState--;
}
int monsAttack(const MONSTER* p)
{
	int damage = 0;
	if (p->att > player.status.def)
		damage = p->att - player.status.def;
	else
		damage = 1;

	return damage;
}

int PlayerAttack(const MONSTER* p)
{
	int damage = 0;
	if (player.status.att > p->def)
		damage = player.status.att - p->def;
	else damage = 1;

	return damage;
}

void winUi(MONSTER* p)
{
	int startX = 40, startY = 5;


	showFrame(40, 15, startX, startY);
	SetPosition(startX + 5, startY + 2, (char*)"----------전투 보상----------", 2);

	SetCursor(startX + 5, startY + 4);
	printf_s("* 획득 골드   : %d G", p->gold);

	SetCursor(startX + 5, startY + 6);
	printf_s("* 획득 경험치 : %d EXP", p->exp);

	char c = ' ';
	while (true)
	{
		c = _getch();
		if (c != 32)break;//어떤키라도 눌렀으면
	}
}
void rewardGain(MONSTER* p)
{
	player.exp += p->exp;
	player.gold += p->gold;
}

void SetPosition(int _x, int _y, char* _str, int _Color)
{
	COORD Pos = { _x, _y }; // 콘솔 순서쌍

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);//커서 위치 이동
	SetColor(_Color);

	printf_s("%s", _str);
}

void SetCursor(int _x, int _y)
{
	COORD Pos = { _x, _y }; // 콘솔 순서쌍

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);//커서 위치 이동
}
//폭, 높이, 시작지점
void showFrame(int w, int h, int startX, int startY)
{
	for (int i = 0; i < h; i++)
	{
		COORD Pos = { startX, startY + i }; // 콘솔 순서쌍
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);//커서 위치 이동
		for (int j = 0; j < w; j++)
		{
			printf(" ");
		}

	}



	for (int i = 0; i < h; i++)
	{
		COORD Pos = { startX, startY + i }; // 콘솔 순서쌍
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);//커서 위치 이동
		for (int j = 0; j < w; j++)
		{
			if (i == 0 && j == 0)printf_s("┌");
			else if (i == 0 && j == w - 1)printf_s("┐");
			else if (i == (h - 1) && j == 0)printf_s("└");
			else if (i == (h - 1) && j == w - 1)printf_s("┘");
			else if (i == 0 || i == h - 1) printf_s("─");
			else if (j == 0 || j == w - 1) printf_s("│");
			else printf_s(" ");
		}
	}
	COORD Pos = { startX + 1, startY + 1 }; // 콘솔 순서쌍
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);//커서 위치 이동
}

void SetColor(int _Color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _Color); //색깔 변환
}

char* SetName()
{
	// ** scanf 함수로 문자열을 입력받기 위해 문자열을 받을수있는 배열을 선언.
	char Buffer[128] = "";
	// ** 문자열을 입력 받음.
	scanf("%s", Buffer);

	// ** 입력 받은값은 배열 이지만, 반환 값은 캐릭터 포인터형이므로 문자열을 복사햐야함.
	// ** 문자열을 복사 하기위해 포인터가 가르키는 공간에 입력받은 문자열이 들아갈만큼의 크기로 메모리 할당.
	char* pName = (char*)malloc(strlen(Buffer) + 1);

	// ** Buffer 가 받은 문자열을 pName 으로 복사.
	strcpy(pName, Buffer);

	// ** 반환.
	return pName;
}