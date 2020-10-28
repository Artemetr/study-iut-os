import matlab.engine as me
import xlwings as xw
import math
import os


def get_args():
    try:
        x, y, z = float(input('Input x: ')), float(input('Input y: ')), float(input('Input z: '))
    except:
        print('Error: Invalid argument')
        exit(1)

    return x, y, z


def run_excel():
    os.system("open -a 'Microsoft Excel.app'")


def get_excel_expression(x, y, z):
    return f'=2^(-{x})*({x}+(ABS({y}))^(1/4))^(1/2)*(EXP({x}-1/SIN({z})))^(1/3)'


def get_matlab_expression(x, y, z):
    return get_excel_expression(x, y, z).lower().replace('=', '')


def save_and_start_matlab_m_file(content):
    filename = 'expression.m'
    with open(filename, 'w') as f:
        f.write(content)
    os.system(f'open {filename}')


def calculate_expression_python(x, y, z):
    return 2 ** (-x) * (x + (abs(y)) ** (1 / 4)) ** (1 / 2) * (math.exp(x - 1 / math.sin(z))) ** (1 / 3)


def calculate_expression_matlab(x, y, z):
    matlab_expr = get_matlab_expression(x, y, z)
    save_and_start_matlab_m_file(matlab_expr)

    eng = me.start_matlab()
    result = eng.eval(matlab_expr)
    eng.quit()

    return result


def calculate_expression_excel(x, y, z):
    run_excel()
    sht = xw.Book().sheets['Sheet1']
    sht.range('A1').formula = get_excel_expression(x, y, z)

    return sht.range('A1').value


def round_to_nine(number):
    return round(number, 9)


def main():
    x, y, z = get_args()

    results = {
        'python': round_to_nine(calculate_expression_python(x, y, z)),
        'excel': round_to_nine(calculate_expression_excel(x, y, z)),
        'matlab': round_to_nine(calculate_expression_matlab(x, y, z)),
    }
    max_res, min_res = max(list(results.values())), min(list(results.values()))

    for name, res in results.items():
        print(f'Result by {name}: {res}{" (max)" if res == max_res else ""}{" (min)" if res == min_res else ""}')


if __name__ == '__main__':
    main()
