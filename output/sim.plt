set terminal pdf
set output "comparision.pdf"
set title "SingleQueueSimulation"
set xlabel "no.of tellers"
set ylabel "avg time spent by customer(in mins)"
plot "datas.txt" using 1:2 with impulse title "Mean time spent by the customer" lw 2
