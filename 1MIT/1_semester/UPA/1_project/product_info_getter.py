"""
Filename: url_extract.py
Authors: Anton Medvedev (xmedve04), Erika Do (xdoeri00)
Date: 2024-10-13
Description: This script extracts a list of cars from the page: https://www.aaaauto.cz.
"""

import requests
from bs4 import BeautifulSoup
import url_extract
import re
import sys

URL = "https://www.aaaauto.cz/cz/skoda-octavia/car.html?id=680974003"

class Auto:
    def __init__(self, url):
        self.price = None
        self.name = None
        self.brand = None
        self.model = None
        self.year = None
        self.tachometer = None
        self.body_type = None
        self.place_to_seat = None
        self.color = None
        self.stk_date = None
        self.get_info(url)

    def get_info(self, url):
        response = requests.get(url)
        soup = BeautifulSoup(response.content, 'html.parser')

        # Find price
        try:
            price = soup.find('strong', class_='carCard__price-value').text
            self.price = re.sub(r'\D', '', price)
        except AttributeError:
            self.price = "Undefined"
            print(url)

        # Find details
        auto_details_tag = soup.find('h3', string='Detail vozu')
        if auto_details_tag:
            ul_tag = auto_details_tag.find_next_sibling('ul', class_='listLined')
            details = ul_tag.find_all('li')

            for detail in details:
                if 'Značka' in detail.text:
                    self.brand = detail.find('strong').text
                elif 'Model' in detail.text:
                    self.model = detail.find('strong').text
                elif 'Rok uvedení do provozu' in detail.text:
                    self.year = detail.find('strong').text
                elif 'Tachometr' in detail.text:
                    tachometer = detail.find('strong').text
                    self.tachometer = re.sub(r'\D', '', tachometer)
                elif 'Karoserie' in detail.text:
                    self.body_type = detail.find('strong').text
                elif 'Míst k sezení' in detail.text:
                    place_to_seat = detail.find('strong').text
                    place_to_seat = place_to_seat.strip()
                    self.place_to_seat = " ".join(place_to_seat.split())
                elif 'Barva' in detail.text:
                    color = detail.find('strong').text
                    self.color = color.strip()
                elif 'STK' in detail.text:
                    stk_date = detail.find('strong').text
                    self.stk_date = stk_date.strip()
                else:
                    pass

        self.name = f"{self.brand} {self.model}, {self.year}"

    def print_tsv_info(self):
        print(f"{self.price}\t{self.name}\t{self.brand}\t{self.model}\t{self.year}\t"
              f"{self.tachometer}\t{self.body_type}\t"
              f"{self.place_to_seat}\t{self.stk_date}\t{self.color}")

def main():
    if sys.stdin.isatty():
        product_urls = url_extract.url_extract(150)
    else:
        product_urls = sys.stdin.read().split('\n')
        product_urls.pop(-1)

    for url in product_urls:
        auto = Auto(url)
        auto.print_tsv_info()


if __name__ == '__main__':
    main()
