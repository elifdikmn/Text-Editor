#include <stack> 
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "LinkedList.h" 
#include <vector>

using namespace std;

LinkedList mainList;
stack<string> userActions;//we will keep all user actions to undo
string fileName, command, userNewTextInput, userNewFileNameToSave, deletedText, undoAction, originalTextOfNode, originalFileName;
vector<string> split(string s, string d);
int currentPage = 1, cur = 0, pageSize = 10, totalLinesAdded = 0, totalPageCount = 0, totalLineCount = 0, uInput1 = 0, uInput2 = 0;
int  getTotalPages(int lineCount, int pageSize);


bool readFile(string filename)
{
	ifstream testFile(filename);

	if (testFile.good())
	{
		originalFileName = filename;

		//init list,stack and file details
		mainList.removeAll();

		while (!userActions.empty())
			userActions.pop();

		string line;
		currentPage = 1;
		totalPageCount = 0;
		totalLineCount = 0;

		// fill list with the lines read from txt
		while (getline(testFile, line)) {
			mainList.insertEnd(line);
			totalLineCount++;
		}

		if (totalLineCount > 0)
			return true;
	}

	cout << "file not found";
	return false;
}

int openMyFile(string fileName)
{
	if (readFile(fileName))
	{
		totalPageCount = getTotalPages(totalLineCount, pageSize);
		mainList.traverse(currentPage, pageSize);
		return 1;
	}

	return 0;
}

void goToPage()
{
	currentPage = uInput1 / pageSize;

	if (currentPage == 0)
		currentPage = 1;
	else if ((uInput1 % pageSize) != 0)
		currentPage++;
}

int main()
{
	int  fileOK = 0;

	while (fileOK == 0)
	{
		cout << "open tarkan.txt" << endl;
		getline(cin, command);

		vector<string> cmm = split(command, " ");
		if (cmm.size() < 2) {
			continue;
		}
		command = cmm[0];
		fileName = cmm[1];


		fileOK = openMyFile(fileName);
	}

	while (true)	// operations on file opened
	{
		getline(cin, command);

		vector<string> cmdArr = split(command, " ");
		command = cmdArr[0];

		if (command == "undo")
		{
			if (userActions.empty())
				cout << "you did not do anything to undo" << endl;
			else
			{
				undoAction = userActions.top();

				vector<string> splittedAction = split(undoAction, "|");


				string command = splittedAction[0];
				string undoParam1 = splittedAction[1];
				string undoParam2 = splittedAction[2];

				if (command == "delete")
				{
					uInput1 = stoi(undoParam1);
					originalTextOfNode = undoParam2;

					mainList.insertt(uInput1, originalTextOfNode);

					totalLineCount++;
					goToPage();
				}

				else if (command == "insert")
				{
					uInput1 = stoi(undoParam1);
					totalLinesAdded = stoi(undoParam2);

					if (totalLinesAdded != 1)
					{
						for (int i = 0; i < totalLinesAdded; i++)
						{
							mainList.removeFromEnd();
							totalLineCount--;
						}
						currentPage = (uInput1 - totalLinesAdded) / pageSize;

						if (currentPage == 0)
							currentPage = 1;
						else if (((uInput1 - totalLinesAdded) % pageSize) != 0)
							currentPage++;
					}
					else
					{
						if (uInput1 > 0 && uInput1 <= totalLineCount)
						{
							mainList.removeAt(uInput1);
							totalLineCount--;
						}
					}
				}

				else if (command == "move")
				{
					uInput1 = stoi(undoParam1);
					uInput2 = stoi(undoParam2);

					mainList.moveNode(uInput2, uInput1);

					goToPage();
				}

				else if (command == "replace")
				{
					uInput1 = stoi(undoParam1);
					originalTextOfNode = undoParam2;

					mainList.switchNode(uInput1, originalTextOfNode);

					goToPage();
				}

				else if (command == "next")
				{
					currentPage = stoi(undoParam1);
				}

				else if (command == "prev")
				{
					currentPage = stoi(undoParam1);
				}

				totalPageCount = getTotalPages(totalLineCount, pageSize);
				mainList.traverse(currentPage, pageSize);
				userActions.pop();

			}
		}

		else if (command == "insert")
		{
			try
			{
				userNewTextInput = cmdArr[2];
				uInput1 = stoi(cmdArr[1]);

				if (uInput1 > 0 && uInput1 <= 2147483647)
				{
					totalLinesAdded = uInput1 - totalLineCount;

					if (totalLinesAdded <= 1)
					{
						totalLinesAdded = 1;
						mainList.insertt(uInput1, userNewTextInput);
						totalLineCount++;
					}
					else
					{
						string tempText = " ";
						for (int i = 1; i <= totalLinesAdded; i++)
						{
							totalLineCount++;
							if (i == totalLinesAdded)
								tempText = userNewTextInput;
							mainList.insertt(totalLineCount, tempText);
						}
					}
					totalPageCount = getTotalPages(totalLineCount, pageSize);
					goToPage();

					mainList.traverse(currentPage, pageSize);
					userActions.push(command + "|" + to_string(uInput1) + "|" + to_string(totalLinesAdded));
				}
				else
					cout << "wrong position" << endl;
			}
			catch (exception ex)
			{
				cout << "wrong position" << endl;
			}
		}

		else if (totalLineCount == 0)
		{
			cout << "there is no line" << endl;
		}

		else if (command == "open")
		{
			fileName = cmdArr[1];
			openMyFile(fileName);
		}

		else if (command == "next")
		{
			cur = currentPage;
			currentPage++;

			if (currentPage > totalPageCount)
			{
				currentPage = totalPageCount; 
			}

			mainList.traverse(currentPage, pageSize);
			userActions.push(command + "|" + to_string(cur));
		}

		else if (command == "prev")
		{
			cur = currentPage;
			currentPage--;
			if (currentPage < 1)
			{
				currentPage = 1;
				cout << "No more pages we are on head!!!" << endl;
			}

			mainList.traverse(currentPage, pageSize);
			userActions.push(command + "|" + to_string(cur));
		}

		else if (command == "delete")
		{
			try
			{
				uInput1 = stoi(cmdArr[1]);

				if (uInput1 > 0 && uInput1 <= totalLineCount)
				{
					deletedText = mainList.removeAt(uInput1);

					totalLineCount--;
					totalPageCount = getTotalPages(totalLineCount, pageSize);


					goToPage();

					if (currentPage > totalPageCount)
						currentPage = totalPageCount;

					mainList.traverse(currentPage, pageSize); 
					userActions.push(command + "|" + to_string(uInput1) + "|" + deletedText);
				}
				else
					cout << "total line count: " << totalLineCount << endl;
			}
			catch (exception ex)
			{
				cout << "total line count: " << totalLineCount << endl;
			}
		}

		else if (command == "replace") {
			try
			{
				userNewTextInput = cmdArr[2];
				uInput1 = stoi(cmdArr[1]);
				 
				if (uInput1 > 0 && uInput1 <= totalLineCount)
				{
					originalTextOfNode = mainList.switchNode(uInput1, userNewTextInput);
					 
					goToPage();

					mainList.traverse(currentPage, pageSize);
					userActions.push(command + "|" + to_string(uInput1) + "|" + originalTextOfNode);
				}
				else
					cout << "wrong position" << endl;
			}
			catch (exception ex)
			{
				cout << "wrong position" << endl;
			}
		}

		else if (command == "move")
		{
			try
			{
				uInput1 = stoi(cmdArr[1]);
				uInput2 = stoi(cmdArr[2]);
				 
				if (uInput2 > totalLineCount || uInput1 > totalLineCount || uInput1 < 1 || uInput2 < 1)
				{
					cout << "wrong position" << endl;
				}
				else
				{
					mainList.moveNode(uInput1, uInput2);
					 
					currentPage = uInput2 / pageSize;

					if (currentPage == 0)
						currentPage = 1;
					else if ((uInput2 % pageSize) != 0)
						currentPage++;
					mainList.traverse(currentPage, pageSize);
					userActions.push(command + "|" + to_string(uInput1) + "|" + to_string(uInput2));
				}
			}
			catch (exception ex)
			{
				cout << "wrong position" << endl;
			}
		}

		else if (command == "save")
		{

			userNewFileNameToSave = cmdArr[1];

			if (userNewFileNameToSave == "")
				userNewFileNameToSave = originalFileName;

			mainList.saveToFile(userNewFileNameToSave);
			cout << userNewFileNameToSave << " created / modified." << endl;
		}

		else
		{
			cout << "wrong command" << endl;
		}
	}
}


vector<string> split(string s, string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

int  getTotalPages(int lineCount, int pageSize) {

	int totalPageCount = (lineCount / pageSize);

	if (totalPageCount == 0)
		totalPageCount++;
	else if (lineCount % pageSize != 0)
		totalPageCount++;

	return totalPageCount;
}
