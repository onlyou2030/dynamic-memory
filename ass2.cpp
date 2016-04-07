/***********************************************************************************
 * CSCI 124 - ass2.cpp - Contains function definitions for pet database program
 * Put you name, login and the date last modified here.
 *
 ***********************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cstring>
using namespace std;

// ============== Constants ==========================================

const char cDataFileName[] = "pets.txt";
const int cMaxRecs = 100;
const int cMaxChars = 30;


// ============= User Defined types ==================================

enum   StatusType{Lost,Found};
enum   PetType{Dog,Cat};
enum   GenderType{Male,Female,Unknown};
struct AgeType{ int Yrs; int Mths;}; // -1 if unknown

struct PetRecord{
	StatusType Status;           // Lost or Found
	PetType Type;                // Dog or Cat
	GenderType Gender;           // Male, Female or Unknown
	char Breed[cMaxChars];       // Bread of dog or cat. eg Bull terrier
	AgeType Age;                 // Age in years and mths. (-1 -1 if unknown)
	char Colour[cMaxChars];      // Colour of pet. eg tan
	char Location[cMaxChars];    // Suburb where found or lost. eg Mt Keira
	char PhoneNo[cMaxChars];     // Ph. of person to contact
};


// ============= Global Data =========================================

PetRecord *gPetRecs[cMaxRecs];
int gNumRecs=0;

// ============= Private Function Prototypes =========================

void DisplayRecord(int i);           // Displays record i on screen
void LowerChar(char &c);             //lower a character
void LowerString(char str[]);        //lower a string
void DeleteOnePet(int i);            //delete a pet by the index in array
void FormatColourString(char str[]); //change colour string to standard format
void WriteFile(ofstream &fout,int i);//write the i+1 record to the file

// ============= Public Function Definitions =========================

void ReadFile()
{
	ifstream fin;
	fin.open(cDataFileName);
	if (!fin.good()){
		cerr << "Could not open file!\n";
		exit(1);
	}
	gNumRecs=0;
	int i;
	for(i=0;i<cMaxRecs;i++){
		gPetRecs[i]=new PetRecord;    
		char Tmp[cMaxChars];
		fin >> Tmp;
		if(fin.fail())break;
		switch(Tmp[0]){
			case'l': gPetRecs[i]->Status = Lost; break;
			case'f': gPetRecs[i]->Status = Found;break;
		}
		fin >> Tmp;
		switch(Tmp[0]){
			case'c': gPetRecs[i]->Type = Cat; break;
			case'd': gPetRecs[i]->Type = Dog; break;
		}
		fin >> Tmp;
		switch(Tmp[0]){
			case'm': gPetRecs[i]->Gender = Male;    break;
			case'f': gPetRecs[i]->Gender = Female;  break;
			default: gPetRecs[i]->Gender = Unknown; break;
		}
		fin.ignore(1,'\n'); //eat tailing '\n'
		fin.getline(gPetRecs[i]->Breed,cMaxChars);
		fin >> gPetRecs[i]->Age.Yrs>>gPetRecs[i]->Age.Mths;
		fin.ignore(1,'\n'); //eat tailing '\n'
		fin.getline(gPetRecs[i]->Colour,cMaxChars);
		fin.getline(gPetRecs[i]->Location,cMaxChars);
		fin.getline(gPetRecs[i]->PhoneNo,cMaxChars);
	}
	gNumRecs=i;
	fin.close();
	cout<< "\nThere are "<< gNumRecs <<" records in the Lost and Found Pet Database\n";
}

void DisplayRecs()
{
	for(int i=0;i<gNumRecs;i++){
		DisplayRecord(i);
		cout<<"Display next record (y/n)> ";
		char Ans;
		cin>>Ans;
		cout<<endl;
		if(Ans=='n') return;
	}
}

void AddRecord()
{
	gPetRecs[gNumRecs]=new PetRecord;  
	char inputChar;  
	cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	cout<<"+               Add Record to Database                +"<<endl;
	cout<<"+              (Enter your pets details)              +"<<endl;
	cout<<"+         (Enter ? or -1 if detail is unknown)        +"<<endl;
	cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	cout<<"Is the pet lost or found?   (l/f) => ";
	cin>>inputChar;
	switch(inputChar){
			case'l': gPetRecs[gNumRecs]->Status = Lost; break;
			case'f': gPetRecs[gNumRecs]->Status = Found;break;
		}
	cout<<"Is the pet a dog or cat?    (d/c) => ";
	cin>>inputChar;
	switch(inputChar){
			case'c': gPetRecs[gNumRecs]->Type = Cat; break;
			case'd': gPetRecs[gNumRecs]->Type = Dog; break;
		}
	cout<<"What gender is the pet?     (m/f) => ";
	cin>>inputChar;
	switch(inputChar){
			case'm': gPetRecs[gNumRecs]->Gender = Male;    break;
			case'f': gPetRecs[gNumRecs]->Gender = Female;  break;
			default: gPetRecs[gNumRecs]->Gender = Unknown; break;
		}
	cout<<"What breed is the pet?            => ";
	cin.ignore(); //eat tailing '\n'
	cin.getline(gPetRecs[gNumRecs]->Breed,cMaxChars);
	LowerString(gPetRecs[gNumRecs]->Breed);  
	cout<<"What age is the pet?     (yy mm ) => ";
	cin >> gPetRecs[gNumRecs]->Age.Yrs>>gPetRecs[gNumRecs]->Age.Mths;
	cout<<"What colour is the pet?           => ";
	cin.ignore(); //eat tailing '\n'
	cin.getline(gPetRecs[gNumRecs]->Colour,cMaxChars);
	LowerString(gPetRecs[gNumRecs]->Colour); 
	FormatColourString(gPetRecs[gNumRecs]->Colour); 
	cout<<"In what suburb was the pet lost?  => ";
	cin.getline(gPetRecs[gNumRecs]->Location,cMaxChars);
	LowerString(gPetRecs[gNumRecs]->Location);  
	cout<<"What is your phone number?        => ";
	cin.getline(gPetRecs[gNumRecs]->PhoneNo,cMaxChars);
	gNumRecs++;
	cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	cout<<"+     A new record has been added to the database     +"<<endl;
	cout<<"+         "<<setw(3)<<gNumRecs<<" records are in the database             +"<<endl;
	cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;

	ofstream fout;
	fout.open("pets.txt", ios::app);
	if (!fout.good()){
		cerr << "Could not open file!\n";
		exit(1);
	}
	WriteFile(fout,gNumRecs-1);
	fout.close();
}

void SearchArray()
{
	cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	cout<<"+                 Search Database                     +"<<endl;
	cout<<"+              (Enter your pets details)              +"<<endl;
	cout<<"+         (Enter ? or -1 if detail is unknown)        +"<<endl;
	cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;

	char tempStatus;
	char tempType;
	char tempGender;
	
	PetRecord *temp=new PetRecord;
	cout<<"Is the pet lost or found?   (l/f) => ";
	cin>>tempStatus;
	cout<<"Is the pet a dog or cat?    (d/c) => ";
	cin>>tempType;
	cout<<"What gender is the pet?     (m/f) => ";
	cin>>tempGender;
	cout<<"What breed is the pet?            => ";
	cin.ignore(); //eat tailing '\n'
	cin.getline(temp->Breed,cMaxChars);
	LowerString(temp->Breed);   
	cout<<"What age is the pet?     (yy mm ) => ";
	cin >> temp->Age.Yrs>>temp->Age.Mths;
	cout<<"What colour is the pet?           => ";
	cin.ignore(); //eat tailing '\n'
	cin.getline(temp->Colour,cMaxChars);
	LowerString(temp->Colour);   
	FormatColourString(temp->Colour);  
	cout<<"In what suburb was the pet lost?  => ";
	cin.getline(temp->Location,cMaxChars);
	LowerString(temp->Location);  

	int i=0;
	while(i<gNumRecs)
	{
		if(
			(tempStatus!='?' && tempStatus!=(gPetRecs[i]->Status==Lost?'l':'f'))||
			(tempType!='?' && tempType!=(gPetRecs[i]->Type==Dog?'d':'c'))       ||
			(tempGender!='?' && tempGender!=(gPetRecs[i]->Gender==Male?'m':'f'))||
			(strcmp(temp->Breed,"?")!=0 && strcmp(temp->Breed,gPetRecs[i]->Breed)!=0)||
			((temp->Age.Yrs!=-1 && temp->Age.Yrs!=gPetRecs[i]->Age.Yrs)||(temp->Age.Mths!=-1 && temp->Age.Mths!=gPetRecs[i]->Age.Mths))||
			(strcmp(temp->Colour,"?")!=0 && strcmp(temp->Colour,gPetRecs[i]->Colour)!=0)||
			(strcmp(temp->Location,"?")!=0 && strcmp(temp->Location,gPetRecs[i]->Location)!=0)
		  ) 
		{
			DeleteOnePet(i); 
			continue;
		}
		i++;
	}

	cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	cout<<"+          There are "<<setw(2)<<gNumRecs<<" pets in the database          +"<<endl;
	cout<<"+           that match the search criteria            +"<<endl;
	cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	delete temp;

}

void CleanUp()
{
	for(int i=0;i<gNumRecs;i++)
		delete gPetRecs[i];
}


// ============= Private Functions Definitions =========================



void DisplayRecord(int i)
{// Displays record i on screen
	cout<<"Status:        ";
	switch(gPetRecs[i]->Status){
		case Lost:  cout << "lost\n"; break;
		case Found: cout << "found\n";break;
	}
	cout<<"Type  :        ";
	switch(gPetRecs[i]->Type){
		case Cat: cout << "cat\n";break;
		case Dog: cout << "dog\n";break;
	}
	cout<<"Gender:        ";
	switch(gPetRecs[i]->Gender){
		case Male:   cout << "male\n";  break;
		case Female: cout << "female\n";break;
	}
	cout<<"Breed:         "<<gPetRecs[i]->Breed<<endl;
	cout<<"Age:           "<<gPetRecs[i]->Age.Yrs<<" Yrs "<<gPetRecs[i]->Age.Mths<<" Mths\n";
	cout<<"Colour:        "<<gPetRecs[i]->Colour<<endl;
	cout<<"Location:      "<<gPetRecs[i]->Location<<endl;
	cout<<"Phone No:      "<<gPetRecs[i]->PhoneNo<<endl;
}


void LowerChar(char &c)
{
	if(c<='Z' && c>='A') c-=32;
}

void LowerString(char str[])
{
	for(int i=0;str[i]!='\0';i++)
		LowerChar(str[i]);
}


void DeleteOnePet(int i)
{
	if(i>=gNumRecs) return;
	swap(gPetRecs[i],gPetRecs[gNumRecs-1]);
	delete gPetRecs[gNumRecs-1];
	gNumRecs--;
}


void FormatColourString(char str[])
{
	int len=strlen(str);
	int i=0;
	while(i<len && str[i]!=' ') 
		i++;
	if(i==len) 
		return;
	int j=i+1;
	while(j<len && str[j]!=' ') 
		j++;
	if(j==len) return;
	j++;
	str[i]='-';
	while(j<len)
	{
		i++;
		str[i]=str[j];
		j++;
	}
	str[++i]='\0';
}


void WriteFile(ofstream &fout,int i)
{
	if(gPetRecs[i]->Status==Lost)
		fout<<"lost";
	else
		fout<<"found";
	if(gPetRecs[i]->Type==Dog)
		fout<<"dog";
	else
		fout<<"cat";
	switch(gPetRecs[i]->Gender){
			case Male: fout<<"male"<<endl;    break;
			case Female: fout<<"female"<<endl;  break;
			case Unknown: fout<<"unknown"<<endl; break;
		}
	fout<<gPetRecs[i]->Breed<<endl;
	fout<<gPetRecs[i]->Age.Yrs<<" "<<gPetRecs[i]->Age.Mths<<endl;
	fout<<gPetRecs[i]->Colour<<endl;
	fout<<gPetRecs[i]->Location<<endl;
	fout<<gPetRecs[i]->PhoneNo<<endl;
}