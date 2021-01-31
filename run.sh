# run server

for ((i=1; i<=28; i++)); do
    printf '\n'
done

# make clean
make all

sudo ./server 80 $(pwd)
