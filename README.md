# Zevra
Zevra - шахматный движок с протоколом UCI. На данный Zevra v1.8.5 на CCRL 40/4 имеет 2273 пункта. Последняя версия по оценкам будет сильнее на 60-80 пунктов ЭЛО.

Official lichess bot: https://lichess.org/@/zevra_chess

Тестирование Zevra: https://sites.google.com/view/scet-testing/zevra

**Команды UCI:**
+ `position [startpos/fen] [moves]` - задать позицию;
+ `go depth [depth]` - поиск лучшего хода с фиксированной глубиной;
+ `go infinite` - бесконечный поиск;
+ `go movetime [msec]` - поиск лучшего хода с фиксированным временем;
+ `go wtime [msec] btime [msec] winc [msec] binc [msec] movestogo [num_moves]` - турнирный режим;
+ `stop` - остановить поиск;
+ `quit` - выход из программы.

**Свои команды:**
+ `posmoves` - Выводит избыточные ходы и FEN текущей позиции со статической оценкой позиции;
+ `move [ex. e2e4]` - Сделать ход в данной позиции (если возможно);
+ `goback` - Сделать возврат хода (если возможно);
+ `perft [depth]` - Выполнить perft-тест.

**Эвристики:**
+ Late Move Reductions;
+ Razoring;
+ Check extensions;
+ Hash table (resizeble);
+ History heuristic;
+ Two killers;
+ Capture sorting (MVV-LVA);
+ Multi-Cut;

**Рейтинги CCRL 40/4:**
+ Zevra v1.8.5 - 2273;
+ Zevra v1.8.4 - 2221;
+ Zevra v1.8.3 - 2214;
+ Zevra v1.8.1 - 2151;
+ Zevra v1.6.1 - 1952;
+ Zevra v1.5 - 1885;
+ Zevra v1.3.1 - 1759;
+ Zevra v1.2.1 - 1753.

**Рейтинги CCRL 40/40:**
+ Zevra v1.8.5 - 2233;
+ Zevra v1.8.4 - 2237;
+ Zevra v1.8.2 - 2198;

**Тестирование Zevra v1.8.6:**

+ Zevra v1.8.6 vs Zevra v1.8.5
```
tc=5s+0.05s, Hash=64mb, book=8moves_GM.pgn
Score of Zevra v1.8.6 r672 vs Zevra v1.8.5 r664: 457 - 244 - 299  [0.607] 1000
Elo difference: 75.15 +/- 18.26
```
```
tc=20s+0.2s, Hash=64mb, book=8moves_GM.pgn
Score of Zevra v1.8.6 r672 vs Zevra v1.8.5 r664: 423 - 192 - 385  [0.616] 1000
Elo difference: 81.73 +/- 17.04
```

+ Zevra Gaunlet
```
# PLAYER                      :  RATING  ERROR  POINTS  PLAYED   (%)
1 GreKo 2018.08               :  2804.4   36.6  1333.0    1400    95
2 Galjoen 0.39.2              :  2418.8   19.0   966.5    1400    69
3 Zevra v1.8.6 r672 popcnt    :  2303.4   16.2  1515.0    3500    43
4 CT800 V1.31 64 bit          :  2266.1   17.6   679.0    1400    49
5 Baislicka                   :  2253.4   18.7   654.0    1400    47
6 Zevra v1.8.5 r664           :  2250.0   ----  1304.0    3500    37
7 Hedgehog 1.9 64-bit         :  2198.8   18.9   548.5    1400    39

White advantage = 26.71 +/- 4.10
Draw rate (equal opponents) = 22.95 % +/- 0.56
```
  
```
1) Zevra v1.8.6 r672 popcnt 2303.4 :   3500 (+1174,=682,-1644),  43.3 %

  vs.                             :  games (    +,   =,    -),   (%) :    Diff,    SD, CFS (%)
  GreKo 2018.08                   :    700 (   12,  48,  640),   5.1 :  -501.0,  18.7,    0.0
  Galjoen 0.39.2                  :    700 (  162, 155,  383),  34.2 :  -115.3,   9.7,    0.0
  CT800 V1.31 64 bit              :    700 (  313, 142,  245),  54.9 :   +37.4,   9.4,  100.0
  Baislicka                       :    700 (  319, 157,  224),  56.8 :   +50.0,   9.2,  100.0
  Hedgehog 1.9 64-bit             :    700 (  368, 180,  152),  65.4 :  +104.7,   9.6,  100.0

2) Zevra v1.8.5 r664        2250.0 :   3500 (+981,=646,-1873),  37.3 %

  vs.                             :  games (   +,   =,    -),   (%) :    Diff,    SD, CFS (%)
  GreKo 2018.08                   :    700 (  12,  38,  650),   4.4 :  -554.4,  18.7,    0.0
  Galjoen 0.39.2                  :    700 ( 127, 134,  439),  27.7 :  -168.8,   9.7,    0.0
  CT800 V1.31 64 bit              :    700 ( 256, 162,  282),  48.1 :   -16.1,   9.0,    3.7
  Baislicka                       :    700 ( 274, 149,  277),  49.8 :    -3.4,   9.6,   36.2
  Hedgehog 1.9 64-bit             :    700 ( 312, 163,  225),  56.2 :   +51.2,   9.7,  100.0
```
