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
const int Scene_NewGame = 1;
const int Scene_Menu2 = 2;
const int Scene_SelectField = 3;
const int Scene_InField = 4;
const int Scene_Battle = 5;
const int Scene_Exit = 6;
//���� ��
int MapState = 0;
//�� ���
const int Map_Samgir = 0;
const int Map_SmallCave = 1;
const int Map_seaBottom = 2;

const int Job_sword = 0;
const int Job_bow = 1;
const int Job_gun = 2;
//������ Ÿ��
const int Item_WP = 0;//����
const int Item_Pro = 1;//��
const int Item_Use = 2;//�Ҹ�
const int Item_ETC = 3;//��Ÿ

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
	int def;
	ITEM item;
}PROTECTION;

typedef struct tagWeapon
{
	int limitLevel;
	int att;
	ITEM item;
}WEAPON;
typedef struct tagPlayer //�÷��̾� ����ü
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
	char* explain;//����
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
void InputInven(ITEM item) //�������� inventory�� �������� ����
{
	for (int i = 0; i < invenIndex; i++)
	{
		//�������� �̹� �κ��丮�� ����ִ��� Ȯ���ϴ� �κ�
		if (inventory[i].item.itemNum == item.itemNum && inventory[i].item.itemType == item.itemType)
		{
			inventory[i].count++;
			inventory[i].index = i;
			return; //count������ �Լ� ����
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
		printf_s("�κ��丮�� �� á���ϴ�.");
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
	WeaponDataInput(0, 1, "�׵�", 5, Item_WP, 100);
	WeaponDataInput(1, 5, "ȯ��", 8, Item_WP, 500);
	WeaponDataInput(2, 10, "û��", 12, Item_WP, 1000);
	WeaponDataInput(3, 1, "���ÿ�Ȱ", 7, Item_WP, 100);
	WeaponDataInput(4, 5, "���", 10, Item_WP, 500);
	WeaponDataInput(5, 10, "ö��", 15, Item_WP, 1000);
	WeaponDataInput(6, 1, "���� ��", 10, Item_WP, 100);
	WeaponDataInput(7, 5, "����", 15, Item_WP, 500);
	WeaponDataInput(8, 10, "ȭ����", 20, Item_WP, 1000);

	ProtectionDataInput(0, 1, "���� ����", 5, Item_Pro, 100);
	ProtectionDataInput(1, 5, "û�� ����", 10, Item_Pro, 500);
	ProtectionDataInput(2, 10, "ö ����", 15, Item_Pro, 1000);

	UseTemDataInput(0, "���", 30, 0, Item_Use, 20);
	UseTemDataInput(1, "����ȯ", 100, 0, Item_Use, 100);
	UseTemDataInput(2, "��ȭ��", 150, 0, Item_Use, 200);
	UseTemDataInput(3, "�Ķ�����", 0, 20, Item_Use, 30);
	UseTemDataInput(4, "���", 0, 40, Item_Use, 200);
	UseTemDataInput(5, "��ȭ��", 0, 80, Item_Use, 300);

	EtcTemDataInput(0, "�ް�", "�������� ǰ�� �ް�", Item_ETC, 0);
	EtcTemDataInput(1, "��", "������� �ֽ�", Item_ETC, 0);
	EtcTemDataInput(2, "�������� ����", "�������� ����", Item_ETC, 0);
	EtcTemDataInput(3, "�η��� ö��", "�������� ǰ�� �ް�", Item_ETC, 0);
	EtcTemDataInput(4, "�η��� Ȱ", "�������� ǰ�� �ް�", Item_ETC, 0);
	EtcTemDataInput(5, "�η��� ����", "�������� ǰ�� �ް�", Item_ETC, 0);
	EtcTemDataInput(6, "�η��� ����", "�������� ǰ�� �ް�", Item_ETC, 0);
	EtcTemDataInput(7, "�η��� īŸ��", "�������� ǰ�� �ް�", Item_ETC, 0);
	EtcTemDataInput(8, "�η��� �ְ�", "�������� ǰ�� �ް�", Item_ETC, 0);

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

	InputInven(useitems[0].item);
	InputInven(useitems[3].item);

	// ** �ܼ�â�� ����� ����.
	system("mode con:cols=120 lines=30");

	// ** �ݼ�â�� �̸��� ����
	system("title ȫ�浿 Framework v0.6");

	// ** ��ü ������ ������.
	//system("color 70");

	DWORD dwTime = GetTickCount64(); // 1/1000 (1000���� 1��)
	int Delay = 1000;

	int UpCount = 0;

	while (true)
	{
		if (dwTime + Delay < GetTickCount64())
		{
			dwTime = GetTickCount64();

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

	SetPosition(startX+3, startY+2, (char*)"�г��� �Է�: ",15);

	SetPosition(startX + 3, startY + 5, (char*)"-- �������� --", 15);

	SetPosition(startX + 3, startY + 7, (char*)"��  �˼���", 15);

	SetPosition(startX + 3, startY + 9, (char*)"��  �ü���", 15);

	SetPosition(startX+ 3, startY + 11, (char*)"��  �Ѽ���", 15);

	SetCursor(startX + 3 + strlen("�г��� �Է�: "), startY + 2);
	player.name = SetName();
	// ** Ŀ���� �Ⱥ��̰� ��.
	HideCursor();

	while (true)
	{
		char c = _getch();

		if (c == 72) //�� ����Ű
		{
			SetPosition(startX + 20, startY + 5 + catal * 2, (char*)"  ", 2);
			if (catal <= 1)
				catal = catalMax;
			else
				catal--;
			SetPosition(startX + 20, startY + 5 + catal * 2, (char*)"��", 2);
		}
		else if (c == 80) //�Ʒ� ����Ű
		{
			SetPosition(startX + 20, startY + 5 + catal * 2, (char*)"  ", 2);
			if (catal == catalMax)
				catal = 1;
			else
				catal++;
			SetPosition(startX + 20, startY + 5 + catal * 2, (char*)"��", 2);
		}
		else if (c == 97) //����
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

//���� �ϱ� ��
void CreatePlayerScene()
{
	printf_s("�̸� �Է�: ");
	player.name = SetName();
	player.level = 1;
	player.gold = 200;
	player.exp = 0;

	printf_s("\n\n���� ����\n1. �˼���\n2. �ü���\n3. �Ѽ���");
	int i = 0;
	scanf("%d", &i);

	EquipWeapon(weapons[0]);
	EquipProtection(protections[0]);

	allotPoints();
	player.currHp = player.status.hp;
	SceneState++;
}
//�ű� ĳ���� ���� �й�
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
	SetPosition(startX + 8, startY + 4, (char*)"--   ���� ����    --", 2);
	SetPosition(startX + 8, startY + 6, (char*)"--   ���� ����    --", 2);
	SetPosition(startX + 8, startY + 8, (char*)"--  �κ��丮 ���� --", 2);
	SetPosition(startX + 8, startY + 10, (char*)"-- ĳ���� ��������--", 2);
	SetPosition(startX + 8, startY + 12, (char*)"--    �ڷ� ����   --", 2);
}
void MenuScene2()
{
	int catal = 1, catalMax = 5;
	
	int startX = 40, startY = 5;
	MenuSceneUI(startX, startY);

	SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"��", 2);

	while (true)
	{
		char c = _getch();

		if (c == 72) //�� ����Ű
		{
			SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"  ", 2);
			if (catal == 1)
				catal = catalMax;
			else
				catal--;
			SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"��", 2);
		}
		else if (c == 80) //�Ʒ� ����Ű
		{
			SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"  ", 2);
			if (catal == catalMax)
				catal = 1;
			else
				catal++;
			SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"��", 2);
		}
		else if (c == 97) //����
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
				SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"��", 2);
			}
			else if (catal == 3)
			{
				c = 0;
				showInven();
				MenuSceneUI(startX, startY);
				SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"��", 2);
			}
			else if (catal == 4)
			{
				c = 0;
				showPlayer();
				MenuSceneUI(startX, startY);
				SetPosition(startX + 30, startY + 2 + catal * 2, (char*)"��", 2);
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
	printf_s("���� ����     : %s", player.weapon.item.name);

	SetCursor(startX + 2, startY + 12);
	printf_s("���� ��   : %s", player.protection.item.name);

	SetCursor(startX + 2, startY + 14);
	printf_s("1. ĳ���� ����â �ݱ�");

	int i = 0;
	scanf("%d", &i);


} //����â ����
void showInven() //�κ��丮 ����
{
	short startX = 30;
	short startY = 2;

	showFrame(60, 22, startX, startY);
	for (int i = 0; i < invenIndex; i++)
	{
		SetCursor(startX + 3, startY + 2 + i * 2);
		printf_s("*�̸�: %s ", inventory[i].item.name);

		if (inventory[i].item.itemType == Item_Pro)
			printf_s(" Ÿ��: �� ");
		else if (inventory[i].item.itemType == Item_WP)
			printf_s(" Ÿ��: ���� ");
		else if (inventory[i].item.itemType == Item_Use)
			printf_s(" Ÿ��: �Ҹ�ǰ ");

		printf_s(" ����: %d", inventory[i].count);
	}

	SetCursor(startX + 2, startY + 18);
	printf_s("1. �κ��丮 �ݱ�");

	int i = 0;
	scanf("%d", &i);
}
void showBattleInvenUI()//��Ʋ���� �κ��丮 ui
{
	int startX = 65, startY = 18;
	showFrame(50, 10, 65, 18);

	SetPosition(startX + 15, startY + 2, (char*)"---  �κ��丮  ---", 2);

	for (int i = 0; i < invenIndex; i++)
	{
		if (inventory[i].item.itemType == Item_Use)
		{
			SetCursor(startX + 2, startY + 4 + (i * 2));
			printf_s("* %s  %d ȸ�� ", useitems[inventory[i].item.itemNum].item.name, useitems[inventory[i].item.itemNum].hpRegain);
		}
	}
}
void BattleInven()
{
	int useTemIndexes[InvenSize];
	for (int i = 0; i < invenIndex; i++)
	{
		if (inventory[i].item.itemType == Item_Use) //���������� �ε����� ����
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
	const int attack = 0;
	const int skill = 1;
	const int item = 2;
	const int exit = 3;

	MONSTER* p[5] = { NULL,NULL,NULL,NULL,NULL };

	int x = 10, y = 2; //�ܼ� ��ǥ

	int exp = 0, gold = 0; //���� �¸��� ����

	int apearNum = rand() % 5 + 1;//���� ���� ��

	for (int i = 0; i < apearNum; i++)//���� ����
	{
		int hidden = rand() % 100;

		if (hidden < 10) p[i] = CreateMonster(maps[MapState].apearMonster[2]); //���� ���� ����
		else if (hidden < 50) p[i] = CreateMonster(maps[MapState].apearMonster[1]); //�߰�
		else p[i] = CreateMonster(maps[MapState].apearMonster[0]); //���� ����
	}

	int PlayCurrHp = player.status.hp;
	showFrame(100, 22, x, y);
	while (true)
	{
		SetCursor(50, 2);
		printf_s("----- ���� ȭ�� -----");

		int j = 0;
		if (apearNum > 3) j = 0;
		else if (apearNum > 1) j = 15;
		else j = 30;

		for (int i = 0; i < apearNum; i++)
		{

			SetCursor(x + 15 + j + (i * 15), y + 1);
			printf_s("����:%d", p[i]->level);

			SetCursor(x + 15 + j + (i * 15), y + 2);
			printf_s("�̸�:%s", p[i]->name);

			SetCursor(x + 15 + j + (i * 15), y + 3);
			printf_s("                      ");
			SetCursor(x + 15 + j + (i * 15), y + 3);
			printf_s("HP:%d/%d", p[i]->currHP, p[i]->Hp);
		}

		SetCursor(50, y + 15);
		printf_s("����:%d", player.level);

		SetCursor(50, y + 16);
		printf_s("�̸�%s", player.name);

		SetCursor(50, y + 17);
		printf_s("                       ");
		SetCursor(50, y + 17);
		printf_s("HP:%d/%d", player.currHp, player.status.hp);

		SetCursor(50, 26);
		printf_s("1.����   2.��ų���  3.�����ۻ�� 4.��������");

		int i = 0;
		scanf_s("%d", &i);

		if (i == attack) //�⺻ ����
		{
			SetCursor(50, 26);
			printf_s("                                            ");

			SetCursor(50, 26);
			printf_s("������ ���� ����");

			int i = 0;
			scanf_s("%d", &i);

			if ((i < 6 && i>0) && (p[i - 1] != NULL) && (p[i - 1]->currHP > 0))
			{
				int damage = PlayerAttack(p[i - 1]);

				SetCursor(50, y + 13);
				printf_s("����");
				Sleep(1000);
				SetCursor(50, y + 13);
				printf_s("     ");

				for (int k = 0; k < apearNum; k++)
				{
					if (k == i - 1)//�ǰݴ��� ����
					{
						SetCursor(x + 15 + j + (k * 15), y + 5);
						printf_s("������: %d", damage);
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
						printf_s("����");
						Sleep(1000);
						SetCursor(x + 15 + j + (k * 15), y + 5);
						printf_s("      ");

						SetCursor(50, y + 13);
						printf_s("������: %d", damage);
						Sleep(1000);
						SetCursor(50, y + 13);
						printf_s("          ");

						SetCursor(50, y + 17);
						printf_s("HP:%d/%d", player.currHp, player.status.hp);
					}
				}
			}
		}
		else if (i == skill)//��ų
		{

		}
		else if (i == item)//������ ���
		{
			BattleInven();
		}
		else if (i == exit)//����
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
int LevelUp()//�������� ������ 1 ��ȯ �ƴϸ� 0 ��ȯ
{
	int isLevelUp = 0;
	while (levelTable[player.level - 1] < player.exp)
	{
		player.level++;
		isLevelUp = 1;

		if (player.job == 0) //���� �й�
		{
			player.status.hp += 10;
			player.status.att += 2;
			player.status.def += 1;
		}
		if (player.job == 1) //���� �й�
		{
			player.status.hp += 5;
			player.status.att += 3;
			player.status.def += 1;
		}
		if (player.job == 2) //���� �й�
		{
			player.status.hp += 3;
			player.status.att += 4;
			player.status.def += 1;
		}

	}
	return isLevelUp;
}

void winUi(int gold, int exp, int isLevelUp)//�̰����� ǥ�õǴ� ui 
{
	int startX = 40, startY = 5;


	showFrame(40, 15, startX, startY);
	SetPosition(startX + 5, startY + 2, (char*)"----------���� ����----------", 2);

	SetCursor(startX + 5, startY + 4);
	printf_s("* ȹ�� ���   : %d G", gold);

	SetCursor(startX + 5, startY + 6);
	printf_s("* ȹ�� ����ġ : %d EXP", exp);

	if (isLevelUp)
	{
		SetCursor(startX + 5, startY + 8);
		printf_s("* ĳ���� ���� ��!!!!");
	}
	SetCursor(startX + 7, startY + 10);
	printf_s("1. ���� ���");

	int i = 0;
	scanf_s("%d", &i);
	if (i == 1) BattleScene();
}
void rewardGain(int exp, int gold) //�÷��̾� ����ȹ��
{
	player.exp += exp;
	player.gold += gold;

	int isLevelUp = LevelUp();
	winUi(gold, exp, isLevelUp);
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
			if (i == 0 && j == 0)printf_s("��");
			else if (i == 0 && j == w - 1)printf_s("��");
			else if (i == (h - 1) && j == 0)printf_s("��");
			else if (i == (h - 1) && j == w - 1)printf_s("��");
			else if (i == 0 || i == h - 1) printf_s("��");
			else if (j == 0 || j == w - 1) printf_s("��");
			else printf_s(" ");
		}
	}
	SetCursor(startX + 1, startY + 1);
	
	
	
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

void shop()
{
	short startX = 40;
	short startY = 2;

	const int WP = 0;//����
	const int PT = 1;//��
	const int USE = 2; //�Ҹ�ǰ

	int page = 0;
	int catal = 0;

	SetCursor(startX + 10, startY + 2);
	printf_s("------ ���� ------");

	while (true)
	{
		showFrame(50, 22, startX, startY);

		page = page % 3;

		if (page == WP)
		{
			catal = catal % 3;
			SetCursor(startX + 25, startY + 2);
			printf_s("        * ����� *");

			for (int i = 0; i < sizeof(weapons) / sizeof(WEAPON); i++)
			{
				if (weapons[i].att != NULL)
				{
					SetCursor(startX + 2, startY + 4 + (i * 4));
					printf_s("*  �̸�:%s     �ݾ�:%d", weapons[i].item.name, weapons[i].item.price);

					SetCursor(startX + 2, startY + 5 + (i * 4));
					printf_s("   ���ݷ�:%d   ��������:%d", weapons[i].att, weapons[i].limitLevel);
				}
			}

			SetCursor(startX + 40, startY + 4 + (catal * 4));
			printf_s("<- ");
		}
		else if (page == PT)
		{
			catal = catal % 3;
			SetCursor(startX + 25, startY + 2);
			printf_s("        * ���� *");
			for (int i = 0; i < sizeof(protections) / sizeof(PROTECTION); i++)
			{
				if (protections[i].def != NULL)
				{
					SetCursor(startX + 2, startY + 4 + (i * 4));
					printf_s("*  �̸�:%s     �ݾ�:%d", protections[i].item.name, protections[i].item.price);

					SetCursor(startX + 2, startY + 5 + (i * 4));
					printf_s("   ����:%d   ��������:%d", protections[i].def, protections[i].limitLevel);
				}
			}

			SetCursor(startX + 40, startY + 4 + (catal * 4));
			printf_s("<- ");
		}
		else if (page == USE)
		{
			catal = catal % 6;
			SetCursor(startX + 25, startY + 2);
			printf_s("        * �Ҹ�ǰ *");
			for (int i = 0; i < sizeof(useitems) / sizeof(USETEM); i++)
			{
				if (useitems[i].hpRegain != NULL)
				{
					SetCursor(startX + 2, startY + 4 + (i * 2));
					printf_s("*  �̸�:%s   ü��ȸ����:%d   �ݾ�:%d", useitems[i].item.name, useitems[i].hpRegain, useitems[i].item.price);
				}
			}

			SetCursor(startX + 40, startY + 4 + (catal * 4));
			printf_s("<- ");
		}

		SetCursor(startX + 2, startY + 20);
		printf_s("1. ����  2. ���� ������ ");

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
		else if (c == 'a') //����
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

int buy(ITEM item)//���� �����ϸ� 1 �����ϸ� 0 ��ȯ
{
	if (player.gold >= item.price)
	{
		InputInven(item);
		player.gold -= item.price;
		printf_s("      ���ż���");
		Sleep(500);
		return 1;
	}
	else return 0;

}

void useTemUse(ITEM item, int index)
{
	if (item.itemType == Item_Use) //�Ҹ� ǰ�̸�
	{
		int hpRe = useitems[item.itemNum].hpRegain;
		int MpRe = useitems[item.itemNum].MpRegain;

		if (player.currHp < player.currHp + hpRe)
			player.currHp = player.status.hp;
		else
			player.currHp += hpRe;
	}
}

void useItem(int invenIndex) //������ ��� �Լ�
{
	if (inventory[invenIndex].item.itemType == Item_Use)
	{
		player.currHp = useitems[inventory[invenIndex].item.itemNum].hpRegain;
	}
}

void Save()
{
	FILE* fPtr = fopen("SaveData.txt", "w");
	if (fPtr == nullptr) printf_s("error : ������ �� �� ����.");

	fprintf(fPtr, "%s %d %d %d %d %d %d %d %d", player.name, player.level, player.job, player.gold, player.exp,
		player.currHp, player.status.att, player.status.def, player.status.hp);


	if (fclose(fPtr) != 0) printf_s(" error: ������ ���� �� ����");
}
void Load()
{
	FILE* fPtr = fopen("SaveData.txt", "r");
	if (fPtr == nullptr) printf_s("error : ������ �� �� ����.");
	else
	{
		while (EOF != fscanf(fPtr, "%s %d %d %d %d %d %d %d %d", player.name, &player.level, &player.job, &player.gold, &player.exp,
			&player.currHp, &player.status.att, &player.status.def, &player.status.hp))
		{

		}
	}
	
	if (fclose(fPtr) != 0) printf_s(" error: ������ ���� �� ����");
}