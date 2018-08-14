#include <stdio.h>
#include <stdlib.h>
/* The ENVIRON variable contains the environment.*/
extern char** environ;



int print_all_env_variables ()
{
char** var;
for (var = environ; *var != NULL; ++var)
printf ("%s\n", *var);

return 0;

}
int main(int argc, char * argv[])
{
 
 char * environment_variable = "SERVER_NAME";
 char * server_name1 = getenv(environment_variable);
 printf("%s before setting the environment %s\n",environment_variable, server_name1);
 char * server_name;

    if (argc>1)
    {
	server_name = argv[1];
    }
    else
    {
    	server_name = "server.my.company.com";
    }
 
    setenv(environment_variable, server_name,1);
    char * server_name_new = getenv(environment_variable);
 printf ("%s set to %s New value is %s\n",environment_variable, server_name, server_name_new);
 return 0;
}
