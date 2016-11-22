for ((i=0; i!=8; i++))
do
  gcc q3.c
  ./a.out 3
  rm a.out cache
done
