rm -rf test_results/
mkdir test_results
./sim 2 8 traces/val_trace_gcc1 >> test_results/pipe_2_8_gcc.txt
./sim 8 8 traces/val_trace_gcc1 >> test_results/pipe_8_8_gcc.txt
./sim 64 1 traces/val_trace_gcc1 >> test_results/pipe_64_1_gcc.txt
./sim 128 8 traces/val_trace_gcc1 >> test_results/pipe_128_8_gcc.txt
./sim 256 8 traces/val_trace_gcc1 >> test_results/pipe_256_8_gcc.txt
if [[ $(diff test_results/pipe_2_8_gcc.txt validation_runs/pipe_2_8_gcc.txt | tee test_results/diff_2_8.txt) ]]; then
    echo "2_8 failure"
else
    echo "2_8 success"
fi
if [[ $(diff test_results/pipe_8_8_gcc.txt validation_runs/pipe_8_8_gcc.txt | tee test_results/diff_8_8.txt) ]]; then
    echo "8_8 failure"
else
    echo "8_8 success"
fi
if [[ $(diff test_results/pipe_64_1_gcc.txt validation_runs/pipe_64_1_gcc.txt | tee test_results/diff_64_1.txt) ]]; then
    echo "64_1 failure"
else
    echo "64_1 success"
fi
if [[ $(diff test_results/pipe_128_8_gcc.txt validation_runs/pipe_128_8_gcc.txt | tee test_results/diff_128_8.txt) ]]; then
    echo "128_8 failure"
else
    echo "128_8 success"
fi
if [[ $(diff test_results/pipe_256_8_gcc.txt validation_runs/pipe_256_8_gcc.txt | tee test_results/diff_256_8.txt) ]]; then
    echo "256_8 failure"
else
    echo "256_8 success"
fi
