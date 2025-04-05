#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#url_extract.py
"""
Filename: url_extract.py
Authors: Anton Medvedev (xmedve04), Erika Do (xdoeri00)
Date: 2024-10-13
Description: This script extracts a list of cars from the page: https://www.aaaauto.cz.
"""

import os
import sys
import io
import requests
import gzip
import argparse
from bs4 import BeautifulSoup

def url_extract(num_of_urls, car_name=""):
    """
    Extract a specified number of URLs from the aaaauto.cz sitemap, optionally filtered by a car name.

    Parameters:
    num_of_urls (int):        The number of URLs to retrieve.
    car_name (str, optional): The name of the car to filter by (default is an empty string).

    Returns:
    list: The list of available products URLs.
    """
    xml = requests.get("https://www.aaaauto.cz/sitemap.xml")
    soup = BeautifulSoup(xml.content, 'xml')
    xml_tag = [loc.text for loc in soup.find_all("loc") if "car" in loc.text] # Extract the url: https://www.aaaauto.cz/sitemap/aaaauto/CZ/https/sitemap-car-1.xml.gz

    gz_file = requests.get(xml_tag[0])
    with gzip.open(io.BytesIO(gz_file.content), 'rt') as file:
        file_content = file.read()

    soup = BeautifulSoup(file_content, 'xml')
    url_set = set()
    for loc in soup.find_all("loc"):
        if car_name.lower() in loc.text:
            url_set.add(loc.text)

    return list(url_set)[:num_of_urls]

parser = argparse.ArgumentParser(prog='url_extract.py', usage='py %(prog)s [options] or python3 %(prog)s [options]')
parser.add_argument('-p', '--print', action='store_true', help="print a formatted list of all available cars")
args, unknown = parser.parse_known_args()

if unknown:
    parser.print_help()
    sys.exit(0)

if args.print:
    for item in url_extract(150, ""):
        print(item, end='\n')
