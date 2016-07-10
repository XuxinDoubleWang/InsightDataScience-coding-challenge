#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct user
{
    char name[100];
    int number;

} ;


int cmp(const void*a,const void*b)
{
    return *(int*)a-*(int*)b;
}


int main()
{
 
//You could read the different test file as following
//char inputfile[] = "../insight_testsuite/tests/test-1-venmo-trans/venmo_input/venmo-trans.txt";

  char inputfile[] = "../insight_testsuite/tests/your-own-test/venmo_input/venmo-trans.txt";
  char line[999999];
  char rubbish[100];
  
  char time[100];
  char target_name[100];
  char actor_name[100];

  char maxtime[100] = {'\0'};

  struct user user_array[1500];
 


  int ui;
    for (ui = 0; ui <= 1499; ui++)
    {
        user_array[ui].number = 0;
    }

  int num_user = 0;
 
  // Open test file
 
  FILE* fp=fopen(inputfile,"r");
  if (fp == NULL)
    {
      fprintf (stderr, "Cannot open file\n");
      exit(-1);
    }


  // read JSON of the whole payment

  while (fgets(line, 999999, fp)!= NULL)
  {
     


    sscanf(line, "%s %s %s %s %s %s",
	   rubbish,
	   time,
	   rubbish,
	   target_name,
	   rubbish,
	   actor_name);

    // find the last time

   if(strcmp(time, maxtime) > 0)
    {
        strcpy(maxtime, time);
    }
 
  }



  
  rewind(fp);

  //Maintain data within the 60-second window  

  char maxday[2], maxhour[2], maxminute[2], maxsecond[2];
  int imaxday, imaxhour, imaxminute, imaxsecond, imax; 
  strncpy(maxday, maxtime + 9, 2);
  strncpy(maxhour, maxtime + 12, 2);
  strncpy(maxminute, maxtime + 15, 2);
  strncpy(maxsecond, maxtime + 18, 2);

  imaxday = atoi(maxday);
  imaxhour = atoi(maxhour);
  imaxminute = atoi(maxminute);
  imaxsecond = atoi(maxsecond);

  imax = imaxsecond + imaxminute * 60 + imaxhour*60*60 + imaxday*60*60*24;
  


  char day[2], hour[2], minute[2], second[2];
  int iday, ihour, iminute, isecond, i; 

  while (fgets(line, 999999, fp)!= NULL)
  {
     

    sscanf(line, "%s %s %s %s %s %s",
	   rubbish,
	   time,
	   rubbish,
	   target_name,
	   rubbish,
	   actor_name);
      
      strncpy(day, time + 9, 2);
      strncpy(hour, time + 12, 2);
      strncpy(minute, time + 15, 2);
      strncpy(second, time + 18, 2);

      iday = atoi(day);
      ihour = atoi(hour);
      iminute = atoi(minute);
      isecond = atoi(second);
      i= isecond + iminute * 60 + ihour*60*60 + iday*60*60*24;

     
     if (imax - i <= 60)
        {
          
            
            
            int met_tar = 0;
            int met_act = 0;
            

            target_name [strlen (target_name)-1] = '\0';
            actor_name [strlen (actor_name)-1] = '\0';

            // Building the Venmo Graph

            int user_idx = 0;
            for (user_idx = 0; user_idx <= num_user-1; user_idx++)
            {
                if (strcmp(user_array[user_idx].name, target_name) == 0) 
                {
                    user_array[user_idx].number++;
                    met_tar = 1;

                }   




                 else if(strcmp(user_array[user_idx].name, actor_name) == 0)
                {
                    user_array[user_idx].number++;
                    met_act = 1;
                }
              
           }
         
            if (met_tar == 0)
            {
                num_user++;
                strcpy(user_array[num_user - 1].name, target_name);
                user_array[num_user - 1].number++;

            }
            

            if  (met_act == 0)
            {
                num_user++;
                strcpy(user_array[num_user - 1].name, actor_name);
                user_array[num_user - 1].number++;
            }            
        }               	            
  } 
   
            //Calculate the rolling median degree

            int times[num_user];

            for(ui = 0; ui <= num_user - 1; ui++)
            {

               times[ui] = user_array[ui].number;
            }

            qsort(times,num_user,sizeof(int),cmp);




             float middle;

             if (num_user %2 == 1){
               middle = times[num_user / 2] ;
             }
              else {middle=(times[num_user/2]+times[num_user/2-1])/2.0 ;}            


    fclose(fp);




   //Write to optput.txt
//You could read the different test file as following
//char inputfile[] = "../insight_testsuite/tests/test-1-venmo-trans/venmo_output/output.txt";

   FILE *fpout = fopen("../insight_testsuite/tests/your-own-test/venmo_output/output.txt","a");

     if(fpout == NULL)

     {
      fprintf (stderr, "Cannot open file\n");
      exit(-1);
    }

     fprintf(fpout,"%f \n",middle);
     fclose(fpout);



return 0;

}
