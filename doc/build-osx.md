Mac OS X Build Instructions and Notes
====================================
This guide will show you how to build hyperstaked(headless client) for OSX.

Notes
-----

* Tested on OS X 10.7 through 10.10 on 64-bit Intel processors only.

* All of the commands should be executed in a Terminal application. The
built-in one is located in `/Applications/Utilities`.

Preparation
-----------

You need to install XCode with all the options checked so that the compiler
and everything is available in /usr not just /Developer. XCode should be
available on your OS X installation media, but if not, you can get the
current version from https://developer.apple.com/xcode/. If you install
Xcode 4.3 or later, you'll need to install its command line tools. This can
be done in `Xcode > Preferences > Downloads > Components` and generally must
be re-done or updated every time Xcode is updated.

You will also need to install [Homebrew](http://brew.sh) in order to install library
dependencies.

The installation of the actual dependencies is covered in the Instructions
sections below.

Instructions: Homebrew
----------------------

#### Install dependencies using Homebrew

        brew install autoconf automake libtool boost miniupnpc openssl pkg-config protobuf qt5

NOTE: Building with Qt4 is still supported, however, could result in a broken UI. As such, building with Qt5 is recommended.

#### Installing berkeley-db4 using Homebrew

The homebrew package for berkeley-db4 has been broken for some time.  It will install without Java though.

Running this command takes you into brew's interactive mode, which allows you to configure, make, and install by hand:
```
$ brew install https://raw.githubusercontent.com/Homebrew/homebrew-core/master/Formula/berkeley-db4.rb -–without-java
```

The rest of these commands are run inside brew interactive mode:
```
/private/tmp/berkeley-db4-UGpd0O/db-4.8.30 $ cd ..
/private/tmp/berkeley-db4-UGpd0O $ db-4.8.30/dist/configure --prefix=/usr/local/Cellar/berkeley-db4/4.8.30 --mandir=/usr/local/Cellar/berkeley-db4/4.8.30/share/man --enable-cxx
/private/tmp/berkeley-db4-UGpd0O $ make
/private/tmp/berkeley-db4-UGpd0O $ make install
/private/tmp/berkeley-db4-UGpd0O $ exit
```

After exiting, you'll get a warning that the install is keg-only, which means it wasn't symlinked to `/usr/local`.  You don't need it to link it to build hyperstake, but if you want to, here's how:

    $ brew link --force berkeley-db4


### Building `hyperstaked`

1. Clone the github tree to get the source code and go into the directory.

        git clone https://github.com/hyperstake/hyperstake.git
        cd hyperstake

2.  Build hyperstaked:

        ./autogen.sh
        ./configure --with-gui=qt5
        make

3.  It is also a good idea to build and run the unit tests:

        make check

4.  (Optional) You can also install hyperstaked to your path:

        make install

Use Qt Creator as IDE
------------------------
You can use Qt Creator as IDE, for debugging and for manipulating forms, etc.
Download Qt Creator from http://www.qt.io/download/. Download the "community edition" and only install Qt Creator (uncheck the rest during the installation process).

1. Make sure you installed everything through homebrew mentioned above
2. Do a proper ./configure --with-gui=qt5 --enable-debug
3. In Qt Creator do "New Project" -> Import Project -> Import Existing Project
4. Enter "hyperstake-qt" as project name, enter src/qt as location
5. Leave the file selection as it is
6. Confirm the "summary page"
7. In the "Projects" tab select "Manage Kits..."
8. Select the default "Desktop" kit and select "Clang (x86 64bit in /usr/bin)" as compiler
9. Select LLDB as debugger (you might need to set the path to your installtion)
10. Start debugging with Qt Creator

Creating a release build
------------------------
You can ignore this section if you are building `hyperstaked` for your own use.

hyperstaked binaries are not included in the Hyperstake-Qt.app bundle.

If you are building `hyperstaked` or `Hyperstake-Qt` for others, your build machine should be set up
as follows for maximum compatibility:

All dependencies should be compiled with these flags:

 -mmacosx-version-min=10.7
 -arch x86_64
 -isysroot $(xcode-select --print-path)/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk

Once dependencies are compiled, you can use the following commands to configure and package the Hyperstake-Qt.app
bundle and create the .dmg disk image that can be distributed.

        ./configure --with-boost-system=boost_system-mt --with-boost-filesystem=boost_filesystem-mt
        make deploy


Running
-------

It's now available at `./hyperstaked`, provided that you are still in the `src`
directory. We have to first create the RPC configuration file, though.

Run `./hyperstaked` to get the filename where it should be put, or just try these
commands:

    echo -e "rpcuser=hyperstakerpc\nrpcpassword=$(xxd -l 16 -p /dev/urandom)" > "/Users/${USER}/Library/Application Support/HyperStake/hyperstake.conf"
    chmod 600 "/Users/${USER}/Library/Application Support/HyperStake/hyperstake.conf"

The next time you run it, it will start downloading the blockchain, but it won't
output anything while it's doing this. This process may take several hours;
you can monitor its process by looking at the debug.log file, like this:

    tail -f $HOME/Library/Application\ Support/HyperStake/debug.log

Other commands:
-------

    ./hyperstaked -daemon # to start the hyperstake daemon.
    ./hyperstake --help  # for a list of command-line options.
    ./hyperstake help    # When the daemon is running, to get a list of RPC commands
