import shutil
from os.path import join

Import("env")

def copy_hex_file(source, target, env):
    # Always copy firmware.hex from the build directory
    hex_path = join(env.subst("$BUILD_DIR"), "firmware.hex")
    # Always copy to sim_circuit/proteus/firmware.hex
    dest_path = join(env.subst("$PROJECT_DIR"), "sim_circuit/proteus", "firmware.hex")
    shutil.copyfile(hex_path, dest_path)
    print("Copied HEX to: " + dest_path)

# Register the post-action to copy firmware.hex after build
env.AddPostAction("$BUILD_DIR/firmware.hex", copy_hex_file)
