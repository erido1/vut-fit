# UPA projekt

Projekt, 1. část: extrakce dat z webu

## Popis

Cílem této části projektu je vyzkoušet si využití webových stránek jako zdroje dat, konkrétně pro automatizované získání:

* Odkazů (seznamu URL) z dynamických webových stránek
* Konkrétních informací z většího množství webových stránek

### Název týmu a seznam řešitelů

* šeřík a angrešt
* Bc. Erika Do (xdoeri00) a Bc. Anton Medvedev (xmedve04)

### URL a název e-shopu, který jste zvolili

* https://www.aaaauto.cz (přesněji https://www.aaaauto.cz/sitemap.xml)
* Autobazar AAA AUTO

### Význam sloupců ve výstupu TSV

* demonstrováno na příkladu: 
```240000    BMW 5, 2015    BMW    5    2015    178902    Sedan    5 míst    None    černá``` (None pro položky, kterým daný parametr chybí)

Cena | Název produktu | Značka | Model | Rok uvedení do provozu | Tachometr | Karoserie | Míst k sezení | STK | Barva
--- | --- | --- |--- |--- |--- |--- |--- |--- |---
240000 | BMW 5, 2015 | BMW | 5 | 2015 | 178902 | Sedan | 5 míst | None | černá

### Testovaná prostředí

* Fedora Linux 40, Python 3.12.4
* Windows 11, Python 3.13.0