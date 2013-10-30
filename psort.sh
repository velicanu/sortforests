if [[ -z "$1" ]]
then
  echo "Usage: ./psort.sh <input-list> <out-dir>"
  exit 1
fi
if [[ -z "$2" ]]
then
  echo "Usage: ./psort.sh <input-list> <out-dir>"
  exit 1
fi

now="sortjob_$(date +"%m_%d_%Y__%H_%M_%S")"
mkdir $now
len=`wc -l $1 | awk '{print $1}'`
filesperjob=$3
cp sort.sh $now
cp $1 $now

NAME="sortForest.C"
g++ $NAME $(root-config --cflags --libs) -Werror -Wall -O2 -o "${NAME/%.C/}.exe"
cp sortForest.exe $now
cat psort.condor | sed "s@log_flag@$now@g" | sed "s@dir_flag@$PWD/$now@g" | sed "s@arg1@$1@g" | sed "s@arg2@$2@g" | sed "s@njobs@$len@g" > $now/psort.condor
echo -=-
cat $now/psort.condor
echo -=-

echo condor_submit $now/psort.condor

