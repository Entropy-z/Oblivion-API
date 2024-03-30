import sys

def bin_to_bytes_with_prefix(file_path):
    try:
        with open(file_path, 'rb') as file:
            bin_data = file.read()
            hex_strings = ['0x{:02x}'.format(byte) for byte in bin_data]
            return hex_strings
    except FileNotFoundError:
        print(f"Error: File '{file_path}' not found.")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <file_path>")
        sys.exit(1)

    file_path = sys.argv[1]
    bytes_with_prefix = bin_to_bytes_with_prefix(file_path)

    print(', '.join(bytes_with_prefix))
