# AVS – Architektury výpočetních systémů 2024/25

## Projekt 1 – Vektorizace

Hodnoceni prvniho projektu AVS: xdoeri00

### Vektorizace po radcich
+1.00 b: spravnost vystupu: (error = 0.011%) (max 1 b)  
+1.00 b: cas 1209 ms do limitu 1300 ms (a spravny vystup) (max 1 b)  
Reseni se umistilo mezi 39.07% - 40.40% nejrychlejsimi resenimi  

### Vektorizace po blocich
+1.00 b: spravnost vystupu: (error = 0.011%) (max 1 b)  
+0.00 b: cas 538 ms do limitu 420 ms (a spravny vystup) (max 1 b)  
+1.00 b: cas 538 ms do limitu 620 ms (a spravny vystup) (max 1 b)  
+1.00 b: cas 538 ms do limitu 900 ms (a spravny vystup) (max 1 b)  
Reseni se umistilo mezi 22.60% nejrychlejsimi resenimi  

### Odpovedi, implementace a grafy
+0.50 b: Analyza v Intel Advisor (Q1) (max 0.5 b)  
+0.50 b: Pristup k datum (Q2) (max 1 b)  
+0.50 b: Roofline a AI (Q3) (max 1 b)  
+0.50 b: Vliv velikosti cache (Q4) (max 1 b)  
+0.00 b: Vliv symetrie na vykonnost (Q5) (max 0.5 b)  
+0.00 b: Graf evaluace (max 0 b) 

+0.00 b: bonus (max 0.0 b)  
**Celkem ziskano**: 7.0 bodu

## Projekt 2 – Paralelizace

Hodnoceni druheho projektu AVS: xdoeri00

## 1: paralelizace puvodniho reseni (5b)
+1.00 b: spravnost vertices a distance: 517224 == 517224 a 0.00 mensi nez 2.0 (max 1 b)  
+1.00 b: cas 205 ms do limitu 240 ms (a spravny vystup) (max 1 b)  
Detekovano SIMD: False  
+1.00 b: Q1.1: kterou smycku paralelizovat (max 1 b)  
+1.00 b: Q1.2: ukladani trojuhelniku (max 1 b)  
+1.00 b: Q1.3: planovani (max 1 b)  

## 2: paralelni pruchod stromem (8b)
+3.00 b: spravnost vertices a distance: 517224 == 517224 a 0.00 mensi nez 2.0 (max 3 b)  
+0.00 b: cas 116 ms do limitu 100 ms (a spravny vystup) (max 2 b)  
+1.00 b: Q2.1: pouziti tasku (max 1 b)  
+1.00 b: Q2.2: cut-off (max 1 b)  
+1.00 b: Q2.3: ukladani trojuhelniku (max 1 b)  
Detekovano SIMD: False  

## 3: grafy skalovani, VTune (7b)
+0.30 b: Q3.1: efektivita skalovani (max 1 b)  
+0.50 b: Q3.2: neefektivita prvni ulohy (max 1 b)  
+1.00 b: Q3.3: tree efektivni pro slabe skalovani? (max 1 b)  
+1.00 b: Q3.4: Slurm - prace na projektu (max 1 b)  
+0.00 b: Q4.1: VTune - 18 jader (max 1 b)  
+0.00 b: Q4.2: VTune - 36 jader (max 1 b)  
+1.00 b: Q4.2: vypocet efektivity (max 1 b)  

+0.00 b: bonus/penalta (max 0.0 b)  
**Celkem ziskano**: 14.8 bodu

