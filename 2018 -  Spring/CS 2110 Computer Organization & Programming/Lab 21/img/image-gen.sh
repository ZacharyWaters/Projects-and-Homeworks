#!/bin/bash

# Cook up some nin10kit magic

for snoop in snoop{1..6}; do
    nin10kit --mode=3 "$snoop" "$snoop.bmp"
done
