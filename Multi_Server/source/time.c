#include <stdio.h>
#include <time.h>

int main ()
{
   time_t rawtime;
   struct tm *info;

   time( &rawtime );

   info = localtime( &rawtime );
   printf("Local time va Local date hien tai la: \n%s", asctime(info));

   return(0);
}