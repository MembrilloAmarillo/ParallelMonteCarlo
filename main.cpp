#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <pthread.h>
#include <ctime>

struct Random
{
  Random() = default;

  static double double_normalized();
  static double double_range( double r1, double r2 );
  
};

double
Random::double_normalized()
{
  return (double)rand() / (RAND_MAX - 1);
}

double
Random::double_range( double r1, double r2 )
{
  if ( r1 > r2 )
    {
      double tmp = r1;
      r1 = r2;
      r2 = tmp;
    }

  double r_num = (double)rand();
  r_num = fmod( r_num, r2 );
  r_num += r1;
  return r_num;
}

void*
Integral_MonteCarlo( void * attr )
{
  int n_iteraciones = *(int*)attr;
  int* n_exitos = (int *)malloc( sizeof(int) );

  srand( time( NULL ) );

  *n_exitos = 0;

  for( int i = 0; i < n_iteraciones; i++ )
    {
      double cx = Random::double_normalized();
      double cy = Random::double_normalized();
      
      if ( (cx*cx + cy*cy) < 1 )
	{
	  ++(*n_exitos);
	}
    }

  return (void*)n_exitos;
}

int
main( int argc, char **argv )
{
  (void)argc;
  (void)argv;

  int n_iteraciones = 10000000 / 8;
  
  pthread_attr_t thread_attr;
  pthread_t thread[8];

  if ( pthread_attr_init( &thread_attr ) != 0 )
    {
      fprintf( stderr, "Failed initializing thread attributes\n" );
      return -1;
    }

  printf( "Attributes initialized\n" );
  for ( size_t i = 0; i < 8; ++i )
    {
      pthread_create( &thread[i], &thread_attr, Integral_MonteCarlo, &n_iteraciones );
    }

  int n_exitos = 0;
  void* tmp_exitos;
  for ( size_t i = 0; i < 8; ++i )
    {
      pthread_join( thread[i], &tmp_exitos );
      n_exitos += *(int*)tmp_exitos;
    }
  pthread_attr_destroy( &thread_attr );

  printf( "Value of PI approximated: %lf\n", 4 * (double)n_exitos / ((double)n_iteraciones * 8) );
  
  return 0;
}
