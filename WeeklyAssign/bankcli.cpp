#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <cmath>
#include "AccountData.pb.cc"
#include <sstream>
#include <chrono>
#include <ctime>



class account;		//holds all the data associated with a single customer account. Currently, just name, ssn,ssn last four, date of creation, and account number;
class accountbank;	//this functions as a hybrid database/ database interface. Ideally, we'd have something like an SQL database or other quality db system, and accountbank would just serve as an interface helper class
class logger;

std::string LogIn(accountbank* db, logger* log);
bool CheckCred(std::string i_user, std::string i_pass, accountbank* db);
void ShowAccounts(accountbank* db);

std::string DisplayAccount(accountbank* db, logger* log);	//return type std::string allows me to carry a "quit" command through the function and into the main menu
std::string SearchName(accountbank* db, logger* log);
std::string NewAccount(accountbank* db, logger* log);
std::string CloseAccount(accountbank* db, logger* log);


int Exponentiate(int base, int power);
std::string NumberToString(int input);
int StringToNumber(std::string number);
float StringWithDecimalToNumber(std::string number);
std::string GetDate();

const int DISPLAYCOUNT = 5;	//this is the maximum number of names displayed in response to a search
const char FILENAME[] = "Totaly Secure Data";
const char LOGNAME[] = "banklog.txt";

//this class holds all data associated with a given account. can be expanded later if an account is intended to hold more data or perform more functions
class account
{
	protected:
		std::string ssn = "";	//social security number, stored as a string
		std::string ssn4 = "";	//last four digits of ssn
	public:
		friend class accountbank;

		std::string custname = "";	//customer name
		std::string opendate = "";	//date account opened
		std::string numb = "";		//account number. Also index of accountbank's alist where account is stored
		float balance = 0;
		
		account(std::string i_custname, std::string i_ssn, std::string i_opendate, std::string i_numb, float i_balance)
		{
			custname = i_custname;
			ssn = i_ssn;
			opendate = i_opendate;
			numb = i_numb;
			if(ssn.length() <= 4)
			{
				ssn4 = ssn;
			}
			else
			{
				unsigned int ssnend = ssn.length() - 4;
				assert(ssnend > 0 );	//shouldn't hit this block if this is untrue
				ssn4 = ssn.substr(ssnend);
			}

			balance = i_balance;
		}

		void Display()
		{
			std::cout << "Customer Name: " << custname << ", Account Number: " << numb << ", Date Opened: " << opendate << std::endl;
		}
		
		void DisplayMore()
		{
			std::cout << "Customer Name: " << custname << ", SSN: "<< ssn4 << ", Account Number: " << numb << ", Date Opened: " << opendate << ", Account Balance: $" << balance << std::endl;
		}
};


//this class records data to a text file while optionally also sending it to the console
class logger
{
public:
	std::ofstream log;
	bool okay = true;
	logger()
	{
		log.open(LOGNAME, std::ios::app);
		
		if(!log.is_open())
		{
			std::cout << "Error opening log file." << std::endl;
			okay = false;
		}
	}

	~logger()
	{
		if(log.is_open())
		{
			log.close();
		}
	}
	//includes datestamp, message, and endline
	void Send(std::string message)
	{
		log << GetDate() << '\t' << message << std::endl;
		std::cout << message << std::endl;
		return;
	}
	//no endline
	void SendStartLine(std::string message)
	{
		log << GetDate() << '\t' << message;
		std::cout << message;
		return;
	}
	//no date or endline
	void SendMidLine(std::string message)
	{
		log << message;
		std::cout << message;
		return;
	}
	//no date
	void SendEndLine(std::string message)
	{
		log << message << std::endl;
		std::cout << message << std::endl;
		return;
	}
	//quiet send. does not cout
	void QSend(std::string message)
	{
		log << GetDate() << '\t' << message << std::endl;
	}
	//no endline
	void QSendStartLine(std::string message)
	{
		log << GetDate() << '\t' << message;
		return;
	}
	//no date or endline
	void QSendMidLine(std::string message)
	{
		log << message;
		return;
	}
	//no date
	void QSendEndLine(std::string message)
	{
		log << message << std::endl;
		return;
	}
};


//This thing functions as a hybrid database/database interface. In a proper version, we'd probably be using an actual database we can encrypt and such, and accountbank would instead be just the interface, ideally using the same functions currently in use to provide a transition that requires no re-writing of external functions
class accountbank
{
	private:
		std::vector<account> alist;
	public:
		friend class account;
	
	bank::ProtAccountList acc_list;	// declare the account list to interact with the protobuf file
	unsigned int account_num_tracker = 0;
	std::string user;
	std::string pass;

	accountbank()
	{	
		//with protobuff this content is unused. Might modify later to automate the datagathering. might not

		//this is where all the "pre-existing accounts" live. make more. or don't.
		//alist.push_back(account("Joe Blow", "555555555", "Dec 12, 2021", "0"));
		//alist.push_back(account("Bob Parsons", "555555532", "Dec 12, 2021", "1"));
		//alist.push_back(account("Bobby Parsons", "555442255", "April 22, 2021", "2"));
		//alist.push_back(account("Bob Parson", "555225576", "Jan 14, 2001", "3"));
		//alist.push_back(account("Steve Blob", "555552323", "Dec 11, 2012", "4"));


	}

	//load to accountbank all the data held in the ProtAccountList
	void LoadFromProto()
	{
		//clear list of accounts before loading from file. Should already be empty, but this doesn't hurt anything
		alist.clear();

		//acc_list.ParseFromIstream() method should be called in main before calling this method. Not sure what will happen if that does not occur, but bad
		for (int i = 0; i < acc_list.protaccount_size(); i++)
		{
			//access one stored protoaccount
			const bank::ProtAccount& pracc = acc_list.protaccount(i);

			//create an account object populated with the data from the protoaccount
			account n_account(pracc.custname(), pracc.ssn(), pracc.opendate(), pracc.numb(), pracc.balance());
			
			//this section for prototyping only
			//if(n_account.opendate.back() == '\n')
			//{
			//	n_account.opendate.pop_back();
			//	std::cout << "Yes, the last char was a newline" << std::endl;
			//}
			
			//add to vector of current accounts
			alist.push_back(n_account);
		}
		
		account_num_tracker = acc_list.highest_acc();
		
		user = acc_list.creds().user();
		pass = acc_list.creds().pass();

		//alist should now contain all accounts that were stored in the file 
	}//end LoadFromProto()

	//output all the accounts
	void ShowAccounts()
	{
		for (unsigned int i = 0; i < alist.size(); i++)
		{
			alist.at(i).Display();
		}
	}
	
	//assign a score to each account to grade its similarity to a search term
	unsigned int ScoreName(std::string s_name, account subject)
	{
		unsigned int score = 0;
		if (subject.custname == s_name)
		{
			score = 9999; //this should exceed any other score 
			return score;
		}
		//implicit else
		unsigned int lastmatch = 0;
		for (unsigned int i = 0; i < s_name.length(); i++)	//for each letter of n_name
		{
			for (unsigned int j = lastmatch ; j < subject.custname.length(); j++) //compare against letters of subject name past the last matched location
			{
				if ( s_name[i] == subject.custname[j])	//if the letters match
				{
					score++;	//add one point to the score
					lastmatch = j + 1;	//save the location of the account name letter so next iteration starts past it
					break;	//exit this loop, advance to next letter of n_name to resume searching
				}
			}
		}

		return score;	//more letters matching in order makes a higher score
	}//end ScoreNames

	std::vector<account*> SearchNames(std::string s_custname)
	{
		std::vector<std::pair<int, unsigned int>> refscore;	//refscore = {index, score}
		for (int i = 0; i < alist.size(); i++)
		{
			std::pair<int, unsigned int> score;
			score.first = i;
			score.second = ScoreName(s_custname, alist.at(i));
			if (score.second >= 1 )		//if score is at least 1
			{
				if (refscore.size() == 0)	//if the vector is empty, create an element
				{
					refscore.push_back(score);
				}
				else	//if the vector is NOT empty, find where that score belongs
				{
					for (unsigned int j = 0; j < refscore.size(); j++)	//compare the score of the current name against the ones already on the list
					{
						if (refscore.at(j).second <= score.second)
						{
							refscore.insert((refscore.begin() + j), score);	//put new element in appropriate location
							if(refscore.size() > DISPLAYCOUNT)
							{
								refscore.pop_back();	//keep it limited to DISPLAYCOUNT
							}

							break;
						}
					}
				}
			}
		}

		std::vector<account*> returnvect;

		for (int i = 0; i < refscore.size(); i++)
		{
			returnvect.push_back(&alist.at(refscore.at(i).first));	//grab the index from element i of refscore, use that index to fetch the appropriate account from alist, pass the address of that account to returnvect as a new element. repeat for each element of refscore
				
		}

		return returnvect; //should be a set of DISPLAYCOUNT addresses for accounts with the best name matches 
	}//end SearchNames

	std::pair<account*, unsigned int> GetAccount(std::string n_numb)
	{
		for(unsigned int i = 0; i < alist.size(); i++)	//go through all accounts
		{
			if (alist.at(i).numb == n_numb)		//if the account number found matches the desired number
			{
				return {&alist.at(i), i};		//return a reference to that account and the index of that account
			}
		}
		//number not found
		return {NULL, 0};
	}


	std::string NewAccount(std::string n_custname, std::string n_ssn)
	{
		std::string returnstring = "";
		unsigned int size = alist.size();
		for (unsigned int i = 0; i < size; i++)
		{
			if ((alist.at(i).custname == n_custname)||(alist.at(i).ssn == n_ssn))
			{
				returnstring = "Account exists";
				return returnstring;
			}
		}
		
		//if no existing account match
		std::string newnum;
		if (size == 0)
		{
			//if there's no accounts, start at account number 1;	
			newnum = NumberToString(1);
		}
		else
		{
			account_num_tracker++;	//new account, increment tracker count
			newnum = NumberToString(account_num_tracker); 
		
			//oldnumb = alist.back().numb;	//old version. Boo! Hiss!
		}

		std::string newdate = GetDate();
		alist.push_back(account(n_custname, n_ssn, newdate, newnum, 0 ));	//new accounts start with a zero balance


		//new stuff goes here
		

		// *** This part is handled in this->Quit()
		// this should return a bank::account*	
		//bank::ProtAccount* new_account;			//ProtAccount because that's the type of pointer being made
		//new_account = acc_list.add_protaccount();	//protaccount because that's what the object was named inside the ProtAccountList
		//new_account->set_custname(n_custname);
		//new_account->set_numb(s_newnum);
		//new_account->set_opendate(newdate);
		//new_account->set_ssn(n_ssn);
		//new_account->set_balance(0); 	//starts empty

		//
		returnstring = "Account Created";

		return returnstring;
	}

	void CloseAccount(std::string d_numb)
	{
		std::pair<account*, unsigned int> gotaccount = GetAccount(d_numb);
		account* isvalid = gotaccount.first;
		unsigned int delete_index = gotaccount.second;

		if(isvalid != NULL)
		{
			alist.erase((alist.begin() + delete_index ), (alist.begin() + delete_index + 1));	//alist.begin() is used to create an iterator, then the addition used to offset from the first position. stops deleting elements at one location past the delete point
		//protobuf portion of delete handled in this->Quit()
		}
	}


	//this clears all data stored in the protobuff file, then stores all data from the accountbank into the new file.
	void Quit()
	{
		acc_list.Clear();	//I *think* this part is optional, but I like being double-safe when the risks are minimal. Second thought- actuall, I don't think it is, since I'm just adding new accounts, and I don't want to duplicate any.

		//push all accounts to the list in the protobuf file
		for(int i = 0; i < alist.size(); i++)
		{
		bank::ProtAccount* new_account;			//ProtAccount because that's the type of pointer being made
		new_account = acc_list.add_protaccount();	//protaccount because that's what the object was named inside the ProtAccountList
		new_account->set_custname(alist.at(i).custname);
		new_account->set_numb(alist.at(i).numb);
		new_account->set_opendate(alist.at(i).opendate);
		new_account->set_ssn(alist.at(i).ssn);
		new_account->set_balance(alist.at(i).balance);
		}
		
		acc_list.set_highest_acc(account_num_tracker);
		//bank::Creds* testcred = acc_list.creds();
		//testcred->set_user(user);
		acc_list.mutable_creds()->set_user(user);
		acc_list.mutable_creds()->set_pass(pass);

	}

};

int main()
{

	GOOGLE_PROTOBUF_VERIFY_VERSION;	//version test. not clear on failure behavior.
	//declare protobuf interface object. this item also owns the protobuf interface "acc_list"	
	accountbank not_database;
	logger captains_log;

	if(!captains_log.okay)
	{
		return -1;
	}
//	std::ofstream captains_log;
//	captains_log.open(LOGNAME, std::ios::app);
//	if(!captains_log.is_open())
//	{
//		std::cout << "Error opening log file." << std::endl;
//		return -1;
//	}
	
	std::ifstream protoin;
	protoin.open(FILENAME, std::ios::in | std::ios::binary);
	if(!protoin.is_open())
	{
		//std::cout << "No account data exists. Creating now.." << std::endl;
		captains_log.Send("No account data exists. Creating now..");
	}
	else
	{
	
		captains_log.Send("Loading account data...");
		//std::cout << "Loading account data..." << std::endl;
		
		//get accounts data
		if(!not_database.acc_list.ParseFromIstream(&protoin))
		{
			captains_log.Send("Error loading account data");
			//std::cout << "Error loading account data" << std::endl;
			//captains_log << GetDate() << "\tError loading account data" << std::endl;
			//quit out because data was not read
			return -1;
		}

		//if file opened and there was no issue parsing the data, load the accountbank with all the accounts from the ProtAccountList
		not_database.LoadFromProto();
		captains_log.Send("Done");
		//std::cout << "Done." << std::endl;
		//captains_log << GetDate() << "\tDone." << std::endl;
		
	}
		
	//login
	if(LogIn(&not_database, &captains_log) == "quit")
	{
		return 0;
	}

	//"home console prompt"
	std::string command = "";

	while(true)
	{
	
		std::cout << "home > ";
		std::getline(std::cin, command);	//there's a bit that skips the "wait for input" part of this on loop one. not sure how to fix yet
		captains_log.QSend(command);
		if (command == "show accounts")
		{
			ShowAccounts(&not_database);
		}
		else if (command == "display account")
		{
			command = DisplayAccount(&not_database, &captains_log);
		}
		else if (command == "search name")
		{
			SearchName(&not_database, &captains_log);
		}
		else if (command == "new account")
		{
			NewAccount(&not_database, &captains_log);
		}
		else if (command == "close account")
		{
			CloseAccount(&not_database, &captains_log);
		}
		else if (command == "help")
		{
			std::cout << "show accounts\n" << "display account\n" << "search name\n" << "new account\n" << "close account\n" << "help\n" << "quit" << std::endl;
		}
		else if (command == "exit")
		{
			captains_log.Send("Invalid instruction. You may have meant \"quit\".");
		}
		else if (command != "quit")
		{
			captains_log.Send("Invalid instruction. Type \"help\" for a list of valid instructions.");
		}//is the lack of an else statement here what is preventing the command == "quit" check?
	
		//this check is made separately in case one of the called functions set command = "quit", as DisplayAccount is intended to do under certain conditions
		if (command == "quit")
		{
			not_database.Quit();
			
			std::ofstream protoout(FILENAME, std::ios::out | std::ios::trunc | std::ios::binary);
			if (!not_database.acc_list.SerializeToOstream(&protoout))
			{
				captains_log.Send("It did done break while writing output");
			}
			protoout.close();
			protoin.close();
			break;
		}
	
	}//endwhile(true)

	//clean up all global variables from the protobuff interface
	google::protobuf::ShutdownProtobufLibrary();
	return 0;
}


std::string LogIn(accountbank* db, logger* log)
{
	bool valid = false;
	std::string user, pass;
	unsigned int count = 0;
	while(valid!= true)
	{
		std::cout << "Username: " << std::endl;
		std::getline(std::cin, user);
		
		log->QSendStartLine("Login attempt by user: ");
		log->QSendEndLine(user);
		//can't get trapped in infinite loop due to lack of valid credentials;
		if(user == "quit")
		{
			return user;
		}

		std::cout << "Password: " << std::endl;
		std::getline(std::cin, pass);
		valid = CheckCred(user, pass, db);	//this should be some fancy encrypted high-security process. Instead it just checks against a single user/pass combination explicitly defined in the current implementation of CheckCred
		if (!valid)
		{
			std::cout << "Incorrect User" << std::endl;
			if(count >= 3)
			{
				if (count > 10)
				{
					std::cout << "Too many attempts" << std::endl;
					log->QSendStartLine("Rejected after ");
					log->QSendMidLine(NumberToString(count));
				       	log->QSendEndLine(" login attempts");
					user = "quit";
					return user;
				}
				std::cout << "Enter Username \"quit\" to quit" << std::endl;
			}
			
		}
		count++;
	}
	//std::getline(std::cin, user); //to catch and hopefully clear the dangling '/n' char
	log->QSendStartLine("Success after ");
	log->QSendMidLine(NumberToString(count));
	log->QSendEndLine(" login attempts");
	user = ""; //clear data
	return user; //because all control paths must return data, but they don't all have to return important data
}

bool CheckCred(std::string i_user, std::string i_pass, accountbank* db)
{
	//this is mostly a placeholder for a real way to deal with credentials
	bool valid = false;
	if(i_user == db->user)
	{
		if (i_pass == db->pass)
		{
			valid = true;
		}
	}
	return valid;
}


//shows all accounts, one line per account, low amount of info per account
void ShowAccounts(accountbank* db)
{
	db->ShowAccounts();
}



//displays a single account chosen by account number. high amount of info. allows credit/debit transactions
std::string DisplayAccount(accountbank* db, logger* log)
{
	std::string command = "";
	log->SendStartLine ("account > ");
	std::cin >> command;
	log->QSendEndLine(command);
	account* acc = db->GetAccount(command).first;
	std::getline(std::cin, command); //to catch and clear the dangling newline in the input stream
	if (acc == NULL)
	{
		log->Send("Invalid account number");
		command = "";
		return command;
	}

	while(command != "home")
	{
		acc->DisplayMore();

		log->QSendStartLine("View Account: ");
		log->QSendEndLine(acc->custname);

		log->SendStartLine("add transaction > home > ");

		std::getline(std::cin, command);

		log->QSendEndLine(command);

		if (command == "add transaction")
		{
			while (command != "home")
			{
				log->SendStartLine("debit > credit > home > ");
				std::getline(std::cin, command);
				log->QSendEndLine(command);
				float amount = 0;
				if (command == "debit")
				{
					log->SendStartLine("amount > ");
					std::getline(std::cin, command);
					log->QSendEndLine(command);
					amount = StringWithDecimalToNumber(command);
					if (acc->balance >= amount)
					{
						acc->balance -= amount;
						log->SendStartLine("New balance: ");
						log->QSendEndLine(NumberToString(floor(acc->balance)));	//log will lose the fractional dollars, this is mostly fine, since its just a tracker
						std::cout  << acc->balance << std::endl; //loop already handles doing this, but inner loop hides it
					}
					else
					{
						log->Send("Insufficient Balance");
					}

				}
				else if (command == "credit")
				{
					log->SendStartLine("amount > ");
					std::getline(std::cin, command);
					log->QSendEndLine(command);
					amount = StringWithDecimalToNumber(command);
					acc->balance += amount;
					log->SendStartLine("New balance: ");
					log->QSendEndLine(NumberToString(floor(acc->balance)));
					std::cout << acc->balance << std::endl; //loop already handles doing this, but inner loop hides it
				}
				else if(command == "quit")	//because sometimes I don't want to deal with intermediate menus
				{
					log->QSend(command);
					return command;	//this *should* allow for direct quit of the program. It *will* allow for direct quit of the program before I'm done with it
				}
				else if (command != "home")
				{
					std::cout << "Invalid instruction, please try again." << std::endl;
				}
			}//endwhile(inner command != "home")	//loops back up to "debit > credit > home > "
		}//if transaction menu was accessed, skips these checks, proceeds to loop back to DisplayMore()
		else if (command == "quit")	//because sometimes I don't want to deal with intermediate menus
		{
			log->QSend(command);
			return command;
		}
		else if(command != "home")
		{
			std::cout << "Invalid Instruction" << std::endl;
			command = "";	//purging whatever junk was here
			return command;
		}
	}//endwhile(command != "home")	//loops back up to DisplayMore()
			
	//assert(command != "quit");	// if command contains "quit" should never reach this line. Verifying this isn't the reason that doesn't work	//asserts probably shouldn't be left in final code
	//purging whatever junk was here. *Should* have "home" in it, but this just makes sure there's no weirdness
	command = "";	

	return command;
}


std::string SearchName(accountbank* db, logger* log)
{
	const std::string romnum[] = {"i" , "ii", "iii", "iv", "v" , "vi"};
	std::string command;

	while(1)
	{
		log->SendStartLine("enter name > ");
		std::getline(std::cin, command);
		log->QSendEndLine(command);
		std::vector<account*> list = db->SearchNames(command);
		unsigned int size = list.size();
		unsigned int index = 0;
		for (unsigned int i = index; i < size; i++)
		{
			log->SendStartLine(romnum[i]);
		       	log->SendMidLine(". ");
			log->SendEndLine(list.at(i)->custname);
			index = i;
		}
		log->SendStartLine(romnum[index + 1]);
	       	log->SendEndLine(". Home");
	       	log->SendStartLine("Select number > ");	//"Home" is capitalized here because that's how it was shown in the spec
		std::getline(std::cin, command);
		log->QSendEndLine(command);
		
		if((command == "Home")||command == "home")	//capitalized version allowed here because that's how it was shown in the spec
		{
			command = "";
			return command;
		}
		else if (command == "quit")
		{
			return command;
		}
		else if(command.length() == 1)
		{
			if((command[0] >= '1')&&(command[0] <= '9'))	//ensure the character in input[0] is a numeral
			{
				int selection = command[0] - '0';	//convert a single digit number in a string into an appropriate integer value
				selection--; //this is to convert from the 1-indexed display to the zero-indexed list
				if (selection < size)
				{
					list.at(selection)->Display();
					log->QSendStartLine("View Account: ");
					log->QSendEndLine(list.at(selection)->custname);
					break;
				}
				else if (selection == size)	//this is the numerical value assigned to "home"
				{
					break;
				}
				else
				{
					std::cout << "invalid number" <<std::endl;
					continue; //returns to the "enter name" prompt
				}
			}
			else	//if the entry is not "Home", "home", "quit", nor a single-character string
			{
				std::cout << "invalid number" <<std::endl;	//returns to the "enter name" prompt
			}
		}
	}
	//std::getline(std::cin, input);	//again, to catch the dangling newline
	command = "";
	return command;
}

std::string NewAccount(accountbank* db, logger* log)
{

	std::string n_name = "", n_ssn = "";

	//GET NAME
	log->SendStartLine("Customer Name > ");
	std::getline(std::cin, n_name);
	log->QSendEndLine(n_name);
	//because I like being able to leave
	if(n_name == "quit")
	{
		return n_name;
	}

	for (int i = 0; i < n_name.length(); i++)	//make sure all characters are spaces or letters
	{
		if (n_name[i] == ' ')
		{
			continue;
		}
		else if ((n_name[i] >= 'A') && (n_name[i] <= 'Z'))
		{
			continue;
		}
		else if ((n_name[i] >= 'a') && (n_name[i] <= 'z'))
		{
			continue;
		}
		else	//could leave this as implicit, but that looked suspicious and accidental
		{
			log->Send("Invalid Name");
			n_name = "";	//clear data because we don't really want to return a name, particularly an invalid one
			return n_name;
		}
	}



	// GET SSN
	unsigned int ssn_size;
	log->SendStartLine("SSN > ");
	std::getline(std::cin, n_ssn);
	log->QSendEndLine("SSN hidden");
	//because I like being able to leave
	if(n_ssn == "quit")
	{
		return n_ssn;
	}
	
	
	for (unsigned int i = 0; i < n_ssn.length(); i++)	//make sure all characters are numeric	//gotta keep re-calculating size each loop because it may change
	{

		if ((n_ssn[i] >= '0') && (n_ssn[i] <= '9'))
		{
			continue;
		}
		else if(n_ssn[i] == ' ')
		{
			n_ssn.erase(i,1);	//erase the space character, but then proceed
			i--;	//decrement i so that the next pass checks the new character that is now in position i after it increments at the end of loop
			continue;
		}
		else	//could leave this as implicit, but that looked suspicious and accidental
		{
			log->Send("Invalid SSN");
			n_ssn = "";	//clear data, because we don't ACTUALLY want to return a SSN, even if there's something imperfect about it
			return n_ssn;
		}

	}

	//if it's too short or long after the spaces are gone, its invalid	
	if(n_ssn.length() != 9)
	{
		log->Send("Invalid SSN");
		n_ssn = "";	//clear data, because we don't ACTUALLY want to return a SSN, even if there's something incorrect about it
		return n_ssn;
	}
		//n_ssn.resize(9); //truncates ssn to 9 digits, because the specification said to limit it. //min/max size check makes this redundant


	//CHECK IF NAME OR SSN EXIST
	log->Send(db->NewAccount(n_name, n_ssn));

	n_name = "";	//clear data because we don't really want to return a name, particularly an invalid one
	return n_name;
//	std::getline(std::cin, n_name);	//again, to catch the dangling newline
}

std::string CloseAccount(accountbank* db, logger* log)
{
	std::string command;
	account* to_delete;
	log->SendStartLine("account > ");
	std::getline(std::cin, command);
	log->QSendEndLine(command);
	to_delete = db->GetAccount(command).first;

	if (to_delete != NULL)
	{
		log->QSendStartLine("Account to close: ");
		log->QSendEndLine(to_delete->custname);

		char confirm = static_cast<char>(NULL);
		std::cout << "Are you sure you wish to close this account? (y/n) > ";
		std::cin >> confirm;
		if(confirm == 'y')
		{
			db->CloseAccount(command);
			log->Send("Account closed");
			std::getline(std::cin, command);	//again, to catch the dangling newline
			command = "";
			return command;
		}
		else
		{
			std::getline(std::cin, command);	//again, to catch the dangling newline
			log->Send("Account not closed");
		}
	}
	else
	{
		log->Send("Account not found");
	}

	command = "";
	return command;
}

//on second thought, I don't think I actually need this function for this assignment
//only works for positive integer powers (including 0) and integer bases
int Exponentiate(int base, int power)
{
	int accumulator = 1;	//anything to the zero-th power = 1
	for (int i = 1; i <= power; i++)
	{
		accumulator *= base;
	}
	return accumulator;
}

std::string NumberToString(int input)
{
	std::string accumulator = "";
	int destructible = input;
	while(destructible != 0)
	{
		accumulator = static_cast<char>((destructible % 10) + '0') + accumulator;
		destructible /= 10;
	}
	return accumulator;
}

int StringToNumber(std::string number)
{ 
	int accumulator = 0;
        int length = static_cast<int>(number.length()) - 1;
        for (int i = 0; i <= length; i++)
        {
            int index = length - i;
            int temp = static_cast<int>(number[index]) - static_cast<int>('0');
            
            if ((temp > 9) || (temp < 0))
            {
                continue;
            }

            int j = 0;

            while (j < i)
            {
                temp = temp * 10;
                j++;
            }
            accumulator = accumulator + temp;
        }

        return accumulator;
    }


float StringWithDecimalToNumber(std::string number)
{
	int decimal_place = -1;
	for (unsigned int i = 0; i < number.length(); i++)
	{
		if (number[i] == '.')
		{
			decimal_place = i;
			break;
		}
	}
	
	std::string int_part_string, frac_part_string;
	int whole_part;
	float frac_part;

	if (decimal_place > 0)
	{
		int_part_string = number.substr(0, decimal_place);	//dont include the decimal itself //second parameter is a number of characters, not an index, so the correct value is decimal place, NOT (decimal_place - 1)
		
		if(decimal_place + 1 < number.length())
		{
			frac_part_string = number.substr(decimal_place + 1);	//don't include the decimal, get the rest of the string 
		}
		else
		{
			frac_part_string = "0";
		}

		whole_part = StringToNumber(int_part_string);
	}
	else if (decimal_place == 0)
	{
		frac_part_string = number.substr(decimal_place + 1);	//don't include the decimal, get the rest of the string 
		whole_part = 0;
	}
	else
	{
		frac_part_string = "0";//no decimal place detected. either a whole number or trash data. Numeric check it? - StringToNumber() does that already
		whole_part = StringToNumber(number);	//if not pure numeric, will give bad data, but not break
	}

	//make frac_part into numbers
	frac_part = StringToNumber(frac_part_string);
	
	//re-scale numbers to be fractional in value
	while(frac_part >= 1.f)
	{
		frac_part/= 10;
	}
	
	//add the two parts together
	frac_part += whole_part;
	//std::cout << "StringWithDecimalToNumber output: " << frac_part << std::endl; //this line mostly for troubleshooting
	return frac_part;	
}
//there's probably a better method, but I doubt anyone will even notice that I did this at all, and it DOES work.
std::string GetDate()
{
	//Get current time
	std::time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	//create an object that can accept << operation
	std::stringstream timestream;
	//load saved time to object
	timestream << std::ctime(&now_time);

	//std::cout << timestream.str() << std::endl;	//this line for testing only
	
	//return a string holding the stream object's data
	std::string timestring = timestream.str();
	if (timestring.back() == '\n')
	{
		timestring.pop_back();	//this format seems to tack a '\n' to the end  of the string for some reason. clearing it if its there (which seems to be 100%)
	}

	return timestring;
}
