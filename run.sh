#!/bin/sh

for S in 8 16 32 64 128 256 ; do
     for N in 1 2 4 8 ; do
          ./sim $S $N val_trace_gcc.txt > runs/{$S}_{$N}_gcc.txt
     done;
done;

for S in 8 16 32 64 128 256 ; do
     for N in 1 2 4 8 ; do
          ./sim $S $N val_trace_perl.txt > runs/{$S}_{$N}_perl.txt
     done;
done;
