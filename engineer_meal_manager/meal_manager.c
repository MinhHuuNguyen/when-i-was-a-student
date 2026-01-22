#include <stdio.h>
#include <stdlib.h>
// #include <conio.h>
#include <curses.h>
#include <windows.h>
#include <string.h>
#include <time.h>

#define title "data/title.txt"
#define proteinTable1 "table/protein1.txt"
#define proteinTable2 "table/protein2.txt"
#define proteinTable3 "table/protein3.txt"
#define proteinTable4 "table/protein4.txt"
#define fatTable1 "table/fat1.txt"
#define fatTable2 "table/fat2.txt"
#define fatTable3 "table/fat3.txt"
#define glucoseTable1 "table/glucose1.txt"
#define glucoseTable2 "table/glucose2.txt"
#define glucoseTable3 "table/glucose3.txt"
#define fiberTable1 "table/fiber1.txt"
#define fiberTable2 "table/fiber2.txt"
#define fiberTable3 "table/fiber3.txt"
#define fiberTable4 "table/fiber4.txt"
#define accountFile "userData/account.txt"
#define aboutFile "data/about.txt"

int isWarned;

double proteinExceed = 0;
double fatExceed = 0;
double glucoseExceed = 0;
double fiberExceed = 0;

double proteinShortage = 0;
double fatShortage = 0;
double glucoseShortage = 0;
double fiberShortage = 0;

typedef struct Dish {
	int id;
	float weight;
}DISHES;

typedef struct Node {
	DISHES Data;
	struct Node *pNext;
}NODE;

typedef struct List {
	NODE *pHead;
	NODE *pTail;
}LIST;

struct Account {
	char name[100];
	char gender;
	float bodyWeight;
	float height;
	int age;
	float bmi;
	float brm;
	float calories;
	float healthyCalories;

	float proteinPerDay;
	float glucosePerDay;
	float fatPerDay;
	float fiberPerDay;

	float proteinEaten;
	float glucoseEaten;
	float fatEaten;
	float fiberEaten;

	float proteinYesterday;
	float glucoseYesterday;
	float fatYesterday;
	float fiberYesterday;

	float proteinBYesterday1;
	float glucoseBYesterday1;
	float fatBYesterday1;
	float fiberBYesterday1;

	float proteinBYesterday2;
	float glucoseBYesterday2;
	float fatBYesterday2;
	float fiberBYesterday2;
} account;

struct TitleDay {
	int day;
	int month;
	int year;
	char titleDay[13];
} TitleDay;

struct Yesterday {
	int yDay;
	int yMonth;
	int yYear;
	char yesterday[13];
} yesterday;

struct DayBeforeYesterday1 {
	int b1Day;
	int b1Month;
	int b1Year;
	char beforeYesterday1[13];
} beforeYesterday1;

struct DayBeforeYesterday2 {
	int b2Day;
	int b2Month;
	int b2Year;
	char beforeYesterday2[13];
} beforeYesterday2;

struct DishesData {
	int id;
	char name[100];
	float calories;
	float water;
	float protein;
	float fat;
	float glucose;
	float fiber;
} dishesData;

//tao 1 file dua theo ngay truy cap  tren may
void getTime() {
	struct tm *infoToday, *infoYesterday, *infoBeforeYesterday1, *infoBeforeYesterday2;
	errno_t err;
	time_t timeToday, timeYesterday, timeBeforeYesterday1, timeBeforeYesterday2;

	time(&timeToday);
	timeYesterday = timeToday - 60 * 60 * 24;
	timeBeforeYesterday1 = timeYesterday - 60 * 60 * 24;
	timeBeforeYesterday2 = timeBeforeYesterday1 - 60 * 60 * 24;

	infoToday 
	err = localtime_s(&timeToday);
	TitleDay.day = infoToday->tm_mday;
	TitleDay.month = 1 + infoToday->tm_mon;
	TitleDay.year = 1900 + infoToday->tm_year;
	sprintf(TitleDay.titleDay, "userData/%d%d%d.txt", TitleDay.day, TitleDay.month, TitleDay.year);

	infoYesterday = localtime(&timeYesterday);
	yesterday.yDay = infoYesterday->tm_mday;
	yesterday.yMonth = 1 + infoYesterday->tm_mon;
	yesterday.yYear = 1900 + infoYesterday->tm_year;
	sprintf(yesterday.yesterday, "userData/%d%d%d.txt", yesterday.yDay, yesterday.yMonth, yesterday.yYear);

	infoBeforeYesterday1 = localtime(&timeBeforeYesterday1);
	beforeYesterday1.b1Day = infoBeforeYesterday1->tm_mday;
	beforeYesterday1.b1Month = 1 + infoBeforeYesterday1->tm_mon;
	beforeYesterday1.b1Year = 1900 + infoBeforeYesterday1->tm_year;
	sprintf(beforeYesterday1.beforeYesterday1, "userData/%d%d%d.txt", beforeYesterday1.b1Day, beforeYesterday1.b1Month, beforeYesterday1.b1Year);

	infoBeforeYesterday2 = localtime(&timeBeforeYesterday2);
	beforeYesterday2.b2Day = infoBeforeYesterday2->tm_mday;
	beforeYesterday2.b2Month = 1 + infoBeforeYesterday2->tm_mon;
	beforeYesterday2.b2Year = 1900 + infoBeforeYesterday2->tm_year;
	sprintf(beforeYesterday2.beforeYesterday2, "userData/%d%d%d.txt", beforeYesterday2.b2Day, beforeYesterday2.b2Month, beforeYesterday2.b2Year);
}

//khoi tao mot danh sach lien ket
void init(LIST *l) {
	l->pHead = l->pTail = NULL;
}

//tao ra mot NODE moi
NODE* getNode(DISHES x) {
	NODE *p = (NODE *)malloc(sizeof(NODE));
	if (p == NULL) {
		return NULL;
	}
	p->Data = x;
	p->pNext = NULL;
	return p;
}

//them NODE vao cuoi danh sach lien ket
void addTail(LIST *l, NODE *p) {
	if (l->pHead == NULL) {
		l->pHead = l->pTail = p;
	}
	else {
		l->pTail->pNext = p;
		l->pTail = p;
	}
}

//giai phong danh sach lien ket
void release(LIST *l) {
	NODE *p;
	while (l->pHead != NULL) {
		p = l->pHead;
		l->pHead = l->pHead->pNext;
		free(p);
	}
}

//doc file
void readFile(const char fileName[], int sleepingTime) {
	int c;
	FILE *file;
	file = fopen(fileName, "r");
	if (file) {
		while ((c = getc(file)) != EOF) {
			putchar(c);
			Sleep(sleepingTime);
		}
		fclose(file);
	}
	else;
}

//chinh sua kich co man hinh den
void resizeConsole(int width, int height) {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}


//di den mot  vi  tri x,y trong man hinh
void goToXY(int x, int y) {
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}


//tao thanh cong cu huong dan o phia cuoi man hinh
void toolbar(char tool[]) {
	int i;
	goToXY(0, 35);
	for (i = 0; i <= 79; i++) {
		printf("_");
	}
	printf("   %s", tool);
}


//hien thi thong so co  the
void bodyStatistics() {
	char bmiResult[100], x;
	if (account.bmi < 15) {
		strcpy(bmiResult, "You are Very Severely Underweight");
	}
	else if (15 <= account.bmi && account.bmi < 16) {
		strcpy(bmiResult, "You are Severely Underweight");
	}
	else if (16 <= account.bmi && account.bmi < 18.5) {
		strcpy(bmiResult, "You are Underweight");
	}
	else if (18.5 <= account.bmi && account.bmi < 25) {
		strcpy(bmiResult, "You are in Healthy Weight");
	}
	else if (25 <= account.bmi && account.bmi < 30) {
		strcpy(bmiResult, "You are Overweight");
	}
	else if (30 <= account.bmi && account.bmi < 35) {
		strcpy(bmiResult, "You are in Obese Class I");
	}
	else if (35 <= account.bmi && account.bmi < 40) {
		strcpy(bmiResult, "You are in Obese Class II");
	}
	else if (40 <= account.bmi) {
		strcpy(bmiResult, "You are in Obese Class III");
	}
	else;
	printf("Your name is: %s\n", account.name);
	if (account.gender == 'm' || account.gender == 'M') {
		printf("You're Male\n");
	}
	else if (account.gender == 'f' || account.gender == 'F') {
		printf("You're Female\n");
	}
	printf("You're: %d years old\n", account.age);
	printf("Your weight is: %0.1f kilogram\n", account.bodyWeight);
	printf("Your height is: %0.1f centimeter\n", account.height);
	printf("Your BMI is: %0.1f => %s\n", account.bmi, bmiResult);
	printf("Your Calories is: %0.1f calories/day \n=> Your Healthy Calories is: %0.1f calories/day\n", account.calories, account.healthyCalories);
	printf("You need %0.1f gam protein per day\n", account.proteinPerDay);
	printf("You need %0.1f gam fat per day\n", account.fatPerDay);
	printf("You need %0.1f gam glucose per day\n", account.glucosePerDay);
	printf("You need %0.1f gam fiber per day\n", account.fiberPerDay);
	printf("BMI is the Body Mass Index\n");
	char test[] = "5 for Back";
	toolbar(test);
	while (x != '5') {
		x = getch();
	}
}


//cung cap  bang gia tri dinh duong
void chooseTableOfFood() {
	char x, medium[100];
	int proteinNumber = 1, fiberNumber = 1, fatNumber = 1, glucoseNumber = 1;
	readFile(title, 0);
	printf("\n\n\n");
	printf("\t\t FOOD TABLE\n");
	printf("\t\t 1. Protein Table\n");
	printf("\t\t 2. fiber Table\n");
	printf("\t\t 3. Glucose Table\n");
	printf("\t\t 4. Fat Table\n");
	printf("\t\t 5. Back\n");
	char test[100] = "1, 2, 3, 4 for Menu, 5 for Back";
	toolbar(test);
	x = getch();
	system("cls");
	printf("\n");
	switch (x) {
	case 49: {
		while (1) {
			if (proteinNumber == 1) {
				strcpy(medium, proteinTable1);
			}
			else if (proteinNumber == 2) {
				strcpy(medium, proteinTable2);
			}
			else if (proteinNumber == 3) {
				strcpy(medium, proteinTable3);
			}
			else if (proteinNumber == 4) {
				strcpy(medium, proteinTable4);
			}
			else;

			system("cls");
			readFile(medium, 0);
			char test[100] = "A, D to switch Table, 5 for Back";
			toolbar(test);
			char c = getch();
			if (c == 'd' || c == 'D') {
				if (proteinNumber == 1) {
					proteinNumber = 2;
				}
				else if (proteinNumber == 2) {
					proteinNumber = 3;
				}
				else if (proteinNumber == 3) {
					proteinNumber = 4;
				}
				else if (proteinNumber == 4) {
					proteinNumber = 1;
				}
				else;
				system("cls");
			}
			else if (c == 'a' || c == 'A') {
				if (proteinNumber == 1) {
					proteinNumber = 4;
				}
				else if (proteinNumber == 2) {
					proteinNumber = 1;
				}
				else if (proteinNumber == 3) {
					proteinNumber = 2;
				}
				else if (proteinNumber == 4) {
					proteinNumber = 3;
				}
				else;
				system("cls");
			}
			else if (c == '5') {
				break;
			}
			else;
		}
		break;
	}
	case 50: {
		while (1) {
			if (fiberNumber == 1) {
				strcpy(medium, fiberTable1);
			}
			else if (fiberNumber == 2) {
				strcpy(medium, fiberTable2);
			}
			else if (fiberNumber == 3) {
				strcpy(medium, fiberTable3);
			}
			else if (fiberNumber == 4) {
				strcpy(medium, fiberTable4);
			}
			else;

			system("cls");
			readFile(medium, 0);
			char test[100] = "A, D to switch Table, 5 for Back";
			toolbar(test);
			char c = getch();
			if (c == 'd' || c == 'D') {
				if (fiberNumber == 1) {
					fiberNumber = 2;
				}
				else if (fiberNumber == 2) {
					fiberNumber = 3;
				}
				else if (fiberNumber == 3) {
					fiberNumber = 4;
				}
				else if (fiberNumber == 4) {
					fiberNumber = 1;
				}
				else;
				system("cls");
			}
			else if (c == 'a' || c == 'A') {
				if (fiberNumber == 1) {
					fiberNumber = 4;
				}
				else if (fiberNumber == 2) {
					fiberNumber = 1;
				}
				else if (fiberNumber == 3) {
					fiberNumber = 2;
				}
				else if (fiberNumber == 4) {
					fiberNumber = 3;
				}
				else;
				system("cls");
			}
			else if (c == '5') {
				break;
			}
		}
		break;
	}
	case 51: {
		while (1) {
			if (glucoseNumber == 1) {
				strcpy(medium, glucoseTable1);
			}
			else if (glucoseNumber == 2) {
				strcpy(medium, glucoseTable2);
			}
			else if (glucoseNumber == 3) {
				strcpy(medium, glucoseTable3);
			}
			else;

			system("cls");
			readFile(medium, 0);
			char test[100] = "A, D to switch Table, 5 for Back";
			toolbar(test);
			char c = getch();
			if (c == 'd' || c == 'D') {
				if (glucoseNumber == 1) {
					glucoseNumber = 2;
				}
				else if (glucoseNumber == 2) {
					glucoseNumber = 3;
				}
				else if (glucoseNumber == 3) {
					glucoseNumber = 1;
				}
				else;
				system("cls");
			}
			else if (c == 'a' || c == 'A') {
				if (glucoseNumber == 1) {
					glucoseNumber = 3;
				}
				else if (glucoseNumber == 2) {
					glucoseNumber = 1;
				}
				else if (glucoseNumber == 3) {
					glucoseNumber = 2;
				}
				else;
				system("cls");
			}
			else if (c == '5') {
				break;
			}
		}

		break;
	}
	case 52: {
		while (1) {
			if (fatNumber == 1) {
				strcpy(medium, fatTable1);
			}
			else if (fatNumber == 2) {
				strcpy(medium, fatTable2);
			}
			else if (fatNumber == 3) {
				strcpy(medium, fatTable3);
			}
			else;

			system("cls");
			readFile(medium, 0);
			char test[100] = "A, D to switch Table, 5 for Back";
			toolbar(test);
			char c = getch();
			if (c == 'd' || c == 'D') {
				if (fatNumber == 1) {
					fatNumber = 2;
				}
				else if (fatNumber == 2) {
					fatNumber = 3;
				}
				else if (fatNumber == 3) {
					fatNumber = 1;
				}
				else;
				system("cls");
			}
			else if (c == 'a' || c == 'A') {
				if (fatNumber == 1) {
					fatNumber = 3;
				}
				else if (fatNumber == 2) {
					fatNumber = 1;
				}
				else if (fatNumber == 3) {
					fatNumber = 2;
				}
				else;
				system("cls");
			}
			else if (c == '5') {
				break;
			}
		}
		break;
	}
	case 53:
		break;
	default:
		;
	}
}


//thiet lap giao dien Menu
void setupUIMenu() {
	char test[100];
	readFile(title, 0);
	printf("\n\n\n");
	printf("\t Welcome to MEAL MANAGER <3\n");
	printf("\t 1. Food Table\n");
	printf("\t 2. Body Statistics\n");
	printf("\t 3. Meal Manager\n");
	printf("\t 4. About\n");
	if (isWarned) {
		printf("\t 5. Warning!!!!!\n");
		printf("\t 6. Exit\n");
		strcpy(test, "1, 2, 3, 4, 5 for Menu, 6 for Exit");
	}
	else {
		printf("\t 5. Exit\n");
		strcpy(test, "1, 2, 3, 4 for Menu, 5 for Exit");
	}
	toolbar(test);
}


//thiet lap giao dien SignIn
int setupUISignIn() {
	char signInVariable;
	readFile(title, 0);
	printf("\n\n\n");
	printf("\t 1. Sign Up\n");
	printf("\t 2. Sigh In\n");
	printf("\t 3. Exit\n");
	char test[100] = "1, 2 for Menu, 3 for Exit";
	toolbar(test);
	fflush(stdin);
	while (1) {
		signInVariable = getch();
		switch (signInVariable) {
		case 49:
			system("cls");
			return 1;
		case 50:
			system("cls");
			return 2;
		case 51:
			return 3;
		default:
			;
		}
	}
}


//nhap va tinh toan thong  so co  the va ghi  vao file
void signUp() {
	FILE *file;
	file = fopen(accountFile, "w");
	if (file == NULL) {
		printf("\nCan't open file or file doesn't exist.");
		exit(0);
	}
	else;
	printf("Your name is: ");
	fflush(stdin);
	gets(account.name);
	do {
		system("cls");
		printf("Your name is: %s", account.name);
		printf("\nYou're (M-Male/F-Female): ");
		scanf("%c", &account.gender);
	} while (account.gender != 'm' && account.gender != 'M' && account.gender != 'f' && account.gender != 'F');

	do {
		system("cls");
		printf("Your name is: %s", account.name);
		printf("\nYou're (M-Male/F-Female): %c", account.gender);
		printf("\nYou're (year): ");
		fflush(stdin);
		scanf("%d", &account.age);
	} while (account.age <= 0);

	do {
		system("cls");
		printf("Your name is: %s", account.name);
		printf("\nYou're (M-Male/F-Female): %c", account.gender);
		printf("\nYou're (year): %d", account.age);
		printf("\nYour weight is (kg): ");
		fflush(stdin);
		scanf("%f", &account.bodyWeight);
	} while (account.bodyWeight <= 0);

	do {
		system("cls");
		printf("Your name is: %s", account.name);
		printf("\nYou're (M-Male/F-Female): %c", account.gender);
		printf("\nYou're (year): %d", account.age);
		printf("\nYour weight is (kg): %0.1f", account.bodyWeight);
		printf("\nYour height is (cm): ");
		fflush(stdin);
		scanf("%f", &account.height);
	} while (account.height <= 0);

	system("cls");
	printf("Your name is: %s", account.name);
	printf("\nYou're (M-Male/F-Female): %c", account.gender);
	printf("\nYou're (year): %d", account.age);
	printf("\nYour weight is (kg): %0.1f", account.bodyWeight);
	printf("\nYour height is (cm): %0.1f", account.height);

	printf("\nEnter to Sign Up");
	char enter;
	do {
		fflush(stdin);
		enter = getchar();
	} while (enter != '\n');

	account.bmi = 10000 * account.bodyWeight / (account.height * account.height);

	if (account.gender == 'm' || account.gender == 'M') {
		account.brm = (13.397 * account.bodyWeight) + (4.799 * account.height) - (5.677 * account.age) + 88.362;
	}
	else if (account.gender == 'f' || account.gender == 'F') {
		account.brm = (9.247 * account.bodyWeight) + (3.098 * account.height) - (4.330 * account.age) + 447.593;
	}
	account.calories = account.brm * 1.2;

	if (account.bmi < 15) {
		account.healthyCalories = account.calories + 500;
	}
	else if (15 <= account.bmi && account.bmi < 16) {
		account.healthyCalories = account.calories + 375;
	}
	else if (16 <= account.bmi && account.bmi < 18.5) {
		account.healthyCalories = account.calories + 250;
	}
	else if (18.5 <= account.bmi && account.bmi < 25) {
		account.healthyCalories = account.calories;
	}
	else if (25 <= account.bmi && account.bmi < 30) {
		account.healthyCalories = 1500;
	}
	else if (30 <= account.bmi && account.bmi < 35) {
		account.healthyCalories = 1200;
	}
	else if (35 <= account.bmi && account.bmi < 40) {
		account.healthyCalories = 1000;
	}
	else if (40 <= account.bmi) {
		account.healthyCalories = 800;
	}
	else;

	account.proteinPerDay = account.healthyCalories / 1000 * 60;
	account.glucosePerDay = account.healthyCalories / 1000 * 145;
	account.fatPerDay = account.healthyCalories / 1000 * 21;
	account.fiberPerDay = 25;
	fprintf(file, "%s-%c-%d-%0.1f-%0.1f-%0.1f-%0.1f-%0.1f-%0.1f-%0.1f-%0.1f-%0.1f-%0.1f", account.name, account.gender, account.age, account.bodyWeight, account.height, account.bmi, account.brm, account.calories, account.healthyCalories, account.proteinPerDay, account.fatPerDay, account.glucosePerDay, account.fiberPerDay);
	fclose(file);
}


//doc thong so luu tu file ra, neu chua co yeu cau signUp
void signIn() {
	int count = 0;
	char c, enter;
	FILE *file;
	file = fopen(accountFile, "r");
	if (file == NULL) {
		printf("\nCan't open file or file doesn't exist.");
		exit(0);
	}
	else;
	while ((c = getc(file)) != EOF) {
		count++;
	}
	fclose(file);

	FILE *file1;
	file1 = fopen(accountFile, "r");
	if (count == 0) {
		printf("\n\tYou haven't signed up. Press H to sign up!");
		do {
			fflush(stdin);
			enter = getch();
		} while (enter != 'h' && enter != 'H');
		system("cls");
		signUp();
	}
	else {
		fflush(stdin);
		fscanf(file1, "%[^-]-%c-%d-%f-%f-%f-%f-%f-%f-%f-%f-%f-%f", &account.name, &account.gender, &account.age, &account.bodyWeight, &account.height, &account.bmi, &account.brm, &account.calories, &account.healthyCalories, &account.proteinPerDay, &account.fatPerDay, &account.glucosePerDay, &account.fiberPerDay);
	};
	fclose(file1);
}


//cung cap  thuc don tham chieu
void designMealMenu() {
	float proteinNotEaten, glucoseNotEaten, fatNotEaten, fiberNotEaten;
	float proteinDishWeight = 0, glucoseDishWeight = 0, fatDishWeight = 0, fiberDishWeight = 0;
	float medium1, medium2, medium3, medium4;
	int random;
	char name[13], proteinDish[100], fatDish[100], glucoseDish[100], fiberDish[100], x;
	proteinNotEaten = account.proteinPerDay - account.proteinEaten;
	glucoseNotEaten = account.glucosePerDay - account.glucoseEaten;
	fatNotEaten = account.fatPerDay - account.fatEaten;
	fiberNotEaten = account.fiberPerDay - account.fiberEaten;

	medium1 = proteinNotEaten;
	medium2 = glucoseNotEaten;
	medium3 = fatNotEaten;
	medium4 = fiberNotEaten;

	while (proteinNotEaten > 0 || glucoseNotEaten > 0 || fatNotEaten > 0 || fiberNotEaten > 0) {
		srand(time(NULL));
		if (fiberNotEaten > 0) {
			random = (rand() % (47)) + 201;
			sprintf(name, "data/%d.txt", random);
			FILE *fileFiber;
			fileFiber = fopen(name, "r");
			if (fileFiber == NULL) {
				printf("\nCan't open file or file doesn't exist.");
				exit(0);
			}
			else;
			fscanf(fileFiber, "%d-%[^-]-%f-%f-%f-%f-%f-%f", &dishesData.id, &dishesData.name, &dishesData.calories, &dishesData.water, &dishesData.protein, &dishesData.fat, &dishesData.glucose, &dishesData.fiber);
			strcpy(fiberDish, dishesData.name);
			fiberDishWeight = (fiberNotEaten * 100 / dishesData.fiber) * 0.95;
			proteinNotEaten = proteinNotEaten - fiberDishWeight * dishesData.protein / 100;
			fatNotEaten = fatNotEaten - fiberDishWeight * dishesData.fat / 100;
			glucoseNotEaten = glucoseNotEaten - fiberDishWeight * dishesData.glucose / 100;
			fclose(fileFiber);
		}
		else;

		if (fatNotEaten > 0) {
			random = (rand() % (28)) + 401;
			sprintf(name, "data/%d.txt", random);
			FILE *fileFat;
			fileFat = fopen(name, "r");
			if (fileFat == NULL) {
				printf("\nCan't open file or file doesn't exist.");
				exit(0);
			}
			else;
			fscanf(fileFat, "%d-%[^-]-%f-%f-%f-%f-%f", &dishesData.id, &dishesData.name, &dishesData.calories, &dishesData.water, &dishesData.protein, &dishesData.fat, &dishesData.glucose, &dishesData.fiber);
			strcpy(fatDish, dishesData.name);
			fatDishWeight = (fatNotEaten * 100 / dishesData.fat) * 0.95;
			proteinNotEaten = proteinNotEaten - fatDishWeight * dishesData.protein / 100;
			glucoseNotEaten = glucoseNotEaten - fatDishWeight * dishesData.glucose / 100;
			fclose(fileFat);
		}
		else;

		if (proteinNotEaten > 0) {
			random = (rand() % (47)) + 101;
			sprintf(name, "data/%d.txt", random);
			FILE *fileProtein;
			fileProtein = fopen(name, "r");
			if (fileProtein == NULL) {
				printf("\nCan't open file or file doesn't exist.");
				exit(0);
			}
			else;
			fscanf(fileProtein, "%d-%[^-]-%f-%f-%f-%f-%f", &dishesData.id, &dishesData.name, &dishesData.calories, &dishesData.water, &dishesData.protein, &dishesData.fat, &dishesData.glucose, &dishesData.fiber);
			strcpy(proteinDish, dishesData.name);
			proteinDishWeight = (proteinNotEaten * 100 / dishesData.protein) * 0.95;
			glucoseNotEaten = glucoseNotEaten - proteinDishWeight * dishesData.glucose / 100;
			fclose(fileProtein);
		}
		else;

		if (glucoseNotEaten > 0) {
			random = (rand() % (26)) + 301;
			sprintf(name, "data/%d.txt", random);
			FILE *fileGlucose;
			fileGlucose = fopen(name, "r");
			if (fileGlucose == NULL) {
				printf("\nCan't open file or file doesn't exist.");
				exit(0);
			}
			else;
			fscanf(fileGlucose, "%d-%[^-]-%f-%f-%f-%f-%f", &dishesData.id, &dishesData.name, &dishesData.calories, &dishesData.water, &dishesData.protein, &dishesData.fat, &dishesData.glucose, &dishesData.fiber);
			strcpy(glucoseDish, dishesData.name);
			glucoseDishWeight = (glucoseNotEaten * 100 / dishesData.glucose) * 0.95;
			fclose(fileGlucose);
		}
		else;

		if (proteinDishWeight == 0 && glucoseDishWeight == 0 && fatDishWeight == 0 && fiberDishWeight == 0) {
			proteinNotEaten = medium1;
			glucoseNotEaten = medium2;
			fatNotEaten = medium3;
			fiberNotEaten = medium4;
		}
		else {
			printf("Your Menu Today Includes:");
			if (proteinDishWeight > 0.0) {
				printf("\n\t %s - %0.1fg", proteinDish, proteinDishWeight);
			}
			else;
			if (glucoseDishWeight > 0.0) {
				printf("\n\t %s - %0.1fg", glucoseDish, glucoseDishWeight);
			}
			else;
			if (fatDishWeight > 0.0) {
				printf("\n\t %s - %0.1fg", fatDish, fatDishWeight);
			}
			else;
			if (fiberDishWeight > 0.0) {
				printf("\n\t %s - %0.1fg", fiberDish, fiberDishWeight);
			}
			else;
			break;
		}
	}
	char test[] = "5 for Back";
	toolbar(test);
	while (x != '5') {
		x = getch();
	}
}

//tinh toan khoi luong  tung loai thuc pham da an
void warning() {
	int i = 0, j = 0, countNumberDishes = 0, a = 1;
	int idYesterday[100], idBYesterday2[100], idBYesterday1[100];
	char c;
	float weightYesterday[100], weightBYesterday1[100], weightBYesterday2[100];

	FILE *fileYesterday;
	fileYesterday = fopen(yesterday.yesterday, "r");
	if (fileYesterday == NULL) {
		a = 0;
	}
	else;

	while ((c = getc(fileYesterday)) != EOF) {
		if (c == '\n') {
			countNumberDishes++;
		}
		else;
	}

	while (i < (13 * countNumberDishes)) {
		fseek(fileYesterday, i, SEEK_SET);
		fflush(stdin);
		fscanf(fileYesterday, "%d-%f", &idYesterday[j], &weightYesterday[j]);
		i += 13;
		j++;
	}
	fclose(fileYesterday);
	int i1;
	for (i1 = 0; i1 < j; i1++) {
		char name[13];
		sprintf(name, "data/%d.txt", idYesterday[i1]);
		FILE *file;
		file = fopen(name, "r");
		if (file == NULL) {
			printf("\nCan't open file or file doesn't exist2.");
			exit(0);
		}
		else;
		fscanf(file, "%d-%[^-]-%f-%f-%f-%f-%f-%f", &dishesData.id, &dishesData.name, &dishesData.calories, &dishesData.water, &dishesData.protein, &dishesData.fat, &dishesData.glucose, &dishesData.fiber);
		account.proteinYesterday += weightYesterday[i1] / 100 * dishesData.protein;
		account.fatYesterday += weightYesterday[i1] / 100 * dishesData.fat;
		account.glucoseYesterday += weightYesterday[i1] / 100 * dishesData.glucose;
		account.fiberYesterday += weightYesterday[i1] / 100 * dishesData.fiber;
		fclose(file);
	}

	countNumberDishes = 0;
	i = 0;
	j = 0;
	FILE *fileBeforeYesterday1;
	fileBeforeYesterday1 = fopen(beforeYesterday1.beforeYesterday1, "r");
	if (fileBeforeYesterday1 == NULL) {
		a = 0;
	}
	else;

	while ((c = getc(fileBeforeYesterday1)) != EOF) {
		if (c == '\n') {
			countNumberDishes++;
		}
		else;
	}

	while (i < (13 * countNumberDishes)) {
		fseek(fileBeforeYesterday1, i, SEEK_SET);
		fflush(stdin);
		fscanf(fileBeforeYesterday1, "%d-%f", &idBYesterday2[j], &weightBYesterday1[j]);
		i += 13;
		j++;
	}
	fclose(fileBeforeYesterday1);

	int i2;
	for (i2 = 0; i2 < j; i2++) {
		char name[13];
		sprintf(name, "data/%d.txt", idBYesterday2[i2]);
		FILE *file;
		file = fopen(name, "r");
		if (file == NULL) {
			printf("\nCan't open file or file doesn't exist4.");
			exit(0);
		}
		else;
		fscanf(file, "%d-%[^-]-%f-%f-%f-%f-%f-%f", &dishesData.id, &dishesData.name, &dishesData.calories, &dishesData.water, &dishesData.protein, &dishesData.fat, &dishesData.glucose, &dishesData.fiber);
		account.proteinBYesterday1 += weightBYesterday1[i2] / 100 * dishesData.protein;
		account.fatBYesterday1 += weightBYesterday1[i2] / 100 * dishesData.fat;
		account.glucoseBYesterday1 += weightBYesterday1[i2] / 100 * dishesData.glucose;
		account.fiberBYesterday1 += weightBYesterday1[i2] / 100 * dishesData.fiber;
		fclose(file);
	}

	countNumberDishes = 0;
	i = 0;
	j = 0;
	FILE *fileBeforeYesterday2;
	fileBeforeYesterday2 = fopen(beforeYesterday2.beforeYesterday2, "r");
	if (fileBeforeYesterday2 == NULL) {
		a = 0;
	}
	else;

	while ((c = getc(fileBeforeYesterday2)) != EOF) {
		if (c == '\n') {
			countNumberDishes++;
		}
		else;
	}

	while (i < (13 * countNumberDishes)) {
		fseek(fileBeforeYesterday2, i, SEEK_SET);
		fflush(stdin);
		fscanf(fileBeforeYesterday2, "%d-%f", &idBYesterday2[j], &weightBYesterday2[j]);
		i += 13;
		j++;
	}
	fclose(fileBeforeYesterday2);

	int i3;
	for (i3 = 0; i3 < j; i3++) {
		char name[13];
		sprintf(name, "data/%d.txt", idBYesterday2[i3]);
		FILE *file;
		file = fopen(name, "r");
		if (file == NULL) {
			printf("\nCan't open file or file doesn't exist6.");
			exit(0);
		}
		else;
		fscanf(file, "%d-%[^-]-%f-%f-%f-%f-%f-%f", &dishesData.id, &dishesData.name, &dishesData.calories, &dishesData.water, &dishesData.protein, &dishesData.fat, &dishesData.glucose, &dishesData.fiber);
		account.proteinBYesterday2 += weightBYesterday2[i3] / 100 * dishesData.protein;
		account.fatBYesterday2 += weightBYesterday2[i3] / 100 * dishesData.fat;
		account.glucoseBYesterday2 += weightBYesterday2[i3] / 100 * dishesData.glucose;
		account.fiberBYesterday2 += weightBYesterday2[i3] / 100 * dishesData.fiber;
		fclose(file);
	}

	if (account.proteinYesterday > account.proteinPerDay && account.proteinBYesterday1 > account.proteinPerDay && account.proteinBYesterday2 > account.proteinPerDay) {
		isWarned = 1;
		proteinExceed = (account.proteinYesterday + account.proteinBYesterday1 + account.proteinBYesterday2) - 3 * account.proteinPerDay;
	}
	else;

	if (account.fatYesterday > account.fatPerDay && account.fatBYesterday1 > account.fatPerDay && account.fatBYesterday2 > account.fatPerDay) {
		isWarned = 1;
		fatExceed = (account.fatYesterday + account.fatBYesterday1 + account.fatBYesterday2) - 3 * account.fatPerDay;
	}
	else;

	if (account.glucoseYesterday > account.glucosePerDay && account.glucoseBYesterday1 > account.glucosePerDay && account.glucoseBYesterday2 > account.glucosePerDay) {
		isWarned = 1;
		glucoseExceed = (account.glucoseYesterday + account.glucoseBYesterday1 + account.glucoseBYesterday2) - 3 * account.glucosePerDay;
	}
	else;

	if (account.fiberYesterday > account.fiberPerDay && account.fiberBYesterday1 > account.fiberPerDay && account.fiberBYesterday2 > account.fiberPerDay) {
		isWarned = 1;
		fiberExceed = (account.fiberYesterday + account.fiberBYesterday1 + account.fiberBYesterday2) - 3 * account.fiberPerDay;
	}
	else;

	if (account.proteinYesterday < account.proteinPerDay && account.proteinBYesterday1 < account.proteinPerDay && account.proteinBYesterday2 < account.proteinPerDay) {
		isWarned = 1;
		proteinShortage = -(account.proteinYesterday + account.proteinBYesterday1 + account.proteinBYesterday2) + 3 * account.proteinPerDay;
	}
	else;

	if (account.fatYesterday < account.fatPerDay && account.fatBYesterday1 < account.fatPerDay && account.fatBYesterday2 < account.fatPerDay) {
		isWarned = 1;
		fatShortage = -(account.fatYesterday + account.fatBYesterday1 + account.fatBYesterday2) + 3 * account.fatPerDay;
	}
	else;

	if (account.glucoseYesterday < account.glucosePerDay && account.glucoseBYesterday1 < account.glucosePerDay && account.glucoseBYesterday2 < account.glucosePerDay) {
		isWarned = 1;
		glucoseShortage = -(account.glucoseYesterday + account.glucoseBYesterday1 + account.glucoseBYesterday2) + 3 * account.glucosePerDay;
	}
	else;

	if (account.fiberYesterday < account.fiberPerDay && account.fiberBYesterday1 < account.fiberPerDay && account.fiberBYesterday2 < account.fiberPerDay) {
		isWarned = 1;
		fiberShortage = -(account.fiberYesterday + account.fiberBYesterday1 + account.fiberBYesterday2) + 3 * account.fiberPerDay;
	}
	else;
	if (a == 0) {
		isWarned = 0;
	}
}


//nhap ID va khoi luong cua thuc  pham  giau  chat dam
void inputProtein(LIST *l) {
	DISHES x;
	int a, b, i = 1;

	while (1) {
		char test[100] = "Enter ID and Weight(g), Enter 0 to move to Fiber";
		toolbar(test);
		goToXY(0, 0);
		readFile("chooseTable/protein.txt", 0);
		printf("\nProtein-rich foods number %d id: ", i);
		fflush(stdin);
		a = scanf("%d", &x.id);
		fflush(stdin);
		if ((x.id < 101 && x.id != 0) || x.id > 148 || a != 1) {
			system("cls");
			release(l);
			i = 1;
			printf("You entered wrong. Please try again!\n");
		}
		else if (x.id == 0) {
			break;
		}
		else {
			while (1) {
				printf("Number %d weight(g) :", i);
				fflush(stdin);
				b = scanf("%f", &x.weight);
				fflush(stdin);
				if (x.weight <= 0 || b != 1) {
					system("cls");
					printf("You entered wrong. Please try again!\n");
					readFile("chooseTable/protein.txt", 0);
					printf("\nProtein-rich foods number %d id: %d\n", i, x.id);
				}
				else {
					break;
				}
			}
			i++;
			NODE *p = getNode(x);
			addTail(l, p);
			system("cls");
		}
	}

	FILE *file;
	file = fopen(TitleDay.titleDay, "a");
	if (file == NULL) {
		printf("\nCan't open file or file doesn't exist.");
		exit(0);
	}
	else;
	if (i == 1) {
		fprintf(file, "101-00000.0\n");
	}
	else {
		NODE *p;
		for (p = l->pHead; p != NULL; p = p->pNext) {
			if (p->Data.weight < 10) {
				fprintf(file, "%d-0000%0.1f\n", p->Data.id, p->Data.weight);
			}
			else if (p->Data.weight < 100) {
				fprintf(file, "%d-000%0.1f\n", p->Data.id, p->Data.weight);
			}
			else if (p->Data.weight < 1000) {
				fprintf(file, "%d-00%0.1f\n", p->Data.id, p->Data.weight);
			}
			else if (p->Data.weight < 10000) {
				fprintf(file, "%d-0%0.1f\n", p->Data.id, p->Data.weight);
			}
			else {
				fprintf(file, "%d-%0.1f\n", p->Data.id, p->Data.weight);
			}
		}
	}

	fclose(file);
	system("cls");
}


//nhap ID va khoi  luong cua thuc pham giau chat xo
void inputFiber(LIST *l) {
	DISHES x;
	int a, b, i = 1;

	while (1) {
		char test[100] = "Enter ID and Weight(g), Enter 0 to move to Glucose";
		toolbar(test);
		goToXY(0, 0);
		readFile("chooseTable/fiber.txt", 0);
		printf("Fiber-rich foods number %d id: ", i);
		fflush(stdin);
		a = scanf("%d", &x.id);
		fflush(stdin);
		if ((x.id < 201 && x.id != 0) || x.id > 248 || a != 1) {
			system("cls");
			release(l);
			i = 1;
			printf("You entered wrong. Please try again!\n");
		}
		else if (x.id == 0) {
			break;
		}
		else {
			while (1) {
				printf("Number %d weight(g) :", i);
				fflush(stdin);
				b = scanf("%f", &x.weight);
				fflush(stdin);
				if (x.weight <= 0 || b != 1) {
					system("cls");
					printf("You entered wrong. Please try again!\n");
					readFile("chooseTable/fiber.txt", 0);
					printf("Fiber-rich foods number %d id: %d\n", i, x.id);
				}
				else {
					break;
				}
			}
			i++;
			NODE *p = getNode(x);
			addTail(l, p);
			system("cls");
		}
	}

	FILE *file;
	file = fopen(TitleDay.titleDay, "a");
	if (file == NULL) {
		printf("\nCan't open file or file doesn't exist.");
		exit(0);
	}
	else;
	if (i == 1) {
		fprintf(file, "201-00000.0\n");
	}
	else {
		NODE *p;
		for (p = l->pHead; p != NULL; p = p->pNext) {
			if (p->Data.weight < 10) {
				fprintf(file, "%d-0000%0.1f\n", p->Data.id, p->Data.weight);
			}
			else if (p->Data.weight < 100) {
				fprintf(file, "%d-000%0.1f\n", p->Data.id, p->Data.weight);
			}
			else if (p->Data.weight < 1000) {
				fprintf(file, "%d-00%0.1f\n", p->Data.id, p->Data.weight);
			}
			else if (p->Data.weight < 10000) {
				fprintf(file, "%d-0%0.1f\n", p->Data.id, p->Data.weight);
			}
			else {
				fprintf(file, "%d-%0.1f\n", p->Data.id, p->Data.weight);
			}
		}
	}
	fclose(file);
	system("cls");
}


//nhap ID va khoi luong cua thuc pham giau chat beo
void inputFat(LIST *l) {
	DISHES x;
	int a, b, i = 1;

	while (1) {
		char test[100] = "Enter ID and Weight(g), Enter 0 to end";
		toolbar(test);
		goToXY(0, 0);
		readFile("chooseTable/fat.txt", 0);
		printf("\nFat-rich foods number %d id: ", i);
		fflush(stdin);
		a = scanf("%d", &x.id);
		fflush(stdin);
		if ((x.id < 401 && x.id != 0) || x.id > 429 || a != 1) {
			system("cls");
			release(l);
			i = 1;
			printf("You entered wrong. Please try again!\n");
		}
		else if (x.id == 0) {
			break;
		}
		else {
			while (1) {
				printf("Number %d weight(g) :", i);
				fflush(stdin);
				b = scanf("%f", &x.weight);
				fflush(stdin);
				if (x.weight <= 0 || b != 1) {
					system("cls");
					printf("You entered wrong. Please try again!\n");
					readFile("chooseTable/fat.txt", 0);
					printf("Fat-rich foods number %d id: %d\n", i, x.id);
				}
				else {
					break;
				}
			}
			i++;
			NODE *p = getNode(x);
			addTail(l, p);
			system("cls");
		}
	}

	FILE *file;
	file = fopen(TitleDay.titleDay, "a");
	if (file == NULL) {
		printf("\nCan't open file or file doesn't exist.");
		exit(0);
	}
	else;
	if (i == 1) {
		fprintf(file, "401-00000.0\n");
	}
	else {
		NODE *p;
		for (p = l->pHead; p != NULL; p = p->pNext) {
			if (p->Data.weight < 10) {
				fprintf(file, "%d-0000%0.1f\n", p->Data.id, p->Data.weight);
			}
			else if (p->Data.weight < 100) {
				fprintf(file, "%d-000%0.1f\n", p->Data.id, p->Data.weight);
			}
			else if (p->Data.weight < 1000) {
				fprintf(file, "%d-00%0.1f\n", p->Data.id, p->Data.weight);
			}
			else if (p->Data.weight < 10000) {
				fprintf(file, "%d-0%0.1f\n", p->Data.id, p->Data.weight);
			}
			else {
				fprintf(file, "%d-%0.1f\n", p->Data.id, p->Data.weight);
			}
		}
	}
	fclose(file);
	system("cls");
}


//nhap ID va khoi luong cua  thuc pham giau chat bot  duong
void inputGlucose(LIST *l) {
	DISHES x;
	int a, b, i = 1;

	while (1) {
		char test[100] = "Enter ID and Weight(g), Enter 0 to move to Fat";
		toolbar(test);
		goToXY(0, 0);
		readFile("chooseTable/glucose.txt", 0);
		printf("\nGlucose-rich foods number %d id: ", i);
		fflush(stdin);
		a = scanf("%d", &x.id);
		fflush(stdin);
		if ((x.id < 301 && x.id != 0) || x.id > 327 || a != 1) {
			system("cls");
			release(l);
			i = 1;
			printf("You entered wrong. Please try again!\n");
		}
		else if (x.id == 0) {
			break;
		}
		else {
			while (1) {
				printf("Number %d weight(g) :", i);
				fflush(stdin);
				b = scanf("%f", &x.weight);
				fflush(stdin);
				if (x.weight <= 0 || b != 1) {
					system("cls");
					printf("You entered wrong. Please try again!\n");
					readFile("chooseTable/glucose.txt", 0);
					printf("\nGlucose-rich foods number %d id: %d\n", i, x.id);
				}
				else {
					break;
				}
			}
			i++;
			NODE *p = getNode(x);
			addTail(l, p);
			system("cls");
		}
	}

	FILE *file;
	file = fopen(TitleDay.titleDay, "a");
	if (file == NULL) {
		printf("\nCan't open file or file doesn't exist.");
		exit(0);
	}
	else;
	if (i == 1) {
		fprintf(file, "301-00000.0\n");
	}
	else {
		NODE *p;
		for (p = l->pHead; p != NULL; p = p->pNext) {
			if (p->Data.weight < 10) {
				fprintf(file, "%d-0000%0.1f\n", p->Data.id, p->Data.weight);
			}
			else if (p->Data.weight < 100) {
				fprintf(file, "%d-000%0.1f\n", p->Data.id, p->Data.weight);
			}
			else if (p->Data.weight < 1000) {
				fprintf(file, "%d-00%0.1f\n", p->Data.id, p->Data.weight);
			}
			else if (p->Data.weight < 10000) {
				fprintf(file, "%d-0%0.1f\n", p->Data.id, p->Data.weight);
			}
			else {
				fprintf(file, "%d-%0.1f\n", p->Data.id, p->Data.weight);
			}
		}
	}
	fclose(file);
	system("cls");
}


//cung cap ID va ten cac loai thuc pham
//nhap ID va khoi luong cac loai thuc pham da an
//in ra khoi luong cac chat dinh duong da an trong ngay
void mealManager() {
	int i = 0, j = 0, countNumberDishes = 0;
	int a[100];
	char c;
	float b[100];
	LIST listInputProtein, listInputfiber, listInputFat, listInputGlucose;
	init(&listInputProtein);
	init(&listInputfiber);
	init(&listInputFat);
	init(&listInputGlucose);

	inputProtein(&listInputProtein);
	inputFiber(&listInputfiber);
	inputGlucose(&listInputGlucose);
	inputFat(&listInputFat);

	FILE *file;
	file = fopen(TitleDay.titleDay, "r");
	if (file == NULL) {
		printf("\nCan't open file or file doesn't exist.");
		exit(0);
	}
	else;

	while ((c = getc(file)) != EOF) {
		if (c == '\n') {
			countNumberDishes++;
		}
		else;
	}

	while (i < (13 * countNumberDishes)) {
		fseek(file, i, SEEK_SET);
		fflush(stdin);
		fscanf(file, "%d-%f", &a[j], &b[j]);
		i += 13;
		j++;
	}
	fclose(file);

	printf("Today, You've eaten: ");
	//	int w = 0;
	int i4;
	for (i4 = 0; i4 < j; i4++) {
		char name[13];

		sprintf(name, "data/%d.txt", a[i4]);
		FILE *file;
		file = fopen(name, "r");
		if (file == NULL) {
			printf("\nCan't open file or file doesn't exist.");
			exit(0);
		}
		else;
		fscanf(file, "%d-%[^-]-%f-%f-%f-%f-%f-%f", &dishesData.id, &dishesData.name, &dishesData.calories, &dishesData.water, &dishesData.protein, &dishesData.fat, &dishesData.glucose, &dishesData.fiber);
		if (b[i4] != 0) {
			printf("\n - %0.1f gam %s\n", b[i4], dishesData.name);
		}
		else;
		account.proteinEaten += b[i4] / 100 * dishesData.protein;
		account.fatEaten += b[i4] / 100 * dishesData.fat;
		account.glucoseEaten += b[i4] / 100 * dishesData.glucose;
		account.fiberEaten += b[i4] / 100 * dishesData.fiber;
		fclose(file);
		//    	w++;
	}
	//	printf("%d", w);
	printf("\n => %0.1f gam protein\n", account.proteinEaten);
	printf("\n => %0.1f gam fat\n", account.fatEaten);
	printf("\n => %0.1f gam glucose\n", account.glucoseEaten);
	printf("\n => %0.1f gam fiber\n", account.fiberEaten);
	designMealMenu();
}


// dua ra canh bao
void UIWarning() {
	if (proteinExceed > 0) {
		printf("\nIn the last 3 days, you've eaten %0.1f g of protein, more than the healthy \namount of protein!!!", proteinExceed);
	}
	else;
	if (fatExceed > 0) {
		printf("\nIn the last 3 days, you've eaten %0.1f g of fat, more than the healthy \namount of fat!!!", fatExceed);
	}
	else;
	if (glucoseExceed > 0) {
		printf("\nIn the last 3 days, you've eaten %0.1f g of glucose, more than the healthy \namount of glucose!!!", glucoseExceed);
	}
	else;
	if (fiberExceed > 0) {
		printf("\nIn the last 3 days, you've eaten %0.1f g of fiber, more than the healthy \namount of fiber!!!", fiberExceed);
	}
	else;

	if (proteinShortage > 0) {
		printf("\nIn the last 3 days, you've eaten %0.1f g of protein, fewer than the healthy \namount of protein!!!", proteinShortage);
	}
	else;
	if (fatShortage > 0) {
		printf("\nIn the last 3 days, you've eaten %0.1f g of fat, fewer than the healthy \namount of fat!!!", fatShortage);
	}
	else;
	if (glucoseShortage > 0) {
		printf("\nIn the last 3 days, you've eaten %0.1f g of glucose, fewer than the healthy \namount of glucose!!!", glucoseShortage);
	}
	else;
	if (fiberShortage > 0) {
		printf("\nIn the last 3 days, you've eaten %0.1f g of fiber, fewer than the healthy \namount of fiber!!!", fiberShortage);
	}
	else;

	printf("\nBe careful!!!!!");
	char test[100] = "5 for Back";
	toolbar(test);
	char x = getch();
	while (x != '5');
}

//gioi thieu thong tin chuong trinh
void about() {
	char x;
	readFile(aboutFile, 10);
	char test[100] = "5 for Back";
	toolbar(test);
	do {
		x = getch();
	} while (x != '5');
}

int main() {
	char menuVariable;
	int calo;
	resizeConsole(700, 700);
	getTime();
	int a = setupUISignIn();
	if (a == 1) {
		signUp();
	}
	else if (a == 2) {
		signIn();
	}
	else if (a == 3) {
		exit(0);
	}
	else;
	warning();
	while (1) {
		system("cls");
		setupUIMenu();
		menuVariable = getch();
		system("cls");
		switch (menuVariable) {
		case 49:
			chooseTableOfFood();
			system("cls");
			break;
		case 50:
			bodyStatistics();
			break;
		case 51:
			mealManager();
			break;
		case 52:
			about();
			break;
		case 53:
			if (isWarned) {
				UIWarning();
				break;
			}
			else {
				exit(0);
			}
		case 54:
			if (isWarned) {
				exit(0);
			}
			else {
				break;
			};
		default:
			break;
			;
		}
		system("cls");
	}
}
