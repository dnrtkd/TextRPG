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
const int Scene_NewGame = 1;
const int Scene_Menu2 = 2;
const int Scene_SelectField = 3;
const int Scene_InField = 4;
const int Scene_Battle = 5;
const int Scene_Exit = 6;
//현재 맵
int MapState = 0;
//맵 상수
const int Map_Samgir = 0;
const int Map_SmallCave = 1;
const int Map_seaBottom = 2;

const int Job_sword = 0;
const int Job_bow = 1;
const int Job_gun = 2;
//아이템 타입
const int Item_WP = 0;//무기
const int Item_Pro = 1;//방어구
const int Item_Use = 2;//소모
const int Item_ETC = 3;//기타

const int InvenSize = 20;

const int levelTable[15] = { 50,100,200,400,700,1500,2500,3500,4500,6000,8000,10000,12000,15000,20000 };


typedef struct tagItme
{
	char* name;
	int itemNum;
	int itemType;
	int price;
}ITEM;

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
	int def;
	ITEM item;
}PROTECTION;

typedef struct tagWeapon
{
	int limitLevel;
	int att;
	ITEM item;
}WEAPON;
typedef struct tagPlayer //플레이어 구조체
{
	int level;
	char* name;
	int exp;
	int gold;
	int job;
	int currHp;
	STATUS status;
	WEAPON weapon;
	PROTECTION protection;
}Player;

typedef struct tagSlot
{
	int index;
	ITEM item;
	int count;
}SLOT;

typedef struct tagMonster
{
	char* name;
	int level;
	int Hp;
	int currHP;
	int att;
	int def;
	int quick;
	int exp;
	int gold;
}MONSTER;
typedef struct tagUseItme
{
	int hpRegain;
	int MpRegain;
	ITEM item;
}USETEM;
typedef struct etcItme
{
	char* explain;//설명
	ITEM item;
}ETCTEM;

Map maps[3];
WEAPON weapons[3];
USETEM useitems[6];
ETCTEM EtcTems[9];
PROTECTION protections[3];
MONSTER monsters[9];

SLOT inventory[InvenSize];
int invenIndex = 0;

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
void winUi(int a, int b, int c);
void rewardGain(int a, int b);
int PlayerAttack(MONSTER* p);
int monsAttack(const MONSTER* p);
void showInven();
void MenuSceneUI(int startX, int startY);
void Save();
void Load();
void CreatePlayer();
void WeaponDataInput(int index, int limitLevel, const char* name, int att, const int type, int price)
{
	weapons[index].limitLevel = limitLevel;
	weapons[index].item.name = (char*)name;
	weapons[index].att = att;
	weapons[index].item.itemNum = index;
	weapons[index].item.itemType = type;
	weapons[index].item.price = price;
}
void ProtectionDataInput(int index, int limitLevel, const char* name, int def, const int type, int price)
{
	protections[index].limitLevel = limitLevel;
	protections[index].item.name = (char*)name;
	protections[index].def = def;
	protections[index].item.itemNum = index;
	protections[index].item.itemType = type;
	protections[index].item.price = price;

}
void UseTemDataInput(int index, const char* name, int hpRe, int mpRe, const int type, int price)
{
	useitems[index].hpRegain = hpRe;
	useitems[index].MpRegain = mpRe;
	useitems[index].item.itemNum = index;
	useitems[index].item.itemType = type;
	useitems[index].item.name = (char*)name;
	useitems[index].item.price = price;
}
void EtcTemDataInput(int index, const char* name, const char* expl, const int type, int price)
{
	EtcTems[index].explain = (char*)expl;
	EtcTems[index].item.itemNum = index;
	EtcTems[index].item.itemType = type;
	EtcTems[index].item.name = (char*)name;
	EtcTems[index].item.price = price;
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
void InputInven(ITEM item) //전역변수 inventory에 아이템을 넣음
{
	for (int i = 0; i < invenIndex; i++)
	{
		//아이템이 이미 인벤토리에 들어있는지 확인하는 부분
		if (inventory[i].item.itemNum == item.itemNum && inventory[i].item.itemType == item.itemType)
		{
			inventory[i].count++;
			inventory[i].index = i;
			return; //count증가후 함수 종료
		}
	}
	if (invenIndex < InvenSize - 1)
	{
		inventory[invenIndex].item = item;
		inventory[invenIndex].count++;
		inventory[invenIndex].index = invenIndex;
		invenIndex++;
	}
	else
	{
		printf_s("인벤토리가 꽉 찼습니다.");
	}
}
void shop();
int buy(ITEM item);
void showBattleInvenUI();
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
	p->currHP = p->Hp;

	return p;
}

Player player;

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
	WeaponDataInput(0, 1, "죽도", 5, Item_WP, 100);
	WeaponDataInput(1, 5, "환도", 8, Item_WP, 500);
	WeaponDataInput(2, 10, "청도", 12, Item_WP, 1000);
	WeaponDataInput(3, 1, "수련용활", 7, Item_WP, 100);
	WeaponDataInput(4, 5, "목궁", 10, Item_WP, 500);
	WeaponDataInput(5, 10, "철궁", 15, Item_WP, 1000);
	WeaponDataInput(6, 1, "수렵 총", 10, Item_WP, 100);
	WeaponDataInput(7, 5, "조총", 15, Item_WP, 500);
	WeaponDataInput(8, 10, "화승총", 20, Item_WP, 1000);

	ProtectionDataInput(0, 1, "각목 갑옷", 5, Item_Pro, 100);
	ProtectionDataInput(1, 5, "청동 갑옷", 10, Item_Pro, 500);
	ProtectionDataInput(2, 10, "철 갑옷", 15, Item_Pro, 1000);

	UseTemDataInput(0, "경단", 30, 0, Item_Use, 20);
	UseTemDataInput(1, "내복환", 100, 0, Item_Use, 100);
	UseTemDataInput(2, "쌍화차", 150, 0, Item_Use, 200);
	UseTemDataInput(3, "파랑가루", 0, 20, Item_Use, 30);
	UseTemDataInput(4, "비약", 0, 40, Item_Use, 200);
	UseTemDataInput(5, "약화제", 0, 80, Item_Use, 300);

	EtcTemDataInput(0, "달걀", "꼬꼬닭이 품은 달걀", Item_ETC, 0);
	EtcTemDataInput(1, "콩", "상경쥐의 주식", Item_ETC, 0);
	EtcTemDataInput(2, "폭신폭신 솜털", "구리왕의 모피", Item_ETC, 0);
	EtcTemDataInput(3, "부러진 철퇴", "꼬꼬닭이 품은 달걀", Item_ETC, 0);
	EtcTemDataInput(4, "부러진 활", "꼬꼬닭이 품은 달걀", Item_ETC, 0);
	EtcTemDataInput(5, "부러진 도끼", "꼬꼬닭이 품은 달걀", Item_ETC, 0);
	EtcTemDataInput(6, "부러진 조총", "꼬꼬닭이 품은 달걀", Item_ETC, 0);
	EtcTemDataInput(7, "부러진 카타나", "꼬꼬닭이 품은 달걀", Item_ETC, 0);
	EtcTemDataInput(8, "부러진 왜갑", "꼬꼬닭이 품은 달걀", Item_ETC, 0);

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

	InputInven(useitems[0].item);
	InputInven(useitems[3].item);

	// ** 콘솔창의 사이즈를 설정.
	system("mode con:cols=120 lines=30");

	// ** 콜솔창의 이름을 설정
	system("title 홍길동 Framework v0.6");

	// ** 전체 배경색을 변경함.
	//system("color 70");

	DWORD dwTime = GetTickCount64(); // 1/1000 (1000분의 1초)
	int Delay = 1000;

	int UpCount = 0;

	while (true)
	{
		if (dwTime + Delay < GetTickCount64())
		{
			dwTime = GetTickCount64();

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
	int Width = (120 / 2) - (strlen("     ,--. ,-----.  ,---.  ,--. ,--.,--.  ,--.      ,------. ,------.  ,----.    ") / 2);
	int Height = 10;

	SetPosition(Width, Height + 1, (char*)"     ,--. ,-----.  ,---.  ,--. ,--.,--.  ,--.      ,------. ,------.  ,----.    ", 1);
	SetPosition(Width, Height + 2, (char*)"     |  |'  .-.  ''   .-' |  | |  ||  ,'.|  |      |  .--. '|  .--. ''  .-./    ", 2);
	SetPosition(Width, Height + 3, (char*)",--. |  ||  | |  |`.  `-. |  | |  ||  |' '  |      |  '--'.'|  '--' ||  | .---. ", 3);
	SetPosition(Width, Height + 4, (char*)"|  '-'  /'  '-'  '.-'    |'  '-'  '|  | `   |,----.|  |\  \ |  | --' '  '--'  | ", 4);
	SetPosition(Width, Height + 5, (char*)" `-----'  `-----' `-----'  `-----' `--'  `--''----'`--' '--'`--'      `------'  ", 5);

	SetColor(2);

	Sleep(3000);

	CreatePlayer();
	
	SceneState++;
}

void CreatePlayer()
{
	int startX = 30, startY = 5;
	int catal = 0, catalMax = 3;
	showFrame(50, 20, startX, startY);

	SetPosition(startX+3, startY+2, (char*)"닉네임 입력: ",15);

	SetPosition(startX + 3, startY + 5, (char*)"-- 직업선택 --", 15);

	SetPosition(startX + 3, startY + 7, (char*)"■  검술사", 15);

	SetPosition(startX + 3, startY + 9, (char*)"■  궁술사", 15);

	SetPosition(startX+ 3, startY + 11, (char*)"■  총술사", 15);

	SetCursor(startX + 3 + strlen("닉네임 입력: "), startY + 2);
	player.name = SetName();
	// ** 커서를 안보이게 함.
	HideCursor();

	while (true)
	{
		char c = _getch();

		if (c == 72) //위 방향키
		{
			SetPosition(startX + 20, startY + 5 + catal * 2, (char*)"  ", 2);
			if (catal <= 1)
				catal = catalMax;
			else
				catal--;
			SetPosition(startX + 20, startY + 5 + catal * 2, (char*)"◀", 2);
		}
		else if (c == 80) //아래 방향키
		{
			SetPosition(startX + 20, startY + 5 + catal * 2, (char*)"  ", 2);
			if (catal == catalMax)
				catal = 1;
			else
				catal++;
			SetPosition(startX + 20, startY + 5 + catal * 2, (char*)"◀", 2);
		}
		else if (c == 97) //선택
		{
			if (catal == 1) player.job = 0;
			if (catal == 2) player.job = 1;
			if (catal == 3) player.job = 2;

			player.level = 1;
			player.gold = 200;
			player.exp = 0;
			allotPoints();
			EquipWeapon(weapons[0]);
			EquipProtection(protections[0]);

			player.currHp = player.status.hp;

			SceneState++;
			break;
		}
	}
}

//새로 하기 씬
void CreatePlayerScene()
{
	printf_s("이름 입력: ");
	player.name = SetName();
	player.level = 1;
	player.gold = 200;
	player.exp = 0;

	printf_s("\n\n직업 선택\n1. 검술사\n2. 궁술사\n3. 총술사");
	int i = 0;
	scanf("%d", &i);

	EquipWeapon(weapons[0]);
	EquipProtection(protections[0]);

	allotPoints();
	player.currHp = player.status.hp;
	SceneState++;
}
//신규 캐릭터 스텟 분배
void allotPoints()
{
	if (player.job == Job_sword)
	{
		player.status.att = 50;
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

void MenuSceneUI(int startX,int startY)
{
	system("cls");

	showFrame(40, 20, startX, startY);
	SetPosition(startX + 8, startY + 4, (char*)"--   지역 선택    --", 2);
	SetPosition(startX + 8, startY + 6, (char*)"--   상점 가기    --", 2);
	SetPosition(startX + 8, startY + 8, (char*)"--  인벤토리 보기 --", 2);
	SetPosition(startX + 8, startY + 10, (char*)"-- 캐릭터 정보보기--", 2);
	SetPosition(startX + 8, startY + 12, (char*)"--    뒤로 가기   --", 2);
}
void MenuScene2()
{
	int catal = 1, catalMax = 5;
	
	int startX = 40, startY = 5;
	MenuSceneUI(startX, startY);

	SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"◀", 2);

	while (true)
	{
		char c = _getch();

		if (c == 72) //위 방향키
		{
			SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"  ", 2);
			if (catal == 1)
				catal = catalMax;
			else
				catal--;
			SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"◀", 2);
		}
		else if (c == 80) //아래 방향키
		{
			SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"  ", 2);
			if (catal == catalMax)
				catal = 1;
			else
				catal++;
			SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"◀", 2);
		}
		else if (c == 97) //선택
		{
			if (catal == 1)
			{
				SceneState++;
				break;
			}
			else if (catal == 2)
			{
				c = 0;
				shop();
				MenuSceneUI(startX, startY);
				SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"◀", 2);
			}
			else if (catal == 3)
			{
				c = 0;
				showInven();
				MenuSceneUI(startX, startY);
				SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"◀", 2);
			}
			else if (catal == 4)
			{
				c = 0;
				showPlayer();
				MenuSceneUI(startX, startY);
				SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"◀", 2);
			}
			else if (catal == 5)
			{
				SceneState--;
				break;
			}
		}
	}
}

void showPlayer()
{
	short startX = 30;
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
	printf_s("장착 무기     : %s", player.weapon.item.name);

	SetCursor(startX + 2, startY + 12);
	printf_s("장착 방어구   : %s", player.protection.item.name);

	SetCursor(startX + 2, startY + 14);
	printf_s("1. 캐릭터 정보창 닫기");

	int i = 0;
	scanf("%d", &i);


} //상태창 보기
void showInven() //인벤토리 보기
{
	short startX = 30;
	short startY = 2;

	showFrame(60, 22, startX, startY);
	for (int i = 0; i < invenIndex; i++)
	{
		SetCursor(startX + 3, startY + 2 + i * 2);
		printf_s("*이름: %s ", inventory[i].item.name);

		if (inventory[i].item.itemType == Item_Pro)
			printf_s(" 타입: 방어구 ");
		else if (inventory[i].item.itemType == Item_WP)
			printf_s(" 타입: 무기 ");
		else if (inventory[i].item.itemType == Item_Use)
			printf_s(" 타입: 소모품 ");

		printf_s(" 개수: %d", inventory[i].count);
	}

	SetCursor(startX + 2, startY + 18);
	printf_s("1. 인벤토리 닫기");

	int i = 0;
	scanf("%d", &i);
}
void showBattleInvenUI()//배틀씬의 인벤토리 ui
{
	int startX = 65, startY = 18;
	showFrame(50, 10, 65, 18);

	SetPosition(startX + 15, startY + 2, (char*)"---  인벤토리  ---", 2);

	for (int i = 0; i < invenIndex; i++)
	{
		if (inventory[i].item.itemType == Item_Use)
		{
			SetCursor(startX + 2, startY + 4 + (i * 2));
			printf_s("* %s  %d 회복 ", useitems[inventory[i].item.itemNum].item.name, useitems[inventory[i].item.itemNum].hpRegain);
		}
	}
}
void BattleInven()
{
	int useTemIndexes[InvenSize];
	for (int i = 0; i < invenIndex; i++)
	{
		if (inventory[i].item.itemType == Item_Use) //사용아이템의 인덱스만 추출
		{
			useTemIndexes[i] = inventory[i].index;
		}
	}

	showBattleInvenUI();

	while (true)
	{
		int i = 0;
		scanf("%d", &i);

		if (i == 1);
		break;
	}
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
	const int attack = 0;
	const int skill = 1;
	const int item = 2;
	const int exit = 3;

	MONSTER* p[5] = { NULL,NULL,NULL,NULL,NULL };

	int x = 10, y = 2; //콘솔 좌표

	int exp = 0, gold = 0; //전투 승리후 보상

	int apearNum = rand() % 5 + 1;//몬스터 출현 수

	for (int i = 0; i < apearNum; i++)//몬스터 출현
	{
		int hidden = rand() % 100;

		if (hidden < 10) p[i] = CreateMonster(maps[MapState].apearMonster[2]); //가장 강한 몬스터
		else if (hidden < 50) p[i] = CreateMonster(maps[MapState].apearMonster[1]); //중간
		else p[i] = CreateMonster(maps[MapState].apearMonster[0]); //가장 약함
	}

	int PlayCurrHp = player.status.hp;
	showFrame(100, 22, x, y);
	while (true)
	{
		SetCursor(50, 2);
		printf_s("----- 전투 화면 -----");

		int j = 0;
		if (apearNum > 3) j = 0;
		else if (apearNum > 1) j = 15;
		else j = 30;

		for (int i = 0; i < apearNum; i++)
		{

			SetCursor(x + 15 + j + (i * 15), y + 1);
			printf_s("레벨:%d", p[i]->level);

			SetCursor(x + 15 + j + (i * 15), y + 2);
			printf_s("이름:%s", p[i]->name);

			SetCursor(x + 15 + j + (i * 15), y + 3);
			printf_s("                      ");
			SetCursor(x + 15 + j + (i * 15), y + 3);
			printf_s("HP:%d/%d", p[i]->currHP, p[i]->Hp);
		}

		SetCursor(50, y + 15);
		printf_s("레벨:%d", player.level);

		SetCursor(50, y + 16);
		printf_s("이름%s", player.name);

		SetCursor(50, y + 17);
		printf_s("                       ");
		SetCursor(50, y + 17);
		printf_s("HP:%d/%d", player.currHp, player.status.hp);

		SetCursor(50, 26);
		printf_s("1.공격   2.스킬사용  3.아이템사용 4.도망가기");

		int i = 0;
		scanf_s("%d", &i);

		if (i == attack) //기본 공격
		{
			SetCursor(50, 26);
			printf_s("                                            ");

			SetCursor(50, 26);
			printf_s("공격할 몬스터 지정");

			int i = 0;
			scanf_s("%d", &i);

			if ((i < 6 && i>0) && (p[i - 1] != NULL) && (p[i - 1]->currHP > 0))
			{
				int damage = PlayerAttack(p[i - 1]);

				SetCursor(50, y + 13);
				printf_s("공격");
				Sleep(1000);
				SetCursor(50, y + 13);
				printf_s("     ");

				for (int k = 0; k < apearNum; k++)
				{
					if (k == i - 1)//피격당한 몬스터
					{
						SetCursor(x + 15 + j + (k * 15), y + 5);
						printf_s("데미지: %d", damage);
						Sleep(1000);
						SetCursor(x + 15 + j + (k * 15), y + 5);
						printf_s("          ");
					}

					SetCursor(x + 15 + j + (k * 15), y + 3);
					printf_s("         ");
					SetCursor(x + 15 + j + (k * 15), y + 3);
					printf_s("HP:%d/%d", p[k]->currHP, p[k]->Hp);
				}

				for (int k = 0; k < apearNum; k++)
				{
					if (p[k]->currHP > 0)
					{
						damage = monsAttack(p[k]);

						SetCursor(x + 15 + j + (k * 15), y + 5);
						printf_s("공격");
						Sleep(1000);
						SetCursor(x + 15 + j + (k * 15), y + 5);
						printf_s("      ");

						SetCursor(50, y + 13);
						printf_s("데미지: %d", damage);
						Sleep(1000);
						SetCursor(50, y + 13);
						printf_s("          ");

						SetCursor(50, y + 17);
						printf_s("HP:%d/%d", player.currHp, player.status.hp);
					}
				}
			}
		}
		else if (i == skill)//스킬
		{

		}
		else if (i == item)//아이템 사용
		{
			BattleInven();
		}
		else if (i == exit)//도망
			break;

		int check = 0;
		for (int i = 0; i < apearNum; i++)
		{
			check += p[i]->currHP;
		}
		if (check == 0)
		{
			for (int i = 0; i < apearNum; i++)
			{
				exp += p[i]->exp;
				gold += p[i]->gold;
			}
			rewardGain(exp, gold);
			Save();
			break;
		}
	}
	for (int i = 0; i < apearNum; i++)
	{
		free(p[i]);
	}
	SceneState--;
}
int monsAttack(const MONSTER* p)
{
	int damage = 0;
	if (p->att > player.status.def)
		damage = p->att - player.status.def;
	else
		damage = 1;
	if (player.currHp > damage)
		player.currHp -= damage;
	else
		player.currHp = 0;

	return damage;
}

int PlayerAttack(MONSTER* p)
{
	int damage = 0;
	if (player.status.att > p->def)
		damage = player.status.att - p->def;
	else damage = 1;

	if (p->currHP > damage)
		p->currHP -= damage;
	else
		p->currHP = 0;

	return damage;
}
int LevelUp()//레벨업을 했으면 1 반환 아니면 0 반환
{
	int isLevelUp = 0;
	while (levelTable[player.level - 1] < player.exp)
	{
		player.level++;
		isLevelUp = 1;

		if (player.job == 0) //스텟 분배
		{
			player.status.hp += 10;
			player.status.att += 2;
			player.status.def += 1;
		}
		if (player.job == 1) //스텟 분배
		{
			player.status.hp += 5;
			player.status.att += 3;
			player.status.def += 1;
		}
		if (player.job == 2) //스텟 분배
		{
			player.status.hp += 3;
			player.status.att += 4;
			player.status.def += 1;
		}

	}
	return isLevelUp;
}

void winUi(int gold, int exp, int isLevelUp)//이겼을때 표시되는 ui 
{
	int startX = 40, startY = 5;


	showFrame(40, 15, startX, startY);
	SetPosition(startX + 5, startY + 2, (char*)"----------전투 보상----------", 2);

	SetCursor(startX + 5, startY + 4);
	printf_s("* 획득 골드   : %d G", gold);

	SetCursor(startX + 5, startY + 6);
	printf_s("* 획득 경험치 : %d EXP", exp);

	if (isLevelUp)
	{
		SetCursor(startX + 5, startY + 8);
		printf_s("* 캐릭터 레벨 업!!!!");
	}
	SetCursor(startX + 7, startY + 10);
	printf_s("1. 전투 계속");

	int i = 0;
	scanf_s("%d", &i);
	if (i == 1) BattleScene();
}
void rewardGain(int exp, int gold) //플레이어 보상획득
{
	player.exp += exp;
	player.gold += gold;

	int isLevelUp = LevelUp();
	winUi(gold, exp, isLevelUp);
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
	for (int i = 0; i < h - 2; i++)
	{
		SetCursor(startX + 1, startY + i + 1);
		for (int j = 0; j < w - 2; j++)
		{
			printf(" ");
		}

	}

	
	for (int i = 0; i < h; i++)
	{
		SetCursor(startX, startY + i);
		for (int j = 0; j < w; j++)
		{
			if (i == 0 && j == 0)printf_s("┏");
			else if (i == 0 && j == w - 1)printf_s("┓");
			else if (i == (h - 1) && j == 0)printf_s("┗");
			else if (i == (h - 1) && j == w - 1)printf_s("┛");
			else if (i == 0 || i == h - 1) printf_s("━");
			else if (j == 0 || j == w - 1) printf_s("┃");
			else printf_s(" ");
		}
	}
	SetCursor(startX + 1, startY + 1);
	
	
	
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

void shop()
{
	short startX = 40;
	short startY = 2;

	const int WP = 0;//무기
	const int PT = 1;//방어구
	const int USE = 2; //소모품

	int page = 0;
	int catal = 0;

	SetCursor(startX + 10, startY + 2);
	printf_s("------ 상점 ------");

	while (true)
	{
		showFrame(50, 22, startX, startY);

		page = page % 3;

		if (page == WP)
		{
			catal = catal % 3;
			SetCursor(startX + 25, startY + 2);
			printf_s("        * 무기류 *");

			for (int i = 0; i < sizeof(weapons) / sizeof(WEAPON); i++)
			{
				if (weapons[i].att != NULL)
				{
					SetCursor(startX + 2, startY + 4 + (i * 4));
					printf_s("*  이름:%s     금액:%d", weapons[i].item.name, weapons[i].item.price);

					SetCursor(startX + 2, startY + 5 + (i * 4));
					printf_s("   공격력:%d   레벨제한:%d", weapons[i].att, weapons[i].limitLevel);
				}
			}

			SetCursor(startX + 40, startY + 4 + (catal * 4));
			printf_s("<- ");
		}
		else if (page == PT)
		{
			catal = catal % 3;
			SetCursor(startX + 25, startY + 2);
			printf_s("        * 방어구류 *");
			for (int i = 0; i < sizeof(protections) / sizeof(PROTECTION); i++)
			{
				if (protections[i].def != NULL)
				{
					SetCursor(startX + 2, startY + 4 + (i * 4));
					printf_s("*  이름:%s     금액:%d", protections[i].item.name, protections[i].item.price);

					SetCursor(startX + 2, startY + 5 + (i * 4));
					printf_s("   방어력:%d   레벨제한:%d", protections[i].def, protections[i].limitLevel);
				}
			}

			SetCursor(startX + 40, startY + 4 + (catal * 4));
			printf_s("<- ");
		}
		else if (page == USE)
		{
			catal = catal % 6;
			SetCursor(startX + 25, startY + 2);
			printf_s("        * 소모품 *");
			for (int i = 0; i < sizeof(useitems) / sizeof(USETEM); i++)
			{
				if (useitems[i].hpRegain != NULL)
				{
					SetCursor(startX + 2, startY + 4 + (i * 2));
					printf_s("*  이름:%s   체력회복양:%d   금액:%d", useitems[i].item.name, useitems[i].hpRegain, useitems[i].item.price);
				}
			}

			SetCursor(startX + 40, startY + 4 + (catal * 4));
			printf_s("<- ");
		}

		SetCursor(startX + 2, startY + 20);
		printf_s("1. 구입  2. 상점 나가기 ");

		char c = _getch();
		if (c == 13)break;

		if (c == 77)
		{
			page++;
		}
		else if (c == 75)
		{
			if (page != 0)page--;
		}
		else if (c == 72)
		{
			if (catal != 0)catal--;
		}
		else if (c == 80)
		{
			catal++;
		}
		else if (c == 'a') //구입
		{
			if (page == WP)
			{
				buy(weapons[catal].item);
			}
			else if (page == PT)
			{
				buy(protections[catal].item);
			}
			else if (page == USE)
			{
				buy(useitems[catal].item);
			}
		}

	}


}

int buy(ITEM item)//구매 성공하면 1 실패하면 0 반환
{
	if (player.gold >= item.price)
	{
		InputInven(item);
		player.gold -= item.price;
		printf_s("      구매성공");
		Sleep(500);
		return 1;
	}
	else return 0;

}

void useTemUse(ITEM item, int index)
{
	if (item.itemType == Item_Use) //소모 품이면
	{
		int hpRe = useitems[item.itemNum].hpRegain;
		int MpRe = useitems[item.itemNum].MpRegain;

		if (player.currHp < player.currHp + hpRe)
			player.currHp = player.status.hp;
		else
			player.currHp += hpRe;
	}
}

void useItem(int invenIndex) //아이템 사용 함수
{
	if (inventory[invenIndex].item.itemType == Item_Use)
	{
		player.currHp = useitems[inventory[invenIndex].item.itemNum].hpRegain;
	}
}

void Save()
{
	FILE* fPtr = fopen("SaveData.txt", "w");
	if (fPtr == nullptr) printf_s("error : 파일을 열 수 없음.");

	fprintf(fPtr, "%s %d %d %d %d %d %d %d %d", player.name, player.level, player.job, player.gold, player.exp,
		player.currHp, player.status.att, player.status.def, player.status.hp);


	if (fclose(fPtr) != 0) printf_s(" error: 파일을 닫을 수 없음");
}
void Load()
{
	FILE* fPtr = fopen("SaveData.txt", "r");
	if (fPtr == nullptr) printf_s("error : 파일을 열 수 없음.");
	else
	{
		while (EOF != fscanf(fPtr, "%s %d %d %d %d %d %d %d %d", player.name, &player.level, &player.job, &player.gold, &player.exp,
			&player.currHp, &player.status.att, &player.status.def, &player.status.hp))
		{

		}
	}
	
	if (fclose(fPtr) != 0) printf_s(" error: 파일을 닫을 수 없음");
}