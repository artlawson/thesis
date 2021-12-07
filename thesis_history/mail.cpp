//example of cpp send mail
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[]){

    int status;
    //next line works from command line
    status = system(R"(echo "this is the body" | mail -s "this is the subject" "artlawson318@gmail.com")");

    return 0;
}