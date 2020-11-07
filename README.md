[![pipeline status](https://gitlab.com/kyb/byte_vector/badges/master/pipeline.svg)](https://gitlab.com/kyb/byte_vector/pipelines?scope=branches) 
[![releases](https://img.shields.io/badge/byte_vector-releases-green.svg?style=flat)](https://gitlab.com/kyb/byte_vector/-/releases) 
[![Gitpod.ready-to-code](https://img.shields.io/badge/Gitpod-ready--to--code-blue?logo=gitpod)](https://gitpod.io/#https://gitlab.com/kyb/byte_vector) 

byte_vector for C++
===================
**byte_vector** is a family of C++ types and functions to provide next functionality:
- serialization to sequence of bytes
- deserialization from sequence of bytes
- print array of bytes in hex to any std::ostream like cout, cerr or stringstream

**byte_vector.hh** is a one-file header-only library for C++17.


Serialization is provided by family of template `operator<<`. 
Deserialization is provided by family of template `operator>>`. 


`byte_vector` is a template instatiation `std::vector<uint8_t>`.  
It provides ability to serialize and deserialize A family of template `operator<<`

