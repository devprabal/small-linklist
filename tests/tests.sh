#!/bin/bash

COLOR_RED='\033[1;31m'
COLOR_BLUE='\033[1;34m'
COLOR_GREEN='\033[1;32m'
COLOR_YELLOW='\033[1;33m'
COLOR_RESET='\033[0m'

ROOT_DIR=..

TEST_MODULES="node" # append space-separated directory names inside one string

function run_tests 
{
    for module in $TEST_MODULES
    do
        make -C ./${ROOT_DIR}/${module}/utc clean
        make -C ./${ROOT_DIR}/${module}/utc test
    done
}

function gen_report
{
    for module in $TEST_MODULES
    do
        make -C ./${ROOT_DIR}/${module}/utc gcov_files
        echo -e "${COLOR_YELLOW}============ GENERATING REPORT for $module ============${COLOR_RESET}"
        rm -f coverage_${module}.info
        lcov --capture --directory ./${ROOT_DIR}/${module}/utc --output-file coverage_${module}.info
    done

    rm -rf report

    REMOVE_PREFIX=$(echo $(pwd) | sed --expression 's|/tests||g')

    genhtml --prefix ${REMOVE_PREFIX} coverage_node.info --output-directory report

    ## use this when other modules are added
    # lcov --add-tracefile coverage_node.info -a coverage2.info ...coverageN -o merged.info
    # genhtml merged.info --output-directory report
}

function clean
{
    for module in $TEST_MODULES
    do
        rm -f coverage_${module}.info
    done
    rm -rf report
}

function test_or_report
{
    if [[ $1 = "tests" ]]
    then
        run_tests
    elif [[ $1 = "report" ]]
    then
        run_tests # needed to generate .gcda files required for report
        gen_report
    elif [[ $1 = "clean" ]]
    then
        clean
    fi
}

test_or_report $1
