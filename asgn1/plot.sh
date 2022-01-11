#!/bin/bash

rm -f /tmp/length.dat
rm -f /tmp/values.dat
rm -f /tmp/maxvalues.dat

for n in {2..10000};do
    echo -n "$n " >> /tmp/length.dat
    ./collatz -n $n | wc -l >> /tmp/length.dat
    echo -n "$n " >> /tmp/values.dat
    ./collatz -n $n | sort -nr | head -n 1  >> /tmp/values.dat
      
done

#sort -nr /tmp/values.dat >> /tmp/maxvalues.dat

gnuplot <<END
    set terminal pdf
    set output "length.pdf"
    set title "Collatz Sequence Lengths"
    set xlabel "n"
    set ylabel "length"
    set zeroaxis
    plot "/tmp/length.dat" with dots
END

gnuplot <<END
    set terminal pdf
    set output "values.pdf"
    set title "Maximum Collatz Sequencce Value"
    set xlabel"n"
    set ylabel "value"
    set xrange [0:10000]
    set yrange [0:100000]
    set zeroaxis
    plot "/tmp/values.dat" with dots 
END

