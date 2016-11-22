for ((i=0; i!=8; i++))
do
  gcc -o q2 q2.c
  ./q2 source destination
  cat destination
  rm -f destination
done
