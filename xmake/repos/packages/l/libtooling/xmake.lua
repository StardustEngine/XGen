package("libtooling")

    set_kind("library")
    set_homepage("https://llvm.org/")
    set_description("The Clang Libtooling")

    if is_plat("windows") then
        if is_arch("x86") then
            set_urls("https://github.com/StardustEngine/LLVM-Build/releases/download/llvm-$(version)/llvm-$(version)-x86.7z")
            add_versions("17.0.6", "d5083e7e65cf86ac5090a279e37d5850a2973e0dfcb6880e247b3c8aa0d1618e")
        else
            set_urls("https://github.com/StardustEngine/LLVM-Build/releases/download/llvm-$(version)/llvm-$(version)-x64.7z")
            add_versions("17.0.6", "b937fec026314af0a193b57eb43c78804505a27640c9627562d2847e7cff9715")
        end
    end

    on_load(function (package)
        package:add("syslinks", "Version", "advapi32", "Shcore", "user32", "shell32", "Ole32", { public = true })
        package:add("linkdirs", "lib")
        for _, lib_path in ipairs(os.files(package:installdir("lib"))) do
            local lib = string.gsub(path.basename(lib_path), "%..*$", "")
            package:add("links", lib)
        end
    end)

    on_install(function (package)
        os.cp("*", package:installdir())
    end)
