from cs50 import get_int


def main():
    h = 0
    while h < 1 or h > 8:
        h = get_int("Height: ")
    printPyramid(h)


# Function definitions
def printPyramid(h):
    for i in range(h, 0, -1):
        # Print left half
        for j in range(h):
            if j < i-1:
                print(" ", end="")
            else:
                print("#", end="")
        # Print gaps
        print("  ", end="")
        # Print right hal
        for k in range(h, 0, -1):
            if k <= i-1:
                print("", end="")
            else:
                print("#", end="")
        print()  # Prints new line


# Main function
main()