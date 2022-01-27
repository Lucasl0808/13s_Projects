#!/bin/bash

rm -f /tmp/insertmoves.dat
rm -f /tmp/quickmoves.dat
rm -f /tmp/batchermoves.dat
rm -f /tmp/heapmoves.dat
rm -f /tmp/insertcomp.dat
rm -f /tmp/quickcomp.dat
rm -f /tmp/heapcomp.dat
rm -f /tmp/batchercomp.dat
rm -f /tmp/sorting.dat
for n  in {0..1000};do
	./sorting -a -n $n -p 0 >> /tmp/sorting.dat
done
awk '/Insertion/ {print $3,$5}' /tmp/sorting.dat >> /tmp/insertmoves.dat
awk '/Quick/ {print $3, $5}' /tmp/sorting.dat >> /tmp/quickmoves.dat
awk '/Batcher/ {print $3,$5}' /tmp/sorting.dat >> /tmp/batchermoves.dat
awk '/Heap/ {print $3, $5}' /tmp/sorting.dat >> /tmp/heapmoves.dat

awk '/Insertion/ {print $3, $7}' /tmp/sorting.dat >> /tmp/insertcomp.dat
awk '/Quick/ {print $3, $5}' /tmp/sorting.dat >> /tmp/quickcomp.dat
awk '/Batcher/ {print $3, $5}' /tmp/sorting.dat >> /tmp/batchercomp.dat
awk '/Heap/ {print $3, $5}' /tmp/sorting.dat >> /tmp/heapcomp.dat

gnuplot <<END
    set terminal pdf
    set output "length_to_moves.pdf"
    set title "Moves Performed"
    set xlabel "elements"
    set ylabel "moves"
    set zeroaxis
    plot "/tmp/insertmoves.dat" with lines title "insertion", \
         "/tmp/quickmoves.dat" with lines title "quick", \
         "/tmp/batchermoves.dat" with lines title "batcher", \
         "/tmp/heapmoves.dat" with lines title "heap"
END

gnuplot <<END
    set terminal pdf
    set output "length_to_compares.pdf"
    set title "Compares Performed"
    set xlabel "elements"
    set ylabel "compares"
    set zeroaxis
    plot "/tmp/insertcomp.dat" with lines title "insertion", \
         "/tmp/quickcomp.dat" with lines title "quick", \
         "/tmp/batchercomp.dat" with lines title "batcher", \
         "/tmp/heapcomp.dat" with lines title "heap"
END
