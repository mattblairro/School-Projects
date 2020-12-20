 
 
echo "Enter a number between 5 and 9"

read varnumb

if [ $varnumb -lt 10 ]
  then
    if [ $varnumb -gt 4 ]
     then
     echo $varnumb
     else 
     echo "That is an invalid output try again"
     read varnumb
     fi
   
 fi