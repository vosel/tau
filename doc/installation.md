# Installation and use

This section describes how to set up the library for use in your C++ project after it was copied to your computer.

## <a name="compiled-vs-headeronly"></a>Compiled vs header-only

There are 2 modes of the library usage in your project.

### 1) Compiled mode.

This mode assumes that the code of the library is added to the project as the part of code base. To use the library this way, add the source files to your project's build path, make sure that all the needed *.cpp files are built by your IDE. All the include statements in your code should use quotation (") marks e.g:
```C++
#include "tau/communications_handling/server_business_logic.h"
```
### 2) Header-only mode.

In this mode you don't have to add any code to your project directly. All you need to do is:
 - add the directory to the `tau` sources to the list of include directories of your project.
 - define the TAU_HEADERONLY macro for the compilation (this will tell the library that it is used in header-only mode)

These steps can be done with compiler switches directly, or by setting up the project preferences in your IDE.

In this mode, the headers of the library should be included with angle brackets surrounding the file name:
```C++
#include <tau/communications_handling/server_business_logic.h>
```

## Dependencies

The most part of the library does not depend on any outside libraries.

The only exception is the `tau::util` namespace. Some of the classes there use boost or non-standard C++ extensions to implement socket I/O. All the code in `tau::util` namespace is non-essential, so the library functionality can be used fully without the need to use any non-standard C++ tools or libraries.

## <a name="cpp11-vs-cpp03"></a> C++11 vs C++03

Another issue to take into consideration is the compatibility with older compiler versions. The library uses some C++11 features, so if the compiler does not support them, a workaround should be used. This is done with `TAU_CPP_03_COMPATIBILITY` macro definition. When this macro is defined, all the features from C++11 are replaced by their older substitutions. Note that this macro does not change the library functionality from the user's point of view.

One of the main changes caused by definition of `TAU_CPP_03_COMPATIBILITY` is switching from `std::shared_ptr` to a library's own substitution implementation of shared pointer (smart pointers are used in `tau::layout_generation`). This implementation is very rudimentary, so there is a way to use more stable implementation of shared pointer here. If there is a `TAU_HAS_BOOST_ACCESS` macro defined, the library will use `boost::shared_ptr` as a substitution of C++11's `std::shared_ptr`, which is more acceptable. Obviously, in this case it is implied that the project has access to the `boost` libraries, otherwise it would not compile.

So, summing up, here is what these macros do:
 - nothing defined: the library will build successfully with any C++11 - compatible compiler
 - `TAU_CPP_03_COMPATIBILITY`, `TAU_HAS_BOOST_ACCESS`: The library will build successfully with any C++03 compiler and `boost` (and will use `boost::shared_ptr` inside it's implementation)
 - `TAU_CPP_03_COMPATIBILITY`: The library will build successfully with any C++03 compiler (using it's own implementation of shared pointers inside it's implementation)
  
 |compiler standard|project uses boost|additional macros to be defined|
 |---|---|---|
 |C++11|yes|no additional options needed|
 |C++11|no|no additional options needed|
 |C++03|yes|`TAU_CPP_03_COMPATIBILITY`, `TAU_HAS_BOOST_ACCESS`|
 |C++03|no|`TAU_CPP_03_COMPATIBILITY`|

