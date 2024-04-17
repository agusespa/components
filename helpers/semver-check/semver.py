import sys


def parse_version(version_string):
    try:
        parts = version_string.split("-")
        version_parts = parts[0].split(".")
        prerelease = parts[1].split(".") if len(parts) > 1 else []
        return version_parts, prerelease
    except:
        print("Invalid semantic version")
        sys.exit(1)


def check_version(parts):
    if len(parts) < 3:
        return False

    for i in range(3):
        if not parts[i].isdigit():
            return False
    return True


def is_version_higher(target_version, merge_request_version):
    tr_parts = parse_version(target_version)
    mr_parts = parse_version(merge_request_version)

    if not check_version(tr_parts[0]) or not check_version(mr_parts[0]):
        print("Invalid semantic version")
        return False

    for i in range(3):
        if int(mr_parts[0][i]) > int(tr_parts[0][i]):
            return True
        elif int(mr_parts[0][i]) < int(tr_parts[0][i]):
            return False

    if not tr_parts[1] and mr_parts[1]:
        return True
    elif tr_parts[1] and not mr_parts[1]:
        return True
    elif not tr_parts[1] and not mr_parts[1]:
        return False

    if tr_parts[1][0] != mr_parts[1][0]:
        return True
    else:
        return int(mr_parts[1][1]) > int(tr_parts[1][1])


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(
            "Not enough arguments (sage: python versions.py <target_version> <merge_request_version>)"
        )
        sys.exit(1)

    target_version = sys.argv[1]
    merge_request_version = sys.argv[2]

    result = is_version_higher(target_version, merge_request_version)

    if result:
        sys.exit(0)
    else:
        sys.exit(1)
