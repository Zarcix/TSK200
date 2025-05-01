add_rules("mode.debug", "mode.release")
add_requires("criterion")

target("tsk")
    set_kind("binary")
    add_files("src/**/*.c", "src/*.c")
    add_includedirs("src", "src/tsk", "src/utils", "src/tsk_misc")

target("test")
    set_kind("binary")
    add_files("test/runner.c")
    add_files("src/**/*.c")  -- reuse same code
    set_languages("c23")
    set_warnings("all", "extra", "pedantic")
    add_packages("criterion")

