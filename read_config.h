#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

FILE *open_config(char filename[]);
char *read_config(FILE *cfg, char option[]);
void close_config(FILE *cfg);
int countlines(FILE *cfg);



FILE *open_config(char filename[]) {
    FILE *cfg;
    cfg = fopen(filename, "r");
    return cfg;
}

char *read_config(FILE *cfg, char option[]) {
        char *line = NULL;
        size_t len = 0;
        size_t read;
        char delim = '='; /*Represents an = in ASCII*/
        char *array[6];
        int lines = countlines(cfg);
        char finalvalue[2048];

        printf("Lines: %i", lines);
        printf("Option: %s", option);

        if (NULL == cfg) {
                return "";
        }
        while ((read = getline(&line, &len, cfg)) != -1) {
                int i = 0;
                printf("ok");
                char *value = strtok(line, &delim);
                while (value != NULL) {
                        array[i++] = value;
                        value = strtok(NULL, &delim);
                }
                
                for (i = 0; i < lines; ++i) {
                        // #ifdef DEBUG
                        printf("%s\n", array[i]);
                        //#endif
                        if (strcmp(array[i], option) == 0 ) { strncpy(finalvalue, array[++i], sizeof(value)); }
                }
                printf("Count %i", i);               
        }
        
        return finalvalue;
}

void close_config(FILE *cfg) {
        fclose(cfg);
}

int countlines(FILE *cfg) {
        int count_lines = 1;

        char chr = getc(cfg);

        while (chr != EOF) {

        //Count whenever new line is encountered

                if (chr == '\n') {

                        count_lines = count_lines + 1;
                }

        //take next character from file.

                chr = getc(cfg);

        }

        return count_lines;
}