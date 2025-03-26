import os
Import("env")

# include toolchain paths
env.Replace(COMPILATIONDB_INCLUDE_TOOLCHAIN=False)

# override compilation DB path
env.Replace(COMPILATIONDB_PATH="compile_commands.json")
