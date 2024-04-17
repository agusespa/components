import unittest
from versions import is_version_higher

class TestVersionComparison(unittest.TestCase):
    def test_higher_major(self):
        self.assertTrue(is_version_higher("1.2.5", "2.0.0"))
        self.assertTrue(is_version_higher("9.2.5", "10.0.0"))

    def test_higher_minor(self):
        self.assertTrue(is_version_higher("1.2.5", "1.3.0"))
        self.assertTrue(is_version_higher("1.9.25", "1.10.1"))

    def test_higher_patch(self):
        self.assertTrue(is_version_higher("1.2.5", "1.2.6"))

    def test_equal_versions(self):
        self.assertFalse(is_version_higher("1.5.0", "1.5.0"))

    def test_non_numeric_parts(self):
        self.assertTrue(is_version_higher("1.2.3", "1.2.3-beta"))
        self.assertFalse(is_version_higher("1.0.1", "1.0.0-rc.1"))
        self.assertTrue(is_version_higher("1.5.0-beta", "1.5.0"))

    def test_leading_zeros(self):
        self.assertTrue(is_version_higher("1.02.0", "1.2.1"))  
        self.assertTrue(is_version_higher("1.2.010", "1.2.0200"))  
        self.assertTrue(is_version_higher("1.02.0", "1.002.1")) 
        self.assertFalse(is_version_higher("1.02.1", "1.002.0")) 

    def test_empty_versions(self):
        self.assertFalse(is_version_higher("", "1.0.0"))
        self.assertFalse(is_version_higher("2.4.1", ""))
        self.assertFalse(is_version_higher("", ""))

    def test_invalid_versions(self):
        self.assertFalse(is_version_higher("1-a", "1.0.0"))
        self.assertFalse(is_version_higher("1.0.2a", "1.0.2a3"))

if __name__ == '__main__':
    unittest.main()
