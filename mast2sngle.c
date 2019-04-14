#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// global varibles
const char delim[2] = "~";
static char filename[] = "./master_topologies/t#.txt";

//declaring methods
void lineError(int lineCount);
char *next(int currentLine);
FILE *my_fopen(const char *__restrict __filename, const char *__restrict __modes);

int main(int argc, char const *argv[])
{
    // arg parsing
    if (argc != 3)
    {
        printf("usage: m2s topology_number pc_ip_adrrs\n");
        return 0;
    }

    //preparing variables
    filename[21] = argv[1][0]; //fixing the usrInput into the string
    char *line = NULL;
    FILE *master, *output;
    size_t len = 0;
    int ret = 0, lineCount = 1;

    //opening input and output files
    //printf("reading: %s\n", filename); //control
    master = my_fopen(filename, "r");
    output = my_fopen("input.txt", "w"); //called "input" because is from client perspective

    fprintf (output, "%s", filename);
    while ((ret = getline(&line, &len, master)) != -1)
    {
        //trim unecesary characters at the end
        ret--;
        while (ret >= 0)
        {
            if (line[ret] > ' ')
                break;
            line[ret] = '\0';
            ret--;
        }

        char *token;

        //try with first token
        token = strtok(line, delim); // 1st token  // node 1
        if (strcmp(token, argv[2]) == 0)
        {
            token = next(lineCount); // 2nd token // node 2
            fprintf (output, "\n%s~", token);
            token = next(lineCount); // 3rd token // cost
            fprintf (output, "%s", token);

            lineCount++;
            continue;
        }

        //try with 2nd token
        char *temp = malloc(sizeof(token));
        strcpy(temp, token);
        token = next(lineCount); // 2nd token  // node 2
        if (strcmp(token, argv[2]) == 0)
        {
            fprintf (output, "\n%s~", temp);
            token = next(lineCount); // 3rd token // cost
            fprintf (output, "%s", token);

            free(temp);
            lineCount++;
            continue;
        }

        free(temp);
        lineCount++;
    }

    fclose(master);
    fclose(output);
    if (line)
        free(line);

    exit(EXIT_SUCCESS);
}

FILE *my_fopen(const char *__restrict __filename, const char *__restrict __modes)
{
    FILE *temp = fopen(__filename, __modes);

    if (temp == NULL) // error opening the file
    {
        perror(__filename);
        exit(EXIT_FAILURE);
    }

    return temp;
}

char *next(int currentLine){
    char * temp = strtok(NULL, delim);
    if (temp == NULL)
        lineError(currentLine);
    return temp;
}

void lineError(int lineCount)
{
    printf("wrong format in %s at line %d", filename, lineCount);
    exit(EXIT_FAILURE);
}