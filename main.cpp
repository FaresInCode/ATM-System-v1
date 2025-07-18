#pragma warning(disable : 4996)
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iomanip>
#include <ctime>
#include <cstdlib>
using namespace std; 


//============================Programe Varibales==============================================================================================
struct stSecurityAnswers 
{
	string Answer1 = "";
	string Answer2 = "";
	string Answer3 = "";
};
struct stAddress
{
	string Country;
	string City;
	string Village;
	string StreetAddress;

};
struct stBanks
{
	stAddress BankAddress;
	string BankName;
	string ClientIBAN;
};
struct sClient
{ 
	stBanks BankData;
	stAddress ClientAddress;
	stSecurityAnswers SecurityAnswers;
	string AccountNumber = "";
	string PinCode = "";
	string FirstName = "";
	string LastName = "";
	string Phone = "";
	short Permissions=0;
	double AccountBalance=0;
	bool MarkForDelete = false;
	string IsUnLocked = "1";
}; 
enum enMainMenueOptions
{
	eQucikWithdraw = 1,
	eNormalWithDraw = 2,
	eDeposit = 3,
	eCheckBalance = 4,
	eMoneyTransfer = 5,
	eClientsLists = 6,
	eUpdateClientPinCode = 7,
	eLockAccount = 8,
	eExit = 9

};
enum enAdminMenueOptions 
{
	enShowAllClientInfo=1,
	enFindClient=2,
	enAddNewClients=3,
	enUpdateClientPinCode=4,
	enUpdateClientInfo=5,
	enDeleteClient=6,
	enLockClient=7,	
	enUnlockClient=8,
	enManagePermition=9,
	enExit=10
};
enum enMainMenuePermissions { 
	pAll = -1, 
	pQucikWithdraw = 1, 
	pNormalWithDraw = 2, 
	pDeposit = 4, 
	pCheckBalance = 8, 
	pMoneyTransfer = 16, 
	pClientsLists = 32, 
	pUpdateClientPinCode = 64, 
	pLockAccount=128
};

//======================Global Variable=========================================
const string ClientsFileName = "Clients.txt";
const string SecurityQuestion1 = "What is your mother's first name?";
const string SecurityQuestion2 = "In what city were you born?";
const string SecurityQuestion3 = "What is the name of your favorite teacher?";
sClient CurrentClient;
string AdminAccountNumber = "F1234";
string AdminPinCode = "1234";
//======================Declaration Functions=================================================================================================

void ShowMainMenue(); 
void GoBackToMainMenue();
void Login();
void ShowQuickWithdrawScreen(); 
void ShowNormalWithDrawScreen();
void ShowDepositScreen();
void PerfromAdminPanelMenueOptions(enAdminMenueOptions);
void GoBackToAdminPanelMainMenue();
void ShowAccessDeniedMessage();
bool CheckAccessPermission(enMainMenuePermissions);
void ManagePermissionScreen();

//==========================Definition Functions================================================================================================

vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString; 
	short pos = 0;
	string sWord = "";
	// define a string variable  // use find() function to get the position of the delimiters  
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos); // store the word  
		if (sWord != "")
		{
			vString.push_back(sWord);
		} S1.erase(0, pos + Delim.length());  // erase() until positon and move to next word.
	}
	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string.    
	}
	return vString;
}
string ConvertClientRecordToLine(sClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.IsUnLocked + Seperator;
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.FirstName + Seperator;
	stClientRecord += Client.LastName + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance)+Seperator;
	stClientRecord += Client.ClientAddress.Country + Seperator;
	stClientRecord += Client.ClientAddress.City + Seperator;
	stClientRecord += Client.ClientAddress.Village + Seperator;
	stClientRecord += Client.ClientAddress.StreetAddress + Seperator;
	stClientRecord += Client.BankData.BankName + Seperator;
	stClientRecord += Client.BankData.ClientIBAN + Seperator;
	stClientRecord += Client.BankData.BankAddress.Country+Seperator;
	stClientRecord += Client.BankData.BankAddress.City+Seperator;
	stClientRecord += Client.BankData.BankAddress.Village+Seperator;
	stClientRecord += Client.BankData.BankAddress.StreetAddress + Seperator;
	stClientRecord += Client.SecurityAnswers.Answer1 + Seperator;
	stClientRecord += Client.SecurityAnswers.Answer2 + Seperator;
	stClientRecord += Client.SecurityAnswers.Answer3 + Seperator;
	stClientRecord += to_string(Client.Permissions);

	return stClientRecord;
}
vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite 
	string DataLine;
	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.      
				DataLine = ConvertClientRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}
sClient ConvertClientLinetoRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string> vClientData;
	vClientData = SplitString(Line, Seperator);
	Client.IsUnLocked = vClientData[0];
	Client.AccountNumber = vClientData[1];
	Client.PinCode = vClientData[2];
	Client.FirstName = vClientData[3];
	Client.LastName = vClientData[4];
	Client.Phone = vClientData[5];
	Client.AccountBalance = stod(vClientData[6]);//cast string to double
	Client.ClientAddress.Country = vClientData[7];
	Client.ClientAddress.City = vClientData[8];
	Client.ClientAddress.Village = vClientData[9];
	Client.ClientAddress.StreetAddress = vClientData[10];
	Client.BankData.BankName = vClientData[11];
	Client.BankData.ClientIBAN = vClientData[12];
	Client.BankData.BankAddress.Country = vClientData[13];
	Client.BankData.BankAddress.City = vClientData[14];
	Client.BankData.BankAddress.Village = vClientData[15];
	Client.BankData.BankAddress.StreetAddress = vClientData[16];
	Client.SecurityAnswers.Answer1 = vClientData[17];
	Client.SecurityAnswers.Answer2 = vClientData[18];
	Client.SecurityAnswers.Answer3 = vClientData[19];
	Client.Permissions = stoi(vClientData[20]);

	return Client;
}
vector <sClient> LoadCleintsDataFromFile(string FileName)
{
	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line = "";
		sClient Client;
		while (getline(MyFile, Line))  
		{
			Client = ConvertClientLinetoRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}

	return vClients;
}

//==========================================================================
bool IsNumberFrom0To9(char OneCharacter)
{
	for (short i = 48; i <=57; i++)
	{
		if (OneCharacter == char(i))
			return true;
	}
	return false;
}
bool IsStringNumbers(string MyString)
{
	for (short i = 0; i < MyString.length();i++)
	{
		if (!IsNumberFrom0To9(MyString[i]))
			return false;
	}
	return true;
}
string ReadPinCode()
{
	string NewPinCode;
	do
	{
		cout << "\nEnter You New PinCode (it should be has 4 number) : ";
		getline(cin >> ws, NewPinCode);
	} while (NewPinCode.length() < 4 || NewPinCode.length() > 4 || !IsStringNumbers(NewPinCode));
	return NewPinCode;
}
void UpdateClientPinCode()
{
	string NewPinCode= ReadPinCode();
	vector<sClient>vClients;
	vClients = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient &Client : vClients)
	{
		if (Client.AccountNumber == CurrentClient.AccountNumber)
		{
			Client.PinCode= NewPinCode;
			cout << "\nPinCode Updated Succesfully\n";
			SaveCleintsDataToFile(ClientsFileName,vClients);
			return;
		}
	}

}
void ShowUpdatePinCodeScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClientPinCode))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	cout << "===========================================\n";
	cout << "\tUpdate PinCode screen\n";
	cout << "===========================================\n";
	UpdateClientPinCode();

}
//==========================================================================

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Client)
{
	vector<sClient>vClient = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient& C : vClient)
	{
		if ((C.AccountNumber == AccountNumber) && (C.PinCode == PinCode))
		{
			Client = C;
			return true;
		}
	}
	return false;

}
char IsAnswerIsYes(string Message)
{
	char Choice=' ';
	cout <<'\n' << Message << " ";
	cin >> Choice;
	return (Choice == 'Y' || Choice == 'y');
}
bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)

{
	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
bool FindClientByAccountNumberAndIbanNumber(string AccountNumber,string IBANNumber, vector<sClient> vClients, sClient& Client)

{
	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber && C.BankData.ClientIBAN==IBANNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{

	if (IsAnswerIsYes("\n\nAre you sure you want perfrom this transaction? y/n ? "))
	{
		for (sClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveCleintsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
				return true;
			}
		}
	}
	return false;
}

//=========================================================================
short ReadQuickWithdrawOption() 
{ 
	short Choice = 0;
	while (Choice < 1 || Choice>9) 
	{ 
		cout << "\nChoose what to do from [1] to [9] ? ";        
		cin >> Choice; 
	} 
	return Choice;
}
short getQuickWithDrawAmount (short QuickWithDrawOption) 
{
	 switch (QuickWithDrawOption) 
	 {
		 case 1: 
			 return  20;
		 case 2: 
			 return 50; 
		 case 3:
			 return 100;
		 case 4:
			 return 200;
		 case 5:
			 return 400; 
		 case 6:
			 return 600;
		 case 7:
			 return 800;
		 case 8: 
			 return 1000;
		 default: 
			 return 0; 
	 } 
}
void PerfromQuickWithdrawOption(short QuickWithDrawOption) 
{
	if (QuickWithDrawOption == 9)//exit 
		return; 
	short WithDrawBalance = getQuickWithDrawAmount(QuickWithDrawOption); 
	if (WithDrawBalance > CurrentClient.AccountBalance) 
	{        
		cout << "\nThe amount exceeds your balance, make another choice.\n";  
		cout << "Press Anykey to continue...";  
		system("pause>0");
		ShowQuickWithdrawScreen();
		return;
	} 
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);  
	CurrentClient.AccountBalance -= WithDrawBalance;

} 
void ShowQuickWithdrawScreen()
{ 
	if (!CheckAccessPermission(enMainMenuePermissions::pQucikWithdraw))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	system("cls");    
	cout << "===========================================\n";   
	cout << "\t\tQucik Withdraw\n";
	cout << "===========================================\n";   
	cout << "\t[1] 20\t\t[2] 50\n";   
	cout << "\t[3] 100\t\t[4] 200\n"; 
	cout << "\t[5] 400\t\t[6] 600\n";    
	cout << "\t[7] 800\t\t[8] 1000\n";    
	cout << "\t[9] Exit\n";   
	cout << "===========================================\n";   
	cout << "Your Balance is " << CurrentClient.AccountBalance;  
	PerfromQuickWithdrawOption(ReadQuickWithdrawOption()); 
}
//==========================================================================

int ReadQuickWithDrawAmont() 
{
	int Amount;     
	cout << "\nEnter an amount multiple of 5's ? ";   
	cin >> Amount; 
	while (Amount % 5 != 0) 
	{
		cout << "\nEnter an amount multiple of 5's ? ";     
		cin >> Amount; 
	} 
	return Amount;
}
void PerfromNormalWithdrawOption()
{
	short WithDrawBalance = ReadQuickWithDrawAmont(); 
	if (WithDrawBalance > CurrentClient.AccountBalance) 
	{ 
		cout << "\nThe amount exceeds your balance, make another choice.\n";     
		cout << "Press Any key to continue...";        
		system("pause>0"); 
		ShowNormalWithDrawScreen();
		return; 
	} 
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);    
	CurrentClient.AccountBalance -= WithDrawBalance; 
}
void ShowNormalWithDrawScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pNormalWithDraw))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	system("cls");
	cout << "===========================================\n";  
	cout << "\t\tNormal Withdraw Screen\n";    
	cout << "===========================================\n";   
	PerfromNormalWithdrawOption(); 
}

//==========================================================================
void ShowCheckBalanceScreen() 
{
	if (!CheckAccessPermission(enMainMenuePermissions::pCheckBalance))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	system("cls"); 
	cout << "===========================================\n";  
	cout << "\t\tCheck Balance Screen\n";   
	cout << "===========================================\n";  
	cout << "Your Balance is " << CurrentClient.AccountBalance << "\n"; 
}
//==========================================================================
double ReadDepositAmount()
{
	double Amount; 
	do
	{	
		cout << "\nEnter a positive Deposit Amount? ";
		cin >> Amount; 

	} while (Amount <= 0);
	return Amount; 
}
void PerfromDepositOption() 
{
	double DepositAmount = ReadDepositAmount(); 
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients); 
	CurrentClient.AccountBalance += DepositAmount;
}
void ShowDepositScreen() 
{ 
	if (!CheckAccessPermission(enMainMenuePermissions::pDeposit))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	system("cls");  
	cout << "===========================================\n";   
	cout << "\t\tDeposit Screen\n";   
	cout << "===========================================\n";    
	PerfromDepositOption();
}
//==========================================================================
string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}
short ReadMainMenueOption(short From,short To) 
{ 	
	short Choice = 0;   
	do
	{
		cout << "\nChoose what do you want to do? From [" << From << " To "<<To<<"]? "; 
		cin >> Choice; 
	} while (Choice<From || Choice >To);
	return Choice;
} 
void CurrentTransactionTecket()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\tCurrent Transaction Details Screen\n";
	cout << "===========================================\n";

	time_t t = time(0); // to get time now
	tm* OurlocalTimenow = localtime(&t);
	cout << "\n----------------Client Details-------------------\n";
	cout << "\nIs Unlocked           : " << CurrentClient.IsUnLocked;
	cout << "\nAccount Number        : " << CurrentClient.AccountNumber;
	cout << "\nFirst Name            : " << CurrentClient.FirstName;
	cout << "\nLast Name             : " << CurrentClient.LastName;
	cout << "\nPhone                 : " << CurrentClient.Phone;
	cout << "\nAccount Balance       : " << CurrentClient.AccountBalance;
	cout << "\nClient country        : " << CurrentClient.ClientAddress.Country;
	cout << "\nClient City           : " << CurrentClient.ClientAddress.City;
	cout << "\nClient Village        : " << CurrentClient.ClientAddress.Village;
	cout << "\nClient Street Address : " << CurrentClient.ClientAddress.StreetAddress;
	cout << "\n\n-------------------Bank details----------------\n\n";
	cout << "\nBank Name             : " << CurrentClient.BankData.BankName;
	cout << "\nClient IBAN           : " << CurrentClient.BankData.ClientIBAN;
	cout << "\nBank Country          : " << CurrentClient.BankData.BankAddress.Country;
	cout << "\nBank City             : " << CurrentClient.BankData.BankAddress.City;
	cout << "\nBank Village          : " << CurrentClient.BankData.BankAddress.Village;
	cout << "\nBank Street Address   : " << CurrentClient.BankData.BankAddress.StreetAddress;
	cout << "\n-----------------------------------\n";
	cout << "\nDay : ";
	cout << OurlocalTimenow->tm_mday << "/"
		<< OurlocalTimenow->tm_mon + 1 << "/"
		<< OurlocalTimenow->tm_year + 1900 << " \n";
	cout << "\nThis Transaction Was Made At Time ";
	cout<< OurlocalTimenow->tm_hour << ":"
		<< OurlocalTimenow->tm_min<<":"
		<< OurlocalTimenow->tm_sec << endl;



}

//=============================================================
double ReadSendingAmount()
{
	double SendingAmout=0;
	cout << "\nEnter Sending Amount : ";
	cin >> SendingAmout;
	return SendingAmout;
}
string ReadIClientIBAN()
{
	string IbanNumber;
	cout << "\nEnter Account IBAN : ";
	cin >> IbanNumber;
	return IbanNumber;
}
string ReadBeneficiaryAccountNumber()
{
	string BeneficiaryAccountNumber;
	do
	{
		cout << "\nEnter Beneficiary Account Number : ";
		cin >> BeneficiaryAccountNumber;
	} while (BeneficiaryAccountNumber == CurrentClient.AccountNumber);
	return BeneficiaryAccountNumber;
}
//=============================================================
void TransferTicketDetails(sClient BeneficiaryClient)
{
	system("cls");
	cout << "===========================================\n";
	cout << "\tCurrent Transaction Details Screen\n";
	cout << "===========================================\n";

	time_t t = time(0); // to get time now
	tm* OurlocalTimenow = localtime(&t);
	cout << "\n===========================================\n";
	cout << "\nSender Details\n";
	cout << "\n===========================================\n";

	cout << "\nAccount Number        : " << CurrentClient.AccountNumber;
	cout << "\nFirst Name            : " << CurrentClient.FirstName ;
	cout << "\nLast Name             : " << CurrentClient.LastName;
	cout << "\nPhone                 : " << CurrentClient.Phone ;
	cout << "\nAccount Balance       : " << CurrentClient.AccountBalance;
	cout << "\nClient Country        : " << CurrentClient.ClientAddress.Country;
	cout << "\nClient City           : " << CurrentClient.ClientAddress.City;
	cout << "\nClient Village        : " << CurrentClient.ClientAddress.Village;
	cout << "\nClient street Address : " << CurrentClient.ClientAddress.StreetAddress;

	cout << "\n\n======================Bank Details=====================\n\n";
	cout << "\nBank Name           : " << CurrentClient.BankData.BankName;
	cout << "\nIBAN Number         : " << CurrentClient.BankData.ClientIBAN;
	cout << "\nBankCountry         : " << CurrentClient.BankData.BankAddress.Country;
	cout << "\nBank City           : " << CurrentClient.BankData.BankAddress.City;
	cout << "\nBank Village        : " << CurrentClient.BankData.BankAddress.Village;
	cout << "\nBank Street Address : " << CurrentClient.BankData.BankAddress.StreetAddress;

	cout << "\n===========================================\n";
	cout << "\tBeneficiary Details\n";
	cout << "\n===========================================\n";

	cout << "\nAccount Number        : " << CurrentClient.AccountNumber;
	cout << "\nFirst Name            : " << CurrentClient.FirstName;
	cout << "\nLast Name             : " << CurrentClient.LastName;
	cout << "\nPhone                 : " << CurrentClient.Phone;
	cout << "\nAccount Balance       : " << CurrentClient.AccountBalance;
	cout << "\nClient Country        : " << CurrentClient.ClientAddress.Country;
	cout << "\nClient City           : " << CurrentClient.ClientAddress.City;
	cout << "\nClient Village        : " << CurrentClient.ClientAddress.Village;
	cout << "\nClient street Address : " << CurrentClient.ClientAddress.StreetAddress;

	cout << "\n\n======================Bank Details=====================\n\n";
	cout << "\nBank Name           : " << CurrentClient.BankData.BankName;
	cout << "\nIBAN Number         : " << CurrentClient.BankData.ClientIBAN;
	cout << "\nBankCountry         : " << CurrentClient.BankData.BankAddress.Country;
	cout << "\nBank City           : " << CurrentClient.BankData.BankAddress.City;
	cout << "\nBank Village        : " << CurrentClient.BankData.BankAddress.Village;
	cout << "\nBank Street Address : " << CurrentClient.BankData.BankAddress.StreetAddress;

	cout << "\nDay : ";
	cout << OurlocalTimenow->tm_mday << "/"
		<< OurlocalTimenow->tm_mon + 1 << "/"
		<< OurlocalTimenow->tm_year + 1900 << " \n";
	cout << "\nThis Transaction Was Made At Time ";
	cout << OurlocalTimenow->tm_hour << ":"
		<< OurlocalTimenow->tm_min << ":"
		<< OurlocalTimenow->tm_sec << endl;


}
bool IsFromTheSameBank(sClient Client1,sClient Client2)
{
	vector<sClient>vClients = LoadCleintsDataFromFile(ClientsFileName);
	return (FindClientByAccountNumber(Client2.AccountNumber,vClients,Client2)?(Client1.BankData.BankName == Client2.BankData.BankName):false);
}

void UpdateCurrentClientAccountBalanceInFile(string AccountNumber)
{
	vector<sClient>vClients = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient& Client : vClients)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			Client.AccountBalance = CurrentClient.AccountBalance;
			vClients = SaveCleintsDataToFile(ClientsFileName, vClients);
			return;
		}
	}
}
void TransferMoneyToExtenalBankAccount(sClient &BeneficiaryClient)
{
	BeneficiaryClient.BankData.ClientIBAN = ReadIClientIBAN();
	double SendingAmount = ReadSendingAmount();
	if (CurrentClient.AccountBalance > SendingAmount)
	{
		vector<sClient>vClient = LoadCleintsDataFromFile(ClientsFileName);
		if (FindClientByAccountNumberAndIbanNumber(BeneficiaryClient.AccountNumber, BeneficiaryClient.BankData.ClientIBAN,vClient, BeneficiaryClient))
		{
			if (IsAnswerIsYes)
			{
				DepositBalanceToClientByAccountNumber(BeneficiaryClient.AccountNumber, SendingAmount, vClient);
				cout <<"\nAmount " << SendingAmount << " is Send Succesfuly To " << BeneficiaryClient.FirstName <<" "<<BeneficiaryClient.LastName << endl;
				CurrentClient.AccountBalance -= SendingAmount;
				cout << "\nYour Current Balance "<<CurrentClient.AccountBalance<<endl;
				UpdateCurrentClientAccountBalanceInFile(CurrentClient.AccountNumber);
			}
		}
		else
		{
			cout << "\nInvalid Account Number/OrIBAN" << endl;
			return;
		}

	}
	else
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		return;
	}


}
void TransferMoneyInSameBank(sClient &BeneficiaryClient)
{
	double SendingAmount = ReadSendingAmount();
	if (CurrentClient.AccountBalance > SendingAmount)
	{
		vector<sClient>vClient = LoadCleintsDataFromFile(ClientsFileName);
		if (FindClientByAccountNumber(BeneficiaryClient.AccountNumber, vClient, BeneficiaryClient))
		{
			if (IsAnswerIsYes)
			{
				DepositBalanceToClientByAccountNumber(BeneficiaryClient.AccountNumber, SendingAmount, vClient);
				cout << "\nAmount " << SendingAmount << " is Send Succesfuly To " << BeneficiaryClient.FirstName<<" "<<BeneficiaryClient.LastName << endl;
				CurrentClient.AccountBalance -= SendingAmount;
				cout << "\nYour Current Balance " << CurrentClient.AccountBalance << endl;
				UpdateCurrentClientAccountBalanceInFile(CurrentClient.AccountNumber);
			}
		}
		else
		{
			cout << "\nInvalid Account Number" << endl;
		}
	
	}
	else
	{
		cout << "\nThe amount exceeds your balance\n";
	}


	


}
void TransferMoneyBetwinAccounts()
{
	sClient BeneficiaryClient;
	BeneficiaryClient.AccountNumber= ReadBeneficiaryAccountNumber();
	if (IsFromTheSameBank(CurrentClient, BeneficiaryClient))
	{
		TransferMoneyInSameBank(BeneficiaryClient);
	}
	else
	{
		TransferMoneyToExtenalBankAccount(BeneficiaryClient);
	}
	TransferTicketDetails(BeneficiaryClient);

}
void ShowTransferMoneyBetwinAccountsScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pMoneyTransfer))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tDeposit Screen\n";
	cout << "===========================================\n";
	TransferMoneyBetwinAccounts();
}

//=============================================================
void ShowSpecifiqueDetailsAboutAllClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pClientsLists))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "First Name";
	cout << "| " << left << setw(40) << "Last Name";
	cout << "| " << left << setw(40) << "IBAN Number";

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In the System!";
	}
	else
	{
		for (sClient& Client : vClients)
		{

			cout << "| " << setw(15) << left << Client.AccountNumber;
			cout << "| " << setw(40) << left << Client.FirstName;
			cout << "| " << setw(40) << left << Client.LastName;
			cout << "| " << setw(40) << left << Client.BankData.ClientIBAN;

			cout << endl;
		}

	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

}
//=============================================================
sClient ReadNewClientInfo()
{

	sClient Client;
	cout << "\nEnter account Number: ";
	getline(cin >> ws, Client.AccountNumber);
	Client.PinCode=ReadPinCode();
	cout << "\nEnter First name: ";
	getline(cin>>ws, Client.FirstName);
	cout << "\nEnter Last name: ";
	getline(cin>>ws, Client.LastName);
	cout << "\nEnter Phone Number: ";
	getline(cin>>ws, Client.Phone);
	cout << "\nEnter Account Balance: ";
	cin>>Client.AccountBalance;
	cout << "\nEnter your country: ";
	getline(cin>>ws,Client.ClientAddress.Country);
	cout << "\nEnter your city: ";
	getline(cin>>ws, Client.ClientAddress.City);
	cout << "\nEnter your village: ";
	getline(cin>>ws, Client.ClientAddress.Village);
	cout << "\nEnter your street address: ";
	getline(cin>>ws, Client.ClientAddress.StreetAddress);
	cout << "\nEnter Bank Name: ";
	getline(cin>>ws, Client.BankData.BankName);
	cout << "\nEnter Client IBAN: ";
	getline(cin>>ws, Client.BankData.ClientIBAN);
	Client.IsUnLocked = "1";
	cout << "\nEnter Country: ";
	getline(cin>>ws, Client.BankData.BankAddress.Country);
	cout << "\nEnter City: ";
	getline(cin>>ws, Client.BankData.BankAddress.City);
	cout << "\nEnter Village: ";
	getline(cin>>ws, Client.BankData.BankAddress.Village);
	cout << "\nEnter Street Address: ";
	getline(cin>>ws, Client.BankData.BankAddress.StreetAddress);
	cout << SecurityQuestion1 << ": ";
	getline(cin>>ws, Client.SecurityAnswers.Answer1);
	cout << SecurityQuestion2 << ": ";
	getline(cin>>ws, Client.SecurityAnswers.Answer2);
	cout << SecurityQuestion3 << ": ";
	getline(cin>>ws, Client.SecurityAnswers.Answer3);
	return Client;

}
void SignUp()
{
	sClient Client = ReadNewClientInfo();
	vector<sClient>vClients=LoadCleintsDataFromFile(ClientsFileName);
	vClients.push_back(Client);
	SaveCleintsDataToFile(ClientsFileName, vClients);
}
void AddNewClients()
{
	char Choice;
	do
	{
		SignUp();
		cout << "\nYour Welcom in our bank you now sign in to explore our fetures\n";
		cout <<endl<< "You want to add more Clients [y/n] ";
		cin >> Choice;
	} while (Choice =='Y'|| Choice =='y');
}
void AddNewClientsScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tATM Add New Clients Screen\n";
	cout << "===========================================\n";
	AddNewClients();

}
//==========================================================================

void PrintClientRecordLine(sClient Client)
{

	cout << "\n| " << left << setw(40) << "Is UnLocked Account";
	cout << "| " << setw(40) << left << Client.IsUnLocked;

	cout << "\n| " << left << setw(40) << "Account Number";
	cout << "| " << setw(40) << left << Client.AccountNumber;

	cout << "\n| " << left << setw(40) << "Pin Code";
	cout << "| " << setw(40) << left << Client.PinCode;

	cout << "\n| " << left << setw(40) << "First Name";
	cout << "| " << setw(40) << left << Client.FirstName;

	cout << "\n| " << left << setw(40) << "Last Name";
	cout << "| " << setw(40) << left << Client.LastName;

	cout << "\n| " << left << setw(40) << "Phone";
	cout << "| " << setw(40) << left << Client.Phone;

	cout << "\n| " << left << setw(40) << "Account Balance";
	cout << "| " << setw(40) << left << Client.AccountBalance;

	cout << "\n| " << left << setw(40) << "Country";
	cout << "| " << setw(40) << left << Client.ClientAddress.Country;

	cout << "\n| " << left << setw(40) << "City";
	cout << "| " << setw(40) << left << Client.ClientAddress.City;

	cout << "\n| " << left << setw(40) << "Village";
	cout << "| " << setw(40) << left << Client.ClientAddress.Village;

	cout << "\n| " << left << setw(40) << "Street Address";
	cout << "| " << setw(40) << left << Client.ClientAddress.StreetAddress;

	cout << "\n| " << left << setw(40) << "Bank Name";
	cout << "| " << setw(40) << left << Client.BankData.BankName;

	cout << "\n| " << left << setw(40) << "Client IBAN";
	cout << "| " << setw(40) << left << Client.BankData.ClientIBAN;

	cout << "\n| " << left << setw(40) << "Bank Country";
	cout << "| " << setw(40) << left << Client.BankData.BankAddress.Country;

	cout << "\n| " << left << setw(40) << "Bank City";
	cout << "| " << setw(40) << left << Client.BankData.BankAddress.City;

	cout << "\n| " << left << setw(40) << "Bank Village";
	cout << "| " << setw(40) << left << Client.BankData.BankAddress.Village;

	cout << "\n| " << left << setw(40) << "Bank Street Address";
	cout << "| " << setw(40) << left << Client.BankData.BankAddress.StreetAddress;

	cout << "\n| " << left << setw(40) << "Security Answer1";
	cout << "| " << setw(40) << left << Client.SecurityAnswers.Answer1;

	cout << "\n| " << left << setw(40) << "Security Answer2";
	cout << "| " << setw(40) << left << Client.SecurityAnswers.Answer2;

	cout << "\n| " << left << setw(40) << "Security Answer3";
	cout << "| " << setw(40) << left << Client.SecurityAnswers.Answer3;

	cout << "\n| " << left << setw(40) << "Permission binary Number";
	cout << "| " << setw(40) << left << Client.Permissions;
}
void PrintClientCard(sClient Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "----------------------------------------------------------";
	cout << "\nIs Unlocked                : " << Client.IsUnLocked;
	cout << "\nAccount Number             : " << Client.AccountNumber;
	cout << "\nPin Code                   : " << Client.PinCode;
	cout << "\nFirst Name                 : " << Client.FirstName;
	cout << "\nLast Name                  : " << Client.LastName;
	cout << "\nPhone                      : " << Client.Phone;
	cout << "\nAccount Balance            : " << Client.AccountBalance;
	cout << "\nClient country             : " << Client.ClientAddress.Country;
	cout << "\nClient City                : " << Client.ClientAddress.City;
	cout << "\nClient Village             : " << Client.ClientAddress.Village;
	cout << "\nClient Street Address      : " << Client.ClientAddress.StreetAddress;
	cout << "\nBank Name                  : " << Client.BankData.BankName;
	cout << "\nClient IBAN                : " << Client.BankData.ClientIBAN;
	cout << "\nBank Country               : " << Client.BankData.BankAddress.Country;
	cout << "\nBank City                  : " << Client.BankData.BankAddress.City;
	cout << "\nBank Village               : " << Client.BankData.BankAddress.Village;
	cout << "\nBank Street Address        : " << Client.BankData.BankAddress.StreetAddress;
	cout << "\nSecurity Answer1           : " << Client.SecurityAnswers.Answer1;
	cout << "\nSecurity Answer2           : " << Client.SecurityAnswers.Answer2;
	cout << "\nSecurity Answer3           : " << Client.SecurityAnswers.Answer3;
	cout << "\nBinary Permission Number   : " << Client.Permissions;

	cout << "----------------------------------------------------------";
}
void ShowAllClientsScreen()
{
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	cout << "\n________________________________________________________________________________________________\n" << endl;
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n________________________________________________________________________________________________\n" << endl;
	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In the System!";
	}
	else
	{
		for (sClient& Client : vClients)
		{
			cout << "\n___________________________ "<<Client.FirstName<<" "<<Client.LastName<<" Account Bank Details _________________________\n";
			PrintClientRecordLine(Client);
			cout << "\n________________________________________________________________________________________________\n" << endl;
			cout << endl;
		}	
	}


}

//==========================================================================
void ShowFindClientScreen()
{

	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	sClient Client;
	string AccountNumber = ReadClientAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		PrintClientCard(Client);
	else
		cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}
//==========================================================================

sClient ChangeClientRecord(string AccountNumber)
{
	sClient Client;

	Client.AccountNumber = AccountNumber;

	cout << ReadPinCode();
	cout << "\nEnter First name: ";
	getline(cin, Client.FirstName);
	cout << "\nEnter Last name: ";
	getline(cin, Client.LastName);
	cout << "\nEnter Phone Number: ";
	getline(cin, Client.Phone);
	cout << "\nEnter Account Balance: ";
	cin >> Client.AccountBalance;
	cout << "\nEnter your country: ";
	cin >> Client.ClientAddress.Country;
	cout << "\nEnter your city: ";
	cin >> Client.ClientAddress.City;
	cout << "\nEnter your village: ";
	cin >> Client.ClientAddress.Village;
	cout << "\nEnter your street address: ";
	cin >> Client.ClientAddress.StreetAddress;
	cout << "\nEnter Bank Name: ";
	getline(cin, Client.BankData.BankName);
	cout << "\nEnter Client IBAN: ";
	getline(cin, Client.BankData.ClientIBAN);
	Client.IsUnLocked = "1";
	cout << "\nEnter Country: ";
	getline(cin, Client.BankData.BankAddress.Country);
	cout << "\nEnter City: ";
	getline(cin, Client.BankData.BankAddress.City);
	cout << "\nEnter Village: ";
	getline(cin, Client.BankData.BankAddress.Village);
	cout << "\nEnter Street Address: ";
	getline(cin, Client.BankData.BankAddress.StreetAddress);
	cout << SecurityQuestion1 << ": ";
	getline(cin, Client.SecurityAnswers.Answer1);
	cout << SecurityQuestion2 << ": ";
	getline(cin, Client.SecurityAnswers.Answer2);
	cout << SecurityQuestion3 << ": ";
	getline(cin, Client.SecurityAnswers.Answer3);

	return Client;

}
bool IsClientExistsByAccountNumber(string AccountNumber, string FileName)
{

	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode

	if (MyFile.is_open())
	{

		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{

			Client = ConvertClientLinetoRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}

		}

		MyFile.close();

	}

	return false;


}
bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{

		PrintClientCard(Client);
		cout << "\n\nAre you sure you want update this client? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{

			for (sClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}

			}

			SaveCleintsDataToFile(ClientsFileName, vClients);

			cout << "\n\nClient Updated Successfully.";
			return true;
		}

	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}

}
void ShowUpdateClientScreen()
{

	cout << "\n-----------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n-----------------------------------\n";

	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

//==========================================================================
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

	for (sClient& C : vClients)
	{

		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}

	}

	return false;

}
bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	sClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		if (IsAnswerIsYes("\n\nAre you sure you want delete this client? y/n ? "))
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveCleintsDataToFile(ClientsFileName, vClients);
			//Refresh Clients
			vClients = LoadCleintsDataFromFile(ClientsFileName);
			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}

}
void ShowDeleteClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n-----------------------------------\n";
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}
//==========================================================================

void UnlockClientManual()
{
	if (IsAnswerIsYes("\nYou sure you want to unlock this account? y/n "))
	{		
		string AccountNumber = ReadClientAccountNumber();
		vector<sClient>vClients = LoadCleintsDataFromFile(ClientsFileName);
		for (sClient& Client : vClients)
		{
			if (Client.AccountNumber == AccountNumber)
			{
				Client.IsUnLocked = "1";
				SaveCleintsDataToFile(ClientsFileName, vClients);
				cout << "\n Client " << AccountNumber << " Is Locked Succesfully " << endl;
				return;
			}
		}

	}
}
void LockClientManual()
{
	
	string AccountNumber = ReadClientAccountNumber();
	vector<sClient>vClients = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient& Client : vClients)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			Client.IsUnLocked = "0";
			SaveCleintsDataToFile(ClientsFileName, vClients);
			cout << "\nAccount with Account Number "<<AccountNumber<<" is locked Succesfully \n";
			return;
		}
	}
	return;

}

//==========================================================================
void PerfromAdminPanelMenueOptions(enAdminMenueOptions AdminMenueOptions)
{

	switch (AdminMenueOptions)
	{

	case enAdminMenueOptions::enShowAllClientInfo:
		system("cls");
		ShowAllClientsScreen();
		GoBackToAdminPanelMainMenue();
		break;
	case enAdminMenueOptions::enFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToAdminPanelMainMenue();
		break;

	case enAdminMenueOptions::enAddNewClients:
		system("cls");
		AddNewClientsScreen();
		GoBackToAdminPanelMainMenue();
		break;

	case enAdminMenueOptions::enUpdateClientInfo:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToAdminPanelMainMenue();
		break;
	case enAdminMenueOptions::enUpdateClientPinCode:
		system("cls");
		ShowUpdatePinCodeScreen();
		GoBackToAdminPanelMainMenue();
		break;

	case enAdminMenueOptions::enDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToAdminPanelMainMenue();
		break;

	case enAdminMenueOptions::enLockClient:
		system("cls");
		LockClientManual();
		GoBackToAdminPanelMainMenue();
		break;

	case enAdminMenueOptions::enUnlockClient:
		system("cls");
		UnlockClientManual();
		GoBackToAdminPanelMainMenue();
		break;

	case enAdminMenueOptions::enExit:
		return;
		break;

	}



}
void ShowATMAdminPanelMenueScreen()
{
	system("cls");
	cout << "\n===========================================\n";
	cout << "\t\tATM Admin Panel Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show All Clients.\n";
	cout << "\t[2] Show Client Details.\n";
	cout << "\t[3] Add New Clients.\n";
	cout << "\t[4] Update Client PinCode\n";
	cout << "\t[5] Update Client Info.\n";
	cout << "\t[6] Delete Client\n";
	cout << "\t[7] Lock Client.\n";
	cout << "\t[8] Unlock Client.\n";
	cout << "\t[9] ExiT.\n";
	cout << "===========================================\n";

	PerfromAdminPanelMenueOptions((enAdminMenueOptions)ReadMainMenueOption(1,9));


}
void GoBackToAdminPanelMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowATMAdminPanelMenueScreen();
}
//==========================================================================

bool IsLockAccount(string AccountNumber)
{
	vector<sClient>vClients = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient& Client : vClients)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			return Client.IsUnLocked == "0";
		}
	}
	return false;
}
void LockAccount()
{
	vector<sClient>vClients = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient& Client : vClients)
	{
		if (Client.AccountNumber == CurrentClient.AccountNumber)
		{
			Client.IsUnLocked = "0";
			SaveCleintsDataToFile(ClientsFileName, vClients);
			cout << "\nyour account is locked (call the bank for help)\n";
			return;
		}
	}
	cout << CurrentClient.AccountNumber << "\nIs not exist\n";
	return;
}
void UnlockClientAccount()
{
	vector<sClient>vClients = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient& Client : vClients)
	{
		if (Client.AccountNumber == CurrentClient.AccountNumber)
		{
			Client.IsUnLocked = "1";
			SaveCleintsDataToFile(ClientsFileName, vClients);
			return;
		}
	}
}
void LockATMAccountScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pLockAccount))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tLock Account Screen\n";
	cout << "===========================================\n";
	if (IsAnswerIsYes("\nYou sure you want to lock this account? [y/n] \n"))
		LockAccount();
}

//==========================================================================
void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenue();
}
void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
	switch (MainMenueOption)
	{

	case enMainMenueOptions::eQucikWithdraw:
		system("cls");
		ShowQuickWithdrawScreen();
		CurrentTransactionTecket();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::eNormalWithDraw:
		system("cls");
		ShowNormalWithDrawScreen();
		CurrentTransactionTecket();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::eDeposit:
		system("cls");
		ShowDepositScreen();
		CurrentTransactionTecket();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::eCheckBalance:
		system("cls");
		ShowCheckBalanceScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eMoneyTransfer:
		system("cls");
		ShowTransferMoneyBetwinAccountsScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eClientsLists:
		system("cls");
		ShowSpecifiqueDetailsAboutAllClientsScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eUpdateClientPinCode:
		system("cls");
		ShowUpdatePinCodeScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eLockAccount:
		system("cls");
		LockATMAccountScreen();
		break;
	case enMainMenueOptions::eExit:
		system("cls");
		Login();
		break;
	}
}
void ShowMainMenue()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tATM Main Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Transfer.\n";
	cout << "\t[6] Show Specific Details. About All Clients \n";
	cout << "\t[7] Update PinCode.\n";
	cout << "\t[8] Lock Account\n";
	cout << "\t[9] Logout.\n";
	cout << "===========================================\n";

	PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption(1, 9));
}
//==========================================================================

void ShowAccessDeniedMessage()
{
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";
}
int ReadPermissionsToSet()
{
	int Permissions = 0;
	char Answer = 'n';

	if (IsAnswerIsYes("\nDo you want to give full access? y/n? "))
		return -1;

	cout << "\nDo you want to give access to : \n";

	if (IsAnswerIsYes("\n Quick Withdraw? y/n "))
		Permissions += enMainMenuePermissions::pQucikWithdraw;

	if (IsAnswerIsYes("\n Normal Withdraw? y/n "))
		Permissions += enMainMenuePermissions::pNormalWithDraw;

	if (IsAnswerIsYes("\n Deposit? y/n "))
		Permissions += enMainMenuePermissions::pDeposit;

	if (IsAnswerIsYes("\n Check balance? y/n "))
		Permissions += enMainMenuePermissions::pCheckBalance;

	if (IsAnswerIsYes("\n Money Transfer? y/n "))
		Permissions += enMainMenuePermissions::pMoneyTransfer;

	if (IsAnswerIsYes("\n Clients List? y/n "))
		Permissions += enMainMenuePermissions::pClientsLists;

	if (IsAnswerIsYes("\n Update PinCode? y/n "))
		Permissions += enMainMenuePermissions::pUpdateClientPinCode;
	if (IsAnswerIsYes("\n Lock Account? y/n "))
		Permissions += enMainMenuePermissions::pLockAccount;

	return Permissions;
}
bool CheckAccessPermission(enMainMenuePermissions Permission)
{
	if (CurrentClient.Permissions == enMainMenuePermissions::pAll)
		return true;
	if ((Permission & CurrentClient.Permissions) == Permission)
		return true;
	else
		return false;
}
void ManagePermissionScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tATM Main Menue Screen\n";
	cout << "===========================================\n";
	sClient ClientToManageHisPermissions;
	ClientToManageHisPermissions.AccountNumber = ReadClientAccountNumber();
	ClientToManageHisPermissions.Permissions = ReadPermissionsToSet();
	vector<sClient>vClients = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient& Client : vClients)
	{

		if (Client.AccountNumber == ClientToManageHisPermissions.AccountNumber)
		{
			Client.Permissions = ClientToManageHisPermissions.Permissions;
			SaveCleintsDataToFile(ClientsFileName, vClients);
			return;
		}
	}



}

//==========================================================================
bool IsCorrectSecurityAnswer()
{

	string Answer1 = "", Answer2 = "", Answer3 = "";
	cout << "\n" << SecurityQuestion1;
	getline(cin >> ws, Answer1);
	cout << SecurityQuestion2;
	getline(cin, Answer2);
	cout << SecurityQuestion3;
	getline(cin, Answer3);
	vector<sClient>vClient = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient& Client : vClient)
	{
		if ((Client.SecurityAnswers.Answer1 == Answer1) && (Client.SecurityAnswers.Answer2 == Answer2) &&(Client.SecurityAnswers.Answer3 == Answer3))
		{
			return true;
		}
	}
	return false;
}
bool  LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
		return true;
	else
		return false;
}
void Login()
{
	bool LoginFaild = false;

	short FailedLogInCounter = 0;
	do
	{
		system("cls");
		cout << "\n---------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n---------------------------------\n";
		if (LoginFaild)
		{
			FailedLogInCounter++;
			if (3 - FailedLogInCounter != 0)
				cout << "\nInvalid Account Number/PinCode! You Just Have " << 3 - FailedLogInCounter << " Other Login try befor you account is going to lock \n";

			if (FailedLogInCounter == 3)
			{
				LockAccount();
				return;
			}
		}		

		CurrentClient.AccountNumber = ReadClientAccountNumber();
		CurrentClient.PinCode = ReadPinCode();
		if (CurrentClient.AccountNumber == AdminAccountNumber && CurrentClient.PinCode == AdminPinCode)
		{
			CurrentClient.Permissions = -1;
			ShowATMAdminPanelMenueScreen();
			return;
		}
		if (IsLockAccount(CurrentClient.AccountNumber))
		{
			cout << "\nThis account is already locked If You want to recover it answer this security questions correctly to reset the PinCode\n";
			if (IsCorrectSecurityAnswer())
			{
				UnlockClientAccount();
				ShowUpdatePinCodeScreen();
				return;
			}
			else
			{
				cout << "\n Incorrect security answers " << endl;
				return;
			}
		}

		LoginFaild = !LoadClientInfo(CurrentClient.AccountNumber, CurrentClient.PinCode);
	} while (LoginFaild);
	ShowMainMenue();
}
//==========================================================================

 int main() 
 { 

	 Login();




	 return 0; 
 
 }








 //ATM project with other fetures
 /*
 #pragma warning(disable : 4996)

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iomanip>
#include <ctime>
using  namespace std; 

struct stAddress
{
	string Country;
	string City;
	string Village;
	string StreetAddress;

};

struct stBanks
{
	stAddress BankAddress;
	string BankName;
	string ClientIBAN;
};

struct sClient
{ 
	stBanks BankData;
	stAddress ClientAddress;
	string AccountNumber;
	string PinCode;
	string Name; 
	string Phone; 
	double AccountBalance;
	bool MarkForDelete = false;
}; 


enum enMainMenueOptions
{
	eQucikWithdraw = 1,
	eNormalWithDraw = 2,
	eDeposit = 3,
	eCheckBalance = 4,
	eMakeTransferInTheSameBank = 5,
	eClientsLists = 6,
	eUpdateClientPinCode = 7,
	eLockAccount = 8,
	eExit = 9

};

const string ClientsFileName = "Clients.txt";
const string LockedAccountsFileName = "LockedAccounts.txt";

sClient CurrentClient;


//======================Declaration Functions============================
void ShowMainMenue(); 
void Login(); 
void ShowQuickWithdrawScreen(); 
void ShowNormalWithDrawScreen();
void ShowDepositScreen();

//==========================Definition Functions======================

vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString; 
	short pos = 0;
	string sWord = "";
	// define a string variable  // use find() function to get the position of the delimiters  
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos); // store the word  
		if (sWord != "")
		{
			vString.push_back(sWord);
		} S1.erase(0, pos + Delim.length());  // erase() until positon and move to next word.
	}
	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string.    
	}
	return vString;
}

string ConvertClientRecordToLine(sClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance)+Seperator;
	stClientRecord += Client.BankData.BankName + Seperator;
	stClientRecord += Client.BankData.ClientIBAN + Seperator;
	stClientRecord += Client.BankData.BankAddress.Country+Seperator;
	stClientRecord += Client.BankData.BankAddress.City+Seperator;
	stClientRecord += Client.BankData.BankAddress.Village+Seperator;
	stClientRecord += Client.BankData.BankAddress.StreetAddress + Seperator;
	return stClientRecord;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite 
	string DataLine;
	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.      
				DataLine = ConvertClientRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}

sClient ConvertClientLinetoRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string> vClientData;
	vClientData = SplitString(Line, Seperator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);//cast string to double
	Client.BankData.BankName = vClientData[5];
	Client.BankData.ClientIBAN = vClientData[6];
	Client.BankData.BankAddress.Country = vClientData[7];
	Client.BankData.BankAddress.City = vClientData[8];
	Client.BankData.BankAddress.Village = vClientData[9];
	Client.BankData.BankAddress.StreetAddress = vClientData[10];

	return Client;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line = "";
		sClient Client;
		while (getline(MyFile, Line))  
		{
			Client = ConvertClientLinetoRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}

	return vClients;
}

//==========================================================================
bool IsNumberFrom0To9(char OneCharacter)
{
	for (short i = 48; i <=57; i++)
	{
		if (OneCharacter == char(i))
			return true;
	}
	return false;
}

bool IsStringNumbers(string MyString)
{
	for (short i = 0; i < MyString.length();i++)
	{
		if (!IsNumberFrom0To9(MyString[i]))
			return false;
	}
	return true;
}

string ReadPinCode()
{
	string NewPinCode;
	do
	{
		cout << "\nEnter You New PinCode (it should be has 4 number) : ";
		cin >> NewPinCode;

	} while (NewPinCode.length() < 4 || NewPinCode.length() > 4 || !IsStringNumbers(NewPinCode));
	return NewPinCode;
}

void UpdateClientPinCode()
{
	string NewPinCode= ReadPinCode();
	vector<sClient>vClients;
	for (sClient Client : vClients)
	{
		if (Client.AccountNumber == CurrentClient.AccountNumber)
		{
			Client.PinCode= NewPinCode;
			cout << "\nPinCode Updated Succesfully\n";
			SaveCleintsDataToFile(ClientsFileName,vClients);
			return;
		}
	}

}

void ShowUpdatePinCodeScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\tQucik Withdraw\n";
	cout << "===========================================\n";
	UpdateClientPinCode();

}

//==========================================================================

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Client)
{
	vector<sClient>vClient = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient C : vClient)
	{
		if ((C.AccountNumber == AccountNumber) && (C.PinCode == PinCode))
		{
			Client = C;
			return true;
		}
	}
	return false;

}

char IsAnswerIsYes()
{
	char Choice;
	cout << "\nYou sure you want to perform this Action? [y/n] : ";
	cin >> Choice;
	return (Choice == 'Y' || Choice == 'y');
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)

{
	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool FindClientByAccountNumberAndIbanNumber(string AccountNumber,string IBANNumber, vector<sClient> vClients, sClient& Client)

{
	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber && C.BankData.ClientIBAN==IBANNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{
	char Answer = 'n';
	cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		for (sClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveCleintsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
				return true;
			}
		}
		return false;
	}
}


//==========================================================================
short ReadQuickWithdrawOption() 
{ 
	short Choice = 0;
	while (Choice < 1 || Choice>9) 
	{ 
		cout << "\nChoose what to do from [1] to [9] ? ";        
		cin >> Choice; 
	} 
	return Choice;
}

short getQuickWithDrawAmount (short QuickWithDrawOption) 
{
	 switch (QuickWithDrawOption) 
	 {
		 case 1: 
			 return  20;
		 case 2: 
			 return 50; 
		 case 3:
			 return 100;
		 case 4:
			 return 200;
		 case 5:
			 return 400; 
		 case 6:
			 return 600;
		 case 7:
			 return 800;
		 case 8: 
			 return 1000;
		 default: 
			 return 0; 
	 } 
}

void PerfromQuickWithdrawOption(short QuickWithDrawOption) 
{
	if (QuickWithDrawOption == 9)//exit 
		return; 
	short WithDrawBalance = getQuickWithDrawAmount(QuickWithDrawOption); 
	if (WithDrawBalance > CurrentClient.AccountBalance) 
	{        
		cout << "\nThe amount exceeds your balance, make another choice.\n";  
		cout << "Press Anykey to continue...";  
		system("pause>0");
		ShowQuickWithdrawScreen();
		return;
	} 
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);  
	CurrentClient.AccountBalance -= WithDrawBalance;

} 

void ShowQuickWithdrawScreen()
{ 
	system("cls");    
	cout << "===========================================\n";   
	cout << "\t\tQucik Withdraw\n";
	cout << "===========================================\n";   
	cout << "\t[1] 20\t\t[2] 50\n";   
	cout << "\t[3] 100\t\t[4] 200\n"; 
	cout << "\t[5] 400\t\t[6] 600\n";    
	cout << "\t[7] 800\t\t[8] 1000\n";    
	cout << "\t[9] Exit\n";   
	cout << "===========================================\n";   
	cout << "Your Balance is " << CurrentClient.AccountBalance;  
	PerfromQuickWithdrawOption(ReadQuickWithdrawOption()); 
}

//==========================================================================

int ReadQuickWithDrawAmont() 
{
	int Amount;     
	cout << "\nEnter an amount multiple of 5's ? ";   
	cin >> Amount; 
	while (Amount % 5 != 0) 
	{
		cout << "\nEnter an amount multiple of 5's ? ";     
		cin >> Amount; 
	} 
	return Amount;
}

void PerfromNormalWithdrawOption()
{
	short WithDrawBalance = ReadQuickWithDrawAmont(); 
	if (WithDrawBalance > CurrentClient.AccountBalance) 
	{ 
		cout << "\nThe amount exceeds your balance, make another choice.\n";     
		cout << "Press Anykey to continue...";        
		system("pause>0"); 
		ShowNormalWithDrawScreen();
		return; 
	} 
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);    
	CurrentClient.AccountBalance -= WithDrawBalance; 
}

void ShowNormalWithDrawScreen()
{
	system("cls");
	cout << "===========================================\n";  
	cout << "\t\tNormal Withdraw Screen\n";    
	cout << "===========================================\n";   
	PerfromNormalWithdrawOption(); 
}

//==========================================================================

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenue();
}

//==========================================================================

void ShowCheckBalanceScreen() 
{
	system("cls"); 
	cout << "===========================================\n";  
	cout << "\t\tCheck Balance Screen\n";   
	cout << "===========================================\n";  
	cout << "Your Balance is " << CurrentClient.AccountBalance << "\n"; 
}

//==========================================================================


double ReadDepositAmount()
{
	double Amount; 
	do
	{	
		cout << "\nEnter a positive Deposit Amount? ";
		cin >> Amount; 

	} while (Amount <= 0);
	return Amount; 
}

void PerfromDepositOption() 
{
	double DepositAmount = ReadDepositAmount(); 
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients); 
	CurrentClient.AccountBalance += DepositAmount;
}

void ShowDepositScreen() 
{ 
	system("cls");  
	cout << "===========================================\n";   
	cout << "\t\tDeposit Screen\n";   
	cout << "===========================================\n";    
	PerfromDepositOption();
}

//==========================================================================

short ReadMainMenueOption() 
{ 	
	short Choice = 0;   
	do
	{
		cout << "\nChoose what do you want to do? [1 to 9]? "; 
		cin >> Choice; 
	} while (Choice<1 || Choice >9);
	return Choice;
} 

void CurrentTransactionTecket()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\tCurrent Transaction Details Screen\n";
	cout << "===========================================\n";

	time_t t = time(0); // to get time now
	tm* OurlocalTimenow = localtime(&t);
	cout << "\nAccount Number  : " << CurrentClient.AccountNumber;
	cout << "\nName            : " << CurrentClient.Name;
	cout << "\nPhone           : " << CurrentClient.Phone;
	cout << "\nAccount Balance : " << CurrentClient.AccountBalance;
	cout << "\nDay : ";
	cout << OurlocalTimenow->tm_mday << "/"
		<< OurlocalTimenow->tm_mon + 1 << "/"
		<< OurlocalTimenow->tm_year + 1900 << " \n";
	cout << "\nThis Transaction Was Made At Time ";
	cout<< OurlocalTimenow->tm_hour << ":"
		<< OurlocalTimenow->tm_min<<":"
		<< OurlocalTimenow->tm_sec << endl;



}

//=============================================================

double ReadSendingAmount()
{
	double SendingAmout=0;
	cout << "\nEnter Sending Amount : ";
	cin >> SendingAmout;
	return SendingAmout;
}

void TransferTicketDetails(sClient BeneficiaryClient)
{
	system("cls");
	cout << "===========================================\n";
	cout << "\tCurrent Transaction Details Screen\n";
	cout << "===========================================\n";

	time_t t = time(0); // to get time now
	tm* OurlocalTimenow = localtime(&t);
	cout << "\n===========================================\n";
	cout << "\nSender Details\n";
	cout << "\n===========================================\n";

	cout << "\nAccount Number      : " << CurrentClient.AccountNumber;
	cout << "\nName                : " << CurrentClient.Name ;
	cout << "\nPhone               : " << CurrentClient.Phone ;
	cout << "\nAccount Balance     : " << CurrentClient.AccountBalance;
	cout << "\nBank Details\n";
	cout << "\nBank Name           : " << CurrentClient.BankData.BankName;
	cout << "\nIBAN Number         : " << CurrentClient.BankData.ClientIBAN;
	cout << "\nBankCountry         : " << CurrentClient.BankData.BankAddress.Country;
	cout << "\nBank City           : " << CurrentClient.BankData.BankAddress.City;
	cout << "\nBank Village        : " << CurrentClient.BankData.BankAddress.Village;
	cout << "\nBank Street Address : " << CurrentClient.BankData.BankAddress.StreetAddress;

	cout << "\n===========================================\n";
	cout << "\nReseaver Details\n";
	cout << "\n===========================================\n";
	cout << "\nAccount Number      : " << BeneficiaryClient.AccountNumber;
	cout << "\nName                : " << BeneficiaryClient.Name;
	cout << "\nBank Name           : " << BeneficiaryClient.BankData.BankName;
	cout << "\nIBAN Number         : " << BeneficiaryClient.BankData.ClientIBAN;
	cout << "\nBankCountry         : " << BeneficiaryClient.BankData.BankAddress.Country;
	cout << "\nBank City           : " << BeneficiaryClient.BankData.BankAddress.City;
	cout << "\nBank Village        : " << BeneficiaryClient.BankData.BankAddress.Village ;
	cout << "\nBank Street Address : " << BeneficiaryClient.BankData.BankAddress.StreetAddress<< endl;
	cout << "\n===========================================\n";

	cout << "\nDay : ";
	cout << OurlocalTimenow->tm_mday << "/"
		<< OurlocalTimenow->tm_mon + 1 << "/"
		<< OurlocalTimenow->tm_year + 1900 << " \n";
	cout << "\nThis Transaction Was Made At Time ";
	cout << OurlocalTimenow->tm_hour << ":"
		<< OurlocalTimenow->tm_min << ":"
		<< OurlocalTimenow->tm_sec << endl;


}

bool IsFromTheSameBank(sClient Client1,sClient Client2)
{
	vector<sClient>vClients = LoadCleintsDataFromFile(ClientsFileName);
	return (FindClientByAccountNumber(Client2.AccountNumber,vClients,Client2)?(Client1.BankData.BankName == Client2.BankData.BankName):false);
}

string ReadIClientIBAN()
{
	string IbanNumber;
	cout << "\nEnter Account IBAN : ";
	cin >> IbanNumber;
	return IbanNumber;
}

string ReadBeneficiaryAccountNumber()
{
	string BeneficiaryAccountNumber;
	do
	{
		cout << "\nEnter Beneficiary Account Number : ";
		cin >> BeneficiaryAccountNumber;
	} while (BeneficiaryAccountNumber == CurrentClient.AccountNumber);
	return BeneficiaryAccountNumber;
}

void UpdateCurrentClientAccountBalanceInFile(string AccountNumber)
{
	vector<sClient>vClients = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient Client : vClients)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			Client.AccountBalance = CurrentClient.AccountBalance;
			vClients = SaveCleintsDataToFile(ClientsFileName, vClients);
			return;
		}
	}
}

void TransferMoneyToExtenalBankAccount(sClient &BeneficiaryClient)
{
	BeneficiaryClient.BankData.ClientIBAN = ReadIClientIBAN();
	double SendingAmount = ReadSendingAmount();
	if (CurrentClient.AccountBalance > SendingAmount)
	{
		vector<sClient>vClient = LoadCleintsDataFromFile(ClientsFileName);
		if (FindClientByAccountNumberAndIbanNumber(BeneficiaryClient.AccountNumber, BeneficiaryClient.BankData.ClientIBAN,vClient, BeneficiaryClient))
		{
			if (IsAnswerIsYes)
			{
				DepositBalanceToClientByAccountNumber(BeneficiaryClient.AccountNumber, SendingAmount, vClient);
				cout <<"\nAmount " << SendingAmount << " is Send Succesfuly To " << BeneficiaryClient.Name << endl;
				CurrentClient.AccountBalance -= SendingAmount;
				cout << "\nYour Current Balance "<<CurrentClient.AccountBalance<<endl;
				UpdateCurrentClientAccountBalanceInFile(CurrentClient.AccountNumber);
			}
		}
		else
		{
			cout << "\nInvalid Account Number/OrIBAN" << endl;
			return;
		}

	}
	else
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		return;
	}


}

void TransferMoneyInSameBank(sClient &BeneficiaryClient)
{
	double SendingAmount = ReadSendingAmount();
	if (CurrentClient.AccountBalance > SendingAmount)
	{
		vector<sClient>vClient = LoadCleintsDataFromFile(ClientsFileName);
		if (FindClientByAccountNumber(BeneficiaryClient.AccountNumber, vClient, BeneficiaryClient))
		{
			if (IsAnswerIsYes)
			{
				DepositBalanceToClientByAccountNumber(BeneficiaryClient.AccountNumber, SendingAmount, vClient);
				cout << "\nAmount " << SendingAmount << " is Send Succesfuly To " << BeneficiaryClient.Name << endl;
				CurrentClient.AccountBalance -= SendingAmount;
				cout << "\nYour Current Balance " << CurrentClient.AccountBalance << endl;
				UpdateCurrentClientAccountBalanceInFile(CurrentClient.AccountNumber);
			}
		}
		else
		{
			cout << "\nInvalid Account Number" << endl;
		}
	
	}
	else
	{
		cout << "\nThe amount exceeds your balance\n";
	}


	


}

void TransferMoneyBetwinAccounts()
{
	sClient BeneficiaryClient;
	BeneficiaryClient.AccountNumber= ReadBeneficiaryAccountNumber();
	if (IsFromTheSameBank(CurrentClient, BeneficiaryClient))
	{
		TransferMoneyInSameBank(BeneficiaryClient);
	}
	else
	{
		TransferMoneyToExtenalBankAccount(BeneficiaryClient);
	}
	TransferTicketDetails(BeneficiaryClient);

}

void ShowTransferMoneyBetwinAccountsScreen()
{

	system("cls");
	cout << "===========================================\n";
	cout << "\t\tDeposit Screen\n";
	cout << "===========================================\n";
	TransferMoneyBetwinAccounts();
}

//=============================================================

void ShowSpecifiqueDetailsAboutAllClientsScreen()
{
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(40) << "IBAN Number";

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In the System!";
	}
	else
	{
		for (sClient Client : vClients)
		{

			cout << "| " << setw(15) << left << Client.AccountNumber;
			cout << "| " << setw(40) << left << Client.Name;
			cout << "| " << setw(40) << left << Client.BankData.ClientIBAN;

			cout << endl;
		}

	}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

}

//=============================================================

vector<string>LoadLockedAccountsDataFromFile(string LockedAccountsFileName)
{
	vector <string> vClients;
	fstream MyFile;
	MyFile.open(LockedAccountsFileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line = "";
		while (getline(MyFile, Line))
		{
			vClients.push_back(Line);
		}
		MyFile.close();
	}
	return vClients;
}

void SaveLockedClientToFile(string FileName, string DataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);//overwrite 
	if (MyFile.is_open())
	{
		MyFile << DataLine << endl;
		MyFile.close();
	}
}

bool IsLockAccount(string AccountNumber)
{
	vector<string>vLockedClients = LoadLockedAccountsDataFromFile(LockedAccountsFileName);
	for (string LC : vLockedClients)
	{
		if (LC == AccountNumber)
			return true;
	}
	return false;
}

void LockAccount()
{
	if (IsAnswerIsYes())
	{
		vector<sClient>vClients = LoadCleintsDataFromFile(ClientsFileName);
		if (FindClientByAccountNumber(CurrentClient.AccountNumber, vClients, CurrentClient))
		{
			SaveLockedClientToFile(LockedAccountsFileName,CurrentClient.AccountNumber);
			cout << "\nYour Account Is Locked Succesfully\n";
		}
	}

}

void LockATMAccountScreen()
{

	system("cls");
	cout << "===========================================\n";
	cout << "\t\tLock Account Screen\n";
	cout << "===========================================\n";
	LockAccount();

}

//==========================================================================

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
	switch (MainMenueOption)
	{

	case enMainMenueOptions::eQucikWithdraw:
		system("cls");
		ShowQuickWithdrawScreen();
		CurrentTransactionTecket();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::eNormalWithDraw:
		system("cls");
		ShowNormalWithDrawScreen();
		CurrentTransactionTecket();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::eDeposit:
		system("cls");
		ShowDepositScreen();
		CurrentTransactionTecket();
		GoBackToMainMenue();
		break;

	case enMainMenueOptions::eCheckBalance:
		system("cls");
		ShowCheckBalanceScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eMakeTransferInTheSameBank:
		system("cls");
		ShowTransferMoneyBetwinAccountsScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eClientsLists:
		system("cls");
		ShowSpecifiqueDetailsAboutAllClientsScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eUpdateClientPinCode:
		system("cls");
		ShowUpdatePinCodeScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eLockAccount:
		system("cls");
		LockATMAccountScreen();
		break;
	case enMainMenueOptions::eExit:
		system("cls");
		Login();
		break;
	}
}

void ShowMainMenue()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tATM Main Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Transfer.\n";
	cout << "\t[6] Show Specific Details. About All Clients \n";
	cout << "\t[7] Update PinCode.\n";
	cout << "\t[8] Lock Account\n";
	cout << "\t[9] Logout.\n";
	cout << "===========================================\n";

	PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

//==========================================================================

bool  LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient)) 
		return true;
	else
		return false; 
}

void Login()
{
	bool LoginFaild = false; 
	string AccountNumber, PinCode; 
	short FailedLogInCounter = 0;
	do 
	{
		system("cls");
		cout << "\n---------------------------------\n";   
		cout << "\tLogin Screen";       
		cout << "\n---------------------------------\n";
		if (LoginFaild)
		{ 			
			FailedLogInCounter++;
			cout << "\nInvalid Account Number/PinCode! You Just Have "<<3-FailedLogInCounter<<" Other Login try \n";

		}
		if (FailedLogInCounter == 3)
			return;
		cout << "Enter Account Number? ";      
		cin >> AccountNumber; 
		if (IsLockAccount(AccountNumber))
		{
			cout << "\nThis account is locked contact the bank to help ";
			return;
		}

		cout << "Enter Pin? ";     
		cin >> PinCode;    
		LoginFaild = !LoadClientInfo(AccountNumber, PinCode); 
	} 
	while (LoginFaild);
	ShowMainMenue(); 
}

//==========================================================================

 int main() 
 { 




	 Login();

	 cout << "\nProgram is end Presse any key Stop...";
	 system("pause>0");
	 return 0; 
 
 }

 */
