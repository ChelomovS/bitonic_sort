import random

tests_number = 10

for test_number in range(tests_number):
    name_of_file = f"test_{test_number + 1:02}.in"

    with open(name_of_file, 'w') as file:
        elems_number = random.randint(100000, 1000000)

        file.write(str(elems_number) + "\n")  

        elems = [str(random.randint(-10000000, 10000000)) for _ in range(elems_number)]
        file.write(" ".join(elems) + "\n")