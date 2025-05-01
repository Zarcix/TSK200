project('TSK 200', 'c')

# Subdir all tsk srcs
subdir('./src/tsk')
subdir('./src/tsk_misc')
subdir('./src/utils')

# Link SRC
src = files(
  'src/tsk.c',
  'src/args.c',
  'src/constants.c',
)
src += tsk_src
src += tsk_misc_src
src += utils_src

# Include Stuff
inc = include_directories('src')

executable('tsk', src, include_directories: inc)