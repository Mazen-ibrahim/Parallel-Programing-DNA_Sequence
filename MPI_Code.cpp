#include<iostream>
#include<cmath>
#include<string.h>
#include<mpi.h>
#include<fstream>
#include <ctime>


#define TestCaseNumber 4
#define  Count_code   1
#define  Sequence_code 0

int main()
{
      double time1, time2, duration, global;

      string TestCases[TestCaseNumber] = { "DNA50.txt","DNA500.txt","DNA50000.txt","DNA5000000.txt" };
      string CountSequence;
      char *sequence; 
      char *Chunk;
      int chunkOfCharacter = 0;
      int start;
      int end;

      MPI_Init(NULL, NULL);

      //get size of each processor
      int size;
      MPI_Comm_size(MPI_COMM_WORLD, &size);
      cout << "Size :" << size << endl;

      //get rank of each processor
      int rank;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      cout << "Rank :" << rank << endl;

      for (int i = 0; i < TestCaseNumber; i++)
      {
        time1 = MPI_Wtime();


        if (rank == 0)
        {

          ifstream TestCase;
          TestCase.open("Add Path of TestCases" + TestCases[i]);
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
          start = (rank - 1) * chunkOfCharacter;
          end = start + chunkOfCharacter;

          Chunk = (char*)malloc(sizeof(char*) * chunkOfCharacter);
          int doneAllocate = 1;
          MPI_Send(&doneAllocate, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

          MPI_Status status2;
          MPI_Recv(Chunk, chunkOfCharacter, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status2);

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

          for (int i = 0; i < chunkOfCharacter; i++)
          {
            if (CountChar)
            {
              switch (Chunk[i])
              {
              case 'A': counterA++; break;
              case 'C': counterC++; break;
              case 'G': counterG++; break;
              case 'T': counterT++; break;
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
                countSequence++;
                pointerCount = 0;
              }
            }


          }


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
          cout << "Test Case: " <<TestCases[i] << endl;
          cout << "A Count: " << sumA << " : " << "C Count: " << sumC << endl;
          cout << "G Count: " << SumG << " : " << "T Count: " << SumT << endl;
          cout << "Count Sequence: " << countSequence << endl;
          printf("Global runtime is %f sec\n", global);
          cout << "-------------------------------" << endl;
          cout << endl;


        }
      }
        MPI_Finalize();
        return 0;
 }
