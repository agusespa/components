import sys


def parse_version(version_string):
    parts = version_string.split("-")
    numbers = parts[0].split(".")
    prerelease = parts[1].split(".") if len(parts) > 1 else []
    return numbers, prerelease


def is_valid_version(version_segments):
    numbers = version_segments[0]
    if len(numbers) != 3:
        print("Invalid semantic version: incorrect lenght")
        return False

    for i in range(3):
        if not numbers[i].isdigit():
            print("Invalid semantic version: not valid integers")
            return False

    if len(version_segments) > 2:
        print("Invalid semantic version: too many segments")
        return False

    return True


def is_version_higher(curr_version, new_version):
    for i in range(3):
        if int(new_version[0][i]) > int(curr_version[0][i]):
            return True
        elif int(new_version[0][i]) < int(curr_version[0][i]):
            return False

    if not curr_version[1] and new_version[1]:
        return True
    elif curr_version[1] and not new_version[1]:
        return True
    elif not curr_version[1] and not new_version[1]:
        return False

    if curr_version[1][0] != new_version[1][0]:
        return True
    else:
        return int(new_version[1][1]) > int(curr_version[1][1])


def main(args):
    if len(args) < 2 or len(args) > 3:
        print(
            "Incorrect argument list, expects: path/to/semver.py <curr_version> <new_version(optional)>"
        )
        sys.exit(1)

    curr_version = args[1]

    if len(args) == 2:
        parsed_version = parse_version(curr_version)
        result = is_valid_version(parsed_version)
        if not result:
            sys.exit(1)
    else:
        parsed_curr_version = parse_version(curr_version)
        result = is_valid_version(parsed_curr_version)
        if not result:
            sys.exit(1)
        parsed_new_version = parse_version(args[2])
        result = is_valid_version(parsed_new_version)
        if not result:
            sys.exit(1)
        result = is_version_higher(parsed_curr_version, parsed_new_version)
        if not result:
            sys.exit(1)

    sys.exit(0)


if __name__ == '__main__':
    main(sys.argv)
