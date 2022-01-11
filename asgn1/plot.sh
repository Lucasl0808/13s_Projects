#!/bin/bash

rm -f /tmp/length.dat
rm -f /tmp/values.dat
rm -f /tmp/histogram.dat

for n in {2..10000};do
    echo -n "$n " >> /tmp/length.dat
    ./collatz -n $n | wc -l >> /tmp/length.dat
    echo -n "$n " >> /tmp/values.dat
    ./collatz -n $n | sort -nr | head -n 1  >> /tmp/values.dat
    ./collatz -n $n | wc -l >> /tmp/histogram.dat
done

cat /tmp/histogram.dat | uniq -c | less  >> /tmp/histograms.dat

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
    set xlabel "n"
    set ylabel "value"
    set xrange [0:10000]
    set yrange [0:100000]
    set zeroaxis
    plot "/tmp/values.dat" with dots 
END

gnuplot <<END
    set terminal pdf
    set output "histogram.pdf"
    set title "Collatz Sequence Length Histogram"
    set xlabel "length"
    set ylabel "frequency"
    set xrange [0:225]
    set yrange [0:200]
    plot "/tmp/histograms.dat" using 2:xticlabels(1) with histograms
END

