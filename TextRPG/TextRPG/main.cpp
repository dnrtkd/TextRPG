#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string>
#include <time.h>
#include <Windows.h>
#include <conio.h>

//�� ��ȯ�� ���� ����
int SceneState = 0;
//�� ��ȯ ���
const int Scene_Logo = 0;
const int Scene_Menu = 1;
const int Scene_NewGame = 2;
const int Scene_Menu2 = 3;
const int Scene_SelectField = 4;
const int Scene_InField = 5;
const int Scene_Battle = 6;
const int Scene_Exit = 7;
//���� ��
int MapState = 0;
//�� ���
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
}Map; //�� ���� ����ü

//����
typedef struct tagStatus
{
	int hp;
	int att;
	int def;
	int quick;
}STATUS;
//��
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
typedef struct tagPlayer //�÷��̾� ����ü
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
		printf_s("���� �Ұ�!");
}

void EquipProtection(PROTECTION p)
{
	if (p.limitLevel <= player.level)
	{
		player.protection = p;
		player.status.def += p.def;
	}
	else
		printf_s("���� �Ұ�!");
}

int main(void)
{
	srand(time(0));

	//��� �ʱ�ȭ
	WeaponDataInput(0, 1, "�׵�", 5);
	WeaponDataInput(1, 5, "ȯ��", 8);
	WeaponDataInput(2, 10, "û��", 12);
	WeaponDataInput(3, 1, "���ÿ�Ȱ", 7);
	WeaponDataInput(4, 5, "���", 10);
	WeaponDataInput(5, 10, "ö��", 15);
	WeaponDataInput(6, 1, "���� ��", 10);
	WeaponDataInput(7, 5, "����", 15);
	WeaponDataInput(8, 10, "ȭ����", 20);

	ProtectionDataInput(0, 1, "���� ����", 5);
	ProtectionDataInput(1, 5, "û�� ����", 10);
	ProtectionDataInput(2, 10, "ö ����", 15);

	MonsterDataInput(0, "������", 1, 30, 5, 5, 5, 30, 20);
	MonsterDataInput(1, "�����", 2, 35, 6, 5, 5, 40, 25);
	MonsterDataInput(2, "������", 3, 60, 7, 7, 5, 60, 50);
	MonsterDataInput(3, "ö�𺴻�", 5, 100, 10, 10, 5, 80, 35);
	MonsterDataInput(4, "���ѱú�", 6, 70, 12, 5, 9, 90, 45);
	MonsterDataInput(5, "���μ�", 7, 150, 12, 8, 6, 150, 90);
	MonsterDataInput(6, "���Ѽ�", 10, 110, 15, 5, 7, 120, 60);
	MonsterDataInput(7, "Į����", 11, 130, 13, 8, 6, 130, 80);
	MonsterDataInput(8, "�繫����", 12, 250, 16, 12, 10, 300, 200);

	MapDataInput(0, "�Ѿ� �α�", 1, 0, 1, 2);
	MapDataInput(1, "���� �꼺", 5, 3, 4, 5);
	MapDataInput(2, "�븶��", 10, 6, 7, 8);
	// ** Ŀ���� �Ⱥ��̰� ��.
	HideCursor();

	// ** �ܼ�â�� ����� ����.
	system("mode con:cols=120 lines=30");

	// ** �ݼ�â�� �̸��� ����
	system("title ȫ�浿 Framework v0.6");

	// ** ��ü ������ ������.
	//system("color 70");

	DWORD dwTime = GetTickCount(); // 1/1000 (1000���� 1��)
	int Delay = 1000;

	int UpCount = 0;

	while (true)
	{
		if (dwTime + Delay < GetTickCount())
		{
			dwTime = GetTickCount();

			// ** �ܼ�â�� �ִ� ��� ������ ����.
			system("cls");
			// ** ���� ����
			SceneManager();
		}
	}

	return 0;
}

void HideCursor()
{
	CONSOLE_CURSOR_INFO Info;

	Info.dwSize = 1;// Ŀ���� ũ��
	Info.bVisible = false; //Ŀ���� ǥ�ÿ���

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);//�μ�: �ܼ� �⺻ �ڵ�, ���� ��
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
		exit(NULL);// ** ���α׷� ����
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
	SetPosition(Width + 10, Height + 12, (char*)"������ �����Ϸ���, EnterŰ�� �����ּ���.", 2);
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

	//printf_s("���� �� ����??\n1. ���ӽ���\n2. ����\n�Է� : ");
	printf_s("1. ���ν���\n");
	printf_s("2. �̾��ϱ�\n");
	printf_s("3. ��������\n");
	int i = 0;
	scanf("%d", &i);

	if (i == 1)
		SceneState++;
	else if (i == 2) SceneState += 2;
	else if (i == 3)
		SceneState = Scene_Exit;
}
//���� �ϱ� ��
void CreatePlayerScene()
{
	printf_s("�̸� �Է�: ");
	player.name = SetName();
	player.level = 1;
	player.gold = 0;
	player.exp = 0;

	printf_s("\n\n���� ����\n1. �˼���\n2. �ü���\n3. �Ѽ���");
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
//�ű� ĳ���� ���� �й�
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

	printf_s("\n1. �ʵ�Ž��\n2. ������Ʈ\n3. �κ��丮\n4. �÷��̾� ����");

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
	printf_s("�÷��̾� �г���: %s", player.name);

	SetCursor(startX + 2, startY + 3);
	printf_s("����          : %d", player.level);

	SetCursor(startX + 2, startY + 4);
	printf_s("����ġ        : %d", player.exp);

	SetCursor(startX + 2, startY + 5);
	printf_s("����          : %d", player.job);

	SetCursor(startX + 2, startY + 6);
	printf_s("���          : %d", player.gold);

	SetCursor(startX + 2, startY + 7);
	printf_s("ü��          : %d", player.status.hp);

	SetCursor(startX + 2, startY + 8);
	printf_s("���ݷ�        : %d", player.status.att);

	SetCursor(startX + 2, startY + 9);
	printf_s("����        : %d", player.status.def);

	SetCursor(startX + 2, startY + 10);
	printf_s("���߷�        : %d", player.status.quick);

	SetCursor(startX + 2, startY + 11);
	printf_s("���� ����     : %s", player.weapon.name);

	SetCursor(startX + 2, startY + 12);
	printf_s("���� ��   : %s", player.protection.name);

	SetCursor(startX + 2, startY + 14);
	printf_s("1. ĳ���� ����â �ݱ�");

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

	printf_s("4. �ڷ� ����\n");
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
			printf_s("���� ����");
	}
	if (i == 4) SceneState--;


}
void InField()
{
	system("cls");

	printf("%s\n", maps[MapState].name);

	printf("1. ���\n2. �κ��丮\n3. �ڷΰ���");

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

	if (hidden < 10) p = CreateMonster(maps[MapState].apearMonster[2]); //���� ���� ����
	else if (hidden < 50) p = CreateMonster(maps[MapState].apearMonster[1]); //�߰�
	else p = CreateMonster(maps[MapState].apearMonster[0]); //���� ����

	int monsCurrHp = p->Hp;
	int PlayCurrHp = player.status.hp;
	while (true)
	{
		system("cls");
		printf_s("���� �̸�: %s\t����: %d\tHP: %d/%d\n\n\n", p->name, p->level, monsCurrHp, p->Hp);
		printf_s("�÷��̾�: %s\t����: %d\tHP: %d/%d\n\n", player.name, player.level, PlayCurrHp, player.status.hp);
		printf_s("1. ����\n2. ����");

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
	SetPosition(startX + 5, startY + 2, (char*)"----------���� ����----------", 2);

	SetCursor(startX + 5, startY + 4);
	printf_s("* ȹ�� ���   : %d G", p->gold);

	SetCursor(startX + 5, startY + 6);
	printf_s("* ȹ�� ����ġ : %d EXP", p->exp);

	char c = ' ';
	while (true)
	{
		c = _getch();
		if (c != 32)break;//�Ű�� ��������
	}
}
void rewardGain(MONSTER* p)
{
	player.exp += p->exp;
	player.gold += p->gold;
}

void SetPosition(int _x, int _y, char* _str, int _Color)
{
	COORD Pos = { _x, _y }; // �ܼ� ������

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);//Ŀ�� ��ġ �̵�
	SetColor(_Color);

	printf_s("%s", _str);
}

void SetCursor(int _x, int _y)
{
	COORD Pos = { _x, _y }; // �ܼ� ������

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);//Ŀ�� ��ġ �̵�
}
//��, ����, ��������
void showFrame(int w, int h, int startX, int startY)
{
	for (int i = 0; i < h; i++)
	{
		COORD Pos = { startX, startY + i }; // �ܼ� ������
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);//Ŀ�� ��ġ �̵�
		for (int j = 0; j < w; j++)
		{
			printf(" ");
		}

	}



	for (int i = 0; i < h; i++)
	{
		COORD Pos = { startX, startY + i }; // �ܼ� ������
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);//Ŀ�� ��ġ �̵�
		for (int j = 0; j < w; j++)
		{
			if (i == 0 && j == 0)printf_s("��");
			else if (i == 0 && j == w - 1)printf_s("��");
			else if (i == (h - 1) && j == 0)printf_s("��");
			else if (i == (h - 1) && j == w - 1)printf_s("��");
			else if (i == 0 || i == h - 1) printf_s("��");
			else if (j == 0 || j == w - 1) printf_s("��");
			else printf_s(" ");
		}
	}
	COORD Pos = { startX + 1, startY + 1 }; // �ܼ� ������
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);//Ŀ�� ��ġ �̵�
}

void SetColor(int _Color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _Color); //���� ��ȯ
}

char* SetName()
{
	// ** scanf �Լ��� ���ڿ��� �Է¹ޱ� ���� ���ڿ��� �������ִ� �迭�� ����.
	char Buffer[128] = "";
	// ** ���ڿ��� �Է� ����.
	scanf("%s", Buffer);

	// ** �Է� �������� �迭 ������, ��ȯ ���� ĳ���� ���������̹Ƿ� ���ڿ��� ���������.
	// ** ���ڿ��� ���� �ϱ����� �����Ͱ� ����Ű�� ������ �Է¹��� ���ڿ��� ��ư���ŭ�� ũ��� �޸� �Ҵ�.
	char* pName = (char*)malloc(strlen(Buffer) + 1);

	// ** Buffer �� ���� ���ڿ��� pName ���� ����.
	strcpy(pName, Buffer);

	// ** ��ȯ.
	return pName;
}