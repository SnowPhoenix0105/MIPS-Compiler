import sys
import os
import time

current_dir = sys.path[0]

def get_time_stamp():
    return time.strftime("%Y-%m-%d_%H-%M-%S", time.localtime(time.time()))


def copy_and_reencode(
    exclude_file_name :list=["global_control.h", "main.cpp"], 
    exclude_header_content :list=["#pragma once\n"],
    exclude_cpp_content :list=[]):
    global current_dir
    source_dir = current_dir + "\\Develop"
    target_dir = current_dir + "\\MIPS-Compiler"
    source_files = os.listdir(source_dir)
    for file_name in source_files:
        if file_name in exclude_file_name:
            continue
        if file_name[-2:] == ".h":
            source_file = open(source_dir + "\\" + file_name, 'r', encoding = 'gbk')
            target_file = open(target_dir + "\\" + file_name, 'w', encoding = 'utf-8')
            for line in source_file.readlines():
                if line in exclude_header_content:
                    continue
                target_file.write(line)
            source_file.close()
            target_file.close()
        elif file_name[-4:] == ".cpp":
            source_file = open(source_dir + "\\" + file_name, 'r', encoding = 'gbk')
            target_file = open(target_dir + "\\" + file_name, 'w', encoding = 'utf-8')
            for line in source_file.readlines():
                if line in exclude_cpp_content:
                    continue
                target_file.write(line)
            source_file.close()
            target_file.close()


def zip_up():
    global current_dir
    source_dir = current_dir + "\\MIPS-Compiler"
    target_dir = current_dir + "\\Zips"
    source_files = os.listdir(source_dir)
    target_file = target_dir + "\\" + get_time_stamp() + ".zip"
    print("\n".join(source_dir + "\\" + file_name                        \
            for file_name in source_files                               \
                if file_name[-2:] == ".h" or file_name[-4:] == ".cpp"   \
                ))
    command =                                                           \
        "7z a -tzip  "                                                   \
        + target_file + " "                                             \
        + " ".join(source_dir + "\\" + file_name                        \
            for file_name in source_files                               \
                if file_name[-2:] == ".h" or file_name[-4:] == ".cpp"   \
                )
    print(command)
    os.system(command)

if __name__ == "__main__":
    copy_and_reencode()
    zip_up()




