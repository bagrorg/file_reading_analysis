for i in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216
do
    echo @
    echo $1 rep_file_$i $2 $5 
    time ../cmake-build-debug/file_reading_analysis -m $1 -b $2 -i rep_file_$i -o $4 -r $5
    echo
done