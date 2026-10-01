#!/bin/bash
mkdir -p batches
for i in $(seq -w 1 10); do
  s1=$RANDOM$RANDOM
  s2=$RANDOM$RANDOM
  sed -e "s/__SEED1__/${s1}/" \
      -e "s/__SEED2__/${s2}/" \
      -e "s/__BATCHID__/batch${i}/" \
      svalue-cu64_template.in > batches/svalue-cu64_batch${i}.in
  echo "Creato batch${i}: seed1=${s1} seed2=${s2}"
done