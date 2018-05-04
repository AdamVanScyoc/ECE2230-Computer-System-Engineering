#!/bin/bash

valgrind ./avanscy.assign3 -q >> output.txt
valgrind ./avanscy.assign3 -q 0 >> output.txt
valgrind ./avanscy.assign3 -q -e >> output.txt
valgrind ./avanscy.assign3 -q -e 0 >> output.txt
valgrind ./avanscy.assign3 -q 100 -e >> output.txt
valgrind ./avanscy.assign3 -q -e 100 >> output.txt
valgrind ./avanscy.assign3 -q 100 -e 100 >> output.txt

