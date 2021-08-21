print("Test python")

class BBPythonBase:
    id = 128

    def __init__(self) -> None:
        print("Hello Python")

    def func0(self, arg0, arg1) -> None:
        print("arg0 = ", arg0)
        print("arg1 = ", arg1)
        print("func0")
        return 1234

config = {
    "width" : 1920,
    "height" : 1080,
    "title" : "C++ call Python"
}