//
// Created by Arpit Jain (ajain74) on 11/1/18.
//
#include <stdio.h>
#include <stdlib.h>
#include "input_parser/reader.h"
#include "input_parser/option_parser.h"

int main(int argc, char *argv[]) {

    struct_input unprocessedInput = parse_and_get_unprocessed_input(argc, argv);
    reader(unprocessedInput);

    // Free targets to build.
    free(unprocessedInput.targets_to_build);

}