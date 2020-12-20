 
echo "Please Enter a number"
read var
num=$var
loop=1

while [ $loop -ne $var  ]
  do
  num=$(((var-loop)*(num)))
  loop=$((loop+1))
  done
echo $num