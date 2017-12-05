//Name: AGRAWAL, Shubhankar
//UID: 3035345306
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
using namespace std;
struct flights //Structure that stores the records working on
{
	string dest;
	string airline;
	string fromdate;
	string todate;
	string price;
	int pricecalc;
	string lastdate;
	string quota;
	int quotacalc;
	bool mark;
};
struct bought //Structure dealing with the countries and total tickets purchased.
{
	string country;
	int number;
	string aircode;
	int totalprice;
};
int i = 0, counted = 0; //Counters for the arrays of the two main structures
//Function declarations
void zeroquota(flights maindb[]);
void displaystruct(flights maindb[]);
void displaycountries(bought tickets[]);
void addpromo(string line, flights maindb[], bool callvalue);
void makearray(flights maindb[]);
void allpromotion(flights maindb[], bool &search);
bool searchperformed(flights maindb[]);
void changemonth(string &lastdate);
bool checking(flights struc, flights maindb[]);
void todayflights(string line, flights maindb[]);
void clearall(flights maindb[], bool &search);
void searchdest(string line, flights maindb[], bool &search, bool &checksearch);
void searchdate(string line, flights maindb[], bool &search, bool &checksearch);
void preferairline(string line, flights maindb[], bool &search);
void noairline(string line, flights maindb[], bool &search);
void buyticket(string line, flights maindb[], bought tickets[], bool &search);
void buypromotion(string line, flights maindb[], bought tickets[]);
void mostpopular(bought tickets[]);
void readinter(ifstream &fin, flights maindb[], bought tickets[]);
void makestructure(bought tickets[]);
/*BONUS:
void lowestcountry(string line, flights maindb[], bought tickets[]);
void lowestairline(string line, flights maindb[]);
*/
int main()
{
	flights maindb[500]; //Array to store all existing promotion records
	bought tickets[200]; //Array to store airport codes and revenue per country
	bool search = false, checksearch = false; //Variables to check if search has been performed
	string input;
	ifstream fin, infi;
	ofstream fout;
	fin.open("mainDB.csv");
	if(fin.fail())
	{
		cout<<"Error opening main database file";
		exit(1);
	}
	fin.close();
	makearray(maindb);
	makestructure(tickets);
	//CHECKING: cout<<"Old database: "<<endl;
	//displaystruct(maindb);
	//displaycountries(tickets);
	infi.open("intermediate.txt");
	if(infi.fail())
	{}
	else
	{
		readinter(infi, maindb, tickets);
		//cout<<"New database: "<<endl;
		//displaystruct(maindb);
		//cout<<endl;
		//displaycountries(tickets);
		//cout<<endl;
	}
	infi.close();
	cout<<"commands:";
	cout<<"\nall | add promo <filename> | today <yyyymmdd> | clear | most | end";
	cout<<"\nbuy #<k> <N> | buy <dest>-<airline>-<from date>-<to date>-<price> <N> |";
	cout<<"\nsearch date <yyyymmdd> | search dest <xxx> | prefer airline <xxx> | no airline <xxx>";
	cout<<"\n//////////////"<<endl;
	getline(cin, input);
	while(input != "end")
	{
		if(input.substr(0, 3) == "add")
		{
			//CHECKING CODE: cout<<"Calling addpromo."<<endl;
			addpromo(input, maindb, false);
		}
		else if(input == "all")
		{
			allpromotion(maindb, search);
		}
		else if(input.substr(0,5) == "today")
		{
			//CHECKING CODE: cout<<"Calling todayflights."<<endl;
			todayflights(input, maindb);
		}
		else if(input == "clear")
		{
			clearall(maindb, search);
		}
		else if(input.substr(0,6) == "search")
		{
			//CHECKING CODE: cout<<"Calling search functions."<<endl;
			if(input.substr(7,4) == "dest")
			{
				//CHECKING CODE: cout<<"Calling searchdest."<<endl;
				searchdest(input, maindb, search, checksearch);
			}
			else if(input.substr(7,4) == "date")
			{
				//CHECKING CODE: cout<<"Calling searchdate."<<endl;
				searchdate(input, maindb, search, checksearch);
			}
		}
		else if(input.substr(0,6) == "prefer")
		{
			//CHECKING CODE: cout<<"Calling preferairline."<<endl;
			preferairline(input, maindb, search);
		}
		else if(input.substr(0,2) == "no")
		{
			//CHECKING CODE: cout<<"Calling no airline."<<endl;
			noairline(input, maindb, search);
		}
		else if(input.substr(0,3) == "buy")
		{
			//CHECKING CODE: cout<<"Calling buyfunctions."<<endl;
			if(input[4] == '#')
			{
				//CHECKING CODE: cout<<"Calling buyticket."<<endl;
				buyticket(input, maindb, tickets, search);
			}
			else
			{
				//CHECKING CODE: cout<<"calling buypromotion"<<endl;
				buypromotion(input, maindb, tickets);
			}
		}
		else if(input == "most")
		{
			//CHECKING CODE: cout<<"Calling mostpopular."<<endl;
			mostpopular(tickets);
		}
		/*BONUS:
		else if(input.substr(0,6) == "lowest")
		{
			if(input.substr(7,7) == "country")
			{
				lowestcountry(input, maindb, tickets);
			}
			else if(input.substr(7,7) == "airline")
			{
				lowestairline(input, maindb);
			}
		}
		*/
		cout<<"commands:";
		cout<<"\nall | add promo <filename> | today <yyyymmdd> | clear | most | end";
		cout<<"\nbuy #<k> <N> | buy <dest>-<airline>-<from date>-<to date>-<price> <N> |";
		cout<<"\nsearch date <yyyymmdd> | search dest <xxx> | prefer airline <xxx> | no airline <xxx>";
		cout<<"\n//////////////"<<endl;
		//CHECKING CODE: cout<<"Number of elements in structure: "<<i<<endl;
		getline(cin, input);
	}
	return 0;
}
void zeroquota(flights maindb[])
{ //Remove records that have 0 quota to free up space.
	for(int z = 0; z < i; z++)
	{
		if(maindb[z].quotacalc == 0)
		{
			for(int y = z; y < i - 1; y++)
			{
				maindb[y] = maindb[y+1];
			}
			z--;
			i--;
		}
	}
}
void displaystruct(flights maindb[])
{ //Display the marked records.
	int counting = 0;
	for(int j = 0; j < i; j++)
	{
		if(maindb[j].mark == true && maindb[j].quotacalc != 0)
		{
			cout<<"  "<<"<"<<counting+1<<">"<<" "<<maindb[j].dest<<"-"<<maindb[j].airline<<"-"<<maindb[j].fromdate<<"-"<<maindb[j].todate<<" HKD"<<maindb[j].pricecalc<<", promotion expires "<<maindb[j].lastdate<<", "<<maindb[j].quotacalc<<" quota"<<", mark: "<<maindb[j].mark<<endl;
			counting++;
		}
	}
}
void displaycountries(bought tickets[])
{ // A function built for checking the total number of tickets bought per country.
	for(int u = 0; u < counted; u++)
	{
		cout<<"Country: "<<tickets[u].country<<", tickets bought: "<<tickets[u].number<<", total revenue: "<<tickets[u].totalprice<<endl;
	}
}
void addpromo(string line, flights maindb[], bool callvalue)
{ //Function to add promotion file to the existing database.
	ifstream fin;
	ofstream fout;
	stringstream temp;
	string key1, key2, filename, newline, flight, details, promdetails, dummy, dest, airline, from, to, quota, last, price;
	temp << line;
	temp >> key1 >> key2 >> filename;
	fin.open(filename.c_str());
	if(fin.fail())
	{
		cout<<"Error opening "<<filename << " file."<<endl;
		//exit(1);
	}
	getline(fin, key1, ':');
	getline(fin, flight);
	if(callvalue == false)
		cout<<"Records added are: "<<endl;
	while(fin.good())
	{
		string newstring;
		string everyline;
		int counter = -1;
		getline(fin, newline);
		while(newline != "#")
		{
			newstring = newstring + newline + "!";
			counter++;
			getline(fin, newline);
		}
		int tempvar = newstring.find('!');
		int position = newstring.find(':');
		string enddate = newstring.substr(position + 1, tempvar - position - 1);
		changemonth(enddate);
		last = enddate;
		position = newstring.find('!');
		for(int j = 0; j < counter; j++)
		{
			int newposition = newstring.find('!', position + 1);
			promdetails = newstring.substr(position + 1, newposition - position - 1);
			position = newposition; 
			details = promdetails.substr(0, 3) + "," + flight + ",";
			promdetails.replace(0, 4, details);
			int topos = promdetails.find("to");
			string firstdate = promdetails.substr(7, topos - 8);
			changemonth(firstdate);
			from = firstdate;
			firstdate = firstdate + ",";
			promdetails.replace(7, topos - 4, firstdate);
			int hkdpos = promdetails.find("HKD");
			string lastdate = promdetails.substr(16,hkdpos - 17);
			changemonth(lastdate);
			to = lastdate;
			promdetails.replace(16, hkdpos - 17, lastdate);
			promdetails.replace(24, 5, ",");
			int quotapos = promdetails.find("quota");
			int promlen = promdetails.length();
			price = promdetails.substr(25, quotapos-26);
			string final = enddate + "," + promdetails.substr(quotapos + 6 , promlen - 1);
			quota = promdetails.substr(quotapos + 6 , promlen - 1);
			promdetails.replace(quotapos, promlen - quotapos, final);
			if(callvalue == false)
				cout<<promdetails<<endl;
			flights fornow;
			stringstream randomest, hola;
			fornow.dest = promdetails.substr(0,3);
			fornow.airline = promdetails.substr(4,2);
			fornow.fromdate = from;
			fornow.todate = to;
			fornow.price = price;
			randomest << price;
			randomest >> fornow.pricecalc;
			fornow.lastdate = last;
			fornow.quota = quota;
			hola << quota;
			hola >> fornow.quotacalc;
			if(checking(fornow, maindb) == false)
			{
				maindb[i] = fornow;
				maindb[i].mark = true;
				i++;
			}
			else
			{
				for(int z = 0; z < i; z++)
				{
					if((maindb[z].dest == promdetails.substr(0,3)) && (maindb[z].airline == promdetails.substr(4,2)) && (maindb[z].fromdate == from) && (maindb[z].todate == to) && (maindb[z].lastdate == last) && (maindb[z].price == price) == true)
					{
						maindb[z].quotacalc = maindb[z].quotacalc + fornow.quotacalc; 
					}
				}
			}
		}
	}
	fin.close();
	if(callvalue == false)
	{
		fout.open("intermediate.txt", ios::app);
		fout << endl << "+" << "," << filename;
		fout.close();
	}
	zeroquota(maindb);
}
void makearray(flights maindb[])
{//Create a basic array from the main database.
	ifstream fin;
	string entry;
	fin.open("mainDB.csv");
	fin.clear();
	fin.seekg(0, ios::beg);
	if(fin.fail())
	{
		cout<<"Error opening main database file."<<endl;
		exit(1);
	}
	i = 0;
	getline(fin, entry);
	while(fin.good())
	{
		int fromdate = 0, todate = 0, lastdate = 0, price = 0, quota = 0;
		getline(fin, maindb[i].dest, ',');
		getline(fin, maindb[i].airline, ',');
		getline(fin, maindb[i].fromdate, ',');
		getline(fin, maindb[i].todate, ',');
		getline(fin, maindb[i].price, ',');
		stringstream temp;
		temp << maindb[i].price;
		temp >> maindb[i].pricecalc;
		getline(fin, maindb[i].lastdate, ',');
		getline(fin, maindb[i].quota, '\n');
		stringstream temp1;
		temp1 << maindb[i].quota;
		temp1 >> maindb[i].quotacalc;
		maindb[i].mark = true;
		i++;
	}
	fin.close();
	zeroquota(maindb);
}
void allpromotion(flights maindb[], bool &search)
{ // Display all promotion information.
	clearall(maindb, search);
	int r = 0;
	for(int h = 0; h < i; h++)
	{
		if(maindb[h].mark == true && maindb[h].quotacalc != 0)
		{
			cout<<"["<<r<<"]"<<" "<<maindb[h].dest<<"-"<<maindb[h].airline<<"-"<<maindb[h].fromdate<<"-"<<maindb[h].todate<<" HKD "<<maindb[h].pricecalc<<", promotion expires "<<maindb[h].lastdate<<", "<<maindb[h].quotacalc<<" quota"<<", mark: "<<maindb[h].mark<<endl;
			r++;
		}
	}
	cout<<"total "<<r<<" records."<<endl;
}
bool searchperformed(flights maindb[])
{ //Check if all records are cleared or not.
	for(int h = 0; h < i; h++)
	{
		if(maindb[h].mark == false)
			return false;
	}
	return true;
}
void changemonth(string &lastdate)
{ //Change the format of the date from 'yyyy mmm dd' to 'yyyymmdd'
	int position = lastdate.find(' ');
	position = lastdate.find(' ', position + 1);
	string calendar[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	for(int j = 0; j < 12; j++)
	{
		if(calendar[j] == lastdate.substr(5,3))
		{
			stringstream temporary;
			string month;
			temporary << j+1;
			temporary >> month;
			if(j + 1 < 10)
			{
				month = "0" + month;
			}
			lastdate.replace(4, 4, month);
		}
	}
	stringstream newval;
	int newdate;
	string date = lastdate.substr(7, 2);
	newval << date;
	newval >> newdate;
	if(newdate < 10)
	{
		date = "0" + date;
	}
	lastdate.replace(6, 3, date);
}
bool checking(flights struc, flights maindb[])
{//Check if the record exists in the database
	for(int x = 0; x < i; x++)
	{
		if((maindb[x].dest == struc.dest) && (maindb[x].airline == struc.airline) && (maindb[x].fromdate == struc.fromdate) && (maindb[x].todate == struc.todate) && (maindb[x].lastdate == struc.lastdate) && (maindb[x].pricecalc == struc.pricecalc))
		{
			return true;
		}
	}
	return false;
}
void todayflights(string line, flights maindb[])
{//Delete all files whose promotion ends before the date mentioned.
	string key1;
	string todaydate;
	stringstream temp;
	ofstream fout;
	temp << line;
	temp >> key1 >> todaydate;
	for(int j = 0; j < i; j++)
	{
		if(maindb[j].lastdate < todaydate)
		{
			//fout <<endl<< "!" <<maindb[j].dest << "," << maindb[j].airline << "," <<maindb[j].fromdate <<","<<maindb[j].todate<<","<<maindb[j].pricecalc<<","<<maindb[j].lastdate<<","<<maindb[j].quotacalc;
			for(int k = j; k < i - 1; k++)
			{
				maindb[k] = maindb[k+1];
			}
			j--;
			i--;
		}
	}
	fout.open("intermediate.txt", ios::app);
	fout << endl <<"!" <<"," <<todaydate;
	fout.close();
}
void clearall(flights maindb[], bool &search)
{//Clear all marked records and bring to initial state.
	for(int hehe = 0; hehe < i; hehe++)
	{
		maindb[hehe].mark = true;
	}
	search = false;
}
void searchdest(string line, flights maindb[], bool &search, bool &checksearch)
{//Mark records having dest as true and others as false
	string checkdest, key1, key2;
	stringstream temp;
	temp << line;
	temp >> key1 >>key2 >> checkdest;
	clearall(maindb, search);
	checksearch = searchperformed(maindb);
	if(checksearch == true)
	{
		for(int h = 0; h < i; h++)
		{
			if(maindb[h].dest == checkdest)
			{
				maindb[h].mark = true;
			}
			else
				maindb[h].mark = false;
		}
		displaystruct(maindb);
		search = true;
		checksearch = false;
	}
	else
	{
		cout<<"Wrong input. No clear performed."<<endl;
		search = false;
	}
}
void searchdate(string line, flights maindb[], bool &search, bool &checksearch)
{//Mark records having date within limits as true and others as false
	string checkdate, key1, key2;
	stringstream temp;
	temp << line;
	temp >> key1 >> key2 >> checkdate;
	clearall(maindb, search);
	checksearch = searchperformed(maindb);
	if(checksearch == true)
	{
		for(int h = 0; h < i; h++)
		{
			if(maindb[h].fromdate <= checkdate && maindb[h].todate >= checkdate)
			{
				maindb[h].mark = true;
			}
			else
				maindb[h].mark = false;
		}
		displaystruct(maindb);
		search = true;
		checksearch = false;
	}
	else
	{
		cout<<"Wrong input. No search performed. "<<endl;
		search = false;
	}
		
}
void preferairline(string line, flights maindb[], bool &search)
{//Look for matching airlines after having performed search and mark as true, others as false.
	string key1, key2, flightname;
	stringstream temp;
	temp << line;
	temp >> key1 >> key2 >> flightname;
	if(search == true)
	{
		for(int h = 0; h < i; h++)
		{
			if(maindb[h].mark == true)
			{
				if(maindb[h].airline != flightname)
				{
					maindb[h].mark = false;
				}
			}
		}
		displaystruct(maindb);
	}
	else
		cout<<"Wrong input. Search not performed."<<endl;

}
void noairline(string line, flights maindb[], bool &search)
{//Mark matching airlines as false, others as true.
	string key1, key2, flightname;
	stringstream temp;
	temp << line;
	temp >> key1 >> key2 >> flightname;
	if(search == true)
	{
		for(int h = 0; h < i; h++)
		{
			if(maindb[h].mark == true)
			{
				if(maindb[h].airline == flightname)
				{
					maindb[h].mark = false;
				}
			}
		}
		displaystruct(maindb);
	}
	else
		cout<<"Wrong input. No search performed."<<endl;
}
void buyticket(string line, flights maindb[], bought tickets[], bool &search)
{//Buy tickets based on the previously performed search using a counter.
	stringstream temp, itemp;
	string key1, serialno;
	int number, howmany;
	temp << line;
	temp >> key1 >> serialno >> howmany;
	serialno = serialno.substr(1, serialno.length());
	itemp << serialno;
	itemp >> number;
	int h = 0, hi = 0;
	bool getout = true, check = true;
	if(search == true)
		getout = true;
	else
		getout = false;
	if(getout == false)
		cout<<"Wrong input. Search not performed. "<<endl;
	while(h <= i && getout == true)
	{
		if(maindb[h].mark == true)
		{
			if(hi == number - 1)
			{
				getout = false;
				if(maindb[h].quotacalc >= howmany)
				{
					maindb[h].quotacalc = maindb[h].quotacalc - howmany;
					for(int hello = 0; hello < counted; hello++)
					{
						if(tickets[hello].aircode.find(maindb[h].dest) != -1)
						{
							tickets[hello].number += howmany;
							tickets[hello].totalprice += (maindb[h].pricecalc * howmany * 0.9); 
							ofstream fout, fileout;
							fout.open("intermediate.txt", ios::app);
							fout <<endl << "-" << maindb[h].dest << "," << maindb[h].airline << "," <<maindb[h].fromdate <<","<<maindb[h].todate<<","<<maindb[h].pricecalc<<","<<maindb[h].lastdate<<","<<howmany;
							fout << endl << "#" << tickets[hello].country << "," << tickets[hello].number << "," <<tickets[hello].totalprice;
							fout.close();
						}
					}
					cout<<howmany<<" ticket(s) purchased."<<endl;
					zeroquota(maindb);
				}
				else
				{
					cout<<"Not enough tickets."<<endl;
				}

			}
			hi++;
		}
		h++;
	}
}
void buypromotion(string line, flights maindb[], bought tickets[])
{//Buy tickets by entering all promotion information.
	int howmany = 0, rate = 0;
	stringstream temp, tem, tem1;
	temp << line;
	string place, flight, from, to, key1, rest, newstring;
	temp >> key1 >> rest >> howmany;
	char dummy;
	int start = -1;
	int position = rest.find('-');
	int leng = rest.length();
	while(position != -1)
	{
		newstring = newstring + rest.substr(start + 1, position - start - 1) + " ";
		start = position;
		position = rest.find('-', start + 1);
	}
	tem << newstring;
	tem >> place >> flight >> from >> to;
	rest = rest.substr(start + 1, leng - start);
	tem1 << rest;
	tem1 >> rate;
	for(int h = 0; h < i; h++)
	{
		if(maindb[h].dest == place && maindb[h].airline == flight && maindb[h].fromdate == from && maindb[h].todate == to && maindb[h].pricecalc == rate)
		{
			if(maindb[h].quotacalc >= howmany)
			{
				maindb[h].quotacalc = maindb[h].quotacalc - howmany;
				for(int yo = 0; yo < counted; yo++)
				{
					if(tickets[yo].aircode.find(maindb[h].dest) != -1)
					{
						tickets[yo].number += howmany;
						tickets[yo].totalprice += (maindb[h].pricecalc * howmany * 0.9);
						ofstream fout, fileout;
						fout.open("intermediate.txt", ios::app);
						fout <<endl << "-" << maindb[h].dest << "," << maindb[h].airline << "," <<maindb[h].fromdate <<","<<maindb[h].todate<<","<<maindb[h].pricecalc<<","<<maindb[h].lastdate<<","<<howmany;
						fout << endl << "#" << tickets[yo].country << "," << tickets[yo].number << "," <<tickets[yo].totalprice;
						fout.close();
						
					}
				}
				cout<<howmany<<" ticket(s) purchased."<<endl;
				zeroquota(maindb);
			}
			else
			{
				cout<<"Not enough tickets."<<endl;
			}
		}
	}
}
void mostpopular(bought tickets[])
{//Display the most popular country.
	ofstream fout, inter2;
	int max = 0;
	for(int random = 0; random < counted; random++)
	{
		if(tickets[random].number > tickets[max].number)
		{
			max = random;
		}
		//CHECKING CODE:cout<<tickets[max].country<<endl;
	}
	fout.open("most.txt");
	fout<<tickets[max].country<<endl;
	fout<<"#"<<tickets[max].number<<endl;
	fout<<"$"<<tickets[max].totalprice<<endl;
	fout.close();
}
void readinter(ifstream &fin, flights maindb[], bought tickets[])
{//Read intermediate file and perform changes on existing database using data obtained.
	string line;
	char first;
	flights now;
	bought ticket;
	getline(fin, line);
	while(fin.good())
	{
		getline(fin, line, ',');
		first = line[0];
		if(first == '-')
		{
			stringstream code, code1;
			now.dest = line.substr(1, line.length()-1);
			getline(fin, now.airline, ',');
			getline(fin, now.fromdate, ',');
			getline(fin, now.todate, ',');
			getline(fin, now.price, ',');
			getline(fin, now.lastdate, ',');
			getline(fin, now.quota);
			code << now.price;
			code >> now.pricecalc;
			code1 << now.quota;
			code1 >> now.quotacalc;
			for(int q = 0; q < i; q++)
			{
				if((maindb[q].dest == now.dest) && (maindb[q].airline == now.airline) && (maindb[q].fromdate == now.fromdate) && (maindb[q].todate == now.todate) && (maindb[q].lastdate == now.lastdate) && (maindb[q].pricecalc == now.pricecalc))
				{
					maindb[q].quotacalc = maindb[q].quotacalc - now.quotacalc;
					zeroquota(maindb);
				}
			}
		}
		else if(first == '+')
		{
			string filename;
			getline(fin, filename);
			filename = "add promo " + filename;
			bool callvalue = true;
			addpromo(filename, maindb, callvalue);
		}
		else if(first == '!')
		{
			string dateofday;
			getline(fin, dateofday);
			for(int q = 0; q < i; q++)
			{
				if(maindb[q].lastdate < dateofday)
				{
					for(int k = q; k < i - 1; k++)
					{
						maindb[k] = maindb[k+1];
					}
					q--;
					i--;
				}
			}
		}
		else if(first == '#')
		{
			ticket.country = line.substr(1, line.length()-1);
			string convert, convert1;
			getline(fin, convert, ',');
			stringstream need, want;
			need << convert;
			need >> ticket.number;
			getline(fin, convert1);
			want << convert1;
			want >> ticket.totalprice;
			for(int l = 0; l < counted; l++)
			{
				if(tickets[l].country == ticket.country)
				{
					tickets[l].totalprice = ticket.totalprice;
					tickets[l].number = ticket.number;
				}
			}		
		}
	}
}
void makestructure(bought tickets[])
{//Make the array of country and revenue from airportcode.txt
	ifstream takein, filein;
	takein.open("airportcode.txt");
	if(takein.fail())
	{
		cout<<"Error opening Airport codes file."<<endl;
		exit(1);
	}
	string code, country;
	while(takein.good())
	{
		int ran = 0;
		bool posi = true;
		getline(takein, code, ',');
		getline(takein, country, '\n');
		for(ran = 0; ran < counted; ran++)
		{
			if(tickets[ran].country == country)
			{
				tickets[ran].aircode = tickets[ran].aircode + " " + code;
				posi = false;
			}
		}
		if(posi == true)
		{
			tickets[counted].country = country;
			tickets[counted].number = 0;
			tickets[counted].aircode += code;
			tickets[counted].totalprice = 0;
			counted++;
		}
	}
	takein.close();
}
/*BONUS:
void lowestcountry(string line, flights maindb[], bought tickets[])
{//Find the lowest costing ticket to a country.
	string country, key1, key2, airlines, airport;
	int index = 0;
	stringstream temp, temp1;
	temp << line;
	bool done = true;
	temp >> key1 >> key2 >> country;
	for(int z = 0; z < counted; z++)
	{
		if(tickets[z].country == country)
		{
			airlines = tickets[z].aircode;
		}
	}
	temp1 << airlines;
	for(int z = 0; z < i; z++)
	{
		maindb[z].mark = false;
	}
	while(temp1 >> airport)
	{
		for(int z = 0; z < i; z++)
		{
			if(maindb[z].dest == airport)
			{
				maindb[z].mark = true;
			}
		}
	}
	cout<<"The flights to the country are: "<<endl;
	displaystruct(maindb);
	for(int z = 0; z < i; z++)
	{
		if(maindb[z].mark == true)
		{
			if(maindb[z].pricecalc < maindb[index].pricecalc)
			{
				index = z;
			}
		}
	}
	for(int z = 0; z < i; z++)
	{
		if(index == z)
		{
			maindb[z].mark = true;
		}
		else
		{
			maindb[z].mark = false;
		}
	}
	cout<<"The lowest price is: "<<endl;
	displaystruct(maindb);
	clearall(maindb, done);
}
void lowestairline(string line, flights maindb[])
{//Find the cheapest ticket of an airline.
	string airline, key1, key2;
	int index = 0;
	bool done = true;
	stringstream temp;
	temp << line;
	temp >> key1 >> key2 >> airline;
	for(int z = 0; z < i; z++)
	{
		if(maindb[z].airline == airline)
		{
			if(maindb[z].pricecalc < maindb[index].pricecalc)
			{
				index = z;
			}
		}
	}
	for(int z = 0; z < i; z++)
	{
		if(index == z)
		{
			maindb[z].mark = true;
		}
		else
		{
			maindb[z].mark = false;
		}
	}
	displaystruct(maindb);
	clearall(maindb, done);
}
*/
