#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define AUTHOR "Luke Haase"
#define VERSION 1.0
#define CONTACT "contact@vloprix.net"
#define HELP_URL "https://github.com/VloPrix/check_bind_zone_nameserver/"
#define DOWNLOAD_URL "https://github.com/VloPrix/check_bind_zone_nameserver/"
#define PROGNAME "check_bind_zone_nameserver"
#define COPYRIGHT "Copyright (c) 2024, Luke Haase,"

enum Returncodes {
        OK,
        WARNING,
        CRITICAL,
        UNKNOWN
};
// Arguments
static struct option longopts[] = {
    {"bind-config-directory", required_argument, 0, 'b'},
    {"nameserver", required_argument, 0, 'n'},
    {"warning", required_argument, 0, 'w'},
    {"critical", required_argument, 0, 'c'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {"verbose", no_argument, 0, 'v'},
    {"timeout", no_argument, 0, 't'},
    {0,0,0,0}
};

// Functions
void print_usage();
void print_help();


int main(int argc, char *argv[]) {

    // Argument options
    int opt;
    char zonedirectory[2048]="/var/lib/bind/", nameserver[2048]="";
    int warningitems=1, criticalitems=2, timeout_interval=60;
    bool verbose=false;

    int long_index = 0;
    while ((opt = getopt_long(argc, argv, "b:n:w:c:hVvt:", longopts, &long_index)) != -1) {
            switch (opt) {
                    case 'b' : strcpy(zonedirectory, optarg);
                            break;
                    case 'n' : strcpy(nameserver, optarg);
                            break;
                    case 'w' : warningitems = atoi(optarg);
                            break;
                    case 'c' : criticalitems = atoi(optarg);
                            break;
                    case 'h' : strcpy(nameserver, optarg);
                            break;
                    case 'V' : printf("%s \nVersion: %.1f\n", PROGNAME, VERSION);
                            exit(EXIT_SUCCESS);
                    case 'v' : verbose = true;
                            break;
                    case 't' : timeout_interval = atoi(optarg);
                            break;
                    default: print_usage();
                            exit(UNKNOWN);
            }
    }

    alarm(timeout_interval);
    if (true == verbose) {printf("Arguments \nConfig directory: %s, Nameserver: %s, Warning: %i, Critical: %i, Timeout: %i\n\n", zonedirectory, nameserver, warningitems, criticalitems, timeout_interval);}

    // Checks if nameserver is specified https://stackoverflow.com/a/50188561
    if (!*nameserver) {print_usage(); printf("CRITICAL: No nameserver specified!"); exit(CRITICAL);}


    return 0;
}
void print_usage() {
        printf ("Usage:\n");
        printf ("%s -H <host_address> [-p port] [-n] [-w seconds] [-c seconds]\n",PROGNAME);
        printf (" [-C SSH-Key] [-f file] [-t timeout]\n");
}

void print_help() {
        printf ("%s, Version: %.1f\n", PROGNAME, VERSION);
        printf ("%s %s\n\n", COPYRIGHT, CONTACT);
        printf ("Plugin to check the age of a file or directory of a remote host. This can be done with SSH or SMB.\n\n\n");
        print_usage();
        printf ("\n\n");
        printf ("-H, --host \n\tHost to connect to\n");
        printf ("-p, --port \n\tPort to use for SSH connection\n");
        printf ("-f, --file \n\tFile or Path to check\n");
        printf ("-u, --ssh-user \n\tUser for SSH authentication\n");
        printf ("-S, --ssh-pubkey \n\tThe SSH Publickey (default: ~/.ssh/id_rsa.pub)\n");
        printf ("-s, --ssh-privkey \n\tThe SSH Privatekey (default: ~/.ssh/id_rsa)\n");
        printf ("-w, --warning \n\tFileage warning in seconds (default: 86400)\n");
        printf ("-c, --critical \n\tFileage critical in seconds (default: 259200)\n");
        printf ("-t, --timeout \n\tTimeout for execution (default: 60 seconds)\n");
        printf ("-n, --no-verify-host \n\tSkip Host verification\n");
        printf ("-h, --help \n\tShow help\n");
        printf ("-v, --verbose \n\tShow verbose output\n");
        printf ("-V, --version \n\tShow version\n");
        
        printf ("\nIf you need help please open an issue on %s\nDownload URL: %s\n", HELP_URL, DOWNLOAD_URL);
}