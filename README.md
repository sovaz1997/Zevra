# Zevra
Zevra - шахматный движок моей разработки. Поддерживает UCI.  

Команды UCI:
+ `position [startpos/fen] [moves]` - задать позицию;
+ `go depth [depth]` - поиск лучшего хода с фиксированной глубиной;
+ `go infinite` - бесконечный поиск;
+ `go movetime [msec]` - поиск лучшего хода с фиксированным временем;
+ `go wtime [msec] btime [msec] winc [msec] binc [msec] movestogo [num_moves]` - турнирный режим;
+ `stop` - остановить поиск;
+ `quit` - выход из программы.

Свои команды:
+ `posmoves` - Выводит избыточные ходы и FEN текущей позиции со статической оценкой позиции;
+ `bench` - Стресс-тест генератора ходов;
+ `move [ex. e2e4]` - Сделать ход в данной позиции (если возможно);
+ `goback` - Сделать возврат хода (если возможно);
+ `perft [depth]` - Выполнить perft-тест.

Эвристики:
+ Late Move Reductions;
+ futility pruning (optional, enable by default);
+ razoring (optional, enable by default);
+ Null Move (optional, disable by default);
+ Check extensions (optional, enable by default);
+ Hash table (no resizeble);
+ History heuristic;
+ Two killers + Mate killer;
+ Capture sorting (MVV-LVA).

Рейтинги CCRL 40/5:
+ Zevra 1.5 r340 - 1888;
+ Zevra 1.3.1 r340 - 1754;
+ Zevra 1.2.1 r290 - 1749;

Предположительно, Zevra v1.6 имеет рейтинг 1930-1960 (относительно рейтингов CCRL).
