How to set up Rasware 2012
--------------------------

Note: Only Windows is supported.


### Getting started with Git ###

1. Create a free GitHub account on [github.com](https://github.com/signup/free).
2. Follow GitHub's great instructions for installing [Git](https://help.github.com/articles/set-up-git) on your computer.

### Forking Rasware ###

1. Once logged into GitHub, go to [ut-ras](https://github.com/ut-ras)'s [Rasware2012](https://github.com/ut-ras/Rasware2012).
2. [Fork](https://help.github.com/articles/fork-a-repo) the project's code through the button on the top right. This will copy the project to your own account.
3. Open Git. Depending on what version is installed, the commands will be slightly different.
4. Do Git Clone to create a copy of the project on your computer. In GitHub for Windows, simply click on `clone` next to the Rasware2012 project.
5. Right click on Rasware2012 then click on `open in explorer` to open the folder containing the project.

### Install Putty ###

1. Go to <http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html>.
2. Under "for Windows on Intel x86" click on [putty.exe](http://the.earth.li/~sgtatham/putty/latest/x86/putty.exe) to save putty to your computer.

### Install Keil uVision ###

1. Go to the LM3S811 Evaluation Kit CD at <http://www.ti.com/tool/ekk-lm3s811-cd>.
2. Click Get Software. To download it you'll need to create an account.
3. Unzip the CD folder and open index.html.
4. Click on Tools in the left-hand menu.
5. Click the Keil logo to "download" and open the installer.
6. Follow the installation dialogue.

### Install the drivers ###

1. Go back to the Tools page in the CD folder.
2. Click on the link uder _ICDI Drivers_.
3. Unzip the drivers and place them in the _EKK-LM3S811-CD-705/Tools_ folder.
4. Plug in a RAS board to the computer with a usb computer.
5. Windows should fail at automattically installing drivers.
6. Go to the Device Manager on your computer.
7. For each listing, right-click and select the _Update Driver_ option.
8. Then choose the "Browse my Computer" option.
9. Browse to where you previously unzipped the drivers and select the Tools folder.
10. Once all of the drivers are installed, note the COM port number that appears in the Device Manager. It should read something like `Stellaris Virtual COM Port (COMX)` where X is the port.

### Run the demo project ###
1. Open the Rasware2012 project on your computer.
2. Navigate to _Rasware2012/Robotathon2012_Projects/RASDemo.uvproj_ and open the file with Keil uVision.
3. Click the compile button ![compile](https://lh3.googleusercontent.com/F3ur_YkxpV0FTuK3uqwsFrixAPynRjNBMCC4TuTMVJ6uQtUPyFY_0pM99ZXhlYX6oTjN7b1owbhBNZEWBLcbbj7yncxjHEjTrR5rl2vHMybFWZUs_yk) and make sure that it compiles without errors.
4. With the RAS board connected, click the load button ![load](https://lh4.googleusercontent.com/g7vuTlR6nvfdUuy2kXQzyhiTVXxg4zz2c4GLOksYryNXstDIiF16932tE5FZjliGWQvuit9wbwaOkAio9n1h6TO2ZCyE0e1uVN0_bKATv9abVMkLh4k) and it should load to the board without errors.
5. Open Putty to connect to the board
    1. Click the `serial` radio button.
    2. Under `serial line` type _COMX_ where X was the COM port that was shown in the Device Manager.
    3. Under `speed` type _115200_
    4. Under `Saved Settings` type _Robotathon2012_ and then click `Save` to keep from having to configure it each time.
    5. Now double click `Robotathon2012` to connect to the board.
4. Hit the reset button on the board. The following should appear on the Putty console.

![Putty Console](https://lh6.googleusercontent.com/1j6WbJfeVEiiIcR5qJFt4liD4wWNRV1PWAuWpxkVxgIXu-kenDPIqj_GoYqYr7DHgpl08iJtTFYDNswYOg2Bl47D0f28NiHIVkOG2U0zK5se6Du5TC4)

5. As the menu indicates, press the numbers 0-5 to use the demos.

### Create a team project ###

1. Create a copy of the RASDemo project in _Rasware2012/Robotathon2012_Projects_.
2. Rename the Project to your team's name.
3. Open up Git on your computer.
4. First, `add` the new project folder to Git so Git can keep track of it.
5. Then, create a `commit` in Git to update Git with what has changed.
6. Finally, `push` in Git to update GitHub online.
7. To prevent every team member from needing to create their own fork, you can add their github accounts to your project's collaborators
8. Navigate to your project on github.
9. Click the `Admin` button on the top right.
10. Select the `Collaborators` option on the right.
11. Type your member's usernames to add them as collaborators.
12. They should now be able to clone and edit the same project through git.


Additional Information
----------------------

Keil uVision (we pronounce it as _Kyle Micro-Vision_) is an _Integrated Development Environment_ or IDE for short. Other popular IDEs are Netbeans, Eclipse, IDLE, and VisualStudio
* What is an IDE? <http://en.wikipedia.org/wiki/Integrated_development_environment>

Git is an implementation of something called _version control_ which we use to maintain code in RAS. You might have heard of SVN, or subversion, which is another popular version control. Although it isn't necessary to do well in Robotaton, learning more about git, GitHub, and version control would be a smart idea.
* What is version control? <http://en.wikipedia.org/wiki/Revision_control>
* Why use version control? <http://stackoverflow.com/questions/1408450/why-should-i-use-version-control>

When running PuTTY, we used _115200_ as the speed of the connection. This is the _baud rate_ of the USB connection on the board.
* What is a baud rate and how is it calculated? <http://en.wikipedia.org/wiki/Baud>
