#!/bin/bash
python url_extract.py -p >> url_test.txt;
head -n 10 url_test.txt | python3 product_info_getter.py