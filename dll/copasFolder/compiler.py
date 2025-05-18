# import subprocess

# # use glut and glew
# file_name = "app"
# include_dir = "dependencies/include"
# lib_dir = "dependencies/lib"
# lib = "-lfreeglut -lglfw3 -lglew32s -lopengl32 -lgdi32"
# preprocessor = "-DGLEW_STATIC"

# def main():
#     # Compile source files
#     subprocess.call(f"g++ -c main.cpp -I {include_dir} {preprocessor} -o main.o", shell=True)

#     # Link object files
#     subprocess.call(f"g++ main.o -o {file_name} -L {lib_dir} {lib} -Wl,--allow-multiple-definition", shell=True)

# if __name__ == "__main__":
#     main()

# use glad
# file_name = "app"
# include_dir = "dependencies/include"
# lib_dir = "dependencies/lib"
# lib = "-lglfw3 -lopengl32 -lgdi32"  

# def main():

#     subprocess.call(f"g++ -c main.cpp -I {include_dir} -o main.o", shell=True)
#     subprocess.call(f"g++ -c glad.c -I {include_dir} -o glad.o", shell=True)
    
#     subprocess.call(f"g++ main.o glad.o -o {file_name} -L {lib_dir} {lib}", shell=True)

# if __name__ == "__main__":
#     main()

import subprocess

# use glut and glew
file_name = "app"
include_dir = "dependencies/include"
lib_dir = "dependencies/lib"
lib = "-lfreeglut -lglfw3 -lglew32s -lopengl32 -lgdi32 -lglu32"
preprocessor = "-DGLEW_STATIC"

def main():
    # Compile source files
    subprocess.call(f"g++ -c main.cpp -I {include_dir} {preprocessor} -o main.o", shell=True)

    # Link object files
    subprocess.call(f"g++ main.o -o {file_name} -L {lib_dir} {lib} -Wl,--allow-multiple-definition", shell=True)

if __name__ == "__main__":
    main()