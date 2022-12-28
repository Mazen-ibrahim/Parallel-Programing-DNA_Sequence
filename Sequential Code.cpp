#include<iostream>
#include<cmath>
#include<string.h>
#include<fstream>
#include <ctime>


#define TestCaseNumber 4
#define  Count_code   1
#define  Sequence_code 0

using namespace std;

int main()
{
	//Sequential code
	int start_s, stop_s, TotalTime = 0;
	start_s = clock();

	string TestCases[TestCaseNumber] = { "DNA50.txt","DNA500.txt","DNA50000.txt","DNA5000000.txt" };
	ifstream TestCase;

	for (int i = 0; i < TestCaseNumber; i++)
	{
		TestCase.open("Put Path of TestCases " + TestCases[i]);
		if (TestCase.is_open())
		{
			//1 - Count(A, C, G, T) appearance.
			bool CountChar = Count_code;
			int counterA = 0;
			int counterC = 0;
			int counterG = 0;
			int counterT = 0;

			//2-Count “GCCAGATATTCCCCCCGTT” appearance
			bool CountAppearance = Sequence_code;
			string SequenceAppearance = "GCCAGATATTCCCCCCGTT";
			int countSequence = 0;
			int pointerCount = 0;

			string Sequence;
			while (TestCase.good())
			{
				TestCase >> Sequence;
				for (int i = 0; i < Sequence.length(); i++)
				{
					if (CountChar)
					{
						switch (Sequence[i])
						{
						case 'A': counterA++; break;
						case 'C': counterC++; break;
						case 'G': counterG++; break;
						case 'T': counterT++; break;
						}
					}

					if (CountAppearance)
					{
						if (Sequence[i] == SequenceAppearance[pointerCount])
							pointerCount++;
						else
							pointerCount = 0;

						if (pointerCount == SequenceAppearance.length())
						{
							countSequence++;
							pointerCount = 0;
						}
					}


				}

			}

			cout << "-------------------------------" << endl;
			cout << TestCases[i] << endl;
			cout << "A Count: " << counterA << " : " << "C Count: " << counterC << endl;
			cout << "G Count: " << counterG << " : " << "T Count: " << counterT << endl;
			cout << "-------------------------------" << endl;
			cout << "Count Sequence: " << countSequence << endl;
		}

		TestCase.close();

		stop_s = clock();
		TotalTime += (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000;
		cout << "time: " << TotalTime << endl;
	}
}