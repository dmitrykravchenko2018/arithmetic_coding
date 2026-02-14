#!/usr/bin/env bash
set -eu

BINARY="./arithm-coding"
TEST_DIR="./test_data"
INPUT_DIR="$TEST_DIR/input"
ENCODED_DIR="$TEST_DIR/encoded"
DECODED_DIR="$TEST_DIR/decoded"
PASS=0
FAIL=0
FAILURES=""

# Build
make -s

# Prepare directories
rm -rf "$TEST_DIR"
mkdir -p "$INPUT_DIR" "$ENCODED_DIR" "$DECODED_DIR"

# Generate test files: name, size, content generator
generate_null() { dd if=/dev/zero bs=1 count="$1" 2>/dev/null; }
generate_random() { dd if=/dev/urandom bs=1 count="$1" 2>/dev/null; }
generate_0to3() { head -c "$1" /dev/urandom | tr '\000-\377' '\000-\003' 2>/dev/null; }

declare -a TESTS=(
    "32b_null:32:null"
    "64b_null:64:null"
    "128b_null:128:null"
    "255b_null:255:null"
    "256b_null:256:null"
    "512b_null:512:null"
    "768b_null:768:null"
    "1k_null:1024:null"
    "2k_null:2048:null"
    "4k_null:4096:null"
    "8k_null:8192:null"
    "12k_null:12288:null"
    "16k_null:16384:null"
    "32b_0to3:32:0to3"
    "12k_0to3:12288:0to3"
    "4k_random:4096:random"
    "1b_null:1:null"
    "2b_null:2:null"
    "3b_null:3:null"
    "1b_0xff:1:random"
)

for entry in "${TESTS[@]}"; do
    IFS=: read -r name size gen <<< "$entry"
    input="$INPUT_DIR/$name.bin"

    case "$gen" in
        null)   generate_null "$size" > "$input" ;;
        random) generate_random "$size" > "$input" ;;
        0to3)   generate_0to3 "$size" > "$input" ;;
    esac
done

# Run tests
echo "============================================"
for entry in "${TESTS[@]}"; do
    IFS=: read -r name size gen <<< "$entry"
    input="$INPUT_DIR/$name.bin"
    encoded="$ENCODED_DIR/$name.enc"
    decoded="$DECODED_DIR/$name.bin"

    echo "Testing: $name"
    $BINARY e "$input" "$encoded"
    $BINARY d "$encoded" "$decoded"

    orig_size=$(wc -c < "$input")
    enc_size=$(wc -c < "$encoded")
    dec_size=$(wc -c < "$decoded")

    if cmp -s "$input" "$decoded"; then
        echo "PASS  ($orig_size -> $enc_size -> $dec_size)"
        PASS=$((PASS + 1))
    else
        echo "FAIL  ($orig_size -> $enc_size -> $dec_size)"
        FAIL=$((FAIL + 1))
        FAILURES="$FAILURES  $name: expected $orig_size bytes, got $dec_size\n"
    fi
    echo "--------------------------------------------"
done

# Summary
echo ""
echo "Results: $PASS passed, $FAIL failed"
if [ "$FAIL" -gt 0 ]; then
    echo ""
    echo "Failures:"
    echo -e "$FAILURES"
    exit 1
fi
