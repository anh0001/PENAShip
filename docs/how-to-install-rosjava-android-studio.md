#How to install rosjava with android studio:
1. http://www.stratom.com/blog/2016/03/03/build-rosjava-library-sources-in-ros-w-android-studio/
2. http://askubuntu.com/questions/634082/how-to-install-android-studio-on-ubuntu
3. http://stackoverflow.com/questions/19272127/sdk-location-not-found-android-studio-gradle
4. http://stackoverflow.com/questions/36683726/failed-to-find-build-tools-revision-23-0-1


# Troubleshooting
There are some problems when we install the first tutorial

## Install ros-indigo-rosjava-build-tools
```
sudo apt-get install ros-indigo-rosjava-build-tools
```

## Clone android core
```
git clone -b indigo https://github.com/<username>/android_core
```

## Fixing proxy error and android home path
```
export JAVA_TOOL_OPTIONS+=" -Djava.net.useSystemProxies=true"
export ANDROID_HOME=/home/anhar/Android/Sdk
```

## Error: Failed to find Build Tools revision 21.1.2
1. Go to /home/[USER]/Android/Sdk/tools and execute:
```
./android list sdk -a
```

Which will show a list like:
```
    Android SDK Tools, revision 24.0.2
    Android SDK Platform-tools, revision 23.0.2
    Android SDK Platform-tools, revision 23.0.1
```
... and many more

2. Then, execute the command (attention! at your computer the third option may be different):
```
./android update sdk -a -u -t 3
```

It will install the 23.0.1 SDK Platform-tools components.

## Push to github the new android_core packages in ~/rosjava_mvn_repo
- Edit ~/android/src/android_core/package.xml
- Increases the version for example to 0.2.1
- Compile again catkin_make
- The in ~/rosjava_mvn_repo
```
  ./update_maven_repo to push to github
```
