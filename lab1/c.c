    #include<stdio.h>
    #include<ctype.h>
    #include<stdlib.h>

    int main(int argc, char const *argv[])
    {
        char c;

         while(1){
             c = getchar();
             if(c == EOF){
                 printf("\n");
                 exit(0);
             }
             if(islower(c)) {
               putchar(toupper(c));
             }
               else{
                     putchar(tolower(c));
                  }
          }
        
        return 0;
    }
    