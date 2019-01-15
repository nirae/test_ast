# Docker test for C program on a Linux Container

## Description
Just a Dockerfile for build a ready linux (debian) container for testing what you want on linux very quickly.

Made specialy for 42 school or people on Mac OS (Not tested on Windows) who's working on C code and want to compile and test it on linux easily. With this container, you'll be able to test, compile, debug, or whatever with your program or what you want.

## Usage

**How it works?**

Move the Dockerfile and all the shell scripts on your project directory.

### Build

Build the image with the shell script **build.sh**

### Run

Run the container with the shell script **run.sh**

The run command mount a volume with your current directory and put all the files in the /app directory in the container.

Now you just have to compile your program or test what you want in the container! The file modifications outside the container will be updated automatically in the container, thanks to the volume.

![Demo](https://media.giphy.com/media/82rc5N3XuhNEXMvBsV/giphy.gif)

Enjoy! :smile:
