# Zevra
Zevra - шахматный движок с протоколом UCI. На данный момент имеет рейтинг ~2220 пунктов ЭЛО на CCRL.
Official lichess bot: https://lichess.org/@/zevra_chess

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
+ Razoring;
+ Check extensions;
+ Hash table (resizeble);
+ History heuristic;
+ Two killers;
+ Capture sorting (MVV-LVA);
+ Multi-Cut;

Рейтинги CCRL 40/4:
+ Zevra 1.8.4 - 2219;
+ Zevra 1.8.3 - 2217;
+ Zevra 1.8.1 - 2146;
+ Zevra 1.6.1 - 1949;
+ Zevra 1.5 - 1882;
+ Zevra 1.3.1 - 1755;
+ Zevra 1.2.1 - 1748.

Рейтинги CCRL 40/40:
+ Zevra 1.8.4 - 2226;
+ Zevra 1.8.2 - 2190;
