#!/bin/bash
# Lancia K simulazioni indipendenti (seed diversi), ognuna nella sua cartella runs/run_XX/
# Uso: bash run_all.sh (eseguito dalla radice del progetto)
set -e

K=10                                   # Numero di run indipendenti
NEV=20000                              # Eventi per run (sovrascrive i 50000 di microyz.in)
EXE="$(pwd)/build/microyz"             # PerCorso dell'eseguibile dentro build/
MAC="$(pwd)/microyz.in"                # La macro di base nella radice

for i in $(seq -w 1 $K); do
  d="runs/run_$i"
  mkdir -p "$d"
  
  # Generazione seed unici per generatore pseudo-casuale
  seed1=$((1000 + 10#$i * 7919))
  seed2=$((5000 + 10#$i * 104729))
  
  # Inserisce i seed in testa e sovrascrive /run/beamOn con $NEV
  { echo "/random/setSeeds $seed1 $seed2"; sed "s|^/run/beamOn.*|/run/beamOn $NEV|" "$MAC"; } > "$d/run.mac"
  
  echo ">>> Run $i/$K (seed $seed1 $seed2) - $(date +%H:%M:%S)"
  ( cd "$d" && "$EXE" run.mac 2>&1 | tee log.txt | grep --line-buffered "Event .* starts" ) || true
  
  if [ -f "$d/yz.root" ]; then
    ls -lh "$d/yz.root"
  else
    echo "⚠️ Attenzione: $d/yz.root non trovato!"
  fi
done

echo "Completato. Output salvati in runs/run_*/yz.root"