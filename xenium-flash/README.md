# xenium-jtag

This is the app that programs the OpenXenium Flash memory CPLD. See the main README for more details 

- This app will be built and installed automatically via the main `install` script.


# xenium-flash

This is the main app that uploads XeniumOS images into the OpenXenium Flash memory. See the main README for more details 

- This app will be built and installed automatically via the main `install` script.

- NOTE - the `xeniumflash.jed` MUST be loaded into the Xilinx CPLD first BEFORE you can upload any Flash memory images. (the `xenium-programmer` script in the main folder will automatically do this for you)

# Building
- just run `make` or run the `install` script in root of this repo to make and install

