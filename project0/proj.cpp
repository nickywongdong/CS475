#include <omp.h>
#include <stdio.h>
#include <math.h>

#define NUMT	         4
#define ARRAYSIZE      100000
#define NUMTRIES       16

float A[ARRAYSIZE];
float B[ARRAYSIZE];
float C[ARRAYSIZE];

int main( )
{
#ifndef _OPENMP
   fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
   return 1;
#endif

   /******   1 Thread    *******/

   omp_set_num_threads( 1 );
   fprintf( stderr, "Using %d thread\n", 1 );

   double maxMegaMults = 0.;
   double sumMegaMults = 0.;

   for( int t = 0; t < NUMTRIES; t++ )
   {
      double time0 = omp_get_wtime( );

#pragma omp parallel for
      for( int i = 0; i < ARRAYSIZE; i++ )
      {
	 C[i] = A[i] * B[i];
      }

      double time1 = omp_get_wtime( );
      double megaMults = (double)ARRAYSIZE/(time1-time0)/1000000.;
      sumMegaMults += megaMults;
      if( megaMults > maxMegaMults )
	 maxMegaMults = megaMults;
   }

   double avgMegaMults = sumMegaMults/(double)NUMTRIES;
   printf( "\tPeak Performance \t= %8.2lf MegaMults/Sec\n", maxMegaMults );
   printf( "\tAverage Performance \t= %8.2lf MegaMults/Sec\n", avgMegaMults );

   double singleThreadSpeed = maxMegaMults;

   /******   4 Threads    *******/

   omp_set_num_threads( NUMT );
   fprintf( stderr, "Using %d threads\n", NUMT );

   maxMegaMults = 0.;
   sumMegaMults = 0.;

   for( int t = 0; t < NUMTRIES; t++ )
   {
      double time0 = omp_get_wtime( );

#pragma omp parallel for
      for( int i = 0; i < ARRAYSIZE; i++ )
      {
	 C[i] = A[i] * B[i];
      }

      double time1 = omp_get_wtime( );
      double megaMults = (double)ARRAYSIZE/(time1-time0)/1000000.;
      sumMegaMults += megaMults;
      if( megaMults > maxMegaMults )
	 maxMegaMults = megaMults;
   }

   avgMegaMults = sumMegaMults/(double)NUMTRIES;
   printf( "\tPeak Performance \t= %8.2lf MegaMults/Sec\n", maxMegaMults );
   printf( "\tAverage Performance \t= %8.2lf MegaMults/Sec\n", avgMegaMults );

   double multipleThreadSpeed = maxMegaMults;


   // note: %lf stands for "long float", which is how printf prints a "double"
   //    // 	//        %d stands for "decimal integer", not "double"
   //


   double S = singleThreadSpeed / multipleThreadSpeed;
   float Fp = (4./3.) * (1. - (1./S));

   printf("4-thread-to-one-thread speedup:\t%8.2lf\n", S);
   printf("Parallel Fraction:\t%f\n", Fp);


   return 0;
}
