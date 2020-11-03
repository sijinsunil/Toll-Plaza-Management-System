#include<iostream>
#include<conio.h>
#include<stdio.h>
#include<fstream>
#include<iomanip>
#include<process.h>
#include<string.h>

using namespace std;

class Customer
{
	private:
		int  m_nCustID;
		char m_CustName[100];
		char m_CustNumber[225];
		char m_CustAreaCode[15];
		struct Date
		{
			int day;
			int month;
			int year;
		}m_CustJoinDate;
	
	public:
		Customer();
		void InputData();
		int  GenerateCustomerID();
		void GenerateCustomerChart();
		int  GetFileSize(ifstream& infile); 
		int  ValidateCustomerID(int nCustID);
		void ShowCustomerDetails(const Customer&);

		void Payment();
		int  CheckPaymentAlreadyMade(int nCustID,int nMonth,int nYear);
		int  CheckBeforeJoinDate(int nCustID,int nMonth,int nYear);
		void GeneratePaymentChart();
		void ShowPaymentDetails(const struct Payment&);

		void GenerateBill();
		int DrawBill(const struct Payment& oPay);

};

int ValidateDate(int nDay,int nMonth,int nYear,int nValidateDay=1);


int ValidateDate(int nDay,int nMonth,int nYear,int nValidateDay)
{
	int nMonthDay=-1;

	// check Month
    if(!(nMonth>=1 && nMonth<=12)) 
		return 0;

	// check Month (Started with 1900)
	if(nYear<2000)
		return 0;

	if(nValidateDay == 0)
		return 1;

	// check Day
	switch(nMonth)
	{	
	case 1:	
	case 3:
	case 5:
	case 7:		    
	case 8:		
	case 10:	
	case 12:
		    nMonthDay = 31;
			break;
	case 4:	
	case 6:	
	case 9:	
	case 11:
		    nMonthDay = 30;
			break;
	case 2:
            // leap year checking 
			nMonthDay = 28;
			break;
	default:
			nMonthDay=-1;
	}
    
	if(!(nDay>=1 && nDay <=nMonthDay))
		return 0;

	return 1;
}


Customer::Customer()
{
}


struct Payment
{
	int CustID,nMonth,nYear,Amt,Balance;
};

#define SCHEME 100



void Customer::InputData()
{
	cout<<"Enter the Name of Customer: ";
	//cin.getline(m_CustName,'\n');
	cin>>m_CustName;

	cout<<"Enter the Vehicle Number : ";
	//cin.getline(m_CustNumber,'\n');
	cin>>m_CustNumber;

	cout<<"Enter the Booth code: ";
	cin>>m_CustAreaCode;

LbDate:
	cout<<"Enter the PassDate\n";
	cout<<"Enter the Day     : ";
	cin>>m_CustJoinDate.day;
	cout<<"Enter the Month   : ";
	cin>>m_CustJoinDate.month;
	cout<<"Enter the Year	 : ";
	cin>>m_CustJoinDate.year;

	// check date
	if(ValidateDate(m_CustJoinDate.day,m_CustJoinDate.month,m_CustJoinDate.year)==0)
	{
		cout<<"Invalid date entered!!! Please retype Join date.\n";
		goto LbDate;
	}

	// open file for writing details.
	ofstream outfile;
	outfile.open("Customer.txt", ios::out|ios::binary|ios::app);

	// Generate CustomerID. if not function returns '0'.
	int nID = GenerateCustomerID();

	if(nID == 0)
		m_nCustID = 100;
	else
		m_nCustID = ++nID; // add '1' to existing CustID.


    outfile.write((char*)this,sizeof(Customer));
	outfile.close();
	cout<<"\n----Customer registered sucessfully----\n";
}

int Customer::GenerateCustomerID()
{
    Customer oCust;
    ifstream infile;
    infile.open("Customer.txt",ios::binary);
    if(infile.fail())
	return -1;
	//cout<<"Couldn't fetch customer details!!!\nPlease check the Customer file.";

	// if Customer file is null..CustID started from '100'.
	infile.seekg(0,ios::end);
	int npos = infile.tellg();
	if(npos == 0)
	{
		infile.close();
		return 0;
	}

   // if not a emptyfile getlast record.
	infile.seekg(0,ios::beg);
	while(npos)
	{
		infile.read((char*)&oCust,sizeof(oCust));
		npos -= sizeof(oCust);
	}
	infile.close();
	return oCust.m_nCustID; // return last CustID

//	return -1;
}

int  Customer::GetFileSize(ifstream& infile)
{
	infile.seekg(0,ios::end);
    return (infile.tellg());
}

void Customer::GenerateCustomerChart()
{
	Customer oCust;
    ifstream infile;
	infile.open("Customer.txt",ios::binary|ios::in);
	if(infile.fail())
	{
		cout<<"*** Empty Customer list ***\n";
		return ;
	}

	int nSize = GetFileSize(infile);
	infile.seekg(0,ios::beg);

	cout<<"\n------------- Customer Chart ----------------------\n";
	cout<<"\tID\tName\tBoothCode\tVehicleNumber\n";
	cout<<"\t**\t****\t*********\t************\n";
	while(nSize)
	{
		infile.read((char*)&oCust,sizeof(oCust));
		ShowCustomerDetails(oCust);
		nSize -= sizeof(oCust);
	}
	cout<<"-----------------------------------------------------\n";
	infile.close();
}

void Customer::ShowCustomerDetails(const Customer &oCust)
{
	cout<<"\t"<<oCust.m_nCustID<<"\t"<<oCust.m_CustName<<"\t"
		<<oCust.m_CustAreaCode<<"\t\t"<<oCust.m_CustNumber<<"\n";
}


void Customer::Payment()
{
    struct Payment oPay;

	int nID;
	int nMonth,nYear,Amt,Balance;

	cout<<"Enter the Customer ID";
	cin>>nID;
	if(ValidateCustomerID(nID) == 0)
	{
		cout<<"You entered invalid customerID!!! Please check customerID.\n";
		return;
	}

LbDate:
	cout<<"Enter the Month & year";
	cin>>nMonth>>nYear;
	//validate month & year.
	if(ValidateDate(0,nMonth,nYear,0) == 0) //last para for not validating day.
	{
		cout<<"Invalid date entered!!! Please retype payment date.\n";
		goto LbDate;
	}

	if(CheckBeforeJoinDate(nID,nMonth,nYear))
	{
		cout<<"Invalid date entered!!! Please check join date.\n";
		goto LbDate;
	}

	ofstream outfile;
	outfile.open("Payment.txt", ios::out|ios::binary|ios::app);

	if(CheckPaymentAlreadyMade(nID,nMonth,nYear))
	{
		cout<<"\n**** Already made **** \n";
		return;
	}

	cout<<"Enter the pass amount : ";
	cin>>Amt;
	Balance=0;
	if(Amt > SCHEME)
	{
		cout<<"Your amount is not correct\n";
		return;
	}
	else if(Amt < SCHEME)
	{
		//100 - user enter amount
		Balance =  SCHEME - Amt;
	}
	else
		Balance = 0;


	oPay.CustID = nID;
	oPay.nMonth = nMonth;
	oPay.nYear  = nYear;
	oPay.Amt    = Amt;
	oPay.Balance= Balance;

	outfile.write((char*)&oPay,sizeof(oPay));
	outfile.close();
}

int Customer::ValidateCustomerID(int nCustID)
{
	Customer oCust;
    ifstream infile;
	infile.open("Customer.txt",ios::binary);
	if(infile.fail())
		return 0;

	int nSize = GetFileSize(infile);
	infile.seekg(0,ios::beg);

	int nFound = 0;
	while(nSize)
	{
		infile.read((char*)&oCust,sizeof(oCust));
	if(oCust.m_nCustID == nCustID)
		{
			nFound = 1;
			break;
		}
		nSize -= sizeof(oCust);
	}
	infile.close();

	if(nFound)
		return 1;

	return 0;
}

int Customer::CheckBeforeJoinDate(int nCustID,int nMonth,int nYear)
{

	Customer oCust;
    ifstream infile;
	infile.open("Customer.txt",ios::binary);
	if(infile.fail())
		return 0;

	int nSize = GetFileSize(infile);
	infile.seekg(0,ios::beg);

	int nFound = 0;
	while(nSize)
	{
		infile.read((char*)&oCust,sizeof(oCust));
	if(oCust.m_nCustID == nCustID)
		{
			if( (nMonth<oCust.m_CustJoinDate.month) || (nYear<oCust.m_CustJoinDate.year))
			{
				nFound = 1;
				break;
			}
		}
		nSize -= sizeof(oCust);
	}
	infile.close();

	if(nFound)
		return 1;

	return 0;
}

int Customer::CheckPaymentAlreadyMade(int nCustID,int nMonth,int nYear)
{
	struct Payment oPay;

    ifstream infile;
	infile.open("Payment.txt",ios::binary);
	if(infile.fail())
		return 0;

	int nSize = GetFileSize(infile);
	infile.seekg(0,ios::beg);

	int nFound = 0;
	while(nSize)
	{
		infile.read((char*)&oPay,sizeof(oPay));
	if((oPay.CustID == nCustID) && (oPay.nMonth == nMonth) && (oPay.nYear == nYear))
		{
			nFound = 1;
			break;
		}
		nSize -= sizeof(oPay);
	}
	infile.close();

	if(nFound)
		return 1;

	return 0;
}

void Customer::GeneratePaymentChart()
{
	struct Payment oPay;
    ifstream infile;
	infile.open("Payment.txt",ios::binary|ios::in);
	if(infile.fail())
	{
		cout<<"*** Empty Payment list ***\n";
		return ;
	}

	int nSize = GetFileSize(infile);
	infile.seekg(0,ios::beg);

	cout<<"\n-------------- Customer Payment Chart -----------------------\n";
	cout<<"\tID\tMonth\tYear\tAmount Paid\tBalance Amount\n";
	cout<<"\t**\t*****\t****\t***********\t**************\n";
	while(nSize)
	{
		infile.read((char*)&oPay,sizeof(oPay));
		ShowPaymentDetails(oPay);
		nSize -= sizeof(oPay);
	}
	cout<<"---------------------------------------------------------------\n";
	infile.close();
}

void Customer::ShowPaymentDetails(const struct Payment& oPay)
{
	cout<<"\t"<<oPay.CustID<<"\t"<<oPay.nMonth<<"\t"<<oPay.nYear<<"\t"
		<<oPay.Amt<<"\t\t"<<oPay.Balance<<"\n";
}


void Customer::GenerateBill()
{
	struct Payment oPay;

	int nID,nMonth,nYear;

	cout<<"\nEnter the Customer ID";
	cin>>nID;
	if(ValidateCustomerID(nID) == 0)
	{
		cout<<"You entered invalid customerID!!! Please check customerID.\n";
		return;
	}

LbDate:
	cout<<"Enter the Month & year";
	cin>>nMonth>>nYear;
	//validate month & year.
	if(ValidateDate(0,nMonth,nYear,0) == 0) //last para for not validating day.
	{
		cout<<"Invalid date entered!!! Please retype payment date.\n";
		goto LbDate;
	}

	if(CheckBeforeJoinDate(nID,nMonth,nYear))
	{
		cout<<"Invalid date entered!!! Please check join date.\n";
		goto LbDate;
	}

    ifstream infile;
	infile.open("Payment.txt",ios::binary);
	if(infile.fail())
	{
		cout<<"*** Empty Payment list ***\n";
		return ;
	}

	int nSize = GetFileSize(infile);
	infile.seekg(0,ios::beg);

	int nFound=0,nGenerate = 1;
	while(nSize)
	{
		infile.read((char*)&oPay,sizeof(oPay));
		if(oPay.CustID == nID && oPay.nMonth == nMonth && oPay.nYear == nYear)
		{
			nFound = 1;
			if(DrawBill(oPay) == 0)
				nGenerate = 0;
			break;
		}
		nSize -= sizeof(oPay);
	}
	if(nFound)
	{
		if(nGenerate == 0)
			cout<<"*** Failed to genearate bill ***\n";
	}
	else
	cout<<"Invalid CustomerID or Date!!! Please check it\n";
	infile.close();

	if(nFound==0 || nGenerate == 0) return;

	// set to printer
	int nOK = system("Print");
	if(nOK == -1)
	{
	   cout<<"Printing Failure\n";
	   return;
	}

	cout<<"*** Printed Successfully ***\n";
}

int Customer::DrawBill(const struct Payment& oPay)
{
	// bill header format
	char *Head    =(char*) "\n---------Customer Receipt---------------\n";
	char *SubHead =(char*) "\nID Month Year Amount Balance\n** ***** **** ***** *******\n";
	char *End     =(char*) "\n----------------------------------------\n";

	ofstream outfile;
	outfile.open("Bill.txt",ios::binary|ios::in);
	if(outfile.fail())
		return 0;

	char Temp[5];

	outfile.write(Head,(strlen(Head)+1));
	outfile.write(SubHead,(strlen(SubHead)+1));

	sprintf(Temp,"%d",oPay.CustID);
	outfile.write(Temp,(strlen(Temp)+1));
	outfile.write((char*)" ",1);

	sprintf(Temp,"%d",oPay.nMonth);
	outfile.write(Temp,(strlen(Temp)+1));
	outfile.write((char*)" ",1);

	sprintf(Temp,"%d",oPay.nYear);
	outfile.write(Temp,(strlen(Temp)+1));
	outfile.write((char*)" ",1);

	sprintf(Temp,"%d",oPay.Amt);
	outfile.write(Temp,(strlen(Temp)+1));
	outfile.write((char*)" ",1);

	sprintf(Temp,"%d",oPay.Balance);
	outfile.write(Temp,(strlen(Temp)+1));
	outfile.write((char*)" ",1);

	//bill footer 
	outfile.write(End,(strlen(End)+1));
	outfile.close();

	return 1;
}



int main(){
	char nChoice;
	char ch;
	Customer m_oCustomer;
  
	do
	{
	system("cls");
		
	void CreateMainMenu();
  	CreateMainMenu();

  	cout<<"Enter the choice";
	cin>>nChoice;

  switch(nChoice)
		{
			case '1':
			 	   //Customer Generation
			 		m_oCustomer.InputData();
			 	    break;
			case '2':
			 		//Chart Prepration
			 		m_oCustomer.GenerateCustomerChart();
			 		break;
			case '3':
			 		//Payment
			 		m_oCustomer.Payment();
			 		break;
			case '4':
					//Payment Chart
			 	    m_oCustomer.GeneratePaymentChart();
			 		break;
			case '5':
					//Payment Chart
				    m_oCustomer.GenerateBill();
			 		break;
			case '6':
					// Help function
          			void Help();
					Help();
					break;
			case '7':
					// About function
          			void About();
					About();
					break;
			case '8':
					// Exit
          			void CloseApplication();
					CloseApplication();
					break;
			default:
					cout<<"Invalid choice\n";
		}
		cout<<"Continue(y/n)";
		cin>>ch;
		void CloseApplication();
		if(ch=='n' || ch=='N') CloseApplication();

	}while(ch == 'y' || ch == 'Y');

    cout<<endl;

}



void CreateMainMenu()
{
	cout<<"****************************\n";
	cout<<"  Toll Booth Management\n";
	cout<<"****************************\n\n";
	cout<<"Main Menu\n";
	cout<<"---------\n";
	cout<<"1. Customer Pass Creation\n";
	cout<<"2. Customer Details\n";
	cout<<"3. Payments\n";
	cout<<"4. Payment List\n";
	cout<<"5. Prepare Bill\n";
	cout<<"6. Help\n";
	cout<<"7. About\n";
	cout<<"8. Exit\n";
}


void Help()
{
   string szHelp = "Unavailable\n";

   cout<<"----------------------------------------------------------\n";
   cout<<szHelp;
   cout<<"----------------------------------------------------------\n";
}

void About()
{
  string szDoneBy = "Done by :- SIJIN S\nLink    :- https://github.com/sijinsunil \n\t \n";
  cout<<"----------------------------------------------------------\n";
  cout<<szDoneBy;
  cout<<"----------------------------------------------------------\n";
}

void CloseApplication()
{
	cout<<"Closing the application\n";
//	Sleep(100);
	exit(0);
}


