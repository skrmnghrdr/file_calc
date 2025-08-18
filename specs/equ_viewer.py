import struct

def read_header(file):
    header_format = '> I Q Q B I 2s'
    header_size = struct.calcsize(header_format)
    header_data = file.read(header_size)
    return struct.unpack(header_format, header_data)

def read_unsolved_equation(file):
    equation_format = '> I B 17s 10s'
    equation_size = struct.calcsize(equation_format)
    equation_data = file.read(equation_size)
    return struct.unpack(equation_format, equation_data)

def read_serialized_equation(equation_bytes):
    serialized_format = '> Q B Q'
    return struct.unpack(serialized_format, equation_bytes)

def print_equation_details(equation):
    operand1, operator, operand2 = read_serialized_equation(equation)
    print(f"Operand 1: {hex(operand1)}")
    print(f"Operator: {hex(operator)}")
    print(f"Operand 2: {hex(operand2)}")

def main(file_path):
    with open(file_path, 'rb') as file:
        magic, file_id, num_equations, flags, offset, num_opt_headers = read_header(file)
        print(f"Magic Number: {hex(magic)}")
        print(f"File ID: {hex(file_id)}")
        print(f"Number of Equations: {hex(num_equations)}")
        print(f"Flags: {hex(flags)}")
        print(f"Offset to Equations: {hex(offset)}")
        print(f"Number of Optional Headers: {num_opt_headers}")

        for _ in range(num_equations):
            equation_id, flags, equation, padding = read_unsolved_equation(file)
            print(f"\nEquation ID: {hex(equation_id)}")
            print(f"Flags: {flags}")
            print_equation_details(equation)

if __name__ == '__main__':
    file_path = '../laboratory/2bc470fa533393e3.equ'
    main(file_path)

