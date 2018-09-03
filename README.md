# Zevra
Zevra - шахматный движок с протоколом UCI. На данный момент имеет рейтинг ~2220 пунктов ЭЛО на CCRL.

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
+ `bench` - Стресс-тест генератора ходов;
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
+ Zevra 1.8.5 - 2273;
+ Zevra 1.8.4 - 2221;
+ Zevra 1.8.3 - 2214;
+ Zevra 1.8.1 - 2151;
+ Zevra 1.6.1 - 1952;
+ Zevra 1.5 - 1885;
+ Zevra 1.3.1 - 1759;
+ Zevra 1.2.1 - 1753.

**Рейтинги CCRL 40/40:**
+ Zevra 1.8.5 - 2293;
+ Zevra 1.8.5 - 2233;
+ Zevra 1.8.4 - 2237;
+ Zevra 1.8.2 - 2198;

**Тестирование Zevra v1.8.6**

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
  1 GreKo 2018.08               :  2807.4   69.2   381.5     400    95
  2 Galjoen 0.39.2              :  2431.7   37.1   283.5     400    71
  3 Zevra v1.8.6 r672 popcnt    :  2299.8   30.5   423.5    1000    42
  4 Baislicka                   :  2272.2   34.6   198.5     400    50
  5 Zevra v1.8.5 r664           :  2250.0   ----   367.5    1000    37
  6 CT800 V1.31 64 bit          :  2246.6   34.3   184.0     400    46
  7 Hedgehog 1.9 64-bit         :  2206.1   34.0   161.5     400    40
  ```
