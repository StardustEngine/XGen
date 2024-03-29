set_project("XGen")
set_version("0.0.1")

add_rules("mode.debug", "mode.release")
set_languages("cxx17")

add_repositories("xgen-repo xmake/repos")
add_requires("mustache", "libtooling", "lua")

target("xgen")
    set_kind("binary")
    add_files("src/**.cpp")
    add_includedirs("include")
    add_packages("mustache", "libtooling", "lua")