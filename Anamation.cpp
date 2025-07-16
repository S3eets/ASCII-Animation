#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>

using namespace std;

vector <string> vPreviouslyUsedFileNames;

HANDLE Color = GetStdHandle(STD_OUTPUT_HANDLE);

struct stAnimationInfo
{
	vector <string> vFileNames;
	short FramesRepeted;
	short Delay;
};

void PrintHowToUse();
void PrintFileNames(vector <string>);
void PrintReorderScreen();

void SetColor(HANDLE Color, short ColorNumber)
{

	SetConsoleTextAttribute(Color, ColorNumber);

	/*
	 * 0 : Black
	 * 1 : Blue
	 * 2 : Green
	 * 3 : Aqua / Cyan
	 * 4 : Red
	 * 5 : Purple / Magenta
	 * 6 : Yellow / Dark Yellow
	 * 7 : Light Gray
	 * 8 : Dark Gray
	 * 9 : Light Blue
	 * 10 : Light Green
	 * 11 : Light Aqua / Light Cyan
	 * 12 : Light Red
	 * 13 : Light Purple / Light Magenta
	 * 14 : Light Yellow
	 * 15 : White
  */


}

void SetCursorVisibility(bool Visible)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = Visible;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}

vector <string> LoadDataFromFileToStringVector(string FileName)
{
	vector <string> vString;

	fstream MyFile;
	MyFile.open(FileName, ios::in); // read mode

	if (MyFile.is_open())
	{
		string Line;
		while (getline(MyFile, Line))
		{
			vString.push_back(Line);
		}

		MyFile.close();
	}
	else
	{

		SetColor(Color, 12);
		cerr << "Error: Could not open file " << FileName << "\n";
		SetColor(Color, 15);
	}

	return vString;
}

vector<vector<string>> LoadAllFrames(const vector<string>& vFileNames)
{
	vector<vector<string>> vAllFrames;
	// it's mean vector of (i) is vector of string 

	for (const string& FileName : vFileNames)
	{
		vAllFrames.push_back(LoadDataFromFileToStringVector(FileName));
		// all lacations have like file.txt as looke like object.
	}

	return vAllFrames;
}

string GetFormAsAText(const vector <string>& vFrame)
{
	string TheForm = "";

	for (const string& Line : vFrame)
	{
		TheForm += Line + "\n";
	}

	return TheForm;
}

void PrintAnmiation(const vector <string>& vFrame, short Delay)
{
	system("cls");
	string TheForm = GetFormAsAText(vFrame);
	cout << TheForm;
	Sleep(Delay);
}

void TurnAnmiation(const stAnimationInfo& AnimationInfo)
{

	vector<vector<string>> vAllFrames = LoadAllFrames(AnimationInfo.vFileNames);

	SetCursorVisibility(false);

	for (short i = 1; i <= AnimationInfo.FramesRepeted; i++)
	{

		for (const vector<string>& Frame : vAllFrames)
		{
			PrintAnmiation(Frame, AnimationInfo.Delay);

		}

	}

	SetCursorVisibility(true);

}

char ReadYesOrNoCharacter()
{
	string sCharacter;
	cin >> sCharacter;

	while (sCharacter.length() != 1 || toupper(sCharacter[0]) != 'Y' && toupper(sCharacter[0]) != 'N')
	{

		SetColor(Color, 12);
		cout << "Invalid Character, Enter a valid One? ";
		SetColor(Color, 15);
		cin >> sCharacter;

	}


	return sCharacter[0];
}

char ReadYesOrNoOrSkipCharacter()
{
	string sCharacter;
	cin >> sCharacter;

	while (sCharacter.length() != 1 || toupper(sCharacter[0]) != 'Y' && toupper(sCharacter[0]) != 'N' && toupper(sCharacter[0] != 'S'))
	{

		SetColor(Color, 12);
		cout << "Invalid Character, Enter a valid One? ";
		SetColor(Color, 15);
		cin >> sCharacter;

	}


	return sCharacter[0];
}

void PressToContinue()
{
	SetColor(Color, 14);
	cout << "\n\nPress any key to Continue...";
	SetColor(Color, 15);
	system("pause>0");
}

bool IsAlphabetic(char Alpha)
{
	char Alphabetic[26] = {
		  'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
	};

	for (short i = 0; i < 26; i++)
	{
		if (tolower(Alpha) == Alphabetic[i])
			return true;
	}

	return false;

}
bool IsPunctuation(char Ch1)
{
	char Punctuation[32] = {
		'!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.',
		'/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`',
		'{', '|', '}', '~'
	};
	for (short i = 0; i < 32; i++)
	{
		if (Punctuation[i] == Ch1)
			return true;
	}

	return false;
}
bool IsFail(string sNumber)
{

	for (short i = 0; i <= sNumber.length() - 1; i++)
	{
		if (IsAlphabetic(sNumber[i]) || IsPunctuation(sNumber[i]))
			return true;
	}

	return false;

}
short FindFirstDigitPosition(string sNumber)
{

	for (short i = 0; i <= sNumber.length() - 1; i++)
	{
		if (isdigit(sNumber[i]))
			return i;
	}

	return 0;
}
string EaresSpaces(string sNumber)
{
	string Result = "";

	sNumber.erase(0, FindFirstDigitPosition(sNumber));

	for (char ch : sNumber)
	{
		if (isdigit(ch))
		{
			Result += ch;
			continue;
		}

		if (ch == ' ')
			return Result;

	}

	return Result;
}
short ReadValiedNumber()
{
	string sNumber = "";
	getline(cin >> ws, sNumber);

	while (IsFail(sNumber))
	{
		SetColor(Color, 12);
		cout << "Invalied Number, Enter a valied one: ";
		SetColor(Color, 15);

		getline(cin >> ws, sNumber);

	}

	sNumber = EaresSpaces(sNumber);

	return (short)stoi(sNumber);
}

short ReadValidNumberInRange(string Message, short From, short To)
{
	short Number = 0;

	cout << Message;
	Number = ReadValiedNumber();

	while (Number < From || Number > To)
	{
		SetColor(Color, 12);
		cout << "Number out of range, Enter in range one: ";
		SetColor(Color, 15);

		Number = ReadValiedNumber();
	}

	return Number;

}
short ReadHowManyFramesRepeted()
{
	return ReadValidNumberInRange("\nHow many Repeated Frames do you want? (1-1000)? ", 1, 1000);
}
short ReadDelay()
{
	return ReadValidNumberInRange("\nNote: 1000ms = 1s\nHow many Frames Delay do you want? (100ms-2000ms)? ", 100, 10000);
}
bool isFileNameExistsInProject(string FileName)
{
	ifstream file(FileName);
	return file.good() ? true : false;
}

bool IsTxtFile(string FileName)
{
	return (FileName.length() >= 4) && (FileName.substr(FileName.length() - 4) == ".txt");
}

string ReadFileNameExistsInProject()
{
	string FileName;

	cout << "\nEnter a File Name? ";

	SetColor(Color, 13);
	getline(cin >> ws, FileName);
	SetColor(Color, 15);

	if (!IsTxtFile(FileName))
		FileName += ".txt";

	while (!isFileNameExistsInProject(FileName))
	{
		if (cin.fail())
		{
			cin.clear();
		}
		cin.ignore(10000, '\n');


		SetColor(Color, 12);
		cout << "File Name [" << FileName << "] Not exists.\n";
		SetColor(Color, 15);
		cout << "Enter a File Name ? ";

		SetColor(Color, 13);
		getline(cin >> ws, FileName);
		SetColor(Color, 15);


		if (!IsTxtFile(FileName))
			FileName += ".txt";

	}

	return FileName;
}

void MakeSureNonExistsFileNameInStringVector(string& FileName, vector <string>& vFileNames)
{

	if (vFileNames.empty())
		return;

	bool IsFound = false;

	while (true)
	{
		for (const string& ExistingName : vFileNames)
		{
			if (FileName == ExistingName)
			{
				IsFound = true;
				break;
			}
		}

		if (!IsFound)
			break;

		SetColor(Color, 12);
		cout << "Repeated!!, Please enter non exists Filename in the System.";
		SetColor(Color, 15);

		FileName = ReadFileNameExistsInProject();
		IsFound = false;
	}


	for (short i = 0; i <= vFileNames.size() - 1; i++)
	{
		if (FileName == vFileNames[i])
		{
			cout << "Repeated!!, Please enter non exists Filename in a System? ";
			FileName = ReadFileNameExistsInProject();
		}
	}
}

char MakeSureHavingMoreOneFileName(vector <string>& vFileNames)
{
	short Count = 0;
	for (const string& FileName : vFileNames)
	{
		Count++;

		if (Count == 2)
			return 'N';

	}

	if (Count == 1)
	{

		SetColor(Color, 12);
		cout << "\nYou must to add Two File at least!";
		SetColor(Color, 15);

		return 'Y'; // Y:Yeas add new Filename
	}

	return 'N';

}
void PrintFileNames(vector <string> vFileName)
{
	short Counter = 1;
	for (const string& FileName : vFileName)
	{
		cout << "\n" << Counter << ") " << FileName;
		Counter++;
	}
	cout << "\n";
}

bool IsOrderRepeatedInReorder(vector <string>& vReorder, short OrderFilename)
{

	string strOrder = vPreviouslyUsedFileNames[OrderFilename - 1];

	for (const string& item : vReorder)
	{
		if (item == strOrder)
			return true;
	}


	return false;
}

void MakeSureNonRepetedOrderFileName(short& OrderFilename, vector <string>& vReorder, vector <string>& vFilename)
{
	if (vReorder.empty())
		return;

	while (IsOrderRepeatedInReorder(vReorder, OrderFilename))
	{
		SetColor(Color, 12);
		OrderFilename = ReadValidNumberInRange("Repeted!!, Pleas enter non repeted Order? ", 1, vFilename.size());
		SetColor(Color, 15);

	}

}

void Reorder(vector <string>& vFilename, vector <string>& vReorder)
{
	short OrderFilename = 0;

	for (short i = 0; i <= vFilename.size() - 1; i++)
	{

		OrderFilename = ReadValidNumberInRange("Order" + to_string(i + 1) + ": ", 1, vFilename.size());

		MakeSureNonRepetedOrderFileName(OrderFilename, vReorder, vFilename);
		vReorder.push_back(vFilename[OrderFilename - 1]);

	}
}

void ReorderFilenames(vector <string>& vFilename)
{
	vector <string> vReorder;
	char Answer = 'Y';

	do
	{
		cout << "\nEnter Number of a new Order:\n";

		vReorder.clear();
		Reorder(vFilename, vReorder);

		vFilename.clear();
		vPreviouslyUsedFileNames.clear();
		vFilename = vPreviouslyUsedFileNames = vReorder;

		cout << "\nNew Order Filename is: ";
		SetColor(Color, 10);
		cout << "\nReorder successfully.";
		SetColor(Color, 15);
		PrintFileNames(vFilename);


		cout << "\nDo you want to Reorder Again? Y/N? ";
		Answer = ReadYesOrNoCharacter();

		if (toupper(Answer) == 'Y')
		{
			system("cls");

			PrintReorderScreen();

			SetColor(Color, 12);
			cout << "\nPrevious Order Filenames:\n";
			SetColor(Color, 15);

			PrintFileNames(vFilename);
		}

	} while (toupper(Answer) == 'Y');

}

void PrintReorderScreen()
{

	SetColor(Color, 6);
	cout << "\n";
	cout << "\t\t\t\t +----------------------------------------------------+\n";
	cout << "\t\t\t\t |                                                    |\n";
	cout << "\t\t\t\t |                  Reorder Screen                    |\n";
	cout << "\t\t\t\t |                                                    |\n";
	cout << "\t\t\t\t +----------------------------------------------------+\n";
	SetColor(Color, 15);

}

void ReorderFilenamesScreen(vector <string>& vFileName)
{

	PrintReorderScreen();

	SetColor(Color, 12);
	cout << "\nPrevious Order Filenames:\n";
	SetColor(Color, 15);

	PrintFileNames(vFileName);

	ReorderFilenames(vFileName);

}

char PreviouslyUsedFileNames(vector <string>& vFileName)
{
	if (vPreviouslyUsedFileNames.empty())
		return 'T'; // Yes, Not Previously Used

	char Answer = 'Y';
	cout << "\nDo you want to recovery previoise used Filename? Y/N/(S)ForSkip? ";

	Answer = ReadYesOrNoOrSkipCharacter();

	if (Answer == 's' || Answer == 'S')
	{
		vFileName = vPreviouslyUsedFileNames;
		return 'F';
	}

	if (Answer == 'y' || Answer == 'Y')
	{
		vFileName = vPreviouslyUsedFileNames;

		SetColor(Color, 10);
		cout << "\nRecovered:";
		SetColor(Color, 15);

		PrintFileNames(vFileName);

		cout << "\nDo you want to Reorder Filenames? Y/N? ";
		Answer = ReadYesOrNoCharacter();
		if (Answer == 'y' || Answer == 'Y')
		{
			system("cls");
			ReorderFilenamesScreen(vFileName);
			PressToContinue();
			PrintHowToUse();

			SetColor(Color, 10);
			cout << "\nNew Order is: ";
			SetColor(Color, 15);

			PrintFileNames(vFileName);
		}

		cout << "\nDo you want to add new Filenames to the System? Y/N? ";
		Answer = ReadYesOrNoCharacter();
		return (Answer == 'y' || Answer == 'Y') ? 'T' : 'F';

		// if true will then read new Filename.
		// if false will then not read new FileName.

	}
	vPreviouslyUsedFileNames.clear();
	return 'T'; // here if Answer is Not Yes, will not do what above.

}

void ReadFile(string& FileName, vector <string>& vFileNames)
{
	char Answer = 'Y';

	do
	{
		FileName = ReadFileNameExistsInProject();
		MakeSureNonExistsFileNameInStringVector(FileName, vFileNames);

		vFileNames.push_back(FileName);
		vPreviouslyUsedFileNames.push_back(FileName);

		SetColor(Color, 10);
		cout << "Added successfully.";
		SetColor(Color, 15);

		cout << "\nDo you want to add more? Y/N? ";
		Answer = ReadYesOrNoCharacter();

		if (Answer == 'n' || Answer == 'N')
		{
			Answer = MakeSureHavingMoreOneFileName(vFileNames);
		}

	} while (Answer == 'y' || Answer == 'Y');

}

vector <string> ReadFileNames()
{
	vector <string> vFileNames;
	string FileName;
	char Answer = 'Y';

	if (PreviouslyUsedFileNames(vFileNames) == 'T')
		ReadFile(FileName, vFileNames);

	return vFileNames;

}

stAnimationInfo ReadAnimationInfo()
{
	stAnimationInfo AnimationInfo;

	AnimationInfo.vFileNames = ReadFileNames();
	AnimationInfo.FramesRepeted = ReadHowManyFramesRepeted();
	AnimationInfo.Delay = ReadDelay();

	return AnimationInfo;

}

void PrintHowToUse()
{
	// [WARNING] Do Not tach this foramt.

	system("cls");
	SetColor(Color, 9);
	cout << "\n  _    _	        _______      _    _";
	cout << "\n | |  | |              |__   __|    | |  | |";
	cout << "\n | |__| | _____      __   | | ___   | |  | |___  ___";
	cout << "\n |  __  |/ _ \\ \\ / \\/ /   | |/ _ \\  | |  | / __|/ _ \\";
	cout << "\n | |  | | (_) \\ V  V /    | | (_) | | |__| \\__ \\  __/";
	cout << "\n |_|  |_|\\___/ \\_/\\_/     |_|\\___/   \\____/|___/\\___|";

	SetColor(Color, 15);

	cout << "\n                                                         ";
	cout << "\n[1] Make two txt Files or more at the same Project File.";
	cout << "\n[2] Add your ASCII Art in a Files.";
	cout << "\n[3] Then you must add same File Names into System.";
	cout << "\n                                                         ";
}

void StartProgram()
{
	char Answer = 'Y';
	stAnimationInfo AnimationInfo;

	do
	{
		system("color 07");
		PrintHowToUse();
		AnimationInfo = ReadAnimationInfo();
		TurnAnmiation(AnimationInfo);

		PressToContinue();
		cout << "\nDo you want to play Again? Y/N? ";
		Answer = ReadYesOrNoCharacter();

	} while (Answer == 'y' || Answer == 'Y');

}

void WelcomeScreen()
{
	SetColor(Color, 6);
	cout << "\n\n\n\n\n\n\n\n";
	cout << "\t\t\t +---------------------------------------------------------------+\n";
	cout << "\t\t\t |                                                               |\n";
	cout << "\t\t\t |             W E L C O M E   T O   T H E   A P P               |\n";
	cout << "\t\t\t |                                                               |\n";
	cout << "\t\t\t |           ASCII Animation - Command Line Edition              |\n";
	cout << "\t\t\t |                                                               |\n";
	cout << "\t\t\t +---------------------------------------------------------------+\n";


	SetColor(Color, 6);
	cout << "\t\t\t Press any key to continue...";
	system("pause>0");
	SetColor(Color, 15);
}

int main()
{
	WelcomeScreen();
	StartProgram();

	return 0;
}