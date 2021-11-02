# Getting Started:

## Ubuntu Installation:
Follow this [Ubuntu 20.04 Full Installation Walkthrough by LearnLinuxTV](https://www.youtube.com/watch?v=G7ffzC4S0A4).

This process includes creating partitions for Ubuntu, installing the OS, and modifying the bootloader.

⚠️ Please proceed with caution. Any mistake may lead to loss of data and accessibility to Windows OS. Do not panic and consult a technical expert if such a situation arises.

## ROS Installation:
ROS version is dependant on Ubuntu version as:
- ROS Melodic - Ubuntu 18.04
- ROS Noetic - Ubuntu 20.04

Follow this link http://wiki.ros.org/ROS/Installation for a detailed installation guide.

1. Setup sources and keys
```sh
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
```
```sh
sudo apt install curl
```
```sh
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
```

2. Installation, replace &lt;distro&gt; with melodic or noetic
```sh
sudo apt update
```
```sh
sudo apt install ros-<distro>-desktop-full
```
```sh
source /opt/ros/<distro>/setup.bash
```
```sh
echo "source /opt/ros/<distro>/setup.bash" >> ~/.bashrc
```

3. Finishing up
```sh
sudo apt install python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential
```
```sh
sudo rosdep init
```
```sh
rosdep update
```

4. Setup Catkin Build
```sh
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu `lsb_release -sc` main" > /etc/apt/sources.list.d/ros-latest.list'
```
```sh
wget http://packages.ros.org/ros.key -O - | sudo apt-key add -
```
```sh
sudo apt-get update
```
```sh
sudo apt-get install python3-catkin-tools
```

## Setup Git SSH:
A device can be registered as a trusted device in your GitHub Account through an SSH key, allowing easy pull and push actions without entering a password every time.

1. Keep pressing Enter key for subsequent steps after running the command below to complete SSH setup with default settings.
```sh
ssh-keygen -t ed25519 -C "your_email@example.com"
```
2. Navigate to your GitHub Account -> Settings -> SSH and GPG Keys -> New SSH Key.

3. Enter any title (your PC name preferably) and copy & paste the generated lines from the terminal using the command below.
```sh
cat ~/.ssh/id_ed25519.pub
```

## Setup ROS Workspace:
```sh
cd ~/ && mkdir tiburon_ws   # creates a new workspace in home directory
```
```sh
cd tiburon_ws && mkdir src  # creates a src folder in tiburon_ws
```
```sh
catkin build                # builds all packages in the workspace
```
```sh
echo "source ~/tiburon_ws/devel/setup.bash" >> ~/.bashrc # automatically source the workspace
```
```sh
source ~/.bashrc            # source bashrc to apply changes
```
