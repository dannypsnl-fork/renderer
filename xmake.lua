add_rules("mode.debug", "mode.release")
set_languages("c11", "c++17")
set_warnings("all", "error")

target("renderer")
    set_kind("binary")
    add_files("src/*.cpp")
