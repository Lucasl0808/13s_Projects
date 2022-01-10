#!/bin/bash

rm -f /tmp/length.dat

for n in {2..10000};do
    echo -n "$n " >> /tmp/length.dat
    ./collatz -n $n | wc -l >> /tmp/length.dat
 
done

gnuplot <<END
    set terminal pdf
    set output "length.pdf"
    set title "Collatz Sequence Lengths"
    set xlabel "n"
    set ylabel "length"
    set zeroaxis
    plot "/tmp/length.dat" with points title "" 
END

