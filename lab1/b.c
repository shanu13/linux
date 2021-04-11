#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>


int main(int argc, char const *argv[])
{    
    
       char c='\0';
     int count = 0;
     while(1){
           c = getchar();
         if(c == EOF){
             fprintf(stderr,"End of Input \n");
             fprintf(stderr, "Total Non Alphnumeric count is %d \n",count);
             exit(0);
         }
        if(!isalpha(c)){
          count++;
            
         }

          putchar(c);  
     }
    
    return 0;
}
