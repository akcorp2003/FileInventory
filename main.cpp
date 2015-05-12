//////////////////////////////////////////////////////////
// InventoryManager v 3.0                               //
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
// Uses                                                 //
//////////////////////////////////////////////////////////

//Used for simplifying a client's need to know what files and their sizes
//are stored on a machine.

//////////////////////////////////////////////////////////
// Design                                               //
//////////////////////////////////////////////////////////

//v 3.0 is a functional program, that is, there will be no objects
//created by the maker besides the objects in BOOST.

//The program will take a directory letter from a user and iterate
//through that directory. The program will output into a file the sizes
//and names of the files found. It will also store the directory location
//at the end of a file. 
//How will FileManager function as it finds more directories within
//the initial directory? FileManager will have on its hand a vector,
//in which all the directories' paths will be stored. As it finishes one 
//folder, it will pop it and analyze the files within the subfolders
//(if applicable). As it ends, the program will make its way back up
//and proceed to the next folder.



#include <iostream>
#include <ctime>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
using namespace boost::filesystem;
using namespace boost::algorithm;
using namespace std;

vector<path> directoryiteration (path dir);
void extensioncollector();
void writer(vector<path> myFiles, string directorysearched);
bool is_file_exist(const char* filename);
string TimeCleanUp(string& time);

//global variables
vector<path> filenames;
vector<string> fileextensions;
map<string, int> ExtensionBook;

typedef map<string, int>::const_iterator c_myiter;
typedef map<string, int>::iterator i_myiter;


/*int main (int argc, char* argv[])
{
	cout << file_size("C:\\Users\\S06242\\Downloads\\111hw07.doc") << " bytes.";
	return 0;
}*/

/*int main ()
{
	string a = "C:\\";
	string input;
	cout << "Please enter a directory followed by :\\ For example, C:\\" << endl;
	cin >> input;
	path letsgo (input); //the file path must have those two \\ in the argument

	if (exists(letsgo))
	{
		if(is_regular_file(letsgo))
			cout << letsgo << " size is " << file_size(letsgo) << endl;
		
		else if (is_directory(letsgo))
		{
			cout << letsgo << "is a directory containing:" << endl;

			typedef vector<path> vec;
			vec yolo;

			//copy(directory_iterator(letsgo), directory_iterator(),
			//	ostream_iterator<directory_entry>(cout, "\n"));

			copy (directory_iterator(letsgo), directory_iterator(), back_inserter(yolo));

			sort(yolo.begin(), yolo.end());

			for (vec::const_iterator it(yolo.begin()); it !=yolo.end(); it++)
			{
				//cout << "   " << *it << endl;
				cout << it->leaf() << endl;
			}
		}

		else
			cout << letsgo << "exists, but is neither a regular file nor a directory" << endl;
	}

	else
		cout << letsgo << "does not exist" << endl;

	return 0;
}*/

int main ()
{
	string input;
	path iterationfinal(input);
	cout << "Welcome to InventoryManager v.3.0." << endl;
	cout << "Please enter a directory followed by :\\ For example, C:\\" << endl;
	cout << "NOTE: Do NOT attempt to iterate through Windows files as it will cause the program to crash." << endl;

	vector<path> files; //this vector will store the final path objects that were collected
	

	for(;;)
	{
		getline(cin, input);
		path iteration(input);
		if(exists(iteration))
		{
			iterationfinal = iteration;
			break;
		}
		else
		{
			cout << iteration << " does not exist" << endl; 
			cout << "Please enter again: " << endl;

		}
	}//end for

	extensioncollector();

	cout << endl;
		
	files = directoryiteration(iterationfinal);

	writer(files, input);



	return 0;
}

void extensioncollector()
{
	c_myiter it1;
	string extensioninput;
	cout << endl;
	cout << "Please enter the extensions you would like FileInventory to look for." << endl;
	cout << "If you would like FileInventory to look for all extensions, press STOP to exit." << endl;
	cout << "Examples of extensions you can enter are: for a standard photo, jpg, for a word document, doc, and so on" << endl;
	for(;;)
	{
		cout << "Enter STOP to finish entering extensions." << endl;
		getline(cin, extensioninput);
		if(extensioninput == "STOP")
		{
			break;
		}

		to_lower(extensioninput);
		//if we locate that the first character is a period, we will remove it
		string temp;
		if(extensioninput[0] == '.')
		{
			unsigned pos = extensioninput.find(".");
			temp =  extensioninput.substr(pos+1);
		}
		else
			temp = extensioninput;
		//go through map and see if the user already entered the extension into the database
		it1 = ExtensionBook.find(temp);
		if(it1 != ExtensionBook.end())
			cout << "You have already entered the extension. Please enter another." << endl;
		else
		{
			ExtensionBook[temp] = 0;
			fileextensions.push_back(temp);
		}		
	}
	return;
}

vector<path> directoryiteration (path dir)
{
	//to extract a filename and convert it to string format
	/*cout << dir.filename() << endl;
	path temp = dir.filename();
	string yolo = temp.string();
	cout << yolo << endl;*/


	directory_iterator x;
	
	for(directory_iterator y(dir); y!=x; y++)
	{
		if(!is_directory(*y))
		{
			string fileex = y->path().extension().string();

			if(fileex.empty())
			{
				i_myiter it;
				it = ExtensionBook.find("No Extension");
				if(it == ExtensionBook.end())
					ExtensionBook["No Extension"] = 1;
				else
					(it->second)++;
				continue;
			}
			
			string temp1 = fileex.substr(1); //remove the period in front of the extension
			
			//search for the appropriate extension if applicable
			if(fileextensions.size() == 0)
			{
				filenames.push_back(y->path());
				
				i_myiter it;

				it = ExtensionBook.find(temp1);
				if(it == ExtensionBook.end()) //no copies of the extension
				{
					ExtensionBook[temp1] = 1;
				}
				else //the extensions exists
				{
					(it->second)++;
				}
			}//end if

			else
			{
				for(int i = 0; i < fileextensions.size(); i++)
				{
					if(fileextensions[i] == temp1)
					{
						//if we found the proper extension in our vector fileextensions
						i_myiter it;
						filenames.push_back(y->path());
						it = ExtensionBook.find(temp1);
						if(it!=ExtensionBook.end())
						{
							(it->second)++; //update the number of files found related to the extension
						}
					}
				}//end for
			}//end else
		}//end if

		//tried a recursive approach to this, either the program dies for some reason when navigating probably 
		//interesting system files or the program keeps on overriding the old file
		else
		{
			cout << "Now sifting through folder " << y->path().filename() << endl;
			directoryiteration(y->path());
		}
	}//end for

	return filenames;
}

bool is_file_exists(const char* filename)
{
	ifstream infile(filename);
	return infile.good();
}

void writer (vector<path> myFiles, string directorysearched)
{
	/*bool checkfexists = is_file_exists("yolo2.txt");
	if(checkfexists)
	{
		cout << "Whoa there!" << endl;
		return;
	}*/
	cout << endl;
	cout << "Beginning writing to file" << endl;
	ofstream outfile("MyInventory.txt");
	ofstream outfileEx("MyInventory.csv");
	if(!outfile)
	{
		cout << "uh oh" << endl;
		return;
	}

	c_myiter iter;
	outfile << "SUMMARY: " << endl;

	for(iter = ExtensionBook.begin(); iter != ExtensionBook.end(); iter++)
	{
		outfile << "Extension \"." << iter->first << "\"" << ": " << iter->second << " files." << endl;
	}

	outfile << endl;

	//if the program finds an utterly void file base
	if(myFiles.size()==0)
	{
		outfile << "None of the file extensions that you requested were found." << endl;
		return;
	}

	outfile << "In the directory " << directorysearched << " contains: " << endl << endl;
	outfileEx << "File Name" << "," << "File Size (Bytes)" << "," << "Date" << endl;

	for(int i = 0; i < myFiles.size(); i++)
	{
		time_t filetime = last_write_time(myFiles[i]);
		char* a = asctime(gmtime(&filetime)); //this line obtained from www.daniweb.com user: vijayan121
		string fetchedtime(a);
		string cleanedtime = TimeCleanUp(fetchedtime);
		
		outfile << myFiles[i].filename() << "     " << file_size(myFiles[i]) << "    " << cleanedtime << endl;
		outfileEx << myFiles[i].filename() << "," << file_size(myFiles[i]) << "," << cleanedtime << endl;
	}

	outfile.close();
	return;
}

string TimeCleanUp (string& time)
{
	time.erase(time.begin()+11, time.end()-5);
	
	return time;
}