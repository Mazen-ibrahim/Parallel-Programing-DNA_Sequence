#include<iostream>
#include<cmath>
#include<string.h>
#include<mpi.h>
#include<fstream>
#include <ctime>
#include <omp.h>

#define TestCaseNumber 4
#define  Count_code   1
#define  Sequence_code 0

int main()
{
  
  double time1, time2, duration, global;
	string CountSequence;
	char* sequence;
	char* Chunk;
	int chunkOfCharacter = 0;

	MPI_Init(NULL, NULL);
	time1 = MPI_Wtime();
	//get rank of each processor
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	cout << "Rank :" << rank << endl;

	//get size of each processor
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	cout << "Size :" << size << endl;

	if (rank == 0)
	{
		string TestCases[4] = { "DNA50.txt","DNA500.txt","DNA50000.txt","DNA5000000.txt" };
		ifstream TestCase;
		TestCase.open("D:/Computer System/Parallel_Programing/DNA_Parallel/" + TestCases[3]);
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

		//Send to each processor chunk of character to work on it
		chunkOfCharacter = (CountSequence.length() / (size - 1));
		chunkOfCharacter++;
		sequence = new char[CountSequence.length()];
		for (int i = 0; i < CountSequence.length(); i++)
		{
			sequence[i] = CountSequence[i];
		}

		for (int i = 1; i < size; i++)
		{
			MPI_Send(&chunkOfCharacter, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

		int allocateArray;
		MPI_Status status1;
		for (int i = 1; i < size; i++)
		{
			MPI_Recv(&allocateArray, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status1);
		}

		for (int i = 1; i < size; i++)
		{
			MPI_Send(&sequence[(i - 1) * chunkOfCharacter], chunkOfCharacter, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		}


	}


	if (rank != 0)
	{
		MPI_Status status1;
		MPI_Recv(&chunkOfCharacter, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status1);
		cout << "Chunk of Character received: " << chunkOfCharacter << endl;

		Chunk = (char*)malloc(sizeof(char*) * chunkOfCharacter);
		int doneAllocate = 1;
		MPI_Send(&doneAllocate, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

		MPI_Status status2;
		MPI_Recv(Chunk, chunkOfCharacter, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status2);

		//-----------------------Open MP code------------------------------//
		double start_s, stop_s, TotalTime = 0;
		start_s = omp_get_wtime();  // to get the real time clock return time in second
		int NumberofThreads = 4;
		int ChunkofCharacterPerThread;
		ChunkofCharacterPerThread = chunkOfCharacter / NumberofThreads;
		ChunkofCharacterPerThread++;
		printf("Chunk Per Thread: %d\n", ChunkofCharacterPerThread);

		//1 - Count(A, C, G, T) appearance.
		int counterA = 0;
		int counterC = 0;
		int counterG = 0;
		int counterT = 0;
		int countSequence = 0;


#pragma omp parallel shared(counterA,counterC,counterG,counterT,Chunk,countSequence) num_threads(NumberofThreads)
		{
			int tid = omp_get_thread_num();
			bool CountChar = Count_code;
			bool CountAppearance = Sequence_code;
			string SequenceAppearance = "GCCAGATATTCCCCCCGTT";

			int pointerCount = 0;
			int start = tid * ChunkofCharacterPerThread;
			int end = start + ChunkofCharacterPerThread;

			if (tid == NumberofThreads - 1)
			{
				if (end > chunkOfCharacter)
					end = chunkOfCharacter;
			}

			int SumA = 0, SumC = 0, SumG = 0, SumT = 0;
			printf("tid: %d   Start: %d     end: %d\n", tid, start, end);
			for (int i = start; i < end; i++)
			{
				if (CountChar)
				{
					switch (Chunk[i])
					{
					case 'A': SumA++; break;
					case 'C': SumC++; break;
					case 'G': SumG++; break;
					case 'T': SumT++; break;
					}
				}

				if (CountAppearance)
				{
					if (Chunk[i] == SequenceAppearance[pointerCount])
						pointerCount++;
					else
						pointerCount = 0;

					if (pointerCount == SequenceAppearance.length())
					{
#pragma omp critical
						countSequence++;
						pointerCount = 0;
					}

				}

			}

#pragma omp critical
			counterA += SumA;
			counterC += SumC;
			counterG += SumG;
			counterT += SumT;



		}
		stop_s = omp_get_wtime();
		TotalTime += (stop_s - start_s) * 1000;
		printf("time of Open_MP : %f msec\n", TotalTime);
		//-----------------------Open MP code------------------------------//



		//Send Counter to master processor
		MPI_Send(&counterA, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(&counterC, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(&counterG, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(&counterT, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(&countSequence, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

		//free array
		free(Chunk);

	}


	time2 = MPI_Wtime();
	duration = time2 - time1;
	MPI_Reduce(&duration, &global, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		int counterA = 0, counterC = 0, counterG = 0, counterT = 0, countSequence = 0;
		int sumA = 0, sumC = 0, SumG = 0, SumT = 0;
		MPI_Status status;
		for (int i = 1; i < size; i++)
		{
			MPI_Recv(&counterA, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			sumA += counterA;
			MPI_Recv(&counterC, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			sumC += counterC;
			MPI_Recv(&counterG, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			SumG += counterG;
			MPI_Recv(&counterT, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			SumT += counterT;
			MPI_Recv(&countSequence, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
		}

		cout << "-------------------------------" << endl;
		cout << "MPI code" << endl;
		cout << "A Count: " << sumA << " : " << "C Count: " << sumC << endl;
		cout << "G Count: " << SumG << " : " << "T Count: " << SumT << endl;
		cout << "Count Sequence: " << countSequence << endl;
		cout << "-------------------------------" << endl;

		printf("MPI runtime is %f sec\n", global);

	}

	MPI_Finalize();
  return 0;
}
