#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream> // Required for std::ostringstream
#include "main.cpp" // Import file mã nguồn chính

namespace py = pybind11;

PYBIND11_MODULE(rsa_uet, m) {
    m.doc() = "Python wrapper for BigNumber RSA method"; // Mô tả thư viện

    py::class_<BigNumber>(m, "BigNumber") // Đăng ký class BigNumber
        .def(py::init<const std::string &>()) // Constructor
        .def("RSA", &BigNumber::RSA, 
             "Perform RSA encryption/decryption",
             py::arg("p"), py::arg("q"))
        .def("modularExponentiation", &BigNumber::modularExponentiation, 
             "Function to perform modular exponentiation (a^b mod n)",
             py::arg("b"), py::arg("n"))
          .def("modularInverse", &BigNumber::modularInverse, 
               "Function to calculate modular inverse using Extended Euclidean Algorithm",
               py::arg("mod"))
          .def("getValue", &BigNumber::getValue, "Get the value of BigNumber as a string")
          .def("__str__", [](const BigNumber &bn) { 
            std::ostringstream oss;
            oss << bn;
            return oss.str(); 
        });
}
