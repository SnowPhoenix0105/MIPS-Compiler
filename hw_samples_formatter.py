import os
import sys
import re


current_dir = sys.path[0]
test_resource_dir = current_dir + "\\UnitTest\\TestResource"
hw_samples_dir = test_resource_dir + "\\hw_samples"


class HWSamplesFormatter:
    def __init__(self, origin_dir: str, serise_num: int):
        self.origin_dir = origin_dir
        self.serise_num = serise_num

    @staticmethod
    def format_expect(origin: str) -> str:
        while origin[-1] == '\n':
            origin = origin[:-1]
        origin += '\n'
        return origin

    def target_dir(self, sub_serise: int, case_num: int) -> str:
        global hw_samples_dir
        return hw_samples_dir + "\\hw_" + str(self.serise_num) + "_" + str(sub_serise) + "_" + str(case_num)

    def target_input(self, sub_serise: int, case_num: int) -> str:
        return self.target_dir(sub_serise, case_num) + "\\0.input"

    def target_expect(self, sub_serise: int, case_num: int) -> str:
        return self.target_dir(sub_serise, case_num) + "\\0.expect"

    def target_source(self, sub_serise: int, case_num: int) -> str:
        return self.target_dir(sub_serise, case_num) + "\\source.c"

    def origin_input(self, sub_serise: int, case_num: int) -> str:
        return self.origin_dir + "\\" + str(sub_serise) + "\\input" + str(case_num) + ".txt"

    def origin_expect(self, sub_serise: int, case_num: int) -> str:
        return self.origin_dir + "\\" + str(sub_serise) + "\\output" + str(case_num) + ".txt"

    def origin_source(self, sub_serise: int, case_num: int) -> str:
        return self.origin_dir + "\\" + str(sub_serise) + "\\testfile" + str(case_num) + ".txt"

    def copy(self, sub_serise: int, case_num: int):
        if not os.path.exists(self.target_dir(sub_serise, case_num)):
            os.makedirs(self.target_dir(sub_serise, case_num))
        with open(self.target_input(sub_serise, case_num), 'w', encoding='utf8') as fo:
            try:
                with open(self.origin_input(sub_serise, case_num), 'r', encoding='utf8') as fi:
                    fo.write(fi.read())
            except FileNotFoundError:
                pass
        with open(self.target_expect(sub_serise, case_num), 'w', encoding='utf8') as fo:
            with open(self.origin_expect(sub_serise, case_num), 'r', encoding='utf8') as fi:
                fo.write(self.format_expect(fi.read()))
        with open(self.target_source(sub_serise, case_num), 'w', encoding='utf8') as fo:
            with open(self.origin_source(sub_serise, case_num), 'r', encoding='utf8') as fi:
                fo.write(fi.read())

    def walk(self):
        testfile_regex = re.compile(r"testfile(\d+).txt")

        sub_serises = os.listdir(self.origin_dir)

        for sub_serise in sub_serises:
            sub_serise_dir = self.origin_dir + "\\" + sub_serise

            cases = os.listdir(sub_serise_dir)
            max_case_num = 0
            for case in cases:
                m = testfile_regex.match(case)
                if m is None:
                    continue
                case_num = int(m.group(1))
                max_case_num = max(max_case_num, case_num)

            for i in range(1, max_case_num + 1):
                self.copy(sub_serise, i)

            
if __name__ == "__main__":
    HW_1_Formatter = HWSamplesFormatter("D:\\Projects\\C++\\MIPS-Compiler\\Zips\\20201117124628596", 1)
    HW_1_Formatter.walk()

    HW_2_Formatter = HWSamplesFormatter("D:\\Projects\\C++\\MIPS-Compiler\\Zips\\20201116145744579", 2)
    HW_2_Formatter.walk()
