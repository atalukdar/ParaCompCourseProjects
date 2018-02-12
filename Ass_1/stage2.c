# include <stdio.h>
# include <time.h>
# include <math.h>
# include <mpi.h>
# include <stdlib.h>
#define UPPER_BOUND  300
#define MODE 1  //0 to force entire task in one process

int main ( int argc, char *argv[] );

void timestamp ()
{ 
# define TIME_SIZE 40
  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  size_t len;
  time_t now;
  now = time( NULL );
  tm = localtime( &now );
  len = strftime( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );
  return;
# undef TIME_SIZE
}

int main ( int argc, char *argv[] )
{
  int i, id;
  int n;
  int ptr;
  int n_hi, n_lo = 1, limit;
  int p, prime, start, count = 0, total_count;
  double wtime;

  //setting limit
  if (argc > 0 && MODE == 1)
    limit = (int)atoi(argv[1]);
  else
    limit = UPPER_BOUND;     /*setting upper bound */  
  int lsqrt = (int)ceil(sqrt(limit));
  
    /*  Initialize MPI. */
  int ierr = MPI_Init ( NULL, NULL );  
    /*  Get the number of processes.*/
  ierr = MPI_Comm_size ( MPI_COMM_WORLD, &p );
    /*  Determine this processes's rank.*/
  ierr = MPI_Comm_rank ( MPI_COMM_WORLD, &id );
  //-------Start of timer -------
  if ( id == 0 )
        wtime = MPI_Wtime( );
  // ---- handling larger cases ----
  if( lsqrt < (int)ceil( limit / p * 1.0 ) )
  {
  if ( id == 0 )
  { timestamp ( );
    printf ( "\n" );
    printf ( "PRIME_MPI\n" );
    printf ( "  C/MPI version\n" );
    printf ( "\n" );
    printf ( "  An MPI example program to count the number of primes.\n" );
    printf ( "  The number of processes is %d", p );
    printf ( "\n" );
  }
  int root = 0;
  // ------- create arrays ------
  if (id != p-1)
    n_hi = (int)(limit/p) *(id+1);
  else 
    n_hi = limit;
  if(id == root)
        n_lo = 2;  
  else
        n_lo = (int)(limit/p) *id;    
  char *arr = (char*) calloc((n_hi - n_lo) , sizeof(char));

  //find next prime
  ptr = n_lo;
  prime = 3;
  while(prime <= n_hi && prime > 1 )
  {  //get next prime  
     if(id == root)
     {   while(arr[ptr-n_lo] == 1)
          {  ptr+=1;    
             if(ptr > lsqrt)  // can comment out maybe
             {  ptr = limit+1;
                break;
              }
           }    
        prime  = ptr;  
        ptr += 1;           
      }
      
    //broadcast prime
    int er_a = prime;
    ierr = MPI_Bcast( &prime, 1, MPI_INT, root, MPI_COMM_WORLD );
    if(prime >= n_hi )
        break;
        
    //eliminate multiples of prime in n_lo to n_hi
    start = (int) ceil((n_lo*1.0)/prime) *  prime ;
    if(id == root)
        start += prime;
    for(i = start - n_lo ;i<n_hi - n_lo ; i += prime )
        arr[i] = 1;  
  }

  // count primes
  if(id != p-1)
  {  for(i = 0;i<n_hi-n_lo;i+=1)
    count+= 1 - arr[i];
  }
  else
  { for(i = 0;i<limit-n_lo;i+=1)
    count+= 1 - arr[i];
  }
  ierr = MPI_Reduce ( &count, &total_count, 1, MPI_INT, MPI_SUM, root , MPI_COMM_WORLD );
  if ( id == root )
    { wtime = MPI_Wtime( ) - wtime;
      printf ( "         N        Pi          Time\n" );
      printf ("  %8d  %8d  %14f\n", limit, total_count, wtime );
    }
//	free(arr);
 }
 else 
 {  
    if ( id == 0 )
    {   timestamp ( );
        printf ( "\n" );
        printf ( "PRIME_MPI\n" );
        printf ( "  C/MPI version\n" );
        printf ( "\n" );
        printf ( "  An MPI example program to count the number of primes.\n" );
        printf ( "  The number of processes is %d but using only root process due to small input.", p );
        printf ( "\n" );
        char *arr = (char*) calloc((limit - 1) , sizeof(char));
        ptr = 2;
        while(ptr<=lsqrt)
        {   if(arr[ptr] == 0)
            {   for( i = 2*ptr - 2; i <= limit - 2; i += ptr )
                        arr[i] = 1;                     
            }
            ptr+=1;
        }
        printf ( " Marked C\n" );
        for(i = 0; i < limit - 2; i++)
           count+=arr[i];
        wtime = MPI_Wtime( ) - wtime;
        printf ( "         N        Pi          Time\n" );
        printf ("  %8d  %8d  %14f\n", limit, total_count, wtime );
//	free(arr);
    }
  }
/*   Terminate. Code for performance measurement */
  if ( id == 0 ) 
  { printf ( "\n");         
    printf ( "PRIME_MPI - Master process:\n");         
    printf ( "  Normal end of execution.\n"); 
    printf ( "\n" );
    timestamp ( );        
  }
  /*   Terminate MPI. */
  ierr = MPI_Finalize ( );
  return 0;
}