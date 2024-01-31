#include <stdio.h>
#include <stdlib.h>
#include "read_config.h"


int main(int argc, char *argv[]) {

    char filename[] = "/home/luke/Documents/programmieren/check_bind_zone_nameserver/test.config";
    char *option = "USER";

    FILE *cfg = open_config(filename);

    if (NULL == cfg) {
        printf("File is null");
    }

    char *value = read_config(cfg, option);

    printf("User: %s", value);

    close_config(cfg);


    return 0;
}