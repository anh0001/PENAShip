# Rosjava & Android studio: Create a new package

https://github.com/irvs/ros_tms/wiki/how-to-configure-rosjava-apps-with-gradle
http://guiklink.github.io/portfolio/projects/6-ROS_Java/

## Create a new package
```sh
$ mkdir -p ~/YOUR_WORKSPACE/src
$ cd src
# catkin_create_android_pkg [package name] [dependecy 1] [dependecy 2] ..
$ catkin_create_android_pkg test_pkg android_core rosjava_core std_msgs
```
This will create the package with:
1. build.gradle
2. CMakeLists.txt
3. settings.gradle
4. package.xml

```sh
$ cd test_pkg
# catkin_create_android_project [-t TARGET_VERSION] [-p ANDROID_PACKAGE_NAME] [-a AUTHOR] [NAME]
$ catkin_create_android_project test_project
$ cd ../..
$ catkin_make
```

# Troubleshooting

## Error: catkin_create_android_project

http://answers.ros.org/question/210516/catkin_create_android_project-throws-error/

This is because catkin_create_android_project actually calls android create project to create a ros_android project, you see the no such file error because catkin_create_android_project can not find the android script, what you need to do is add a PATH env varible.

If you use Linux and bash, you need to add something to the .bashrc like this: PATH="WHAT_IS_ALREADY_THERE:YOUR_ANDROID_SCRIPT_PATH" and then source .bashrc in terminal.

Normally, the android script is at YOUR_SDK_ROOT/tools.

```sh
export PATH="/home/anhar/Android/Sdk/tools:$PATH"
```

## Error:Execution failed for task ':test_project:compileDebugJava'.
```
> Cannot find System Java Compiler. Ensure that you have installed a JDK (not just a JRE) and configured your JAVA_HOME system variable to point to the according directory.
```

http://ubuntuhandbook.org/index.php/2015/01/install-openjdk-8-ubuntu-14-04-12-04-lts/

In your android-studio find it in File->Other Settings->Default Project Structure->SDKs. Then i changed JDK home path. /usr/lib/jvm/java-8-oracle

Install the latest openjdk 8:
```sh
sudo add-apt-repository ppa:openjdk-r/ppa
sudo apt-get update
sudo apt-get install openjdk-8-jdk
```

If you have more than one Java versions installed on your system. Run below command set the default Java:

```sh
sudo update-alternatives --config java
```

set default Java Compiler by running:

```sh
sudo update-alternatives --config javac
java -version
```

## Problem with gradle version of android studio
Delete or rename ~/.gradle folder and start again android studio. Android studio will start and load the respective gradle version.

## Unsupported method: AndroidProject.getPluginGeneration() while running project
http://stackoverflow.com/questions/39591531/unsupported-method-androidproject-getplugingeneration-while-running-project

It can occur because AS was checking availability of the Instant Run feature. The fixed is to disable Instant Run:

Windows & Linux:
File -> Settings -> Build, Execution, Deployment -> Instant Run.

Mac:
Android Studio -> Preferences -> Build, Execution, Deployment -> Instant Run.

## Error installing .apk for android studio
http://stackoverflow.com/questions/28704636/insufficient-permissions-for-device-in-android-studio-workspace-running-in-opens

The fix was to enable both 'debug over USB' and 'connect as MTP'. The opposite is true when using the same device and Android Studio and adb on OSX though - to allow Android Studio to recognise the handset, I need to disable 'connect as MTP'.
