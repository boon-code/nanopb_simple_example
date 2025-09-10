Simple nanopb project
=====================

This project uses nanopb: https://github.com/nanopb/nanopb

Pre-requisists:

- Python >= 3.7


# Setup

1. Setup a virtual environment: `python3 -m venv venv`
2. Install dependencies:

        . venv/bin/activate && python -m pip install -r requirements.txt

# Generate the example files

To generate the C files for `simple.proto` do the following:

        . venv/bin/activate && nanopb_generator simple.proto

# Compile and run the example (gcc)

        ./build-simple-gcc.sh && ./test

# Compile and run with Zig

        zig build run
