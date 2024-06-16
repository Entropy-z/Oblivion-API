import sys
import math
import pefile

def calculate_entropy(data):
    entropy = 0
    if not data:
        return 0
    for x in range(256):
        p_x = float(data.count(x)) / len(data)
        if p_x > 0:
            entropy += - p_x * math.log(p_x, 2)
    return entropy

def main():
    if len(sys.argv) != 2:
        print("Usage: python script.py <exe>")
        return
    
    pe_file = sys.argv[1]

    try:
        pe = pefile.PE(pe_file)
        for section in pe.sections:
            section_name = section.Name.decode('utf-8').strip('\x00')
            section_entropy = calculate_entropy(section.get_data())
            print(f"{section_name}\t{section_entropy}")
    except Exception as e:
        print("Erro:", e)

if __name__ == "__main__":
    main()
