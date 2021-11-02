add_rules("mode.debug", "mode.release")
set_languages("c11", "c++17")

target("renderer")
    set_kind("binary")
    add_files("src/*.cpp")
    if is_mode("debug") then
        add_defines("DEBUG")
    end
