#include<iostream>
#include<cmath>
#include<string.h>
#include<fstream>
#include <ctime>
#include <omp.h>

#define TestCaseNumber 4
#define  Count_code   1
#define  Sequence_code 0

int main()
{
    double start_s, stop_s, TotalTime = 0;
	  string CountSequence;
	  int countSequence = 0;
	  int GlobalCounterA = 0;
	  int GlobalCounterC = 0;
	  int GlobalCounterG = 0;
	  int GlobalCounterT = 0;
	  int NumberofThreads = 5;
	  int ChunkofCharacter;
	  string TestCases[4] = { "DNA50.txt","DNA500.txt","DNA50000.txt","DNA5000000.txt" };
	  ifstream TestCase;
	  for (int t = 0; t < TestCaseNumber; t++)
	  {
		  start_s = omp_get_wtime();  // to get the real time clock return time in second
		  TestCase.open("D:/Computer System/Parallel_Programing/DNA_Parallel/" + TestCases[t]);
		  if (TestCase.is_open())
		  {
			  string Sequence;

			  while (TestCase.good())
			  {
				  TestCase >> Sequence;
				  CountSequence += Sequence;
			  }
		  }
		  TestCase.close();

		  ChunkofCharacter = CountSequence.length() / NumberofThreads;
		  printf("-------------------------------\n");
		  printf("Chunk: %d\n", ChunkofCharacter);

#pragma omp parallel shared(GlobalCounterA,GlobalCounterC,GlobalCounterG,GlobalCounterT,CountSequence,countSequence) num_threads(NumberofThreads)
			 {
				 int tid = omp_get_thread_num();
				 bool Count = Count_code;
				 bool sequence = Sequence_code;
				 string SequenceAppearance = "GCCAGATATTCCCCCCGTT";

				 int pointerCount = 0;
				 int start = tid * ChunkofCharacter;
				 int end = start + ChunkofCharacter;
				 int SumA = 0, SumC = 0, SumG = 0, SumT = 0;
				 printf("tid: %d   Start: %d     end: %d\n", tid, start, end);
				 for (int i = start; i < end; i++)
				 {
					 if (Count)
					 {
						 switch (CountSequence[i])
						 {
						 case 'A': SumA++; break;
						 case 'C': SumC++; break;
						 case 'G': SumG++; break;
						 case 'T': SumT++; break;
						 }
					 }

					 if (sequence)
					 {
						 if (CountSequence[i] == SequenceAppearance[pointerCount])
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

 #pragma omp critical
				 GlobalCounterA += SumA;
				 GlobalCounterC += SumC;
				 GlobalCounterG += SumG;
				 GlobalCounterT += SumT;



			 }

			 printf("Open MP Code\n");
			 printf("Count A: %d     :     Count C: %d  \n", GlobalCounterA, GlobalCounterC);
			 printf("Count G: %d     :     Count T: %d  \n", GlobalCounterG, GlobalCounterT);
			 printf("Count Sequence: %d\n", countSequence);
			 stop_s = omp_get_wtime();
			 TotalTime = (stop_s - start_s) * 1000;
			 printf("time: %f msec\n", TotalTime);
			 printf("-------------------------------\n\n");
		 }
}
