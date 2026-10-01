#!/bin/bash
mkdir -p batches

for i in $(seq -w 1 10); do
  echo "=== Lancio batch${i} ==="
  export BATCH_TAG="batch${i}"
  ./build/svalue svalue-cu64_batch${i}.in

  # Sposta gli output appena creati dentro batches/
  mv "cu64_batch${i}.root" batches/ 2>/dev/null
  mv "s_batch${i}.txt" batches/ 2>/dev/null
done

echo "Tutti i batch completati."