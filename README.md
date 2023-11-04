# itsnotwallpaperengine
to work with a fork of hyprpaper (https://github.com/Dimethylebutane/hyprpaper)

inspired by https://gitlab.com/amini-allight/wayland-vulkan-example
tested in a two display config with only one exposed for this so and without IPC (see hyprpaper.conf in build/)
running Hyprland on Arch

This project as a big issue: vulkan frame presentation trigger wl\_display\_dispatch and Hyprpaper tick function is called each frame :/
