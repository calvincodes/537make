//
// Created by Arpit Jain on 11/3/18.
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "option_parser.h"

struct_input parse_and_get_unprocessed_input(int argc, char **argv) {

    int optionAsInt;

    struct_input unprocessedInputArg;

    while ((optionAsInt = getopt(argc, argv, "f:")) != -1) {

        switch (optionAsInt) {

            case 'f':
                unprocessedInputArg.make_file_name = optarg;
                break;

            default: // Error Input Option
                fprintf(stderr, "Usage: %s [-f <file_name>]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // INTERESTING ONE HERE :)
    // In case the user passes some redundant arguments, say 537ps -p 123 -v ABC
    // now, for this case, the count of arguments passed > the count of (option + option argument) read.
    // So, putting the same as a if clause to detect this anomaly and report error.
    if (argc > optind) {
        for (int i = 0; i < argc-1; ++i) {
            fprintf(stderr, "Redundant argument %s passed\n", argv[argc-1]);
        }
        exit(EXIT_FAILURE);
    }

    return unprocessedInputArg;
}
