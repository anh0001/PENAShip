#How to install OpenFrameworks on Ubuntu 14.04 LTS Arm-based Odroid

* Download latest [openframeworks v0.9.8](http://openframeworks.cc/versions/v0.9.8/of_v0.9.8_linuxarmv7l_release.tar.gz) for armv7
* Extract
* cd to the script
```
cd of_v0.9.8_linux64_release/script/linux/ubuntu
```
* Edit install_dependencies.sh
```
sudo nano install_dependencies.sh
```
Replace this line 
```
if [ $GCC_MAJOR_GT_4 -eq 1 ]; then
```
with,
```
#if [ $GCC_MAJOR_GT_4 -eq 1 ]; then
if [ 1 ]; then
```
This is to force to always build poco from source due to ABI error as follows:
```
undefined reference to xxx (std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned long, H5::DataType const&) const'
```
If you get linker errors about undefined references to symbols that involve types in the std::__cxx11 namespace or the tag [abi:cxx11] then it probably indicates that you are trying to link together object files that were compiled with different values for the _GLIBCXX_USE_CXX11_ABI macro. This commonly happens when linking to a third-party library that was compiled with an older version of GCC. If the third-party library cannot be rebuilt with the new ABI then you will need to recompile your code with the old ABI. From [GNU ABI](https://gcc.gnu.org/onlinedocs/gcc-5.2.0/libstdc++/manual/manual/using_dual_abi.html).

* Clear Ubuntu update list
```
sudo rm -r /var/lib/apt/lists/
sudo apt-get update
```
The source lists seems to be corrupted when we install libgstreamer1.0-dev package. As pointed out in this [issue](https://github.com/openframeworks/openFrameworks/issues/3610).
```
Reading package lists... Done
Building dependency tree
Reading state information... Done
Some packages could not be installed. This may mean that you have
requested an impossible situation or if you are using the unstable
distribution that some required packages have not yet been created
or been moved out of Incoming.
The following information may help to resolve the situation:

The following packages have unmet dependencies:
 libgstreamer-plugins-base1.0-dev : Depends: libgstreamer-plugins-base1.0-0 (= 1.2.4-1~ubuntu1) but 1.4.0-1 is to be installed
                                    Depends: libgstreamer1.0-dev (>= 1.2.0) but it is not going to be installed
                                    Depends: gir1.2-gst-plugins-base-1.0 (= 1.2.4-1~ubuntu1) but 1.4.0-1 is to be installed
E: Unable to correct problems, you have held broken packages.
```
That seems an error in the repositories, as far as i know 14.04 has gstreamer 1.2.something not 1.4 but in your logs it seems to be trying to install gstreamer 1.4 but then some packages have version 1.2.

* Run install_dependencies.sh
```
sudo -E ./install_dependencies.sh
sudo -E ./install_codecs.sh
```

* Build OpenFrameworks
```
cd of_v0.9.7_linux64_release/libs/openFrameworksCompiled/project
make Release
```

* Test with an example
```
cd of_v0.9.7_linux64_release/examples/graphics/polygonExample
make
```
You will encounter error
```
error while loading shared libraries: libGLESv1_CM.so.1: cannot open shared object file: No such file or directory
make: *** [run] Error 127
```
Copy libGLESv1_CM.so from mali-egl (not mesa-egl as odroid uses mali driver) to ./bin/libs, as pointed in [this](http://forum.odroid.com/viewtopic.php?f=61&t=2893).
```
cp /usr/lib/arm-linux-gnueabihf/mali-egl/libGLESv1_CM.so ./bin/libs/
mv ./bin/libs/libGLESv1_CM.so ./bin/libs/libGLESv1_CM.so.1
```