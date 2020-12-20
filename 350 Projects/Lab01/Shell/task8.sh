#/bin/sh

tot=1

until [ -z “$1” ]
do
  tot = (($tot * $1))
  shift
done
echo “$tot”

exit 0 
