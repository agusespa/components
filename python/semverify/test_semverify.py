import unittest
from semverify import *


class TestVersionHelperFunctions(unittest.TestCase):
    def test_parse_version(self):
        # Test valid version string without prerelease
        self.assertEqual(parse_version("1.2.3"), (["1", "2", "3"], []))

        # Test valid version string with prerelease
        self.assertEqual(parse_version("1.2.3-alpha"), (["1", "2", "3"], ["alpha"]))

        # Test invalid version string with prerelease
        self.assertEqual(parse_version("1.2.3alpha"), (["1", "2", "3alpha"], []))

        # Test valid version string with prerelease and number
        self.assertEqual(
            parse_version("1.2.3-alpha.1"), (["1", "2", "3"], ["alpha", "1"])
        )

        # Test valid version string with prerelease containing numbers and strings
        self.assertEqual(
            parse_version("1.2.3-alpha.10.beta"),
            (["1", "2", "3"], ["alpha", "10", "beta"]),
        )

        # Test empty version string
        self.assertEqual(parse_version(""), ([""], []))

    def test_is_valid_version(self):
        # Test valid semantic version
        self.assertTrue(is_valid_version((["1", "2", "3"], [])))

        # Test valid semantic version with prerelease
        self.assertTrue(is_valid_version((["1", "2", "3"], ["alpha", "1"])))

        # Test invalid semantic version (too short)
        self.assertFalse(is_valid_version((["1", "2"], [])))

        # Test invalid semantic version (too many numbers)
        self.assertFalse(is_valid_version((["1", "2", "3", "4"], [])))

        # Test invalid semantic version (non-numeric numbers)
        self.assertFalse(is_valid_version((["1", "2", "a"], [])))

        # Test invalid semantic version (too many segments)
        self.assertFalse(is_valid_version((["1", "2", "3"], ["alpha", "1"], ["extra"])))

        self.assertTrue(is_valid_version((["0", "54", "0"], ["beta"])))
        self.assertTrue(is_valid_version((["01", "054", "0111"], [])))
        self.assertFalse(is_valid_version((["0", "54", "3dev"], [])))
        self.assertFalse(is_valid_version((["a0", "54", "0"], ["beta"])))
        self.assertFalse(is_valid_version((["0", " 54", "0"], ["beta"])))
        self.assertFalse(is_valid_version((["0", " 54", "1 0"], [])))
        self.assertFalse(is_valid_version((["", "", ""], [])))


class TestVersionComparison(unittest.TestCase):
    def test_higher_major(self):
        self.assertTrue(is_version_higher((["1", "2", "5"], []), (["2", "0", "0"], [])))
        self.assertTrue(
            is_version_higher((["9", "2", "5"], []), (["10", "0", "0"], []))
        )

    def test_higher_minor(self):
        self.assertTrue(is_version_higher((["1", "2", "5"], []), (["1", "3", "0"], [])))
        self.assertTrue(
            is_version_higher((["1", "9", "25"], []), (["1", "10", "1"], []))
        )

    def test_higher_patch(self):
        self.assertTrue(is_version_higher((["1", "2", "5"], []), (["1", "2", "6"], [])))

    def test_equal_versions(self):
        self.assertFalse(
            is_version_higher((["1", "5", "0"], []), (["1", "5", "0"], []))
        )

    def test_non_numeric_parts(self):
        self.assertTrue(
            is_version_higher((["1", "2", "3"], []), (["1", "2", "3"], ["beta"]))
        )
        self.assertTrue(
            is_version_higher((["1", "5", "0"], ["beta"]), (["1", "5", "0"], []))
        )
        self.assertFalse(
            is_version_higher((["1", "0", "1"], []), (["1", "0", "0"], ["rc", "1"]))
        )

    def test_leading_zeros(self):
        self.assertTrue(
            is_version_higher((["1", "02", "0"], []), (["1", "2", "1"], []))
        )
        self.assertFalse(
            is_version_higher((["1", "02", "1"], []), (["1", "002", "0"], []))
        )


class TestMainFunction(unittest.TestCase):
    def test_main_with_two_arguments(self):
        with self.assertRaises(SystemExit) as cm:
            main(["script_name.py", "1.2.3"])
        self.assertEqual(cm.exception.code, 0)

    def test_main_with_three_arguments(self):
        with self.assertRaises(SystemExit) as cm:
            main(["script_name.py", "1.2.3", "1.2.4"])
        self.assertEqual(cm.exception.code, 0)

    def test_main_with_one_argument(self):
        with self.assertRaises(SystemExit) as cm:
            main(["script_name.py"])
        self.assertEqual(cm.exception.code, 1)

    def test_main_with_three_invalid_arguments(self):
        with self.assertRaises(SystemExit) as cm:
            main(["script_name.py", "1.2.3-dev", "1.2.2-dev"])
        self.assertEqual(cm.exception.code, 1)

    def test_main_with_no_arguments(self):
        with self.assertRaises(SystemExit) as cm:
            main([])
        self.assertEqual(cm.exception.code, 1)

    def test_main_with_too_many_arguments(self):
        with self.assertRaises(SystemExit) as cm:
            main(["script_name.py", "1.2.3", "1.2.4", "extra"])
        self.assertEqual(cm.exception.code, 1)

    def test_main_with_two_valid_arguments(self):
        with self.assertRaises(SystemExit) as cm:
            main(["script_name.py", "1.2.3-dev"])
        self.assertEqual(cm.exception.code, 0)

    def test_main_with_two_invalid_arguments(self):
        with self.assertRaises(SystemExit) as cm:
            main(["script_name.py", "1.2.3dev"])
        self.assertEqual(cm.exception.code, 1)


if __name__ == "__main__":
    unittest.main()
