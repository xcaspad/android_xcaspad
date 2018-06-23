# Xcas Pad
### Computer algebra system source code project for Android

Get it at [Google play](https://play.google.com/store/apps/details?id=org.kde.necessitas.mucephi.android_xcas)

As maths kernel, Xcas Pad embed a port of the Giac/Xcas library.
Giac/Xcas library is a powerful maths kernel used by many platforms, like the Hewlett Packard HP Prime calculator. 
Also users of the Texas Instruments calculators(TI89, Voyage 200, TI Nspire) could find very familiar the CAS.

Thanks to Bernard Parisse for write and share [Giac library](http://www-fourier.ujf-grenoble.fr/~parisse/giac.html)

## How to build

* Firs of All download and setup [Android Studio](https://developer.android.com/studio/) to build the Apk.
* Download and setup Android NDK Revision 10e (only this ndk revision are supported to build it)

```console
foo@bar:~$ wget https://dl.google.com/android/repository/android-ndk-r10e-linux-x86_64.zip
foo@bar:~$ unzip android-ndk-r10e-linux-x86_64.zip -d android-ndk-r10
```

* Download giac sorce code and compile JNI libs

```console
foo@bar:~$ mkdir build_xcas_pad && cd build_xcas_pad
foo@bar:~$ wget https://www-fourier.ujf-grenoble.fr/~parisse/debian/dists/stable/main/source/giac_1.4.9-1.tar.gz
foo@bar:~$ tar -xvf giac_1.4.9-1.tar.gz
foo@bar:~$ cd giac-1.4.9
foo@bar:~$ cp src/config.h.android src/config.h
foo@bar:~$ git clone https://github.com/xcaspad/android_xcaspad.git
foo@bar:~$ cd android_xcaspad/jni/
foo@bar:~$ ~/android-ndk-r10/android-ndk-r10e/ndk-build
```

* Once again JNI libs(armeabi-v7a, x86) are compiled we can procced to open the Android Studio project and build the Apk.  
