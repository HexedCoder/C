def main():
    with open("input", "r") as file:
        lines = file.readlines()

    income = 0
    num_lines = 0
    for line in lines:
        line = line.split("\t")
        
        if int (line[3]) >= 50 and int(line[3]) <= 59:
            income += int(line[4])
            num_lines += 1

    print(income / num_lines)

if __name__ == "__main__":
    try:
        main()

    except (ValueError, FileNotFoundError, KeyboardInterrupt) as exc:
        print(exc)

