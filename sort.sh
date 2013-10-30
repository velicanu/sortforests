if [[ -z "$1" ]]
then
  echo "Usage: ./condor.sh <fileNumber> <list_of_files.txt> <outfolder>"
  exit 1
fi
if [[ -z "$2" ]]
then
  echo "Usage: ./condor.sh <fileNumber> <list_of_files.txt> <outfolder>"
  exit 1
fi
if [[ -z "$3" ]]
then
  echo "Usage: ./condor.sh <fileNumber> <list_of_files.txt> <outfolder>"
  exit 1
fi

start=$(($1+1))
mkdir mergedTmp
echo | awk -v start=$1 -v filelist=$2 '{print "./sortForest.exe \""start"\" \""filelist"\""}' | bash
mv *.root $3

echo "job done successfully"
