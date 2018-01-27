# Zevra
Zevra - шахматный движок с протоколом UCI. На данный момент имеет рейтинг ~2150-2250 пунктов ЭЛО (отн. CCRL 40/5).

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
+ razoring;
+ Check extensions;
+ Hash table (resizeble);
+ History heuristic;
+ Two killers + Mate killer;
+ Capture sorting (MVV-LVA).

Рейтинги CCRL 40/5:
+ Zevra 1.6.1 - 1937;
+ Zevra 1.5 - 1888;
+ Zevra 1.3.1 - 1754;
+ Zevra 1.2.1 - 1749.
