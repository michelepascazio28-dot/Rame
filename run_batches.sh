#!/bin/bash
cd build

for i in $(seq -w 1 10); do
  echo "=== Lancio batch${i} ==="
  export BATCH_TAG="batch${i}"
  ./svalue ../batches/svalue-cu64_batch${i}.in
done

echo "Tutti i batch completati."