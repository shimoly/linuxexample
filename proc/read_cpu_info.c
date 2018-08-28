#include <stdio.h>
#include <string.h>
/* Returns the clock speed of the system’s CPU in MHz, as reported by /proc/cpuinfo. On a multiprocessor machine, returns the speed of the first CPU. On error returns zero. */
char* parse(char * property_file_name, char* propery_name)
{
    FILE *fp;
    char buffer[2048];
    size_t bytes_read;
    char *match;
    float clock_speed = 0;
    /* Read the entire contents of /proc/cpuinfo into the buffer. */
    fp = fopen (property_file_name, "r");
    bytes_read = fread (buffer, 1, sizeof (buffer), fp);
    fclose (fp);
    /* Bail if read failed or if buffer isn’t big enough. */
    if (bytes_read == 0)
    {
        printf("Bytes read is zero: %ld\n", bytes_read);
        return NULL;
    }
    if (bytes_read == sizeof(buffer))
    {
        printf("Bytes read is max: %ld\n", bytes_read);
        return 0;
    }
   
       
    /* NUL-terminate the text. */
    buffer[bytes_read] = '\0';
    /* Locate the line that starts with “cpu MHz”. */
    match = strstr(buffer, propery_name);
    if (match == NULL)
    {
        printf("No match\n");
        return NULL;
    }
    return match;
}
float get_value(char * property_file_name, char* propery_name)
{
    float value = 0;
    /* Parse the line to extract the clock speed. */
    char buffer[1024];
    sprintf(buffer, "%s : %%f",propery_name);
    char *start_position = parse(property_file_name, propery_name);
    if (start_position != NULL)
        sscanf(start_position, buffer, &value);
    return value;
}
int main(int argc, char *argv[])
{
    printf("CPU clock speed: %4.0f MHz\n", get_value("/proc/cpuinfo", "cpu MHz"));
    printf("cache size: %4.0f MHz\n", get_value("/proc/cpuinfo", "cache size"));
    printf("bogomips: %4.0f MHz\n", get_value("/proc/cpuinfo", "bogomips"));
    
}