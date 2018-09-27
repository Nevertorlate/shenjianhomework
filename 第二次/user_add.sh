var3="用户"
if [ $# == 2 ] ;
then
#var=$(cat initpwd.dat)

ncount=0
ucount=0
while read linepwd
do 
   array[$ncount]=$linepwd
   ((ncount++))
done < $2




while read line
do
  if [ ${line:0:1} = "#" ] ;then
    :    
  else
    OIFS=$IFS; IFS=" "; set -- $line; aa=$1; bb=$2; IFS=$OIFS
    if [ -z "$bb" ] ;then
     :
    else
      echo 已建立$var3"u"${aa}[${bb}]
      useradd u$aa
      chown -R u$aa /home/u$aa
      #echo "$var"|passwd u$aa --stdin
      echo "${array[ucount]}"|passwd u$aa --stdin
      ((ucount++))
      usermod -L u$aa
      chage -d 0 u$aa
      usermod -U u$aa
    fi
fi
done < $1

elif [ $# -lt 2 ] ;
then 
  echo "参数少于两个"
else
  echo "参数多于两个"

fi
