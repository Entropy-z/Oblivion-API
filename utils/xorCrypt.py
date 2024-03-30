import sys

def xor_encrypt(input_file, output_file, key):
    with open(input_file, 'rb') as f:
        data = bytearray(f.read())

    key = bytearray(key)

    for i in range(len(data)):
        data[i] ^= key[i % len(key)]

    with open(output_file, 'wb') as f:
        f.write(data)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python xorCrypt.py <input_file> <output_file> <key>")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]
    key = bytes.fromhex(sys.argv[3])

    try:
        xor_encrypt(input_file, output_file, key)
        print("Operação XOR concluída com sucesso!")
    except Exception as e:
        print(f"Erro ao executar a operação XOR: {e}")