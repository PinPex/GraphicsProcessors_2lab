g++ main.cpp -o main -lpthread
nvcc -arch=sm_60 ./main.cu -o main_cu
./main
./main_cu