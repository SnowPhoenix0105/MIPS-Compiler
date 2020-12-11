import sys
import os
import time
import random
import re

current_dir = sys.path[0]
test_resource_dir = current_dir + "\\UnitTest\\TestResource"

class InputAndExpectGenerator:
    def __init__(self, work_dir):
        super().__init__()
        self.work_dir = work_dir
         # list of bool, True means require an integer, False means require a charactor
        self.input_type_list = []  
        self.char_table = [chr(a + i) for i in range(26) for a in map(ord, ['a', 'A'])]
        self.char_table += [str(i) for i in range(10)]
        self.char_table += ['+', '-', '*', '/']


    def input_file_name(self, num: int)->str:
        global test_resource_dir
        return test_resource_dir + '\\' + self.work_dir + '\\{:d}.input'.format(num)

    def expect_file_name(self, num: int)->str:
        global test_resource_dir
        return test_resource_dir + '\\' + self.work_dir + '\\{:d}.expect'.format(num)

    def source_c_file_name(self)->str:
        global test_resource_dir
        return test_resource_dir + '\\' + self.work_dir + '\\source.c'

    def source_cpp_file_name(self)->str:
        global test_resource_dir
        return test_resource_dir + '\\' + self.work_dir + '\\source.cpp'

    def exe_file_name(self)->str:
        global test_resource_dir
        return test_resource_dir + '\\' + self.work_dir + '\\source.exe'

    
    def scan_scanfs(self):
        global_chars = set()
        global_ints = set()
        local_chars = set()
        local_ints = set()
        using_global = True
        func_dec_regex = re.compile(r"(?:(?:int)|(?:char))[ \t]+[0-9a-zA-Z_]+\(([0-9a-zA-Z, \t_]*)\)")
        vars_regex = re.compile(r"([a-zA-Z][a-zA-Z0-9_]*)(?:\[\d+\])*")
        scanf_regex = re.compile(r"scanf\(([a-zA-Z][a-zA-Z0-9_]*)\)")
        with open(self.source_c_file_name(), 'r', encoding='utf8') as f:
            for line in f.readlines():
                line = line.lstrip()
                func_m = func_dec_regex.match(line)
                if func_m is not None:
                    using_global = False
                    local_chars.clear()
                    local_ints.clear()
                    params = func_m.group(1)
                    for para in params.split(','):
                        para = para.lstrip()
                        if para[:4] == "char":
                            local_chars.add(para[4:].lstrip())
                        else:
                            local_ints.add(para[3:].lstrip())
                    continue
                if line[:4] == "char":
                    for var in vars_regex.findall(line[4:]):
                        if using_global:
                            global_chars.add(var)
                        else:
                            local_chars.add(var)
                    continue
                if line[:3] == "int":
                    for var in vars_regex.findall(line[3:]):
                        if using_global:
                            global_ints.add(var)
                        else:
                            local_ints.add(var)
                    continue
                scanf_m = scanf_regex.match(line)
                if scanf_m is not None:
                    var = scanf_m.group(1)
                    if var in local_chars:
                        self.input_type_list.append(False)
                    elif var in local_ints:
                        self.input_type_list.append(True)
                    elif var in global_chars:
                        self.input_type_list.append(False)
                    else:
                        self.input_type_list.append(True)
    
    def format_source_from_c_to_cpp(self):
        with open(self.source_cpp_file_name(), 'w', encoding='utf8') as cppf:
            cppf.write(
                """
                #include <iostream>
                namespace mm {
                void scanf(char& c) { std::cin >> c; }
                void scanf(int& i) { std::cin >> i; }
                void printf(const char* msg) { std::cout << msg << std::endl; }
                void printf(const char* msg, char c) { std::cout << msg << c << std::endl; }
                void printf(const char* msg, int c) { std::cout << msg << c << std::endl; }
                void printf(char c) { std::cout << c << std::endl; }
                void printf(int c) { std::cout << c << std::endl; }
                """
            )
            with open(self.source_c_file_name(), 'r', encoding='utf8') as cf:
                for line in cf.readlines():
                    if line.lstrip()[:4] == "case" or line.lstrip()[:7] == "default":
                        cppf.write("break;\n")
                    cppf.write(line)
            cppf.write(
                """
                }
                int main(int argc, char const *argv[]) { mm::main(); return 0; }
                """
            )

    def compile_cpp_to_exe(self):
        command = "g++ -m32 " + self.source_cpp_file_name() + " -o " + self.exe_file_name()
        os.system(command)

    def gen_input(self, num:int):
        with open(self.input_file_name(num), 'w', encoding='utf8') as f:
            for is_int in self.input_type_list:
                if is_int:
                    f.write(str(random.randint(-512, 512)) + '\n')
                else:
                    f.write(self.char_table[random.randint(0, len(self.char_table) + 1)] + '\n')

    def run_exe_and_reencode(self, num: int):
        command = self.exe_file_name() + " > " + self.expect_file_name(num) + " < " + self.input_file_name(num)
        os.system(command)
            
    def auto_gen(self, count: int, need_input: bool=True):
        self.scan_scanfs()
        self.format_source_from_c_to_cpp()
        self.compile_cpp_to_exe()
        for i in range(count):
            if need_input:
                self.gen_input(i)
            self.run_exe_and_reencode(i)
        

if __name__ == "__main__":
    # generator = InputAndExpectGenerator("auto_samples\\sample1")
    # generator.auto_gen(1)
    # generator = InputAndExpectGenerator("auto_samples\\sample2")
    # generator.auto_gen(4)
    # generator = InputAndExpectGenerator("auto_samples\\sample3")
    # generator.auto_gen(1)
    # generator = InputAndExpectGenerator("auto_samples\\sample4")
    # generator.auto_gen(1)   
    # generator = InputAndExpectGenerator("auto_samples\\sample5")
    # generator.auto_gen(1)
    # generator = InputAndExpectGenerator("auto_samples\\sample6")
    # generator.auto_gen(1)
    # generator = InputAndExpectGenerator("auto_samples\\sample7")
    # generator.auto_gen(4)
    # generator = InputAndExpectGenerator("auto_samples\\sample8")
    # generator.auto_gen(1)
    # generator = InputAndExpectGenerator("auto_samples\\sample9")
    # generator.auto_gen(1, False)
    # generator = InputAndExpectGenerator("auto_samples\\div")
    # generator.auto_gen(10, True)
    # generator = InputAndExpectGenerator("auto_samples\\o1_3")
    # generator.auto_gen(1, False)
    generator = InputAndExpectGenerator("auto_samples\\mov")
    generator.auto_gen(10, True)