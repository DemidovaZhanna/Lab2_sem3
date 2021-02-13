import qbs

CppApplication {
    consoleApplication: true
    files: [
        "binary_search_tree.h",
        "IDictionary.h",
        "Game.h",       
        "Player.h",
        "Person.h",
        "Computer.h",
        "interface.h",
        "main.cpp",
        "tests.hpp",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
