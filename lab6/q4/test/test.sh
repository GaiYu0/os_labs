for ((i=1; i!=33; i++))
do
  gcc q4.c -DTIMES=$i
  echo times $i
  for target in Lincoln
  do
    ./a.out $target .
  done
done
